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
#ifdef CPU_X86
#define ARG_SPILLOVER 0
#else
#define ARG_SPILLOVER 3 // max 4 args via regs, rest via stack
#endif
#define LBDREG R4       // register base used for passing args to functions

#define DEBUG_ASM_SRC

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
static Cell* prototype_type_error;
static Cell* prototype_nil;
static Cell* prototype_int;
static Cell* prototype_any;
static Cell* prototype_void;
static Cell* prototype_struct;
static Cell* prototype_struct_def;
static Cell* prototype_stream;
static Cell* prototype_string;
static Cell* prototype_symbol;
static Cell* prototype_lambda;
static Cell* prototype_cons;

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

void debug_break(Cell* arg) {
  printf("argr0: %p\r\n",arg);
  exit(0);
}

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
  else if (arg.type == ARGT_ENV) {
    // argument is an environment table entry, load e->cell->ar.value
    jit_lea(dreg, arg.env);
    jit_ldr(dreg);
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_REG) {
    // argument comes from a register
    jit_movr(dreg, arg.slot);
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_REG_INT) {
    if (dreg!=arg.slot) {
      jit_movr(dreg, arg.slot);
    }
  }
  else if (arg.type == ARGT_STACK) {
    //printf("loading int from stack slot %d + sp %d to reg %d\n",arg.slot,f->sp,dreg);
    jit_ldr_stack(dreg, PTRSZ*(f->sp-arg.slot));
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_STACK_INT) {
    //printf("loading int from stack_int sp %d - slot %d to reg %d\n",f->sp,arg.slot,dreg);
    jit_ldr_stack(dreg, PTRSZ*(f->sp-arg.slot));
  }
  else {
    jit_movi(dreg, 0xdeadbeef);
  }
}

void load_cell(int dreg, Arg arg, Frame* f) {
  if (arg.type == ARGT_CONST) {
    // argument is a constant like 123, "foo"
    jit_movi(dreg, (jit_word_t)arg.cell);
  }
  else if (arg.type == ARGT_ENV) {
    jit_lea(dreg, arg.env);
    jit_ldr(dreg);
  }
  else if (arg.type == ARGT_REG) {
    jit_movr(dreg, arg.slot);
  }
  else if (arg.type == ARGT_REG_INT) {
    jit_call(alloc_int, "alloc_int");
    jit_movr(dreg,R0);
  }
  else if (arg.type == ARGT_STACK) {
    //printf("loading cell from stack slot %d + sp %d to reg %d\n",arg.slot,f->sp,dreg);
    jit_ldr_stack(dreg, PTRSZ*(f->sp-arg.slot));
  }
  else if (arg.type == ARGT_STACK_INT) {
    int adjust = 0;
    //printf("loading cell from stack_int sp %d - slot %d + adjust %d = %d to reg %d\n",f->sp,arg.slot,adjust,f->sp-arg.slot+adjust,dreg);
    if (dreg!=ARGR0) {jit_push(ARGR0,ARGR0); adjust++;}
    if (dreg!=R0) {jit_push(R0,R0); adjust++;}
    jit_ldr_stack(ARGR0, PTRSZ*(f->sp-arg.slot+adjust));
    jit_call(alloc_int, "alloc_int");
    jit_movr(dreg,R0);
    if (dreg!=R0) jit_pop(R0,R0);
    if (dreg!=ARGR0) jit_pop(ARGR0,ARGR0);
  }
  else {
    printf("<load_cell unhandled arg.type: %d>\r\n",arg.type);
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
            printf("<analyze_fn error: malformed let!>\r\n");
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

int compatible_type(int given, int required) {
  if (given == required) return 1;
  if ((given == TAG_STR || given == TAG_BYTES) &&
      (required == TAG_STR || required == TAG_BYTES)) return 1;
  return 0;
}

Cell* clean_return(int args_pushed, Frame* frame, Cell* compiled_type) {
  if (args_pushed) {
    jit_inc_stack(args_pushed*PTRSZ);
    frame->sp-=args_pushed;
  }

  return compiled_type;
}

// returns a prototype cell that can be used for type information
Cell* compile_expr(Cell* expr, Frame* frame, Cell* return_type) {
  Cell* compiled_type = prototype_any;
  Arg* fn_frame = frame->f;
  Cell* opsym, *args, *orig_args, *signature_args, *op, *orig_op=NULL;
  env_entry* op_env;
  char* op_name;
  
  int is_let = 0;
  int argi = 0;
  int args_pushed = 0;
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
        return value; // FIXME TODO forbid later type change
      } else {
        printf("<undefined symbol %s>\r\n",(char*)expr->ar.addr);
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
    printf("<error: non-symbol in operator position>\r\n");
    return 0;
  }

  op_name = (char*)opsym->ar.addr;
  op_env = lookup_global_symbol(op_name);

  if (!op_env || !op_env->cell) {
    printf("<error: undefined symbol %s in operator position>\r\n",op_name);
    return 0;
  }
  op = op_env->cell;
  
  //printf("op tag: %d\n",op->tag);
  if (op->tag == TAG_BUILTIN) {
    signature_args = op->dr.next;

    if (op->ar.value == BUILTIN_LET) {
      is_let = 1;
    }
  }
  else if (op->tag == TAG_LAMBDA) {
    signature_args = car((Cell*)(op->ar.addr));
  }
  else if (op->tag == TAG_STRUCT_DEF) {
    signature_args = NULL;
    orig_op = op;
    op_env = lookup_global_symbol("new");
    op = op_env->cell;
  }
  else {
    printf("<error: non-lambda/struct symbol %s in operator position>\r\n",(char*)opsym->ar.addr);
    return 0;
  }

  //printf("[op] %s\n",debug_buf);
  //lisp_write(signature_args, debug_buf, sizeof(debug_buf));
  //printf("[sig] %s\n",debug_buf);

#ifdef DEBUG_ASM_SRC
  char* debug_buf = malloc(256);
  lisp_write(expr, debug_buf, 255);
  jit_comment(debug_buf);
  free(debug_buf);
#endif

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
    snprintf(arg_name,sizeof(arg_name),"a%d",argi+1);
    // 1. is the arg the required type? i.e. a pointer or a number?

    if (signature_arg && signature_arg->tag == TAG_CONS) {
      // named argument
      snprintf(arg_name,sizeof(arg_name),"%s",(char*)(car(signature_arg)->ar.addr));
      
      //printf("named arg: %s\r\n",arg_name);
      signature_arg = cdr(signature_arg);
    }

    /*if (signature_args) {
      char dbg[256];
      lisp_write(signature_args,dbg,256);
      printf("!! %s sig: %s\r\n",opsym->ar.addr,dbg);
    }*/

    if (arg && (!signature_args || signature_arg)) {
      int given_tag = arg->tag;
      int sig_tag = 0;
      if (signature_arg) sig_tag = signature_arg->tag;
      
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
          sig_tag = TAG_INT;
          signature_arg = prototype_int;
        } else {
          //printf("ANY mode of let\r\n");
          // but cells are ok, too
          sig_tag = TAG_ANY;
          signature_arg = prototype_any;
        }
      }

      if (!signature_args) {
        // any number of arguments allowed
        argdefs[argi].cell = arg;
        argdefs[argi].type = ARGT_STACK;
      }
      else if (sig_tag == TAG_LAMBDA) {
        // lazy evaluation by form
        argdefs[argi].cell = arg;
        argdefs[argi].type = ARGT_LAMBDA;
      }
      else if (arg->tag == TAG_CONS) {
        // eager evaluation
        // nested expression
        Cell* cons_type = compile_expr(arg, frame, signature_arg);
        if (!cons_type) return NULL; // failure
        given_tag = cons_type->tag;
        
        argdefs[argi].cell = NULL; // cell is in R0 at runtime
        argdefs[argi].slot = ++frame->sp; // record sp at this point

        if (given_tag == TAG_INT) {
          argdefs[argi].type = ARGT_STACK_INT;
        } else {
          argdefs[argi].type = ARGT_STACK;
        }

        if (given_tag == TAG_STRUCT) {
          // struct extraction
          Cell** fields = cons_type->ar.addr;
          Cell** def_fields = fields[0]->ar.addr;
          argdefs[argi].type_name = def_fields[0]->ar.addr;
          //printf("!!! nested struct name extracted: %s (arg# %d argt %d) !!!\r\n",argdefs[argi].type_name,argi,argdefs[argi].type);
        }
        
        jit_push(R0,R0);
        args_pushed++;
        
      }
      else if (given_tag == TAG_SYM && sig_tag != TAG_SYM) {
        // symbol given, lookup (indirect)
        //printf("indirect symbol lookup (name: %p)\n",arg->ar.value);

        int arg_frame_idx = get_sym_frame_idx(arg->ar.addr, fn_frame, 0);

        // argument passed to function in register
        if (arg_frame_idx>=0) {
          argdefs[argi] = fn_frame[arg_frame_idx];

          //printf("argument %s from stack frame.\n", arg->ar.addr);
          //printf("-> cell %p slot %d type %d\n", fn_frame[arg_frame_idx].cell, fn_frame[arg_frame_idx].slot, fn_frame[arg_frame_idx].type);
        } else {
          argdefs[argi].env = lookup_global_symbol((char*)arg->ar.addr);
          argdefs[argi].type = ARGT_ENV;
          
          //printf("argument %i:%s from environment.\n", argi, arg->ar.addr);
        }
        //printf("arg_frame_idx: %d\n",arg_frame_idx);

        if (!argdefs[argi].env && arg_frame_idx<0) {
          printf("<undefined symbol %s given for argument %s of %s>\r\n",(char*)arg->ar.addr,arg_name,op_name);
          return 0;
        }
      }
      else if (compatible_type(given_tag, sig_tag) || sig_tag==TAG_ANY) {
        argdefs[argi].cell = arg;
        argdefs[argi].slot = argi-1;
        argdefs[argi].type = ARGT_CONST;

        if (given_tag == TAG_SYM || given_tag == TAG_CONS || given_tag == TAG_INT || given_tag == TAG_STR || given_tag == TAG_BYTES) {
          //argdefs[argi].type = ARGT_CONST;
        }
        //printf("const arg of type %d at %p\n",arg->tag,arg);
      } else {
        // check if we can typecast
        // else, fail with type error

        printf("<type mismatch for argument %s of %s (given %s, expected %s)>\r\n",arg_name,op_name,tag_to_str(given_tag),tag_to_str(sig_tag));
        return 0;
      }
    } else {
      if (!arg && signature_arg) {
        // missing arguments
        printf("<argument %s of %s missing!>\r\n",arg_name,op_name);
        return 0;
      } else if (arg && !signature_arg) {
        // surplus arguments
        printf("<surplus arguments to %s!>\r\n",op_name);
        return 0;
      }
    }
    
    argi++;
  } while (argi<MAXARGS && (args = cdr(args)) && (!signature_args || (signature_args = cdr(signature_args))));

  // args are prepared, execute op

  if (op->tag == TAG_BUILTIN) {
    switch (op->ar.value) {
    case BUILTIN_BITAND: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_andr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_BITNOT: {
      load_int(ARGR0,argdefs[0], frame);
      jit_notr(ARGR0);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_BITOR: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_orr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_BITXOR: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_xorr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_SHL: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_shlr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_SHR: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_shrr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_ADD: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_addr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_SUB: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_subr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_MUL: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_mulr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_DIV: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_divr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_MOD: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_modr(ARGR0,R2);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_GT: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_movi(R3,0);
      jit_subr(ARGR0,R2);
      jit_movneg(ARGR0,R3);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_LT: {
      load_int(R2,argdefs[0], frame);
      load_int(ARGR0,argdefs[1], frame);
      jit_movi(R3,0);
      jit_subr(ARGR0,R2);
      jit_movneg(ARGR0,R3);
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }
    case BUILTIN_EQ: {
      load_int(R1, argdefs[0], frame);
      load_int(R2, argdefs[1], frame);
      jit_movi(R0,0);
      jit_movi(R3,1);
      jit_cmpr(R1,R2);
      jit_moveq(R0,R3);
      if (return_type->tag == TAG_ANY) {
        jit_movr(ARGR0,R0);
        jit_call(alloc_int, "alloc_int");
      }
      else {
        compiled_type = prototype_int;
        // int is in R0 already
      }
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
      is_reg = 0;
      offset = MAXARGS + frame->locals;
      fidx = get_sym_frame_idx(argdefs[0].cell->ar.addr, fn_frame, 0);

      if (fidx >= 0) {
        // existing stack entry
        offset = fidx;
        //printf("+~ frame entry %s, existing stack-local idx %d (type %d)\n",fn_frame[offset].name,fn_frame[offset].slot,fn_frame[offset].type);

        // is_int from existing entry
        if (fn_frame[offset].type == ARGT_REG_INT ||
            fn_frame[offset].type == ARGT_STACK_INT) {
          is_int = 1;
        }
        
        if (fn_frame[offset].type == ARGT_REG_INT ||
            fn_frame[offset].type == ARGT_REG) {
          is_reg = 1;
        } 
        
      } else {
        if ((argdefs[1].type == ARGT_REG_INT ||
           argdefs[1].type == ARGT_STACK_INT ||
           (argdefs[1].type == ARGT_CONST && argdefs[1].cell->tag == TAG_INT)
         )) {
          is_int = 1;
        }

        // create new stack entry for this let
        fn_frame[offset].name = argdefs[0].cell->ar.addr;
        fn_frame[offset].type_name = argdefs[1].type_name; // copy inferred type
        fn_frame[offset].cell = NULL;
        if (is_int) {
          fn_frame[offset].type = ARGT_STACK_INT;
          //printf("new let %s inferred INT\n",argdefs[0].cell->ar.addr);
        } else {
          fn_frame[offset].type = ARGT_STACK;
          //printf("new let %s inferred ANY\n",argdefs[0].cell->ar.addr);
        }
        fn_frame[offset].slot = -frame->locals;

#ifdef DEBUG_ASM_SRC
        debug_buf = malloc(256);
        snprintf(debug_buf,255,"++ frame entry %s, new stack-local idx %d, is_int %d\n",fn_frame[offset].name,fn_frame[offset].slot,is_int);
        jit_comment(debug_buf);
        free(debug_buf);
#endif
        
        frame->locals++;
        if (frame->locals+MAXARGS>=MAXFRAME) {
          printf("<error: too many locals (maximum %d)>\r\n",MAXFRAME-MAXARGS);
        }
      }
      
      if (is_int) {
        jit_comment("(let) load int");
        load_int(R0, argdefs[1], frame);
        compiled_type = prototype_int;
      } else {
        jit_comment("(let) load cell");
        load_cell(R0, argdefs[1], frame);
        compiled_type = prototype_any;
      }

      if (!is_reg) {
        jit_comment("(let) store to stack");
        jit_str_stack(R0,PTRSZ*(frame->sp-fn_frame[offset].slot));
      }
      
      if (is_reg) {
#ifdef DEBUG_ASM_SRC
        debug_buf = malloc(256);
        snprintf(debug_buf,255,"(let) store %s to reg %d",fn_frame[offset].name,fn_frame[offset].slot);
        jit_comment(debug_buf);
        free(debug_buf);
#endif
        jit_movr(fn_frame[offset].slot, R0);
      }
      
      if (compiled_type->tag == TAG_INT && return_type->tag == TAG_ANY) {
        jit_comment("(let) box int");
        jit_movr(ARGR0,R0);
        jit_call(alloc_int, "alloc_int");
        compiled_type = prototype_any;
      } else {
      }
      break;
    }
    case BUILTIN_FN: {
      Cell* fn_body, *fn_args, *lambda;
      Arg fn_new_frame[MAXFRAME];
      int num_lets, i, j, spo_count, fn_argc;
      Cell* compiled_type;
      char label_fn[64];
      char label_fe[64];
      
      Frame* nframe_ptr;
      Frame nframe = {fn_new_frame, 0, 0, frame->stack_end};
#if CPU_ARM||__AMIGA||CPU_X86
      Label* fn_lbl;
#endif
      
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
        Cell* arg;
        Cell* arg_prototype = prototype_any;
        fn_new_frame[j].type = 0;
        fn_new_frame[j].name = NULL;
        fn_new_frame[j].type_name = NULL;
        
        if (j>=ARG_SPILLOVER) { // max args passed in registers
          fn_new_frame[j].type = ARGT_STACK;
          fn_new_frame[j].slot = -num_lets - (j - ARG_SPILLOVER) - 2;
          spo_count++;
        }
        else {
          fn_new_frame[j].type = ARGT_REG;
          fn_new_frame[j].slot = j + LBDREG;
        }
        
        if (argdefs[j].cell->tag == TAG_SYM) {
          fn_new_frame[j].name = argdefs[j].cell->ar.addr;
        } else if (argdefs[j].cell->tag == TAG_CONS) {
          env_entry* type_env = NULL;
          Cell* type_cell = car(cdr(argdefs[j].cell));
          
          if (!type_cell) {
            printf("<missing struct-name in (arg-name struct-name) declaration>\r\n");
            return 0;
          }
          if (type_cell->tag!=TAG_SYM) {
            printf("<non-symbol struct-name in (arg-name struct-name) declaration>\r\n");
            return 0;
          }
          fn_new_frame[j].name = car(argdefs[j].cell)->ar.addr;
          fn_new_frame[j].type_name = type_cell->ar.addr;

          type_env = lookup_global_symbol(fn_new_frame[j].type_name);
          if (!type_env || !type_env->cell) {
            printf("<undefined struct-name %s in (arg-name struct-name) declaration>\r\n",fn_new_frame[j].type_name);
            return 0;
          }
          if (type_env->cell->tag!=TAG_STRUCT_DEF) {
            printf("<struct-name %s in (arg-name struct-name) declaration does not resolve to a struct definition>\r\n",fn_new_frame[j].type_name);
            return 0;
          }
          
          // TODO other types!
          arg_prototype = type_env->cell;
        } else {
          // illegal type
          printf("<error: only symbols or (symbol typename) allowed in fn signature>\r\n");
          return 0;
        }
        
        arg = alloc_cons(alloc_sym(fn_new_frame[j].name),arg_prototype);
        fn_args = alloc_cons(arg,fn_args);
        fn_argc++;

        //printf("arg j %d: %s\r\n",j,fn_new_frame[j].name);
      }
      //char sig_debug[128];
      //lisp_write(fn_args, sig_debug, sizeof(sig_debug));
      //printf("signature: %s\n",sig_debug);


      //lisp_write(fn_body, sig_debug, sizeof(sig_debug));
      
      lambda = alloc_lambda(alloc_cons(fn_args,fn_body));
      lambda->dr.next = 0;

      sprintf(label_fn,"L0_%p",lambda);
      sprintf(label_fe,"L1_%p",lambda);
      
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

        //printf("frame copied: %p args: %p\r\n",nframe_ptr,nframe_ptr->f);
      } else {
        nframe_ptr = &nframe;
      }

      //nframe_ptr->parent_frame = frame;

      // TODO here we can introduce function return types
      compiled_type = compile_expr(fn_body, nframe_ptr, prototype_any); // new frame, fresh sp
      if (!compiled_type) return 0;

      //printf(">> fn has %d args and %d locals. predicted locals: %d\r\n",fn_argc,nframe.locals,num_lets);
      
      jit_inc_stack(num_lets*PTRSZ);
      jit_inc_stack(PTRSZ);
      jit_ret();
      jit_label(label_fe);
      jit_lea(R0,lambda);
      
#if CPU_ARM||__AMIGA||CPU_X86
      fn_lbl = find_label(label_fn);
      //printf("fn_lbl idx: %d code: %p\r\n",fn_lbl->idx,code);
      lambda->dr.next = code + fn_lbl->idx;
      //printf("fn_lbl next: %p\r\n",lambda->dr.next);
#endif
      
      break;
    }
    case BUILTIN_IF: {
      Cell* then_type=NULL;
      Cell* else_type=NULL;
      char label_skip[64];
      sprintf(label_skip,"Lelse_%d",++label_skip_count);
      
      // load the condition
      load_int(R0, argdefs[0], frame);

      // compare to zero
      jit_cmpi(R0,0);
      jit_je(label_skip);

      // then
      then_type = compile_expr(argdefs[1].cell, frame, return_type);
      if (!then_type) return 0;

      // else
      if (argdefs[2].cell) {
        char label_end[64];
        sprintf(label_end,"Lendif_%d",++label_skip_count);
        jit_jmp(label_end);
        
        jit_label(label_skip);
        else_type = compile_expr(argdefs[2].cell, frame, return_type);
        if (!else_type) return 0;
        
        jit_label(label_end);
      } else {
        jit_label(label_skip);
      }

      if (return_type->tag!=TAG_VOID && then_type && else_type && then_type->tag!=else_type->tag) {
        printf("<incompatible then/else types of if: %s/%s, return type: %s>\r\n",tag_to_str(then_type->tag),tag_to_str(else_type->tag),tag_to_str(return_type->tag));
        return 0;
      }
      
      break;
    }
    case BUILTIN_WHILE: {
      char label_loop[64];
      char label_skip[64];
      char label_skip2[64];
      sprintf(label_loop, "Lloop_%d",++label_skip_count);
      sprintf(label_skip, "Lskip_%d",label_skip_count);
      sprintf(label_skip2,"Lskip2_%d",label_skip_count);
      
      jit_label(label_loop);
      
      compiled_type = compile_expr(argdefs[0].cell, frame, prototype_int);
      if (!compiled_type) return 0;

      // load the condition
      if (compiled_type->tag != TAG_INT) {
        jit_ldr(R0);
      }

      // compare to zero
      jit_cmpi(R0,0);
      jit_je(label_skip);

      // while body
      compiled_type = compile_expr(argdefs[1].cell, frame, return_type);
      if (!compiled_type) return 0;

      jit_jmp(label_loop);
      jit_label(label_skip);

      if (return_type->tag == TAG_ANY) {
        // if the while never executed, we have to create a zero int cell
        // from r0
        jit_cmpi(R0,0);
        jit_jne(label_skip2);
        jit_call(alloc_int,"alloc_int");
        jit_label(label_skip2);
      }
      
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
        Cell* compiled_type;
        if (car(cdr(args))) {
          // discard all returns except for the last one
          compiled_type = compile_expr(arg, frame, prototype_void);
        } else {
          compiled_type = compile_expr(arg, frame, return_type);
        }
        
        if (!compiled_type) return 0;
        args = cdr(args);
      }
      break;
    }
    case BUILTIN_LIST: {
      Cell* arg;
      int n = 0, i;
      args = orig_args;
      while ((arg = car(args))) {
        Cell* compiled_type = compile_expr(arg, frame, prototype_any);
        if (!compiled_type) return 0;
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
    case BUILTIN_STRUCT: {
      Cell* key;
      Cell* arg;
      Cell* name_sym;
      int n = 0, i;
      args = cdr(orig_args);
      name_sym = car(orig_args);

      // struct knows its own name
      jit_lea(R0,name_sym);
      jit_push(R0,R0);
      
      while ((key = car(args))) {
        if (key->tag != TAG_SYM) {
          printf("<every second argument of struct following the struct'sname has to be a symbol>\r\n");
          return 0;
        }
        jit_lea(R0,key);
        jit_push(R0,R0);

        args = cdr(args);
        arg = car(args);
        if (!arg) return 0;
        
        args = cdr(args);
        
        Cell* compiled_type = compile_expr(arg, frame, prototype_any);
        if (!compiled_type) return 0;
        
        jit_push(R0,R0);
        frame->sp+=2;
        n+=2;
      }
      n++; // account for name
      
      jit_movi(ARGR0,n);
      jit_call(alloc_struct_def, "struct:alloc_struct_def");
      jit_movr(R1,R0);
      jit_ldr(R1); // load addr of cell array
      jit_addi(R1,n*PTRSZ);
      for (i=0; i<n; i++) {
        jit_addi(R1,-PTRSZ);
        jit_pop(R3,R3);
        frame->sp--;
        jit_stra(R1); // strw from r3
      }

      // load the struct name
      jit_lea(ARGR0,name_sym);
      jit_movr(ARGR1,R0);
      push_frame_regs(frame->f);
      jit_call2(insert_global_symbol, "insert_global_symbol");
      pop_frame_regs(frame->f);
      
      break;
    }
    case BUILTIN_NEW: {
      Cell* arg;
      if (orig_op) {
        // (struct-def …)
        arg = orig_op;
      } else {
        // (new struct-def …)
        arg = argdefs[0].env->cell;
      }
      //printf("[new] arg: %p\r\n",arg);
      //printf("[new] struct size %d\r\n",arg->dr.size/2);

      // arg points to struct definition which is TAG_VEC
      if (arg->tag != TAG_STRUCT_DEF) {
        printf("<(new) requires a struct definition>\r\n");
        return 0;
      }

      jit_lea(ARGR0,arg);
      jit_call(alloc_struct,"new:alloc_struct");

      compiled_type = alloc_struct(arg); // prototype
      
      break;
    }
    case BUILTIN_SGET: {
      Cell* struct_def;
      char* lookup_name = argdefs[1].cell->ar.addr;
      Cell** struct_elements;
      int num_fields;
      int found=0;
      if (argdefs[0].type == ARGT_ENV) {
        struct_def = argdefs[0].env->cell;
        struct_def = car(car(struct_def));
      }
      else if (argdefs[0].type_name) {
        //printf("[sget] arg type name %s\r\n",argdefs[0].type_name);
        env_entry* type_env = lookup_global_symbol(argdefs[0].type_name);
        
        struct_def = type_env->cell;
        //printf("[sget] struct_def %p\r\n",struct_def);
      }
      else {
        printf("<untyped value passed to sget (field %s)>\r\n",lookup_name);
        return 0;
      }

      // arg points to struct definition which is TAG_VEC
      if (struct_def->tag != TAG_STRUCT_DEF) {
        printf("<(sget) requires a struct>\r\n");
        return 0;
      }
      num_fields = struct_def->dr.size/2;
      struct_elements = (Cell**)(struct_def->ar.addr);
      
      //printf("[sget] struct_elements %p\r\n",struct_elements);
      //printf("[sget] lookup %s\r\n",lookup_name);

      for (int i=0; i<num_fields; i++) {
        if (!strcmp(lookup_name,(char*)struct_elements[1+i*2]->ar.addr)) {
          //printf("field found at index %d\r\n",i);
          load_cell(R0,argdefs[0],frame);
          jit_ldr(R0);
          jit_addi(R0,(i+1)*PTRSZ);
          jit_ldr(R0);
          found = 1;

          // extract and return the field type (prototype)
          compiled_type = struct_elements[1+i*2+1];
          if (compiled_type->tag != TAG_STRUCT) {
            compiled_type = prototype_any; // FIXME
          }
          
          break;
        }
      }

      if (!found) {
        printf("<sget field %s not found!>\r\n",lookup_name);
        jit_movi(R0,0);
        return 0;
      }
      
      break;
    }
    case BUILTIN_SPUT: {
      Cell* struct_def;
      char* lookup_name = argdefs[1].cell->ar.addr;
      Cell** struct_elements;
      int num_fields;
      int found=0;
      if (argdefs[0].type == ARGT_ENV) {
        struct_def = argdefs[0].env->cell;
        struct_def = car(car(struct_def));
      } else if (argdefs[0].type_name) {
        env_entry* type_env = lookup_global_symbol(argdefs[0].type_name);
        if (type_env) {
          struct_def = type_env->cell;
        } else {
          printf("<sput: struct type %s not found.>\r\n",argdefs[0].type_name);
          return 0;
        }
      } else {
        printf("<indirect struct field access not yet implemented.>");
        return 0;
      }

      // arg points to struct definition which is TAG_VEC
      if (struct_def->tag != TAG_STRUCT_DEF) {
        printf("<(sput) requires a struct>\r\n");
        return 0;
      }
      num_fields = struct_def->dr.size/2;
      struct_elements = (Cell**)(struct_def->ar.addr);
      //printf("[sput] lookup %s\r\n",lookup_name);

      for (int i=0; i<num_fields; i++) {
        if (!strcmp(lookup_name,(char*)struct_elements[1+i*2]->ar.addr)) {
          //printf("[sput] field found at index %d\r\n",i);
          load_cell(R2,argdefs[0],frame);
          jit_movr(R0,R2);
          jit_ldr(R2);
          jit_addi(R2,(i+1)*PTRSZ);
          load_cell(R3,argdefs[2],frame); // TODO type check!
          jit_stra(R2);
          found = 1;
          break;
        }
      }

      if (!found) {
        printf("<sput field %s not found!>\r\n",lookup_name);
        jit_movi(R0,0);
        return 0;
      }
      
      break;
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
      jit_lea(R2,prototype_nil);
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
      jit_lea(R2,prototype_nil);
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
    case BUILTIN_GET8: {
      char label_skip[64];
      char label_ok[64];
      sprintf(label_skip,"Lskip_%d",++label_skip_count);
      sprintf(label_ok,"Lok_%d",label_skip_count);
      
      load_cell(R1,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      jit_movr(R0,R1); // save original cell in r0

      // todo: compile-time checking would be much more awesome
      // type check
      jit_addi(R1,2*PTRSZ);
      jit_ldr(R1);
      jit_cmpi(R1,TAG_BYTES); // todo: better perf with mask?
      jit_je(label_ok);
      jit_cmpi(R1,TAG_STR);
      jit_je(label_ok);

      // wrong type
      jit_movi(R3, 0);
      jit_jmp(label_skip);

      // good type
      jit_label(label_ok);
      jit_movr(R1,R0); // get original cell from r3

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
      jit_movi(R3, 0);
      jit_ldrb(R1); // data in r3

      jit_label(label_skip);
      
      jit_movr(ARGR0, R3);
      
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }

    case BUILTIN_GET16: {
      char label_skip[64];
      char label_ok[64];
      sprintf(label_skip,"Lskip_%d",++label_skip_count);
      sprintf(label_ok,"Lok_%d",label_skip_count);
      
      load_cell(R1,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      jit_movr(R0,R1); // save original cell in r0

      // todo: compile-time checking would be much more awesome
      // type check
      jit_addi(R1,2*PTRSZ);
      jit_ldr(R1);
      jit_cmpi(R1,TAG_BYTES); // todo: better perf with mask?
      jit_je(label_ok);
      jit_cmpi(R1,TAG_STR);
      jit_je(label_ok);

      // wrong type
      jit_movi(R3, 0);
      jit_jmp(label_skip);

      // good type
      jit_label(label_ok);
      jit_movr(R1,R0); // get original cell from r3

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
      jit_movi(R3, 0);
      jit_ldrs(R1); // data in r3

      jit_label(label_skip);
      
      jit_movr(ARGR0, R3);
      
      if (return_type->tag == TAG_ANY) jit_call(alloc_int, "alloc_int");
      else {
        compiled_type = prototype_int;
        jit_movr(R0,ARGR0);
      }
      break;
    }

    case BUILTIN_PUT8: {
      char label_skip[64];
      sprintf(label_skip,"Lskip_%d",++label_skip_count);
      
      load_cell(R0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      load_int(R3,argdefs[2], frame); // byte to store -> R3

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
    case BUILTIN_PUT16: {
      char label_skip[64];
      sprintf(label_skip,"Lskip_%d",++label_skip_count);
      
      load_cell(R0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      load_int(R3,argdefs[2], frame); // byte to store -> R3

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
      jit_strs(R1); // address is in r1, data in r3

      jit_label(label_skip);
      
      break;
    }
    case BUILTIN_GET32: {
      load_int(R2,argdefs[1], frame); // offset -> R2
      load_cell(R3,argdefs[0], frame);
      jit_ldr(R3); // string address
      jit_movi(R1,2); // offset * 4
      jit_shlr(R2,R1);
      jit_addr(R3,R2);
      jit_ldrw(R3); // load to r3
      
      jit_movr(ARGR0, R3); // FIXME
      jit_call(alloc_int,"alloc_int");
      
      break;
    }
    case BUILTIN_PUT32: {
      char label_skip[64];
      sprintf(label_skip,"Lskip_%d",++label_skip_count);
    
      load_cell(R1,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      load_int(R3,argdefs[2], frame); // word to store -> R3

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
      jit_host_call_enter();
      jit_call2(lisp_write_to_cell,"lisp_write_to_cell");
      jit_host_call_exit();
      break;
    }
    case BUILTIN_READ: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_host_call_enter();
      jit_call(read_string_cell,"read_string_cell");
      jit_host_call_exit();
      break;
    }
    case BUILTIN_EVAL: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_host_call_enter();
      jit_call(platform_eval,"platform_eval");
      jit_host_call_exit();
      break;
    }
    case BUILTIN_SIZE: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_addi(ARGR0,PTRSZ); // fetch size -> R0
      jit_ldr(ARGR0);
      if (return_type->tag == TAG_ANY) {
        jit_call(alloc_int, "alloc_int");
      } else if (return_type->tag == TAG_INT) {
        jit_movr(R0,ARGR0);
        compiled_type = prototype_int;
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
      jit_host_call_enter();
      jit_call(lisp_print,"lisp_print");
      jit_host_call_exit();
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_MOUNT: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_host_call_enter();
      jit_call2(fs_mount,"fs_mount");
      jit_host_call_exit();
      break;
    }
    case BUILTIN_MMAP: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_host_call_enter();
      jit_call(fs_mmap,"fs_mmap");
      jit_host_call_exit();
      break;
    }
    case BUILTIN_OPEN: {
      load_cell(ARGR0,argdefs[0], frame);
      push_frame_regs(frame->f);
      jit_host_call_enter();
      jit_call(fs_open,"fs_open");
      jit_host_call_exit();
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_RECV: {
      load_cell(ARGR0,argdefs[0], frame);
      push_frame_regs(frame->f);
      jit_host_call_enter();
      jit_call(stream_read,"stream_read");
      jit_host_call_exit();
      pop_frame_regs(frame->f);
      break;
    }
    case BUILTIN_SEND: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      push_frame_regs(frame->f);
      jit_host_call_enter();
      jit_call2(stream_write,"stream_write");
      jit_host_call_exit();
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
    
    for (j=argi-2; j>=0; j--) {
      if (j>=ARG_SPILLOVER) {
        // pass arg on stack

        load_cell(R0, argdefs[j], frame);
        jit_push(R0,R0);
        spo_adjust++;
        frame->sp++;
      } else {
        // pass arg in reg (LBDREG + slot)
        
        if (argdefs[j].type == ARGT_REG) {
          // FIXME kludge?
          if (1 || argdefs[j].slot<j+LBDREG) {
            int offset = ((pushed+spo_adjust) - (argdefs[j].slot-LBDREG) - 1);
            // register already clobbered, load from stack
            //printf("-- loading clobbered reg %d from stack offset %d to reg %d\n",argdefs[j].slot,offset,LBDREG+j);
            jit_ldr_stack(LBDREG+j, offset*PTRSZ);
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

    jit_callr(R0); // the call!
    
    if (spo_adjust) {
      jit_inc_stack(spo_adjust*PTRSZ);
      frame->sp-=spo_adjust;
    }

    pop_frame_regs(frame->f);
    frame->sp-=pushed;
  }

#ifdef CPU_X64
  fflush(jit_out);
#endif

  // at this point, registers R1-R6 are filled, execute
  return clean_return(args_pushed, frame, compiled_type);
}

env_t* get_global_env() {
  return global_env;
}

void init_compiler() {
  Cell** signature = malloc(sizeof(Cell*)*3);

  //printf("[compiler] creating global env hash table\r\n");
  global_env = sm_new(1000);

  //printf("[compiler] init_allocator\r\n");
  init_allocator();

  prototype_nil = alloc_nil();
  prototype_type_error = alloc_error(ERR_INVALID_PARAM_TYPE);
  consed_type_error = alloc_cons(prototype_type_error,prototype_nil);
  prototype_any = alloc_int(0);
  prototype_any->tag = TAG_ANY;
  prototype_void = alloc_int(0);
  prototype_void->tag = TAG_VOID;
  prototype_symbol = alloc_sym("symbol");
  prototype_int = alloc_int(0);
  prototype_struct = alloc_int(0);
  prototype_struct->tag = TAG_STRUCT;
  prototype_struct_def = alloc_int(0);
  prototype_struct_def->tag = TAG_STRUCT_DEF;
  prototype_stream = alloc_int(0);
  prototype_stream->tag = TAG_STREAM;
  prototype_string = alloc_string_copy("string");
  prototype_lambda = alloc_int(0);
  prototype_lambda->tag = TAG_LAMBDA;
  prototype_cons = alloc_cons(alloc_nil(),alloc_nil());

  insert_symbol(alloc_sym("nil"), prototype_nil, &global_env);
  insert_symbol(alloc_sym("type_error"), consed_type_error, &global_env);
  
  //printf("[compiler] inserting symbols\r\n");

  signature[0]=prototype_symbol; signature[1]=prototype_any;
  insert_symbol(alloc_sym("def"), alloc_builtin(BUILTIN_DEF, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("let"), alloc_builtin(BUILTIN_LET, alloc_list(signature, 2)), &global_env);
  
  signature[0]=prototype_struct_def; signature[1]=prototype_symbol; signature[2]=prototype_any;
  insert_symbol(alloc_sym("new"), alloc_builtin(BUILTIN_NEW, alloc_list(signature, 1)), &global_env);
  signature[0]=prototype_struct;
  insert_symbol(alloc_sym("sget"), alloc_builtin(BUILTIN_SGET, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("sput"), alloc_builtin(BUILTIN_SPUT, alloc_list(signature, 3)), &global_env);

  signature[0]=prototype_int; signature[1]=prototype_int;
  insert_symbol(alloc_sym("+"), alloc_builtin(BUILTIN_ADD, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("-"), alloc_builtin(BUILTIN_SUB, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("*"), alloc_builtin(BUILTIN_MUL, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("/"), alloc_builtin(BUILTIN_DIV, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("%"), alloc_builtin(BUILTIN_MOD, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("bitand"), alloc_builtin(BUILTIN_BITAND, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("bitor"),  alloc_builtin(BUILTIN_BITOR, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("bitnot"), alloc_builtin(BUILTIN_BITNOT, alloc_list(signature, 1)), &global_env);
  insert_symbol(alloc_sym("bitxor"), alloc_builtin(BUILTIN_BITXOR, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("shl"),    alloc_builtin(BUILTIN_SHL, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("shr"),    alloc_builtin(BUILTIN_SHR, alloc_list(signature, 2)), &global_env);
  
  //printf("[compiler] arithmetic\r\n");
  
  insert_symbol(alloc_sym("lt"), alloc_builtin(BUILTIN_LT, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("gt"), alloc_builtin(BUILTIN_GT, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("eq"), alloc_builtin(BUILTIN_EQ, alloc_list(signature, 2)), &global_env);
  
  //printf("[compiler] compare\r\n");
  
  signature[0]=prototype_int; signature[1]=prototype_lambda; signature[2]=prototype_lambda; 
  insert_symbol(alloc_sym("if"), alloc_builtin(BUILTIN_IF, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("fn"), alloc_builtin(BUILTIN_FN, NULL), &global_env);
  insert_symbol(alloc_sym("while"), alloc_builtin(BUILTIN_WHILE, NULL), &global_env);
  insert_symbol(alloc_sym("do"), alloc_builtin(BUILTIN_DO, NULL), &global_env);
  
  signature[0]=prototype_any;
  insert_symbol(alloc_sym("print"), alloc_builtin(BUILTIN_PRINT, alloc_list(signature, 1)), &global_env);
  
  //printf("[compiler] flow\r\n");
  
  signature[0]=prototype_cons;
  insert_symbol(alloc_sym("car"), alloc_builtin(BUILTIN_CAR, alloc_list(signature, 1)), &global_env);
  insert_symbol(alloc_sym("cdr"), alloc_builtin(BUILTIN_CDR, alloc_list(signature, 1)), &global_env);
  
  signature[0]=prototype_any; signature[1]=prototype_any;
  insert_symbol(alloc_sym("cons"), alloc_builtin(BUILTIN_CONS, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("list"), alloc_builtin(BUILTIN_LIST, NULL), &global_env);
  insert_symbol(alloc_sym("quote"), alloc_builtin(BUILTIN_QUOTE, NULL), &global_env);
  insert_symbol(alloc_sym("struct"), alloc_builtin(BUILTIN_STRUCT, NULL), &global_env);
  //insert_symbol(alloc_sym("map"), alloc_builtin(BUILTIN_MAP), &global_env);

  //printf("[compiler] lists\r\n");
  
  signature[0]=prototype_string;
  signature[1]=prototype_string;
  insert_symbol(alloc_sym("concat"), alloc_builtin(BUILTIN_CONCAT, alloc_list(signature, 2)), &global_env);
  
  signature[0]=prototype_string;
  signature[1]=prototype_int;
  signature[2]=prototype_int;
  insert_symbol(alloc_sym("substr"), alloc_builtin(BUILTIN_SUBSTR, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("put8"), alloc_builtin(BUILTIN_PUT8, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("get8"), alloc_builtin(BUILTIN_GET8, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("put16"), alloc_builtin(BUILTIN_PUT16, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("get16"), alloc_builtin(BUILTIN_GET16, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("get32"), alloc_builtin(BUILTIN_GET32, alloc_list(signature, 2)), &global_env);
  insert_symbol(alloc_sym("put32"), alloc_builtin(BUILTIN_PUT32, alloc_list(signature, 3)), &global_env);
  insert_symbol(alloc_sym("size"), alloc_builtin(BUILTIN_SIZE, alloc_list(signature, 1)), &global_env);
  
  signature[0]=prototype_int;
  insert_symbol(alloc_sym("alloc"), alloc_builtin(BUILTIN_ALLOC, alloc_list(signature, 1)), &global_env);
  insert_symbol(alloc_sym("alloc-str"), alloc_builtin(BUILTIN_ALLOC_STR, alloc_list(signature, 1)), &global_env);

  signature[0]=prototype_any;
  insert_symbol(alloc_sym("bytes->str"), alloc_builtin(BUILTIN_BYTES_TO_STR, alloc_list(signature, 1)), &global_env);

  //printf("[compiler] strings\r\n");
  
  signature[0]=prototype_any;
  signature[1]=prototype_string;
  insert_symbol(alloc_sym("write"), alloc_builtin(BUILTIN_WRITE, alloc_list(signature,2)), &global_env);
  insert_symbol(alloc_sym("eval"), alloc_builtin(BUILTIN_EVAL, alloc_list(signature,1)), &global_env);
  signature[0]=prototype_string;
  insert_symbol(alloc_sym("read"), alloc_builtin(BUILTIN_READ, alloc_list(signature,1)), &global_env);

  signature[0]=prototype_string;
  signature[1]=prototype_cons;
  insert_symbol(alloc_sym("mount"), alloc_builtin(BUILTIN_MOUNT, alloc_list(signature,2)), &global_env);
  insert_symbol(alloc_sym("open"), alloc_builtin(BUILTIN_OPEN, alloc_list(signature,1)), &global_env);
  insert_symbol(alloc_sym("mmap"), alloc_builtin(BUILTIN_MMAP, alloc_list(signature,1)), &global_env);
  
  signature[0]=prototype_stream;
  signature[1]=prototype_any;
  insert_symbol(alloc_sym("recv"), alloc_builtin(BUILTIN_RECV, alloc_list(signature,1)), &global_env);
  insert_symbol(alloc_sym("send"), alloc_builtin(BUILTIN_SEND, alloc_list(signature,2)), &global_env);

  
  //printf("[compiler] write/eval\r\n");
  
  insert_symbol(alloc_sym("gc"), alloc_builtin(BUILTIN_GC, NULL), &global_env);
  insert_symbol(alloc_sym("symbols"), alloc_builtin(BUILTIN_SYMBOLS, NULL), &global_env);

  insert_symbol(alloc_sym("debug"), alloc_builtin(BUILTIN_DEBUG, NULL), &global_env);
  
  printf("[compiler] interim knows %u symbols. enter (symbols) to see them.\r\n", sm_get_count(global_env));
}
