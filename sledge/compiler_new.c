#include "minilisp.h"
#include "reader.h"
#include "writer.h"
#include "alloc.h"
#include "compiler_new.h"
#include "stream.h"
//#include "utf8.c"

#define env_t StrMap
static env_t* global_env = NULL;

//#define CHECK_BOUNDS    // enforce boundaries of array put/get
#define ARG_SPILLOVER 3 // max 4 args (0-3) via regs, rest via stack
#define LBDREG R4       // register base used for passing args to functions

static int debug_mode = 0;

env_entry* lookup_global_symbol(char* name) {
  env_entry* res;
  int found = sm_get(global_env, name, (void**)&res);
  //printf("[lookup] %s res: %p\n",name,res);
  if (!found) return NULL;
  return res;
}

Cell* insert_symbol(Cell* symbol, Cell* cell, env_t** env) {
  env_entry* e;
  int found = sm_get(*env, symbol->ar.addr, (void**)&e);
  
  //printf("sm_get res: %d\r\n",found);
  
  if (found) {
    e->cell = cell;
    //printf("[insert_symbol] update %s entry at %p (cell: %p value: %d)\r\n",symbol->ar.addr,e,e->cell,e->cell->ar.value);
    return e->cell;
  }
    
  e = malloc(sizeof(env_entry));
  memcpy(e->name, (char*)symbol->ar.addr, symbol->dr.size);
  e->cell = cell;

  //printf("[insert_symbol] %s entry at %p (cell: %p)\r\n",symbol->ar.addr,e,e->cell);
  sm_put(*env, e->name, e);

  return e->cell;
}

Cell* insert_global_symbol(Cell* symbol, Cell* cell) {
  return insert_symbol(symbol, cell, &global_env);
}

#define TMP_PRINT_BUFSZ 1024

static FILE* jit_out;
static Cell* cell_heap_start;
static int label_skip_count = 0;
static char temp_print_buffer[TMP_PRINT_BUFSZ];
static Cell* consed_type_error;
static Cell* reusable_type_error;
static Cell* reusable_nil;

#ifdef CPU_ARM
#include "jit_arm_raw.c"
#define PTRSZ 4
#endif

#ifdef CPU_X64
#include "jit_x64.c"
#define PTRSZ 8
#endif

#ifdef CPU_X86
#include "jit_x86.c"
#define PTRSZ 4
#endif

#ifdef __AMIGA
#include "jit_m68k.c"
#define PTRSZ 4
#endif

Cell* lisp_print(Cell* arg) {
  lisp_write(arg, temp_print_buffer, TMP_PRINT_BUFSZ);
  printf("%s\r\n",temp_print_buffer);
  return arg;
}

void load_int(int dreg, Arg arg, Frame* f) {
  if (arg.type == ARGT_CONST) {
    // argument is a constant like 123, "foo"
    jit_movi(dreg, (jit_word_t)arg.cell->ar.value);
  }
  else if (arg.type == ARGT_CELL) {
    if (arg.cell == NULL) {
      // not sure what this is
      //if (dreg!=R0) jit_movr(dreg, R0);
      if (dreg!=R1+arg.slot) {
        jit_movr(dreg, R1+arg.slot); // FIXME: really true?
      }
      jit_ldr(dreg);
    } else {
      // argument is a cell pointer
      jit_lea(dreg, arg.cell);
      jit_ldr(dreg);
    }
  }
  else if (arg.type == ARGT_ENV) {
    // argument is an environment table entry, load e->cell->ar.value
    jit_lea(dreg, arg.env);
    jit_ldr(dreg);
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_REG) {
    // argument comes from a register
    jit_movr(dreg, LBDREG+arg.slot);
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_INT) {
    if (dreg!=R1+arg.slot) {
      jit_movr(dreg, R1+arg.slot); // FIXME: really true?
    }
  }
  else if (arg.type == ARGT_STACK) {
    jit_ldr_stack(dreg, PTRSZ*(arg.slot+f->sp));
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_STACK_INT) {
    jit_ldr_stack(dreg, PTRSZ*(arg.slot+f->sp));
  }
  else {
    jit_movi(dreg, 0xdeadbeef);
  }
}

void load_cell(int dreg, Arg arg, Frame* f) {
  if (arg.type == ARGT_CELL || arg.type == ARGT_CONST) {
    if (arg.cell == NULL) {
      // not sure what this is
      jit_movr(dreg, R1+arg.slot); // FIXME: really true?
    } else {
      // argument is a cell pointer
      jit_lea(dreg, arg.cell);
    }
  }
  else if (arg.type == ARGT_ENV) {
    jit_lea(dreg, arg.env);
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_REG) {
    jit_movr(dreg, LBDREG+arg.slot);
  }
  else if (arg.type == ARGT_STACK) {
    jit_ldr_stack(dreg, PTRSZ*(arg.slot+f->sp));
  }
  else if (arg.type == ARGT_STACK_INT) {
    // FIXME possible ARGR0 clobbering
    int adjust = 0;
    if (dreg!=ARGR0) {jit_push(ARGR0,ARGR0); adjust++;}
    if (dreg!=R0) {jit_push(R0,R0); adjust++;}
    jit_ldr_stack(ARGR0, PTRSZ*(arg.slot+f->sp+adjust));
    jit_call(alloc_int, "alloc_int");
    jit_movr(dreg,R0);
    if (dreg!=R0) jit_pop(R0,R0);
    if (dreg!=ARGR0) jit_pop(ARGR0,ARGR0);
  }
  else {
    jit_movi(dreg, 0xdeadcafe);
  }
}

int get_sym_frame_idx(char* argname, Arg* fn_frame, int ignore_regs) {
  int i;
  if (!fn_frame) return -1;
  
  for (i=0; i<MAXFRAME; i++) {
    if (fn_frame[i].name) {
      //printf("<< get_sym_frame_idx %i (type %d, reg = %d, looking for %s): %s\n",i,fn_frame[i].type,ARGT_REG,argname,fn_frame[i].name);
      
      if (!((fn_frame[i].type == ARGT_REG) && ignore_regs)) {
        if (!strcmp(argname, fn_frame[i].name)) {
          //printf("!! get_sym_frame_idx %i (type %d): %s\n",i,fn_frame[i].type,fn_frame[i].name);
          //printf("returning %d\n",i);
          return i;
        }
      }
    }
  }
  return -1;
}

// TODO: optimize!
int push_frame_regs(Arg* fn_frame) {
  int pushreg=0;
  int pushstack=0;
  int i;
  
  if (!fn_frame) return 0;
  
  for (i=0; i<MAXFRAME; i++) {
    if (fn_frame[i].type == ARGT_REG) {
      pushreg++;
    }
  }
  //printf("pushing %d frame regs\n",pushreg);
  if (pushreg) {
    jit_push(LBDREG,LBDREG+pushreg-1);
  }
  return pushreg;
}

int pop_frame_regs(Arg* fn_frame) {
  int pushreg=0;
  int pushstack=0;
  int i;
  
  if (!fn_frame) return 0;
  
  for (i=0; i<MAXFRAME; i++) {
    if (fn_frame[i].type == ARGT_REG) {
      pushreg++;
    }
  }
  //printf("popping %d frame regs\n",pushreg);
  if (pushreg) {
    jit_pop(LBDREG,LBDREG+pushreg-1);
  }
  return pushreg;
}

static char* analyze_buffer[MAXFRAME];
int analyze_fn(Cell* expr, Cell* parent, int num_lets) {
  if (expr->tag == TAG_SYM) {
    env_entry* op_env = lookup_global_symbol(expr->ar.addr);
    if (op_env) {
      Cell* op = op_env->cell;
      if (op->tag == TAG_BUILTIN) {
        //printf("analyze_fn: found builtin: %s\n",expr->ar.addr);
        if (op->ar.value == BUILTIN_LET) {
          Cell* sym = car(cdr(parent));
          if (sym) {
            int existing = 0, i;
            for (i=0; i<num_lets; i++) {
              if (!strcmp(analyze_buffer[i], sym->ar.addr)) {
                //printf("-- we already know local %s\r\n",sym->ar.addr);
                existing = 1;
                break;
              }
            }
            if (!existing) {
              analyze_buffer[num_lets] = sym->ar.addr;
              num_lets++;
            }
          } else {
            printf("!! analyze error: malformed let!\r\n");
          }
        }
      }
    }
  }
  else if (expr->tag == TAG_CONS) {
    if (car(expr)) {
      num_lets = analyze_fn(car(expr), expr, num_lets);
    }
    if (cdr(expr)) {
      num_lets = analyze_fn(cdr(expr), expr, num_lets);
    }
  }
  return num_lets;
}

int compile_expr(Cell* expr, Frame* frame, int return_type) {
  int compiled_type = TAG_ANY;
  Arg* fn_frame = frame->f;
  Cell* opsym, *args, *orig_args, *signature_args, *op;
  env_entry* op_env;
  
  int is_let = 0;
  int argi = 0;
  Arg argdefs[MAXARGS];

  if (!expr) return 0;
  if (!frame) return 0;
  
  if (expr->tag != TAG_CONS) {
    if (expr->tag == TAG_SYM) {
      int arg_frame_idx = get_sym_frame_idx(expr->ar.addr, fn_frame, 0);
      env_entry* env;
      
      if (arg_frame_idx>=0) {
        load_cell(R0, fn_frame[arg_frame_idx], frame);
        return compiled_type;
      }

      env = lookup_global_symbol(expr->ar.addr);
      if (env) {
        Cell* value = env->cell;
        jit_movi(R0,(jit_word_t)env);
        jit_ldr(R0);
        return value->tag; // FIXME TODO forbid later type change
      } else {
        printf("undefined symbol %s\n",expr->ar.addr);
        jit_movi(R0,0);
        return 0;
      }
    } else {
      // return the expr
      jit_movi(R0,(jit_word_t)expr);
      return compiled_type;
    }
    return 0;
  }

  cell_heap_start = get_cell_heap();
  
  opsym = car(expr);
  args = cdr(expr);
  orig_args = args; // keep around for specials forms like DO
  signature_args = NULL;

  if (!opsym || opsym->tag != TAG_SYM) {
    printf("[compile_expr] error: non-symbol in operator position.\n");
    return 0;
  }

  op_env = lookup_global_symbol(opsym->ar.addr);

  if (!op_env || !op_env->cell) {
    printf("[compile_expr] error: undefined symbol %s in operator position.\n",opsym->ar.addr);
    return 0;
  }
  op = op_env->cell;
  
  //printf("op tag: %d\n",op->tag);
  if (op->tag == TAG_BUILTIN) {
    signature_args = op->dr.next;

    if (op->ar.value == BUILTIN_LET) {
      is_let = 1;
    }
    
  } else if (op->tag == TAG_LAMBDA) {
    signature_args = car((Cell*)(op->ar.addr));
  } else {
    printf("[compile-expr] error: non-lambda symbol %s in operator position.\n",opsym->ar.addr);
    return 0;
  }

  //printf("[op] %s\n",debug_buf);
  //lisp_write(signature_args, debug_buf, sizeof(debug_buf));
  //printf("[sig] %s\n",debug_buf);

  if (debug_mode) {
    char* debug_buf = malloc(256);
    push_frame_regs(frame->f);
    lisp_write(expr, debug_buf, 256);
    jit_push(R0, ARGR1);
    jit_lea(ARGR0, debug_buf);
    jit_lea(ARGR1, frame);
    jit_call(debug_handler,"dbg");
    jit_pop(R0, ARGR1);
    pop_frame_regs(frame->f);
  }
  
  // first, we need a signature

  do {
    Cell* arg = car(args);
    Cell* signature_arg = car(signature_args);
    char arg_name[32];
    snprintf(arg_name,sizeof(arg_name),"a%d",argi+1,arg_name,10);
    // 1. is the arg the required type? i.e. a pointer or a number?

    if (signature_arg && signature_arg->tag == TAG_CONS) {
      // named argument
      snprintf(arg_name,sizeof(arg_name),car(signature_arg)->ar.addr);
      signature_arg = cdr(signature_arg);
    }

    if (arg && (!signature_args || signature_arg)) {
      int given_tag = arg->tag;

      if (is_let && argi==1) {
        int type_hint = -1;
        // check the symbol to see if we already have type information
        int fidx = get_sym_frame_idx(argdefs[0].cell->ar.addr, fn_frame, 1);
        if (fidx>=0) {
          //printf("existing type information for %s: %d\r\n", argdefs[0].cell->ar.addr,fn_frame[fidx].type);
          type_hint = fn_frame[fidx].type;
        }
      
        if (given_tag == TAG_INT || type_hint == ARGT_STACK_INT) {
          //printf("INT mode of let\r\n");
          // let prefers raw integers!
          signature_arg->ar.value = TAG_INT;
        } else {
          //printf("ANY mode of let\r\n");
          // but cells are ok, too
          signature_arg->ar.value = TAG_ANY;
        }
      }

      if (!signature_args) {
        // any number of arguments allowed
        argdefs[argi].cell = arg;
        argdefs[argi].type = ARGT_CELL;
      }
      else if (signature_arg->ar.value == TAG_LAMBDA) {
        // lazy evaluation by form
        argdefs[argi].cell = arg;
        argdefs[argi].type = ARGT_LAMBDA;
      }
      else if (arg->tag == TAG_CONS) {
        // eager evaluation
        // nested expression
        if (argi>0) {
          // save registers
          // FIXME RETHINK

          jit_push(R1,R1+argi-1);
          frame->sp+=(1+argi-1);
        }
        given_tag = compile_expr(arg, frame, signature_arg->ar.value);
        if (given_tag<1) return given_tag; // failure
        
        argdefs[argi].cell = NULL; // cell is in R0 at runtime
        argdefs[argi].slot = argi;

        if (given_tag == TAG_INT) {
          argdefs[argi].type = ARGT_INT;
          jit_movr(R1+argi,ARGR0);
        } else {
          argdefs[argi].type = ARGT_CELL;
          jit_movr(R1+argi,R0);
        }
        
        if (argi>0) {
          jit_pop(R1,R1+argi-1);
          frame->sp-=(1+argi-1);
        }
      }
      else if (given_tag == TAG_SYM && signature_arg->ar.value != TAG_SYM) {
        // symbol given, lookup (indirect)
        //printf("indirect symbol lookup (name: %p)\n",arg->ar.value);

        int arg_frame_idx = get_sym_frame_idx(arg->ar.addr, fn_frame, 0);

        // argument passed to function in register
        if (arg_frame_idx>=0) {
          argdefs[argi] = fn_frame[arg_frame_idx];

          //printf("argument %s from stack frame.\n", arg->ar.addr);
        } else {
          argdefs[argi].env = lookup_global_symbol((char*)arg->ar.addr);
          argdefs[argi].type = ARGT_ENV;
          
          //printf("argument %s from environment.\n", arg->ar.addr);
        }
        //printf("arg_frame_idx: %d\n",arg_frame_idx);

        if (!argdefs[argi].env && arg_frame_idx<0) {
          printf("undefined symbol %s given for argument %s.\n",arg->ar.addr,arg_name);
          return 0;
        }
      }
      else if (given_tag == signature_arg->ar.value || signature_arg->ar.value==TAG_ANY) {
        argdefs[argi].cell = arg;
        argdefs[argi].slot = argi-1;
        argdefs[argi].type = ARGT_CELL;

        if (given_tag == TAG_SYM || given_tag == TAG_CONS || given_tag == TAG_INT || given_tag == TAG_STR || given_tag == TAG_BYTES) {
          argdefs[argi].type = ARGT_CONST;
          //printf("const arg of type %d at %p\n",arg->tag,arg);
        }
      } else {
        // check if we can typecast
        // else, fail with type error

        printf("!! type mismatch for argument %s (given %s, expected %s)!\n",arg_name,tag_to_str(given_tag),tag_to_str(signature_arg->ar.value));
        return 0;
      }
    } else {
      if (!arg && signature_arg) {
        // missing arguments
        printf("!! argument %s missing!\n",arg_name);
        return 0;
      } else if (arg && !signature_arg) {
        // surplus arguments
        printf("!! surplus arguments!\n");
        return 0;
      }
    }
    
    argi++;
  } while (argi<MAXARGS && (args = cdr(args)) && (!signature_args || (signature_args = cdr(signature_args))));

  if (op->tag == TAG_BUILTIN) {
    switch (op->ar.value) {
    case BUILTIN_BITAND: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_andr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_BITOR: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_orr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_BITXOR: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_xorr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_SHL: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_shlr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_SHR: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_shrr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_ADD: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_addr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_SUB: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_subr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_MUL: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_mulr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_DIV: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_divr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_MOD: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_modr(ARGR0,R2);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_GT: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_movi(R3,0);
      jit_subr(ARGR0,R2);
      jit_movneg(ARGR0,R3);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_LT: {
      load_int(R2,argdefs[0], frame);
      load_int(ARGR0,argdefs[1], frame);
      jit_movi(R3,0);
      jit_subr(ARGR0,R2);
      jit_movneg(ARGR0,R3);
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_DEF: {
      // TODO in the future, we could pre-allocate symbols
      // and especially their types based on type inference
      
      jit_lea(ARGR0,argdefs[0].cell); // load symbol address
      load_cell(ARGR1,argdefs[1],frame);
      
      push_frame_regs(frame->f);
      jit_call2(insert_global_symbol, "insert_global_symbol");
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_LET: {
      int is_int, offset, fidx, is_reg;
      
      if (!frame->f) {
        printf("<error: let is not allowed on global level, only in fn>\r\n");
        return 0;
      }
      
      is_int = 0;
      offset = MAXARGS + frame->locals;
      fidx = get_sym_frame_idx(argdefs[0].cell->ar.addr, fn_frame, 0);

      // el cheapo type inference
      if (1 &&
          (argdefs[1].type == ARGT_INT ||
           argdefs[1].type == ARGT_STACK_INT ||
           (argdefs[1].type == ARGT_CONST && argdefs[1].cell->tag == TAG_INT) ||
           (fidx>=0 && fn_frame[fidx].type == ARGT_STACK_INT) // already defined as int TODO: error on attempted type change
         )) {
        load_int(R0, argdefs[1], frame);
        is_int = 1;
        compiled_type = TAG_INT;
      } else {
        load_cell(R0, argdefs[1], frame);
        compiled_type = TAG_ANY;
      }

      is_reg = 0;
      
      if (fidx >= 0) {
        // existing stack entry
        offset = fidx;
        //printf("+~ frame entry %s, existing stack-local idx %d\n",fn_frame[offset].name,fn_frame[offset].slot);

        if (fn_frame[offset].type == ARGT_REG) {
          is_reg = 1;
        }
      } else {
        fn_frame[offset].name = argdefs[0].cell->ar.addr;
        fn_frame[offset].cell = NULL;
        if (is_int) {
          fn_frame[offset].type = ARGT_STACK_INT;
        } else {
          fn_frame[offset].type = ARGT_STACK;
        }
        fn_frame[offset].slot = frame->locals;
        //printf("++ frame entry %s, new stack-local idx %d, is_int %d\n",fn_frame[offset].name,fn_frame[offset].slot,is_int);
        frame->locals++;
      }

      if (!is_reg) {
        jit_str_stack(R0,PTRSZ*(fn_frame[offset].slot+frame->sp));
      }
      
      if (compiled_type == TAG_INT && return_type == TAG_ANY) {
        jit_movr(ARGR0,R0);
        jit_call(alloc_int, "alloc_int");
        compiled_type = TAG_ANY;
      }

      if (is_reg) {
        jit_movr(LBDREG + fn_frame[offset].slot, R0);
        printf("let %s to reg: %d\r\n",fn_frame[offset].name, LBDREG + fn_frame[offset].slot);
      }
      
      break;
    }
    case BUILTIN_FN: {
      Cell* fn_body, *fn_args, *lambda;
      Arg fn_new_frame[MAXFRAME];
      int num_lets, i, j, spo_count, fn_argc, tag;
      char label_fn[64];
      char label_fe[64];
      
      Frame* nframe_ptr;
      Frame nframe = {fn_new_frame, 0, 0, frame->stack_end};
      Label* fn_lbl;
      
      if (argi<2) {
        printf("error: trying to define fn without body.\n");
        return 0;
      }
      
      // body
      fn_body = argdefs[argi-2].cell;

      // estimate stack space for locals
      num_lets = analyze_fn(fn_body,NULL,0);
      
      // scan args (build signature)
      fn_args = alloc_nil();
      
      for (i=0; i<MAXFRAME; i++) {
        fn_new_frame[i].type = 0;
        fn_new_frame[i].slot = -1;
        fn_new_frame[i].name = NULL;
      }

      spo_count = 0;

      fn_argc = 0;
      for (j=argi-3; j>=0; j--) {
        Cell* arg = alloc_cons(alloc_sym(argdefs[j].cell->ar.addr),alloc_int(TAG_ANY));
        fn_args = alloc_cons(arg,fn_args);

        if (j>=ARG_SPILLOVER) { // max args passed in registers
          fn_new_frame[j].type = ARGT_STACK;
          fn_new_frame[j].slot = num_lets + 2 + ((argi-3)-j);
          spo_count++;
        }
        else {
          fn_new_frame[j].type = ARGT_REG;
          fn_new_frame[j].slot = j;
        }
        fn_new_frame[j].name = argdefs[j].cell->ar.addr;
        fn_argc++;

        //printf("arg j %d: %s\r\n",j,fn_new_frame[j].name);
      }
      //char sig_debug[128];
      //lisp_write(fn_args, sig_debug, sizeof(sig_debug));
      //printf("signature: %s\n",sig_debug);


      //lisp_write(fn_body, sig_debug, sizeof(sig_debug));
      
      lambda = alloc_lambda(alloc_cons(fn_args,fn_body));
      lambda->dr.next = 0;

      sprintf(label_fn,"f0_%p",lambda);
      sprintf(label_fe,"f1_%p",lambda);
      
      jit_jmp(label_fe);
      jit_label(label_fn);
      jit_movi(R2,(jit_word_t)lambda|STACK_FRAME_MARKER);
      jit_push(R2,R2);
      
      jit_dec_stack(num_lets*PTRSZ);

      if (debug_mode) {
        Arg* nargs_ptr;

        // in debug mode, we need a copy of the frame definition at runtime
        nframe_ptr = malloc(sizeof(Frame));
        memcpy(nframe_ptr, &nframe, sizeof(Frame));
        nargs_ptr = malloc(sizeof(Arg)*MAXFRAME);
        memcpy(nargs_ptr, nframe.f, sizeof(Arg)*MAXFRAME);
        nframe_ptr->f = nargs_ptr;

        printf("frame copied: %p args: %p\r\n",nframe_ptr,nframe_ptr->f);
      } else {
        nframe_ptr = &nframe;
      }
      
      tag = compile_expr(fn_body, nframe_ptr, TAG_ANY); // new frame, fresh sp
      if (!tag) return 0;

      //printf(">> fn has %d args and %d locals. predicted locals: %d\r\n",fn_argc,nframe.locals,num_lets);
      
      jit_inc_stack(num_lets*PTRSZ);
      jit_inc_stack(PTRSZ);
      jit_ret();
      jit_label(label_fe);
      jit_lea(R0,lambda);
      
#if CPU_ARM||__AMIGA
      fn_lbl = find_label(label_fn);
      //printf("fn_lbl idx: %d code: %p\r\n",fn_lbl->idx,code);
      lambda->dr.next = code + fn_lbl->idx;
      //printf("fn_lbl next: %p\r\n",lambda->dr.next);
#endif
      
      break;
    }
    case BUILTIN_IF: {
      int tag;
      char label_skip[64];
      sprintf(label_skip,"else_%d",++label_skip_count);
      
      // load the condition
      load_int(R0, argdefs[0], frame);

      // compare to zero
      jit_cmpi(R0,0);
      jit_je(label_skip);

      tag = compile_expr(argdefs[1].cell, frame, return_type);
      if (!tag) return 0;

      // else?
      if (argdefs[2].cell) {
        char label_end[64];
        sprintf(label_end,"endif_%d",++label_skip_count);
        jit_jmp(label_end);
        
        jit_label(label_skip);
        tag = compile_expr(argdefs[2].cell, frame, return_type);
        if (!tag) return 0;
        
        jit_label(label_end);
      } else {
        jit_label(label_skip);
      }
      
      break;
    }
    case BUILTIN_WHILE: {
      int compiled_type;
      
      char label_loop[64];
      char label_skip[64];
      sprintf(label_loop,"loop_%d",++label_skip_count);
      sprintf(label_skip,"skip_%d",label_skip_count);
      
      jit_label(label_loop);
      
      compiled_type = compile_expr(argdefs[0].cell, frame, TAG_INT);
      if (!compiled_type) return 0;
      
      // load the condition
      if (compiled_type != TAG_INT) {
        jit_ldr(R0);
        jit_cmpi(R0,0);
      } else {
        jit_cmpi(ARGR0,0);
      }
      //load_int(R1,argdefs[0]);
      // compare to zero
      jit_je(label_skip);

      compiled_type = compile_expr(argdefs[1].cell, frame, return_type);
      if (!compiled_type) return 0;

      jit_jmp(label_loop);
      jit_label(label_skip);
      
      break;
    }
    case BUILTIN_DO: {
      Cell* arg;
      args = orig_args;

      if (!car(args)) {
        printf("<empty (do) not allowed>\r\n");
        return 0;
      }
      
      while ((arg = car(args))) {
        int tag;
        if (car(cdr(args))) {
          // discard all returns except for the last one
          tag = compile_expr(arg, frame, TAG_VOID);
        } else {
          tag = compile_expr(arg, frame, return_type);
        }
        
        if (!tag) return 0;
        args = cdr(args);
      }
      break;
    }
    case BUILTIN_LIST: {
      Cell* arg;
      int n = 0, i;
      args = orig_args;
      while ((arg = car(args))) {
        int tag = compile_expr(arg, frame, TAG_ANY);
        if (!tag) return 0;
        jit_push(R0,R0);
        frame->sp++;
        args = cdr(args);
        n++;
      }
      jit_call(alloc_nil, "list:alloc_nil");
      jit_movr(ARGR1,R0);
      for (i=0; i<n; i++) {
        jit_pop(ARGR0,ARGR0);
        frame->sp--;
        jit_call2(alloc_cons, "list:alloc_cons");
        jit_movr(ARGR1,R0);
      }
      break; // FIXME
    }
    case BUILTIN_QUOTE: {
      Cell* arg;
      args = orig_args;

      if (!car(args)) {
        printf("<empty (quote) not allowed>\r\n");
        return 0;
      }
      
      arg = car(args);
      jit_lea(R0,arg);
      break;
    }
    case BUILTIN_CAR: {
      load_cell(R0,argdefs[0], frame);
      
      // type check -------------------
      jit_movr(R1,R0);
      jit_addi(R1,2*PTRSZ);
      jit_ldr(R1);
      jit_lea(R2,consed_type_error);
      jit_cmpi(R1,TAG_CONS);
      jit_movne(R0,R2);
      // ------------------------------
      
      jit_ldr(R0);
      jit_lea(R2,reusable_nil);
      jit_cmpi(R0,0); // check for null cell
      jit_moveq(R0,R2);
      break;
    }
    case BUILTIN_CDR: {
      load_cell(R0,argdefs[0], frame);
      jit_addi(R0,PTRSZ);

      // type check -------------------
      jit_movr(R1,R0);
      jit_addi(R1,PTRSZ); // because already added PTRSZ
      jit_ldr(R1);
      jit_lea(R2,consed_type_error);
      jit_cmpi(R1,TAG_CONS);
      jit_movne(R0,R2);
      // ------------------------------

      jit_ldr(R0);
      jit_lea(R2,reusable_nil);
      jit_cmpi(R0,0); // check for null cell
      jit_moveq(R0,R2);
      break;
    }
    case BUILTIN_CONS: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call2(alloc_cons,"alloc_cons");
      break;
    }
    case BUILTIN_CONCAT: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call2(alloc_concat,"alloc_concat");
      break;
    }
    case BUILTIN_SUBSTR: {
      load_cell(ARGR0,argdefs[0], frame);
      load_int(ARGR1,argdefs[1], frame);
      load_int(ARGR2,argdefs[2], frame);
      jit_call3(alloc_substr,"alloc_substr");
      break;
    }
    case BUILTIN_GET: {
      char label_skip[64];
      char label_ok[64];
      sprintf(label_skip,"skip_%d",++label_skip_count);
      sprintf(label_ok,"ok_%d",label_skip_count);
      
      load_cell(R1,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2

      // init r3
      jit_movi(R3, 0);

      // todo: compile-time checking would be much more awesome
      // type check
      jit_addi(R1,2*PTRSZ);
      jit_ldr(R1);
      jit_cmpi(R1,TAG_BYTES); // todo: better perf with mask?
      jit_je(label_ok);
      jit_cmpi(R1,TAG_STR);
      jit_je(label_ok);

      // wrong type
      jit_jmp(label_skip);

      // good type
      jit_label(label_ok);
      load_cell(R0,argdefs[0], frame);

#ifdef CHECK_BOUNDS
      // bounds check -----
      jit_movr(R1,R0);
      jit_addi(R1,PTRSZ);
      jit_ldr(R1);
      jit_cmpr(R2,R1);
      jit_jge(label_skip);
      // -------------------
#endif
      
      jit_movr(R1,R0);
      jit_ldr(R1); // string address
      jit_addr(R1,R2);
      jit_ldrb(R1); // data in r3

      jit_label(label_skip);
      
      jit_movr(ARGR0, R3);
      
      if (return_type == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else compiled_type = TAG_INT;
      break;
    }
    case BUILTIN_PUT: {
      char label_skip[64];
      sprintf(label_skip,"skip_%d",++label_skip_count);
      
      load_int(R3,argdefs[2], frame); // byte to store -> R3
      load_int(R2,argdefs[1], frame); // offset -> R2
      load_cell(R0,argdefs[0], frame);

#ifdef CHECK_BOUNDS
      // bounds check -----
      jit_movr(R1,R0);
      jit_addi(R1,PTRSZ);
      jit_ldr(R1);
      jit_cmpr(R2,R1);
      jit_jge(label_skip);
      // -------------------
#endif

      jit_movr(R1,R0);
      jit_ldr(R1); // string address
      jit_addr(R1,R2);
      jit_strb(R1); // address is in r1, data in r3

      jit_label(label_skip);
      
      break;
    }
    case BUILTIN_GET32: {
      load_cell(R3,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      jit_ldr(R3); // string address
      jit_addr(R3,R2);
      jit_ldrw(R3); // load to r3
      
      jit_movr(ARGR0, R3); // FIXME
      jit_call(alloc_int,"alloc_int");
      
      break;
    }
    case BUILTIN_PUT32: {
      char label_skip[64];
      sprintf(label_skip,"skip_%d",++label_skip_count);
    
      load_int(R3,argdefs[2], frame); // word to store -> R3
      load_int(R2,argdefs[1], frame); // offset -> R2
      load_cell(R1,argdefs[0], frame);

#ifdef CHECK_BOUNDS
      // bounds check -----
      jit_movr(R1,R0);
      jit_addi(R1,PTRSZ);
      jit_ldr(R1);
      jit_cmpr(R2,R1);
      jit_jge(label_skip);
      // -------------------
#endif

      // TODO: 32-bit align
      
      jit_movr(R1,R0);
      jit_ldr(R1); // string address
      jit_addr(R1,R2);
      jit_strw(R1); // store from r3
      
      jit_label(label_skip);
      jit_movr(R0,R1);
      jit_call(alloc_int,"debug");
      
      break;
    }
    case BUILTIN_MMAP: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(fs_mmap,"fs_mmap");
      break;
    }
    case BUILTIN_ALLOC: {
      load_int(ARGR0,argdefs[0], frame);
      jit_call(alloc_num_bytes,"alloc_bytes");
      break;
    }
    case BUILTIN_ALLOC_STR: {
      load_int(ARGR0,argdefs[0], frame);
      jit_call(alloc_num_string,"alloc_string");
      break;
    }
    case BUILTIN_BYTES_TO_STR: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(alloc_string_from_bytes,"alloc_string_to_bytes");
      break;
    }
    case BUILTIN_WRITE: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call2(lisp_write_to_cell,"lisp_write_to_cell");
      break;
    }
    case BUILTIN_READ: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(read_string_cell,"read_string_cell");
      break;
    }
    case BUILTIN_EVAL: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(platform_eval,"platform_eval");
      break;
    }
    case BUILTIN_SIZE: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_addi(ARGR0,PTRSZ); // fetch size -> R0
      jit_ldr(ARGR0);
      if (return_type == TAG_ANY) {
        jit_call(alloc_int, "alloc_int");
      } else if (return_type == TAG_INT) {
        jit_movr(R0,ARGR0);
        compiled_type = TAG_INT;
      }
      
      break;
    }
    case BUILTIN_GC: {
      push_frame_regs(frame->f);
      jit_lea(ARGR0,global_env);
      jit_movi(ARGR1,(jit_word_t)frame->stack_end);
      jit_movr(ARGR2,RSP);
      jit_call3(collect_garbage,"collect_garbage");
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_SYMBOLS: {
      jit_lea(ARGR0,global_env);
      jit_call(list_symbols,"list_symbols");
      break;
    }
    case BUILTIN_DEBUG: {
      //jit_call(platform_debug,"platform_debug");
      break;
    }
    case BUILTIN_PRINT: {
      load_cell(ARGR0,argdefs[0], frame);
      push_frame_regs(frame->f);
      jit_call(lisp_print,"lisp_print");
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_MOUNT: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call2(fs_mount,"fs_mount");
      break;
    }
    case BUILTIN_OPEN: {
      load_cell(ARGR0,argdefs[0], frame);
      push_frame_regs(frame->f);
      jit_call(fs_open,"fs_open");
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_RECV: {
      load_cell(ARGR0,argdefs[0], frame);
      push_frame_regs(frame->f);
      jit_call(stream_read,"stream_read");
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_SEND: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      // FIXME clobbers stuff
      push_frame_regs(frame->f);
      jit_call2(stream_write,"stream_write");
      pop_frame_regs(frame->f);
      break;
    }
    }
  } else {
    // λλλ lambda call λλλ

    int spo_adjust = 0, j;
    
    // save our args

    int pushed = push_frame_regs(frame->f);
    frame->sp+=pushed;

    /*Cell* dbg1 = alloc_string_copy("---- debug stack save ----");
    Cell* dbg2 = alloc_string_copy("---- end debug stack  ----");
    
    push_frame_regs(frame->f);
    
    jit_lea(ARGR0,dbg1);
      jit_call(lisp_print,"debug stack");
    
    for (int i=0; i<pushed; i++) {
      jit_ldr_stack(ARGR0, i*PTRSZ);
      jit_call(lisp_print,"debug stack");
    }
    jit_lea(ARGR0,dbg2);
    jit_call(lisp_print,"debug stack");
    pop_frame_regs(frame->f);*/
    
    /*if (argi>1) {
      jit_push(LBDREG, LBDREG+argi-2);
      frame->sp+=(1+argi-2);
    }*/
    
    for (j=0; j<argi-1; j++) {
      if (j>=ARG_SPILLOVER) {
        // pass arg on stack
          
        load_cell(R0, argdefs[j], frame);
        jit_push(R0,R0);
        spo_adjust++;

        frame->sp++;
      } else {
        // pass arg in reg (LBDREG + slot)
        
        if (argdefs[j].type == ARGT_REG) {
          if (argdefs[j].slot<j) {
            // register already clobbered, load from stack
            printf("-- loading clobbered reg %d from stack to reg %d\n",argdefs[j].slot,LBDREG+j);
            jit_ldr_stack(LBDREG+j, (pushed-1-argdefs[j].slot)*PTRSZ);
          } else {
            // no need to move a reg into itself
            if (argdefs[j].slot!=j) {
              load_cell(LBDREG+j, argdefs[j], frame);
            }
          }
        }
        else {
          load_cell(LBDREG+j, argdefs[j], frame);
        }
      }
    }
    
    jit_lea(R0,op_env);
    jit_ldr(R0); // load cell
    jit_addi(R0,PTRSZ); // &cell->dr.next
    jit_ldr(R0); // cell->dr.next
    jit_callr(R0);
    if (spo_adjust) {
      jit_inc_stack(spo_adjust*PTRSZ);
      frame->sp-=spo_adjust;
    }

    pop_frame_regs(frame->f);
    frame->sp-=pushed;
    
    /*if (argi>1) {
      jit_pop(LBDREG, LBDREG+argi-2);
      frame->sp-=(1+argi-2);
    }*/
  }

#ifdef CPU_X64
  fflush(jit_out);
#endif

  // at this point, registers R1-R6 are filled, execute
  return compiled_type;
}

env_t* get_global_env() {
  return global_env;
}

void init_compiler() {
  Cell** signature = malloc(sizeof(Cell*)*3);

  printf("[compiler] creating global env hash table\r\n");
  global_env = sm_new(1000);

  printf("[compiler] init_allocator\r\n");
  init_allocator();

  reusable_nil = alloc_nil();
  reusable_type_error = alloc_error(ERR_INVALID_PARAM_TYPE);
  consed_type_error = alloc_cons(reusable_type_error,reusable_nil);

  insert_symbol(alloc_sym("nil"), reusable_nil, &global_env);
  insert_symbol(alloc_sym("type_error"), consed_type_error, &global_env);
  
  printf("[compiler] inserting symbols\r\n");

  signature[0]=alloc_int(TAG_SYM); signature[1]=alloc_int(TAG_ANY);
  insert_symbol(alloc_sym("def"), alloc_builtin(BUILTIN_DEF, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("let"), alloc_builtin(BUILTIN_LET, alloc_list(signature, 2)), &global_env);

  signature[0]=alloc_int(TAG_INT); signature[1]=alloc_int(TAG_INT);
  insert_symbol(alloc_sym("+"), alloc_builtin(BUILTIN_ADD, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("-"), alloc_builtin(BUILTIN_SUB, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("*"), alloc_builtin(BUILTIN_MUL, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("/"), alloc_builtin(BUILTIN_DIV, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("%"), alloc_builtin(BUILTIN_MOD, alloc_list(signature, 2)), &global_env);
  
  insert_symbol(alloc_sym("bitand"), alloc_builtin(BUILTIN_BITAND, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("bitor"), alloc_builtin(BUILTIN_BITOR, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("bitxor"), alloc_builtin(BUILTIN_BITOR, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("shl"), alloc_builtin(BUILTIN_SHL, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("shr"), alloc_builtin(BUILTIN_SHR, alloc_list(signature, 2)), &global_env);
  
  printf("[compiler] arithmetic\r\n");
  
  insert_symbol(alloc_sym("lt"), alloc_builtin(BUILTIN_LT, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("gt"), alloc_builtin(BUILTIN_GT, alloc_list(signature, 2)), &global_env);
  
  printf("[compiler] compare\r\n");
  
  signature[0]=alloc_int(TAG_INT); signature[1]=alloc_int(TAG_INT); signature[2]=alloc_int(TAG_LAMBDA); 
  insert_symbol(alloc_sym("if"), alloc_builtin(BUILTIN_IF, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("fn"), alloc_builtin(BUILTIN_FN, NULL), &global_env);
  insert_symbol(alloc_sym("while"), alloc_builtin(BUILTIN_WHILE, NULL), &global_env);
  insert_symbol(alloc_sym("do"), alloc_builtin(BUILTIN_DO, NULL), &global_env);
  
  signature[0]=alloc_int(TAG_ANY);
  insert_symbol(alloc_sym("print"), alloc_builtin(BUILTIN_PRINT, alloc_list(signature, 1)), &global_env);
  
  printf("[compiler] flow\r\n");
  
  signature[0]=alloc_int(TAG_CONS);
  insert_symbol(alloc_sym("car"), alloc_builtin(BUILTIN_CAR, alloc_list(signature, 1)), &global_env);
  insert_symbol(alloc_sym("cdr"), alloc_builtin(BUILTIN_CDR, alloc_list(signature, 1)), &global_env);
  
  signature[0]=alloc_int(TAG_ANY); signature[1]=alloc_int(TAG_ANY);
  insert_symbol(alloc_sym("cons"), alloc_builtin(BUILTIN_CONS, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("list"), alloc_builtin(BUILTIN_LIST, NULL), &global_env);
  insert_symbol(alloc_sym("quote"), alloc_builtin(BUILTIN_QUOTE, NULL), &global_env);
  //insert_symbol(alloc_sym("map"), alloc_builtin(BUILTIN_MAP), &global_env);

  printf("[compiler] lists\r\n");
  
  signature[0]=alloc_int(TAG_STR);
  signature[1]=alloc_int(TAG_STR);
  insert_symbol(alloc_sym("concat"), alloc_builtin(BUILTIN_CONCAT, alloc_list(signature, 2)), &global_env);
  
  signature[0]=alloc_int(TAG_STR);
  signature[1]=alloc_int(TAG_INT);
  signature[2]=alloc_int(TAG_INT);
  insert_symbol(alloc_sym("substr"), alloc_builtin(BUILTIN_SUBSTR, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("put"), alloc_builtin(BUILTIN_PUT, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("get"), alloc_builtin(BUILTIN_GET, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("get32"), alloc_builtin(BUILTIN_GET32, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("put32"), alloc_builtin(BUILTIN_PUT32, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("size"), alloc_builtin(BUILTIN_SIZE, alloc_list(signature, 1)), &global_env);
  
  signature[0]=alloc_int(TAG_INT);
  insert_symbol(alloc_sym("alloc"), alloc_builtin(BUILTIN_ALLOC, alloc_list(signature, 1)), &global_env);
  insert_symbol(alloc_sym("alloc-str"), alloc_builtin(BUILTIN_ALLOC_STR, alloc_list(signature, 1)), &global_env);

  signature[0]=alloc_int(TAG_ANY);
  insert_symbol(alloc_sym("bytes->str"), alloc_builtin(BUILTIN_BYTES_TO_STR, alloc_list(signature, 1)), &global_env);

  printf("[compiler] strings\r\n");
  
  signature[0]=alloc_int(TAG_ANY);
  signature[1]=alloc_int(TAG_STR);
  insert_symbol(alloc_sym("write"), alloc_builtin(BUILTIN_WRITE, alloc_list(signature,2)), &global_env);
  insert_symbol(alloc_sym("eval"), alloc_builtin(BUILTIN_EVAL, alloc_list(signature,1)), &global_env);
  signature[0]=alloc_int(TAG_STR);
  insert_symbol(alloc_sym("read"), alloc_builtin(BUILTIN_READ, alloc_list(signature,1)), &global_env);

  signature[0]=alloc_int(TAG_STR);
  signature[1]=alloc_int(TAG_CONS);
  insert_symbol(alloc_sym("mount"), alloc_builtin(BUILTIN_MOUNT, alloc_list(signature,2)), &global_env);
  insert_symbol(alloc_sym("open"), alloc_builtin(BUILTIN_OPEN, alloc_list(signature,1)), &global_env);
  insert_symbol(alloc_sym("mmap"), alloc_builtin(BUILTIN_MMAP, alloc_list(signature,1)), &global_env);
  
  signature[0]=alloc_int(TAG_STREAM);
  signature[1]=alloc_int(TAG_ANY);
  insert_symbol(alloc_sym("recv"), alloc_builtin(BUILTIN_RECV, alloc_list(signature,1)), &global_env);
  insert_symbol(alloc_sym("send"), alloc_builtin(BUILTIN_SEND, alloc_list(signature,2)), &global_env);

  
  printf("[compiler] write/eval\r\n");
  
  insert_symbol(alloc_sym("gc"), alloc_builtin(BUILTIN_GC, NULL), &global_env);
  insert_symbol(alloc_sym("symbols"), alloc_builtin(BUILTIN_SYMBOLS, NULL), &global_env);

  insert_symbol(alloc_sym("debug"), alloc_builtin(BUILTIN_DEBUG, NULL), &global_env);
  
  printf("sledge knows %u symbols. enter (symbols) to see them.\r\n", sm_get_count(global_env));
}
