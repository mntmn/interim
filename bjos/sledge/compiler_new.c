#include "minilisp.h"
#include "reader.h"
#include "writer.h"
#include "alloc.h"
#include "machine.h"
#include "compiler_new.h"
#include "stream.h"
#include "utf8.c"

#define env_t StrMap
static env_t* global_env = NULL;

env_entry* lookup_global_symbol(char* name) {
  env_entry* res;
  int found = sm_get(global_env, name, (void**)&res);
  //printf("[lookup] %s res: %p\n",name,res);
  if (!found) return NULL;
  return res;
}

Cell* insert_symbol(Cell* symbol, Cell* cell, env_t** env) {
  env_entry* e;
  int found = sm_get(*env, symbol->addr, (void**)&e);
  
  //printf("sm_get res: %d\r\n",found);
  
  if (found) {
    e->cell = cell;
    //printf("[insert_symbol] update %s entry at %p (cell: %p value: %d)\r\n",symbol->addr,e,e->cell,e->cell->value);
    return e->cell;
  }
    
  e = malloc(sizeof(env_entry));
  memcpy(e->name, (char*)symbol->addr, symbol->size);
  e->cell = cell;

  //printf("[insert_symbol] %s entry at %p (cell: %p)\r\n",symbol->addr,e,e->cell);
  sm_put(*env, e->name, e);

  return e->cell;
}

Cell* insert_global_symbol(Cell* symbol, Cell* cell) {
  return insert_symbol(symbol, cell, &global_env);
}

#define LBDREG R4

static FILE* jit_out;
static Cell* cell_heap_start;
static int label_skip_count = 0;
static char temp_print_buffer[1024];
static Cell* consed_type_error;

#ifdef CPU_ARM
#include "jit_arm_raw.c"
#define PTRSZ 4
#endif

#ifdef CPU_X64
#include "jit_x64.c"
#define PTRSZ 8
#endif

typedef enum arg_t {
  ARGT_CONST,
  ARGT_CELL,
  ARGT_ENV,
  ARGT_LAMBDA,
  ARGT_REG,
  ARGT_INT,
  ARGT_STACK
} arg_t;

typedef struct Arg {
  arg_t type;
  Cell* cell;
  env_entry* env;
  int slot;
  char* name;
} Arg;

typedef struct Frame {
  Arg* f;
  int sp;
  int locals;
} Frame;

Cell* lisp_print(Cell* arg) {
  lisp_write(arg, temp_print_buffer, sizeof(temp_print_buffer)-1);
  printf("%s\r\n",temp_print_buffer);
  return arg;
}

void load_int(int dreg, Arg arg, Frame* f) {
  if (arg.type == ARGT_CONST) {
    // argument is a constant like 123, "foo"
    jit_movi(dreg, (jit_word_t)arg.cell->value);
  }
  else if (arg.type == ARGT_CELL) {
    if (arg.cell == NULL) {
      // not sure what this is
      //if (dreg!=R0) jit_movr(dreg, R0);
      jit_movr(dreg, R1+arg.slot); // FIXME: really true?
      jit_ldr(dreg);
    } else {
      // argument is a cell pointer
      jit_lea(dreg, arg.cell);
      jit_ldr(dreg);
    }
  }
  else if (arg.type == ARGT_ENV) {
    // argument is an environment table entry, load e->cell->value
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
    // do nothing
  }
  else if (arg.type == ARGT_STACK) {
    jit_ldr_stack(dreg, PTRSZ*(arg.slot+f->sp));
    jit_ldr(dreg);
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
  else {
    jit_movi(dreg, 0xdeadcafe);
  }
}

#define MAXARGS 6
#define MAXFRAME 16 // maximum 16-6 local vars

int get_sym_frame_idx(char* argname, Arg* fn_frame, int ignore_regs) {
  if (!fn_frame) return -1;
  
  for (int i=0; i<MAXFRAME; i++) {
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
void push_frame_regs(Arg* fn_frame) {
  if (!fn_frame) return;
  
  int pushreg=0;
  int pushstack=0;
  for (int i=0; i<MAXFRAME; i++) {
    if (fn_frame[i].type == ARGT_REG) {
      pushreg++;
    }
  }
  printf("pushing %d frame regs\n",pushreg);
  if (pushreg) {
    jit_push(LBDREG,LBDREG+pushreg-1);
  }
}

void pop_frame_regs(Arg* fn_frame) {
  if (!fn_frame) return;
  
  int pushreg=0;
  int pushstack=0;
  for (int i=0; i<MAXFRAME; i++) {
    if (fn_frame[i].type == ARGT_REG) {
      pushreg++;
    }
  }
  printf("popping %d frame regs\n",pushreg);
  if (pushreg) {
    jit_pop(LBDREG,LBDREG+pushreg-1);
  }
}

// sp = local stack offset

int compile_expr(Cell* expr, Frame* frame, int return_type) {

  int compiled_type = TAG_ANY;
  Arg* fn_frame = frame->f;
  
  if (expr->tag != TAG_CONS) {
    if (expr->tag == TAG_SYM) {
      
      int arg_frame_idx = get_sym_frame_idx(expr->addr, fn_frame, 0);
      if (arg_frame_idx>=0) {
        load_cell(R0, fn_frame[arg_frame_idx], frame);
        return compiled_type;
      }

      env_entry* env = lookup_global_symbol(expr->addr);
      if (env) {
        Cell* value = env->cell;
        jit_movi(R0,(jit_word_t)value);
      } else {
        printf("undefined symbol %s\n",expr->addr);
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
  
  Cell* opsym = car(expr);
  Cell* args = cdr(expr);
  Cell* orig_args = args; // keep around for specials forms like DO
  Cell* signature_args = NULL;

  char debug_buf[256];

  //printf("opsym tag: %d\n",opsym->tag);

  if (!opsym || opsym->tag != TAG_SYM) {
    printf("[compile_expr] error: non-symbol in operator position.\n");
    return 0;
  }

  env_entry* op_env = lookup_global_symbol(opsym->addr);

  if (!op_env || !op_env->cell) {
    printf("[compile_expr] error: undefined symbol %s in operator position.\n",opsym->addr);
    return 0;
  }
  Cell* op = op_env->cell;
  
  //printf("op tag: %d\n",op->tag);
  if (op->tag == TAG_BUILTIN) {
    signature_args = op->next;
  } else if (op->tag == TAG_LAMBDA) {
    signature_args = car((Cell*)(op->addr));
  }
  
  //lisp_write(op, debug_buf, sizeof(debug_buf));
  //printf("[op] %s\n",debug_buf);
  //lisp_write(signature_args, debug_buf, sizeof(debug_buf));
  //printf("[sig] %s\n",debug_buf);
  
  // first, we need a signature

  int argi = 0;
  Arg argdefs[MAXARGS];

  do {
    Cell* arg = car(args);
    Cell* signature_arg = car(signature_args);
    char arg_name[32];
    snprintf(arg_name,sizeof(arg_name),"a%d",argi+1,arg_name,10);
    // 1. is the arg the required type? i.e. a pointer or a number?

    if (signature_arg && signature_arg->tag == TAG_CONS) {
      // named argument
      snprintf(arg_name,sizeof(arg_name),car(signature_arg)->addr);
      signature_arg = cdr(signature_arg);
    }

    if (arg && (!signature_args || signature_arg)) {
      int given_tag = arg->tag;

      if (!signature_args) {
        // any number of arguments allowed
        argdefs[argi].cell = arg;
        argdefs[argi].type = ARGT_CELL;
      }
      else if (signature_arg->value == TAG_LAMBDA) {
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
        given_tag = compile_expr(arg, frame, signature_arg->value);
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
      else if (given_tag == TAG_SYM && signature_arg->value != TAG_SYM) {
        // symbol given, lookup (indirect)
        //printf("indirect symbol lookup (name: %p)\n",arg->value);

        int arg_frame_idx = get_sym_frame_idx(arg->addr, fn_frame, 0);

        // argument passed to function in register
        if (arg_frame_idx>=0) {
          argdefs[argi] = fn_frame[arg_frame_idx];

          printf("argument %s from stack frame.\n", arg->addr);
        } else {
          argdefs[argi].env = lookup_global_symbol((char*)arg->addr);
          argdefs[argi].type = ARGT_ENV;
          
          printf("argument %s from environment.\n", arg->addr);
        }
        //printf("lookup result: %p\n",argptrs[argi]);

        if (!argdefs[argi].env && arg_frame_idx<0) {
          printf("undefined symbol %s given for argument %s!\n",arg->addr,arg_name);
          return 0;
        }
      }
      else if (given_tag == signature_arg->value || signature_arg->value==TAG_ANY) {
        argdefs[argi].cell = arg;
        argdefs[argi].slot = argi-1;
        argdefs[argi].type = ARGT_CELL;

        if (given_tag == TAG_INT || given_tag == TAG_STR || given_tag == TAG_BYTES) {
          argdefs[argi].type = ARGT_CONST;
          //printf("const arg of type %d at %p\n",arg->tag,arg);
        }
      } else {
        // check if we can typecast
        // else, fail with type error

        printf("type mismatch for argument %s (given %d, expected %d)!\n",arg_name,given_tag,signature_arg->value);
        return 0;
      }
    } else {
      if (!arg && signature_arg) {
        // missing arguments
        printf("argument %s missing!\n",arg_name);
        return 0;
      } else if (arg && !signature_arg) {
        // surplus arguments
        printf("surplus arguments!\n");
        return 0;
      }
    }
    
    argi++;
  } while (argi<MAXARGS && (args = cdr(args)) && (!signature_args || (signature_args = cdr(signature_args))));

  if (op->tag == TAG_BUILTIN) {
    switch (op->value) {
    case BUILTIN_ADD: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_addr(ARGR0,R2);
      if (return_type == TAG_INT) return TAG_INT;
      jit_call(alloc_int, "alloc_int");
      break;
    }
    case BUILTIN_SUB: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_subr(ARGR0,R2);
      if (return_type == TAG_INT) return TAG_INT;
      jit_call(alloc_int, "alloc_int");
      break;
    }
    case BUILTIN_MUL: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_mulr(ARGR0,R2);
      if (return_type == TAG_INT) return TAG_INT;
      jit_call(alloc_int, "alloc_int");
      break;
    }
    case BUILTIN_DIV: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_divr(ARGR0,R2);
      if (return_type == TAG_INT) return TAG_INT;
      jit_call(alloc_int, "alloc_int");
      break;
    }
    case BUILTIN_GT: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_subr(R2,ARGR0);
      jit_movi(ARGR0,0);
      jit_movi(R2,1);
      jit_movneg(ARGR0,R2);
      if (return_type == TAG_INT) return TAG_INT;
      jit_call(alloc_int, "alloc_int");
      break;
    }
    case BUILTIN_LT: {
      load_int(ARGR0,argdefs[0], frame);
      load_int(R2,argdefs[1], frame);
      jit_subr(ARGR0,R2);
      jit_movi(ARGR0,0);
      jit_movi(R2,1);
      jit_movneg(ARGR0,R2);
      if (return_type == TAG_INT) return TAG_INT;
      jit_call(alloc_int, "alloc_int");
      break;
    }
    case BUILTIN_DEF: {
      if (argdefs[1].type == ARGT_CONST) {
        jit_lea(ARGR1,argdefs[1].cell); // load cell
      } else if (argdefs[1].type == ARGT_CELL) {
        jit_movr(ARGR1,R0);
      } else if (argdefs[1].type == ARGT_ENV) {
        jit_lea(ARGR1,argdefs[1].env);
        jit_ldr(ARGR1); // load cell
      } else if (argdefs[1].type == ARGT_REG) {
        jit_movr(ARGR1, LBDREG+argdefs[1].slot);
      }
      jit_lea(ARGR0,argdefs[0].cell); // load symbol address
      
      jit_call(insert_global_symbol, "insert_global_symbol");
      break;
    }
    case BUILTIN_LET: {
      // TODO why not just load_cell?!
      /*if (argdefs[1].type == ARGT_CONST) {
        jit_lea(R0,argdefs[1].cell); // load cell
      } else if (argdefs[1].type == ARGT_CELL) {
        // already in R0
      } else if (argdefs[1].type == ARGT_ENV) {
        jit_lea(R0,argdefs[1].env);
        jit_ldr(R0); // load cell
      } else if (argdefs[1].type == ARGT_REG) {
        jit_movr(ARGR1, LBDREG+argdefs[1].slot);
        }*/
      load_cell(R0, argdefs[1], frame);

      int offset = argi-1 + frame->locals;
      
      int fidx = get_sym_frame_idx(argdefs[0].cell->addr, fn_frame, 1);

      //printf("fidx: %d\n",fidx);
      
      if (fidx >= 0) {
        // existing stack entry
        offset = fidx;
        printf("+~ frame entry %s, existing stack-local idx %d\n",fn_frame[offset].name,fn_frame[offset].slot);
      } else {
        fn_frame[offset].name = argdefs[0].cell->addr;
        fn_frame[offset].cell = NULL;
        fn_frame[offset].type = ARGT_STACK;
        fn_frame[offset].slot = frame->locals;
        printf("++ frame entry %s, new stack-local idx %d\n",fn_frame[offset].name,fn_frame[offset].slot);
        frame->locals++;
      }
      
      jit_str_stack(R0,PTRSZ*(fn_frame[offset].slot+frame->sp));
      //jit_stack_offset++;
      
      break;
    }
    case BUILTIN_FN: {
      if (argi<2) {
        printf("error: trying to define fn without body.\n");
        return 0;
      }
      
      // scan args (build signature)
      Cell* fn_args = alloc_nil();
      Arg fn_new_frame[MAXFRAME];
      for (int i=0; i<MAXFRAME; i++) {
        fn_new_frame[i].type = 0;
        fn_new_frame[i].slot = -1;
        fn_new_frame[i].name = NULL;
      }

      int slotc = 0;
      for (int j=argi-3; j>=0; j--) {
        Cell* arg = alloc_cons(alloc_sym(argdefs[j].cell->addr),alloc_int(TAG_ANY));
        fn_args = alloc_cons(arg,fn_args);
        
        fn_new_frame[j].type = ARGT_REG;
        fn_new_frame[j].slot = j;
        fn_new_frame[j].name = argdefs[j].cell->addr;
      }
      char sig_debug[128];
      lisp_write(fn_args, sig_debug, sizeof(sig_debug));
      //printf("signature: %s\n",sig_debug);

      // body
      Cell* fn_body = argdefs[argi-2].cell;

      lisp_write(fn_body, sig_debug, sizeof(sig_debug));
      
      Cell* lambda = alloc_lambda(alloc_cons(fn_args,fn_body));
      lambda->next = 0;

      char label_fn[64];
      char label_fe[64];
      sprintf(label_fn,"f0_%p",lambda);
      sprintf(label_fe,"f1_%p",lambda);
      
      jit_jmp(label_fe);
      jit_label(label_fn);
      jit_dec_stack(MAXFRAME*PTRSZ);
      Frame nframe = {fn_new_frame, 0, 0};
      compile_expr(fn_body, &nframe, TAG_ANY); // new frame, fresh sp
      jit_inc_stack(MAXFRAME*PTRSZ);
      jit_ret();
      jit_label(label_fe);
      jit_lea(R0,lambda);
      
#ifdef CPU_ARM
      Label* fn_lbl = find_label(label_fn);
      printf("fn_lbl idx: %d code: %p\n",fn_lbl->idx,code);
      lambda->next = code + fn_lbl->idx;
      printf("fn_lbl next: %p\n",lambda->next);
#endif
      
      break;
    }
    case BUILTIN_IF: {
      // load the condition
      load_int(R1,argdefs[0], frame);

      char label_skip[64];
      sprintf(label_skip,"else_%d",++label_skip_count);
      
      // compare to zero
      jit_cmpi(R1,0);
      jit_je(label_skip);

      compile_expr(argdefs[1].cell, frame, return_type);

      // else?
      if (argdefs[2].cell) {
        char label_end[64];
        sprintf(label_end,"endif_%d",label_skip_count);
        jit_jmp(label_end);
        
        jit_label(label_skip);
        compile_expr(argdefs[2].cell, frame, return_type);
        jit_label(label_end);
      } else {
        jit_label(label_skip);
      }
      
      break;
    }
    case BUILTIN_WHILE: {
      // load the condition
      char label_loop[64];
      sprintf(label_loop,"loop_%d",++label_skip_count);
      char label_skip[64];
      sprintf(label_skip,"skip_%d",label_skip_count);
      
      jit_label(label_loop);
      
      int compiled_type = compile_expr(argdefs[0].cell, frame, TAG_INT);
      if (compiled_type != TAG_INT) {
        jit_ldr(R0);
        jit_cmpi(R0,0);
      } else {
        jit_cmpi(ARGR0,0);
      }
      //load_int(R1,argdefs[0]);
      // compare to zero
      jit_je(label_skip);

      compile_expr(argdefs[1].cell, frame, return_type);

      jit_jmp(label_loop);
      jit_label(label_skip);
      
      break;
    }
    case BUILTIN_DO: {
      args = orig_args;
      Cell* arg;
      while ((arg = car(args))) {
        compile_expr(arg, frame, return_type);
        args = cdr(args);
      }
      break;
    }
    case BUILTIN_LIST: {
      args = orig_args;
      Cell* arg;
      int n = 0;
      while ((arg = car(args))) {
        compile_expr(arg, frame, TAG_ANY);
        jit_push(R0,R0);
        frame->sp++;
        args = cdr(args);
        n++;
      }
      jit_call(alloc_nil, "list:alloc_nil");
      jit_movr(ARGR1,R0);
      for (int i=0; i<n; i++) {
        jit_pop(ARGR0,ARGR0);
        frame->sp--;
        jit_call(alloc_cons, "list:alloc_cons");
        jit_movr(ARGR1,R0);
      }
      break; // FIXME
    }
    case BUILTIN_QUOTE: {
      args = orig_args;
      Cell* arg = car(args);
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
      break;
    }
    case BUILTIN_CDR: {
      load_cell(R0,argdefs[0], frame);
      jit_addi(R0,PTRSZ); // TODO depends on machine word size

      // type check -------------------
      jit_movr(R1,R0);
      jit_addi(R1,PTRSZ);
      jit_ldr(R1);
      jit_lea(R2,consed_type_error);
      jit_cmpi(R1,TAG_CONS);
      jit_movne(R0,R2);
      // ------------------------------

      jit_ldr(R0);
      break;
    }
    case BUILTIN_CONS: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call(alloc_cons,"alloc_cons");
      break;
    }
    case BUILTIN_CONCAT: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call(alloc_concat,"alloc_concat");
      break;
    }
    case BUILTIN_SUBSTR: {
      load_cell(ARGR0,argdefs[0], frame);
      load_int(ARGR1,argdefs[1], frame);
      load_int(ARGR2,argdefs[2], frame);
      jit_call(alloc_substr,"alloc_substr");
      break;
    }
    case BUILTIN_GET: {
      char label_skip[64];
      sprintf(label_skip,"skip_%d",++label_skip_count);

      jit_movi(R1,0);    
      load_cell(R3,argdefs[0], frame);
      load_int(R2,argdefs[1], frame); // offset -> R2
      jit_cmpi(R2,0);
      jit_jneg(label_skip); // negative offset?

      jit_movr(R0,R3);
      jit_addi(R0,PTRSZ); // fetch size -> R0
      jit_ldr(R0);

      jit_subr(R0,R2);
      jit_jneg(label_skip); // overflow? (R2>R0)
      jit_je(label_skip); // overflow? (R2==R0)

      jit_movr(R1,R2);
      jit_ldr(R3); // string address
      jit_addr(R1,R3);
      jit_ldrb(R1);

      jit_label(label_skip);
      
      jit_movr(ARGR0, R0); // FIXME
      jit_call(alloc_int,"alloc_int");
      break;
    }
    case BUILTIN_PUT: {
      char label_skip[64];
      //char label_noskip[64];
      sprintf(label_skip,"skip_%d",++label_skip_count);
      //sprintf(label_noskip,"noskip_%d",label_skip_count);
    
      load_cell(R1,argdefs[0], frame);
      jit_push(R1,R1);
      frame->sp++;
      load_int(R2,argdefs[1], frame); // offset -> R2
      jit_cmpi(R2,0);
      jit_jneg(label_skip); // negative offset?
      load_int(R3,argdefs[2], frame); // byte to store -> R3

      jit_movr(R0,R1);
      jit_addi(R0,PTRSZ); // fetch size -> R0
      jit_ldr(R0);

      jit_subr(R0,R2);
      jit_jneg(label_skip); // overflow? (R2>R0)
      jit_je(label_skip); // overflow? (R2==R0)

      jit_ldr(R1); // string address
      jit_addr(R1,R2);
      jit_strb(R1); // strb is always from R3
      
      //jit_jmp(label_noskip);
      
      jit_label(label_skip);
      jit_pop(R0,R0); // restore arg0
      frame->sp--;
      //jit_lea(R0,alloc_error(ERR_OUT_OF_BOUNDS));
      //jit_label(label_noskip);
      
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
      //char label_skip[64];
      //char label_noskip[64];
      //sprintf(label_skip,"skip_%d",++label_skip_count);
    
      load_cell(R1,argdefs[0], frame);
      //jit_movr(R8,R1);
      load_int(R2,argdefs[1], frame); // offset -> R2
      //jit_cmpi(R2,0);
      //jit_jneg(label_skip); // negative offset?
      load_int(R3,argdefs[2], frame); // word to store -> R3

      //jit_movr(R0,R1);
      //jit_addi(R0,PTRSZ); // fetch size -> R0
      //jit_ldr(R0);

      // FIXME
      //jit_subr(R0,R2);
      //jit_jneg(label_skip); // overflow? (R2>R0)
      //jit_je(label_skip); // overflow? (R2==R0)

      jit_ldr(R1); // string address
      jit_addr(R1,R2);
      jit_strw(R1); // store from r3
      
      //jit_jmp(label_noskip);
      
      //jit_label(label_skip);
      //jit_pop(R0,R8); // restore arg0
      //load_cell(R0,argdefs[0]);
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
    case BUILTIN_WRITE: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call(lisp_write_to_cell,"lisp_write_to_cell");
      break;
    }
    case BUILTIN_READ: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_ldr(ARGR0);
      jit_call(read_string,"read_string");
      break;
    }
    case BUILTIN_SIZE: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_addi(ARGR0,PTRSZ); // fetch size -> R0
      jit_ldr(ARGR0);
      jit_call(alloc_int,"alloc_int");
      
      break;
    }
    case BUILTIN_GC: {
      jit_lea(ARGR0,global_env);
      jit_call(collect_garbage,"collect_garbage");
      break;
    }
    case BUILTIN_PRINT: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(lisp_print,"lisp_print");
      break;
    }
    case BUILTIN_MOUNT: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      jit_call(fs_mount,"fs_mount");
      break;
    }
    case BUILTIN_OPEN: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(fs_open,"fs_open");
      break;
    }
    case BUILTIN_RECV: {
      load_cell(ARGR0,argdefs[0], frame);
      jit_call(stream_read,"stream_read");
      break;
    }
    case BUILTIN_SEND: {
      load_cell(ARGR0,argdefs[0], frame);
      load_cell(ARGR1,argdefs[1], frame);
      // FIXME clobbers stuff
      jit_call(stream_write,"stream_write");
      break;
    }
    }
  } else {
    // lambda call
    //printf("-> would jump to lambda at %p\n",op->next);
    if (argi>1) {
      jit_push(LBDREG, LBDREG+argi-2);
      frame->sp+=(1+argi-2);
    }
    
    for (int j=0; j<argi-1; j++) {
      if (argdefs[j].type == ARGT_REG) {
        if (argdefs[j].slot<j) {
          // register already clobbered, load from stack
          printf("-- loading clobbered reg %d from stack to %d\n",argdefs[j].slot,LBDREG+j);
          jit_ldr_stack(LBDREG+j, (argdefs[j].slot)*PTRSZ);
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
    jit_call(op->next,"lambda");
    if (argi>1) {
      jit_pop(LBDREG, LBDREG+argi-2);
      frame->sp-=(1+argi-2);
    }
  }

  fflush(jit_out);

  // at this point, registers R1-R6 are filled, execute
  return compiled_type;
}


void init_compiler() {
  
  //memdump(0x6f460,0x200,0);
  //uart_getc();
  
  //printf("malloc test: %p\r\n",malloc(1024));

  printf("[compiler] creating global env hash table…\r\n");
  global_env = sm_new(1000);

  printf("[compiler] init_allocator…\r\n");
  init_allocator();

  printf("[compiler] init_filesystems…\r\n");
  filesystems_init();

  consed_type_error = alloc_cons(alloc_error(ERR_INVALID_PARAM_TYPE),alloc_nil());
  
  printf("[compiler] inserting symbols…\r\n");
  
  insert_symbol(alloc_sym("def"), alloc_builtin(BUILTIN_DEF, alloc_list((Cell*[]){alloc_int(TAG_SYM), alloc_int(TAG_ANY)}, 2)), &global_env);
  insert_symbol(alloc_sym("let"), alloc_builtin(BUILTIN_LET, alloc_list((Cell*[]){alloc_int(TAG_SYM), alloc_int(TAG_ANY)}, 2)), &global_env);

  insert_symbol(alloc_sym("+"), alloc_builtin(BUILTIN_ADD, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_INT)}, 2)), &global_env);
  insert_symbol(alloc_sym("-"), alloc_builtin(BUILTIN_SUB, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_INT)}, 2)), &global_env);
  insert_symbol(alloc_sym("*"), alloc_builtin(BUILTIN_MUL, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_INT)}, 2)), &global_env);
  insert_symbol(alloc_sym("/"), alloc_builtin(BUILTIN_DIV, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_INT)}, 2)), &global_env);
  //insert_symbol(alloc_sym("%"), alloc_builtin(BUILTIN_MOD), &global_env);
  
  printf("[compiler] arithmetic…\r\n");
  
  insert_symbol(alloc_sym("lt"), alloc_builtin(BUILTIN_LT, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_INT)}, 2)), &global_env);
  insert_symbol(alloc_sym("gt"), alloc_builtin(BUILTIN_GT, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_INT)}, 2)), &global_env);
  
  printf("[compiler] compare…\r\n");
  
  insert_symbol(alloc_sym("if"), alloc_builtin(BUILTIN_IF, alloc_list((Cell*[]){alloc_int(TAG_INT), alloc_int(TAG_LAMBDA), alloc_int(TAG_LAMBDA)}, 3)), &global_env);
  insert_symbol(alloc_sym("fn"), alloc_builtin(BUILTIN_FN, NULL), &global_env);
  insert_symbol(alloc_sym("while"), alloc_builtin(BUILTIN_WHILE, NULL), &global_env);
  insert_symbol(alloc_sym("print"), alloc_builtin(BUILTIN_PRINT, alloc_list((Cell*[]){alloc_int(TAG_ANY)}, 1)), &global_env);
  insert_symbol(alloc_sym("do"), alloc_builtin(BUILTIN_DO, NULL), &global_env);
  
  printf("[compiler] flow…\r\n");
  
  insert_symbol(alloc_sym("car"), alloc_builtin(BUILTIN_CAR, alloc_list((Cell*[]){alloc_int(TAG_CONS)}, 1)), &global_env);
  insert_symbol(alloc_sym("cdr"), alloc_builtin(BUILTIN_CDR, alloc_list((Cell*[]){alloc_int(TAG_CONS)}, 1)), &global_env);
  insert_symbol(alloc_sym("cons"), alloc_builtin(BUILTIN_CONS, alloc_list((Cell*[]){alloc_int(TAG_ANY), alloc_int(TAG_ANY)}, 2)), &global_env);
  insert_symbol(alloc_sym("list"), alloc_builtin(BUILTIN_LIST, NULL), &global_env);
  insert_symbol(alloc_sym("quote"), alloc_builtin(BUILTIN_QUOTE, NULL), &global_env);
  //insert_symbol(alloc_sym("map"), alloc_builtin(BUILTIN_MAP), &global_env);

  printf("[compiler] lists…\r\n");
  
  insert_symbol(alloc_sym("concat"), alloc_builtin(BUILTIN_CONCAT, alloc_list((Cell*[]){alloc_int(TAG_STR), alloc_int(TAG_STR)}, 2)), &global_env);
  insert_symbol(alloc_sym("substr"), alloc_builtin(BUILTIN_SUBSTR, alloc_list((Cell*[]){alloc_int(TAG_STR), alloc_int(TAG_INT), alloc_int(TAG_INT)}, 3)), &global_env);
  insert_symbol(alloc_sym("get"), alloc_builtin(BUILTIN_GET, alloc_list((Cell*[]){alloc_int(TAG_STR), alloc_int(TAG_INT)}, 2)), &global_env);
  insert_symbol(alloc_sym("put"), alloc_builtin(BUILTIN_PUT, alloc_list((Cell*[]){alloc_int(TAG_STR), alloc_int(TAG_INT), alloc_int(TAG_INT)}, 3)), &global_env);
  insert_symbol(alloc_sym("get32"), alloc_builtin(BUILTIN_GET32, alloc_list((Cell*[]){alloc_int(TAG_BYTES), alloc_int(TAG_INT)}, 2)), &global_env);
  insert_symbol(alloc_sym("put32"), alloc_builtin(BUILTIN_PUT32, alloc_list((Cell*[]){alloc_int(TAG_BYTES), alloc_int(TAG_INT), alloc_int(TAG_INT)}, 3)), &global_env);
  insert_symbol(alloc_sym("size"), alloc_builtin(BUILTIN_SIZE, alloc_list((Cell*[]){alloc_int(TAG_STR)}, 1)), &global_env);
  insert_symbol(alloc_sym("alloc"), alloc_builtin(BUILTIN_ALLOC, alloc_list((Cell*[]){alloc_int(TAG_INT)}, 1)), &global_env);
  insert_symbol(alloc_sym("alloc-str"), alloc_builtin(BUILTIN_ALLOC_STR, alloc_list((Cell*[]){alloc_int(TAG_INT)}, 1)), &global_env);

  printf("[compiler] strings…\r\n");
  
  /*insert_symbol(alloc_sym("uget"), alloc_builtin(BUILTIN_UGET), &global_env);
  insert_symbol(alloc_sym("uput"), alloc_builtin(BUILTIN_UPUT), &global_env);
  insert_symbol(alloc_sym("usize"), alloc_builtin(BUILTIN_USIZE), &global_env);

  printf("[compiler] get/put…\r\n");*/
  
  insert_symbol(alloc_sym("write"), alloc_builtin(BUILTIN_WRITE, alloc_list((Cell*[]){alloc_int(TAG_ANY), alloc_int(TAG_STR)},2)), &global_env);
  insert_symbol(alloc_sym("read"), alloc_builtin(BUILTIN_READ, alloc_list((Cell*[]){alloc_int(TAG_STR)},1)), &global_env);
  insert_symbol(alloc_sym("eval"), alloc_builtin(BUILTIN_EVAL, alloc_list((Cell*[]){alloc_int(TAG_ANY)},1)), &global_env);

  
  insert_symbol(alloc_sym("mount"), alloc_builtin(BUILTIN_MOUNT, alloc_list((Cell*[]){alloc_int(TAG_STR), alloc_int(TAG_CONS)},2)), &global_env);
  insert_symbol(alloc_sym("open"), alloc_builtin(BUILTIN_OPEN, alloc_list((Cell*[]){alloc_int(TAG_STR)},1)), &global_env);
  insert_symbol(alloc_sym("mmap"), alloc_builtin(BUILTIN_MMAP, alloc_list((Cell*[]){alloc_int(TAG_STR)},1)), &global_env);
  insert_symbol(alloc_sym("recv"), alloc_builtin(BUILTIN_RECV, alloc_list((Cell*[]){alloc_int(TAG_STREAM)},1)), &global_env);
  insert_symbol(alloc_sym("send"), alloc_builtin(BUILTIN_SEND, alloc_list((Cell*[]){alloc_int(TAG_STREAM),alloc_int(TAG_ANY)},2)), &global_env);

  
  printf("[compiler] write/eval…\r\n");
  
  /*insert_symbol(alloc_sym("pixel"), alloc_builtin(BUILTIN_PIXEL), &global_env);
  insert_symbol(alloc_sym("rectfill"), alloc_builtin(BUILTIN_RECTFILL), &global_env);
  insert_symbol(alloc_sym("flip"), alloc_builtin(BUILTIN_FLIP), &global_env);
  insert_symbol(alloc_sym("blit"), alloc_builtin(BUILTIN_BLIT), &global_env);
  insert_symbol(alloc_sym("blit-mono"), alloc_builtin(BUILTIN_BLIT_MONO), &global_env);
  insert_symbol(alloc_sym("blit-mono-inv"), alloc_builtin(BUILTIN_BLIT_MONO_INV), &global_env);
  insert_symbol(alloc_sym("blit-string"), alloc_builtin(BUILTIN_BLIT_STRING), &global_env);
  insert_symbol(alloc_sym("inkey"), alloc_builtin(BUILTIN_INKEY), &global_env);
  
  printf("[compiler] graphics…\r\n");
  */
  insert_symbol(alloc_sym("gc"), alloc_builtin(BUILTIN_GC, NULL), &global_env);
  /*insert_symbol(alloc_sym("symbols"), alloc_builtin(BUILTIN_SYMBOLS), &global_env);
  insert_symbol(alloc_sym("load"), alloc_builtin(BUILTIN_LOAD), &global_env);
  insert_symbol(alloc_sym("save"), alloc_builtin(BUILTIN_SAVE), &global_env);
  
  printf("[compiler] gc/load/save…\r\n");
  
  insert_symbol(alloc_sym("udp-poll"), alloc_builtin(BUILTIN_UDP_POLL), &global_env);
  insert_symbol(alloc_sym("udp-send"), alloc_builtin(BUILTIN_UDP_SEND), &global_env);

  printf("[compiler] udp…\r\n");
  
  insert_symbol(alloc_sym("tcp-bind"), alloc_builtin(BUILTIN_TCP_BIND), &global_env);
  insert_symbol(alloc_sym("tcp-connect"), alloc_builtin(BUILTIN_TCP_CONNECT), &global_env);
  insert_symbol(alloc_sym("tcp-send"), alloc_builtin(BUILTIN_TCP_SEND), &global_env);

  printf("[compiler] tcp…\r\n");
  
  insert_symbol(alloc_sym("sin"), alloc_builtin(BUILTIN_SIN), &global_env);
  insert_symbol(alloc_sym("cos"), alloc_builtin(BUILTIN_COS), &global_env);
  insert_symbol(alloc_sym("sqrt"), alloc_builtin(BUILTIN_SQRT), &global_env);

  printf("[compiler] math.\r\n");*/
  
  int num_syms = sm_get_count(global_env);
  printf("sledge knows %u symbols. enter (symbols) to see them.\r\n", num_syms);
}
