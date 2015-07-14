#include "minilisp.h"
#include "reader.h"
#include "writer.h"
#include "alloc.h"

#include "machine.h"
#include "blit.h"

#include <math.h>

void memdump(jit_word_t start,uint32_t len,int raw);

typedef enum builtin_t {
  BUILTIN_ADD,
  BUILTIN_SUB,
  BUILTIN_MUL,
  BUILTIN_DIV,
  BUILTIN_MOD,

  BUILTIN_LT,
  BUILTIN_GT,
  BUILTIN_EQ,

  BUILTIN_WHILE,

  BUILTIN_DEF,
  BUILTIN_IF ,
  BUILTIN_FN ,

  BUILTIN_CAR,
  BUILTIN_CDR,
  BUILTIN_CONS,
  BUILTIN_LIST,

  BUILTIN_ALLOC,
  BUILTIN_ALLOC_STR,
  BUILTIN_CONCAT,
  BUILTIN_SUBSTR,

  BUILTIN_GET,
  BUILTIN_PUT,
  BUILTIN_SIZE,

  BUILTIN_UGET,
  BUILTIN_UPUT,
  BUILTIN_USIZE,

  BUILTIN_TYPE,
  BUILTIN_LET,
  BUILTIN_QUOTE,
  BUILTIN_MAP,
  BUILTIN_DO,

  BUILTIN_EVAL,
  BUILTIN_WRITE,

  BUILTIN_PRINT,

  BUILTIN_PIXEL,
  BUILTIN_FLIP,
  BUILTIN_RECTFILL,
  BUILTIN_BLIT,
  BUILTIN_BLIT_MONO,
  BUILTIN_BLIT_MONO_INV,
  BUILTIN_BLIT_STRING,
  BUILTIN_INKEY,

  BUILTIN_ALIEN,
  BUILTIN_GC,
  BUILTIN_SYMBOLS,
  BUILTIN_LOAD,
  BUILTIN_SAVE,
  BUILTIN_UDP_POLL,
  BUILTIN_UDP_SEND,
  BUILTIN_TCP_CONNECT,
  BUILTIN_TCP_BIND,
  BUILTIN_TCP_SEND,

  BUILTIN_SIN,
  BUILTIN_COS,
  BUILTIN_SQRT
} builtin_t;

#define env_t StrMap

static env_t* global_env = NULL;

static Cell* coerce_int_cell; // recycled cell used to return coereced integers
static Cell* error_cell; // recycled cell used to return errors

// store JIT states for nested jitting
static size_t* jit_state_stack;
int jit_state_stack_usage = 0;

Cell* lookup_global_symbol(char* name) {
  //HASH_FIND_STR(global_env, name, res);
  env_entry* res;
  int found = sm_get(global_env, name, (void**)&res);
  if (!found) return NULL;
  return res->cell;
}

Cell* lookup_symbol(char* name, env_t** env) {
  env_entry* res;
  //HASH_FIND_STR(*env, name, res);
  int found = sm_get(*env, name, (void**)&res);
  if (!found) return NULL;
  return res->cell;
}

env_entry* intern_symbol(Cell* symbol, env_t** env) {
  env_entry* e;
  //HASH_FIND_STR(*env, symbol->addr, e);
  int found = sm_get(*env, symbol->addr, (void**)&e);
  if (!found) {
    e = malloc(sizeof(env_entry));
    strcpy(e->name, (char*)symbol->addr);
    e->cell = NULL;
    //HASH_ADD_STR(*env, name, e);
    sm_put(*env, e->name, &e);
  }
  //printf("intern: %s at %p cell %p\n",symbol->addr,e,e->cell);
  return e;
}

Cell* insert_symbol(Cell* symbol, Cell* cell, env_t** env) {
  env_entry* e;
  printf("sm_get %s\r\n",symbol->addr);
  //HASH_FIND_STR(*env, symbol->addr, e);
  int found = sm_get(*env, symbol->addr, (void**)&e);
  
  printf("sm_get res: %d\r\n",found);
  
  /*if (cell) {
    printf("insert_symbol %s <- %x\n",symbol->addr,cell->value);
  } else {
    printf("insert_symbol %s <- NULL\n",symbol->addr);
    }*/

  if (found) {
    e->cell = cell;
    return e->cell;
  }
  printf("++ alloc env entry %s (%d), symbol size %d\r\n",symbol->addr,sizeof(env_entry),symbol->size);
    
  e = malloc(sizeof(env_entry));
  memcpy(e->name, (char*)symbol->addr, symbol->size);
  e->cell = cell;

  //HASH_ADD_STR(*env, name, e);
  sm_put(*env, e->name, &e);

  return e->cell;
}

static int stack_reg = 0;

void debug_push(jit_word_t val, jit_word_t sp) {
  printf("XX push %p <- %lx  stack_base: %p\n",sp,val,stack_base);
}

void debug_pop(jit_word_t val, jit_word_t sp) {
  printf("XX pop %p -> %lx\n",sp,val);
}

jit_word_t debug_tmp = 0;

void stack_push(int reg, void** sp)
{
  jit_ldi(JIT_V0, sp);
  jit_str(JIT_V0, reg);
  jit_addi(JIT_V0, JIT_V0, sizeof(jit_word_t));
  jit_sti(sp, JIT_V0);
  
  //printf("stack_push sp: %p\n",*sp);
  //*sp += sizeof(jit_word_t);
  
  /*jit_sti(&debug_tmp, reg);
  jit_prepare();
  jit_pushargr(reg);
  jit_pushargr(JIT_V0);
  jit_finishi(debug_push);
  jit_ldi(reg, &debug_tmp);*/
  
  /*if (stack_reg == 0) {
    jit_movr(JIT_V0, reg);
  } else if (stack_reg == 1) {
    jit_movr(JIT_V1, reg);
  } else if (stack_reg == 2) {
    jit_movr(JIT_V2, reg);
  }
  stack_reg = (stack_reg+1)%4;*/
}

void stack_pop(int reg, void** sp)
{
  //*sp -= sizeof(jit_word_t);
  /*printf("stack_pop sp: %p\n",*sp);

  if (*sp<stack_base) {
    printf("stack underflow!\n");
    exit(2);
    }*/

  jit_ldi(JIT_V0, sp);
  jit_subi(JIT_V0, JIT_V0, sizeof(jit_word_t));
  jit_ldr(reg, JIT_V0);
  jit_sti(sp, JIT_V0);
  
  /*jit_sti(&debug_tmp, reg);
  jit_prepare();
  jit_pushargr(reg);
  jit_pushargr(JIT_V0);
  jit_finishi(debug_pop);
  jit_ldi(reg, &debug_tmp);*/

  /*stack_reg = stack_reg-1;
  if (stack_reg<0) stack_reg = 0;
  
  if (stack_reg == 0) {
    jit_movr(reg, JIT_V0);
  } else if (stack_reg == 1) {
    jit_movr(reg, JIT_V1);
  } else if (stack_reg == 2) {
    jit_movr(reg, JIT_V2);
    }*/
}

void stack_set(int reg, void** sp)
{
  jit_sti(sp, JIT_V0);
}

int compile_applic(int retreg, Cell* list, tag_t required);

static Cell* debug_current_expr;

int argnum_error(char* usage) {
  char tmp[1024];
  lisp_write(debug_current_expr,tmp,1023);

  printf("argument error in %s (%p). correct usage: %s.\n",tmp,debug_current_expr,usage);
  jit_movi(JIT_R0, (jit_word_t)error_cell);
  return 0;
}

Cell* int_cell_regs;


int box_int(int retreg, tag_t required) {
  if (required == TAG_PURE_INT || required == TAG_VOID) return 1;
  if (required != TAG_INT && required != TAG_ANY) {
    printf("<cannot cast int result to tag %d>",required);
    return 0;
  }
  //printf("-- box_int retreg: %d\n",retreg);

  jit_prepare();
  jit_pushargr(retreg);  
  jit_finishi(alloc_int);
  jit_retval(retreg);
  return 1;
}

int unbox_int(int retreg) {
  jit_ldr(retreg, retreg);
  return 1;
}


// returns 1 on success
// returns 0 on failure (type mismatch)
int compile_arg(int retreg, Cell* arg, tag_t required) {
  if (!arg) {
    return argnum_error("missing argument");
  }
  
  debug_current_expr = arg;
  
  /*char buffer2[512];
  char buffer[512];
  lisp_write(arg, buffer2, 400);
  sprintf(buffer,"c_arg %s tag %d\n",buffer2,required);
  jit_note(buffer, __LINE__);*/
  
  jit_word_t tag = TAG_PURE_INT; // null = 0
  tag = arg->tag;
  
  if (tag == TAG_SYM) {
    char* sym = arg->addr;
    env_entry* e = intern_symbol(arg, &global_env);
    
    if (!e->cell) {
      // undefined symbol
      if (required == TAG_INT || required == TAG_PURE_INT || required == TAG_ANY || required == TAG_VOID) {
        // return non-existence (zero)
        e->cell = alloc_int(0);
      } else if (required == TAG_CONS) {
        // FIXME adhoc
        e->cell = alloc_nil();
      } else {
        printf("<compile_arg: undefined symbol %s>\n",sym);
        return 0;
      }
    }
    
    arg = e->cell;
    tag = arg->tag;

    // FIXME this assumes that symbol table entries' tags never change

    // load cell from symbol table
    jit_movi(retreg, (jit_word_t)e);
    jit_ldr(retreg, retreg);
  }
  else if (tag == TAG_CONS) {
    return compile_applic(retreg, arg, required);
  }
  else {
    // load cell directly
    jit_movi(retreg, (jit_word_t)arg);
  }

  //printf("arg: %p, tag: %d, required: %d\n",arg,tag,required);
  
  if (tag == TAG_INT) {
    if (required == TAG_PURE_INT) {
      unbox_int(retreg);
      return 1;
    }
    else if (required == TAG_INT || required == TAG_ANY || required == TAG_VOID) {
      return 1;
    }
    else {
      printf("<type mismatch. got boxed int, need %d>\n",required);
      return 0;
    }
  }
  else if (tag == TAG_PURE_INT) {
    if (required == TAG_PURE_INT || required == TAG_VOID) {
      return 1;
    }
    else if (required == TAG_INT || required == TAG_ANY) {
      char tmp[1024];
      lisp_write(arg,tmp,1023);
      printf("++ compile_arg int allocation at: %s\n",tmp);
      // box int
      jit_prepare();
      jit_pushargr(retreg);
      jit_finishi(alloc_int);
      jit_retval(retreg);
      // FIXME: mark this allocation as temporary
      // until "consumed"?
    } else {
      printf("<type mismatch. got unboxed int, need %d>\n",required);
      return 0;
    }
  }
  else {
    if (required == TAG_PURE_INT) {
      // other cells can't be coerced to pure integers, so we return existence, 1
      jit_movi(retreg, 1);
      return 1;
    }
    if (required == TAG_ANY || required == TAG_VOID || tag == required) {
      return 1;
    }
    
    printf("<type mismatch. got %d, need %d>\n",tag,required);
    return 0;
  }    
}

#define ARITH_ARGS()                                    \
  if (!compile_arg(JIT_R0, car(args), TAG_PURE_INT)) return 0;\
  stack_push(JIT_R0, &stack_ptr);\
  if (!compile_arg(JIT_R1, car(cdr(args)), TAG_PURE_INT)) return 0;\
  stack_pop(JIT_R0, &stack_ptr);\
  
int compile_add(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  jit_addr(retreg, JIT_R0, JIT_R1);
  return box_int(retreg, required);
}

int compile_sub(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  jit_subr(retreg, JIT_R0, JIT_R1);
  return box_int(retreg, required);
}

int compile_mul(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  jit_mulr(retreg, JIT_R0, JIT_R1);
  return box_int(retreg, required);
}

int compile_div(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  jit_divr(retreg, JIT_R0, JIT_R1);
  return box_int(retreg, required);
}

int compile_mod(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  
  //stack_push(JIT_R2, &stack_ptr);
  jit_movr(JIT_R2, JIT_R0);
  jit_divr(JIT_R0, JIT_R0, JIT_R1);
  jit_mulr(JIT_R0, JIT_R0, JIT_R1);
  jit_subr(retreg, JIT_R2, JIT_R0);
  //stack_pop(JIT_R2, &stack_ptr);
  
  return box_int(retreg, required);
}

int compile_lt(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  jit_ltr(retreg, JIT_R0, JIT_R1);
  return box_int(retreg, required);
}

int compile_gt(int retreg, Cell* args, tag_t required) {
  ARITH_ARGS();
  jit_gtr(retreg, JIT_R0, JIT_R1);
  return box_int(retreg, required);
}


// FIXME: cheap way of detecting (tail) recursion
// later, manage this as a part of compiler state
// that is passed around
static Cell* currently_compiling_fn_sym = NULL;
static Cell* currently_compiling_fn_op = NULL;
static jit_node_t* currently_compiling_fn_label = NULL;

void undefined_fn_stub() {
  printf("!! undefined_fn_stub() called.\n");
}

int compile_def(int retreg, Cell* args, tag_t required) {
  //if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(def symbol definition)");
  
  Cell* sym = car(args);
  Cell* value = car(cdr(args));

  int detect_fn = 0;

  // analysis of what we are defining
  if (value) {
    if (value->tag == TAG_CONS) {
      Cell* opsym = car(value);
      if (opsym && opsym->tag == TAG_SYM) {
        Cell* op = lookup_symbol(opsym->addr, &global_env);
        if (op && op->value == BUILTIN_FN) {
          // we are binding a function
          currently_compiling_fn_sym = sym;
          detect_fn = 1;
          printf("-- compiling fn %s\r\n",currently_compiling_fn_sym->addr);

          // FIXME: recursion is broken
          
          env_entry* stub_e = intern_symbol(sym, &global_env);
          stub_e->cell = alloc_lambda(0);
          stub_e->cell->next = (void*)undefined_fn_stub;
        }
      }
    }
  }

  int success = 0;
  if (required == TAG_PURE_INT) {
    success = compile_arg(retreg, value, TAG_INT);
  } else if (required == TAG_VOID) {
    success = compile_arg(retreg, value, TAG_ANY);
  } else {
    success = compile_arg(retreg, value, required);
  }
  
  if (!success) {
    char tmp[1024];
    lisp_write(value,tmp,1023);

    printf("<type mismatch in def %s, required %d, got: %s>\n",sym->addr,required,tmp);
    return 0;
  }

  //printf("interning: %s\n",sym->addr);

  env_entry* e = intern_symbol(sym, &global_env);
  if (!e->cell) {
    e->cell = alloc_int(0); // reserve at compile time
  }
  
  jit_sti(&e->cell, retreg);

  if (required == TAG_PURE_INT) {
    unbox_int(retreg);
  }
  
  if (detect_fn) {
    currently_compiling_fn_sym = NULL;
    currently_compiling_fn_label = NULL;
  }

  return 1;
}

static char temp_print_buffer[1024];

Cell* do_print(Cell* arg) {
  lisp_write(arg, temp_print_buffer, sizeof(temp_print_buffer)-1);
  printf("%s\r\n",temp_print_buffer);
  return arg;
}

int compile_print(int retreg, Cell* args, tag_t required) {
  if (!car(args)) return argnum_error("(print a)");
  Cell* arg = car(args);

  int r = compile_arg(retreg, arg, TAG_ANY);
  if (!r) {
    printf("<could not convert print arg to TAG_ANY>\r\n");
    return 0;
  }

  jit_prepare();
  jit_pushargr(retreg);
  jit_finishi(do_print);
  jit_retval(retreg);

  if (required == TAG_PURE_INT) {
    jit_movi(retreg, 0);
  }
  return 1;
}

Cell* make_symbol_list() {
  Cell* end = alloc_nil();
  /*for (env_entry* e=global_env; e != NULL; e=e->hh.next) {
    end = alloc_cons(alloc_string_copy(e->name), end);
    }*/
  return end;
}

int compile_symbol_list(int retreg) {
  jit_prepare();
  jit_finishi(make_symbol_list);
  jit_retval(retreg);
  return 1;
}

int compile_gc(int retreg) {
  jit_prepare();
  jit_pushargi((jit_word_t)global_env);
  jit_finishi(collect_garbage);
  jit_retval(retreg);
  return 1;
}

int compile_do(int retreg, Cell* args, tag_t required) {
  if (!car(args)) return argnum_error("(do op1 op2 …)");
  int is_last = !(car(cdr(args)));
  int success = compile_arg(retreg, car(args), is_last?required:TAG_VOID);

  if (!success) return 0;
  
  while ((args = cdr(args)) && car(args)) {
    is_last = !(car(cdr(args)));
    success = compile_arg(retreg, car(args), is_last?required:TAG_VOID);
    if (!success) return 0;
  }

  return 1;
}

static int num_funcs = 0;

void push_jit_state() {
  *jit_state_stack = (jit_word_t)_jit;
  jit_state_stack++;
  
  jit_state_stack_usage++;
}

void pop_jit_state() {
  jit_state_stack--;
  _jit = (jit_state_t*)*jit_state_stack;
  
  jit_state_stack_usage--;
}

int compile_fn(int retreg, Cell* args, tag_t required) {
  if (!car(args)) {
    argnum_error("(fn arg1 arg2 … (body))");
    return 0;
  }
  
  // args 0..n-2 = parameter symbols
  // arg n-1 = body

  num_funcs++;

  Cell* args_saved = args;

#ifdef DEBUG
  if (currently_compiling_fn_sym) {
    printf("-- compile_fn %s\r\n",currently_compiling_fn_sym->addr);
  } else {
    printf("-- compile_fn (closure)\r\n");
  }
#endif
  
  // skip to the body
  //printf("args: %p %s %p\n",car(args),car(args)->addr,args->next);
  
  while (car(args) && cdr(args) && car(cdr(args))) {
    //printf("arg: %p %s %p\n",args,car(args)->addr,args->next);

    Cell* sym = car(args);
    args = cdr(args);
  }
  //printf("body: %p %d %p\n",args,args->tag,args->addr);

  if (jit_state_stack_usage>=49) {
    printf("<compile_fn error: jit_state_stack overflow.>\r\n");
    return 0;
  }

  push_jit_state();
  
  _jit = jit_new_state();
  jit_node_t* fn_label = jit_note(__FILE__, __LINE__);
  jit_prolog();
  
  jit_node_t* fn_body_label = jit_label();

  Cell* res = alloc_lambda(args_saved);
  // store info for potential recursion
  // currently_compiling_fn_label = fn_body_label;
  currently_compiling_fn_label = fn_label;
  currently_compiling_fn_op = res;

  // compile body
  // TODO: save _jit_saved on a stack
  int success = compile_arg(JIT_R0, car(args), TAG_ANY);

  if (success) {
    jit_retr(JIT_R0);
    jit_epilog();

    // res->addr will point to the args
    res->next = jit_emit();

    //printf("-- emitted at %p in %p\n",res->next,res);
    //memdump(res->next,0x100,0);
  
/*#ifdef DEBUG
    printf("<assembled: %p>\n",res->next);
    jit_disassemble();
    printf("--------------------------------------\n");
    #endif*/
  }
  
  jit_clear_state();
  
  pop_jit_state();

  if (success) {
    // return the allocated lambda
    jit_movi(retreg, (jit_word_t)res);
  } else {
    if (currently_compiling_fn_sym) {
      printf("<could not compile_fn %s>\r\n",currently_compiling_fn_sym->addr);
    } else {
      printf("<could not compile_fn (anonymous)>\r\n");
    }
    jit_movi(retreg, 0);
  }
  return success;
}

// args in this case is an array of Cells
Cell* call_dynamic_lambda(Cell** lbd_and_args, int args_supplied) {
  Cell* lbd = lbd_and_args[0];
  Cell** args = &lbd_and_args[1];

  // TODO: typecheck for TAG_LAMBDA

  Cell* pargs = (Cell*)lbd->addr;

  /*char buf[128];
  lisp_write(pargs, buf, 128);

  printf("-- dyn lambda formal params are %s\n",buf);*/

  // 1. push and clobber environment

  Cell** stack = malloc(args_supplied*sizeof(Cell*));

  for (int i=0; i<args_supplied; i++) {
    if (car(cdr(pargs))) {
      Cell* parg = car(pargs);
      env_entry* arge = intern_symbol(parg, &global_env);

      //static char buf[256];
      //lisp_write(args[i],buf,255);
      //printf("-- dyn arg %d (%s): %s\r\n",i,parg->addr,buf);

      stack[i] = arge->cell;
      // clobber
      arge->cell = args[i];
    } else {
      printf("-! too many args supplied\n");
      break;
    }
    pargs = cdr(pargs);
  }
  
  // 2. dispatch

  Cell* result = (Cell*)((funcptr)lbd->next)();

  // 3. pop clobbered environment

  pargs = (Cell*)lbd->addr;
  for (int i=0; i<args_supplied; i++) {
    if (car(cdr(pargs))) {
      Cell* parg = car(pargs);
      env_entry* arge = intern_symbol(parg, &global_env);
      // restore
      arge->cell = stack[i];
    } else {
      break;
    }
    pargs = cdr(pargs);
  }

  free(stack);
  return result;
}

int compile_dynamic_lambda(int retreg, int lbd_reg, Cell* args, tag_t requires, env_t** env) {
  // apply a function whose parameters we only learn at runtime

  // push the actual lambda to call
  stack_push(JIT_R0, &stack_ptr);

  // push any args
  int success = 0;
  int i = 0;
  while (i<10 && car(args)) {
    int res = compile_arg(JIT_R0, car(args), TAG_ANY);
      
    if (!res) {
      printf("<could not compile dynamic fn arg %d\n>",i);
      success = 0;
      // store new value
    }

    stack_push(JIT_R0, &stack_ptr);
    i++;
    args = cdr(args);
  }

  jit_prepare();
  jit_subi(JIT_V0, JIT_V0, (1+i)*sizeof(jit_word_t)); // layout: lbd,arg0,arg1,arg2... FIXME: hardcore stack hack
  stack_set(JIT_V0, &stack_ptr);
  jit_pushargr(JIT_V0);
  jit_pushargi(i);
  jit_finishi(call_dynamic_lambda);
  jit_retval(retreg);
  success = 1;

  return success;
}

// compile application of a compiled function
int compile_lambda(int retreg, Cell* lbd, Cell* args, tag_t requires, env_t** env, int recursion) {
  jit_node_t* ret_label = jit_note(__FILE__, __LINE__);

  //printf("<lambda: %p>\n",lbd->next);
  
  Cell* args_orig = args;
  Cell* pargs = (Cell*)lbd->addr;
  Cell* pargs_orig = pargs;

  env_entry* arges[10]; // max args 10

  int i = 0;
  int success = 1;

  // pass 0: save old symbol values
  while (i<10 && car(args) && car(pargs)) {
    // ignore the last arg, which is the function body
    if (car(cdr(pargs))) {
      Cell* sym = car(pargs);
      
      /*char buffer[64];
      sprintf(buffer,"save arg: %d %s\n",i,sym->addr);
      jit_note(buffer, __LINE__);*/

      // FIXME: possible optimization when pushing the same arg twice
      // (in subcall), but rare?

      //printf("prototype sym: %p\n",sym);
      env_entry* arge = intern_symbol(sym, env);
      jit_ldi(JIT_R0, &arge->cell); 
      stack_push(JIT_R0, &stack_ptr);
      
      int res = compile_arg(JIT_R0, car(args), TAG_ANY);
      
      if (!res) {
        printf("<could not compile fn arg %d\n>",i);
        success = 0;
        // store new value
      } else {
        jit_sti(&arge->cell, JIT_R0);
      }

      arges[i] = arge;
      
      i++;
    }
    pargs = cdr(pargs);
    args = cdr(args);
  }
  
  // pass 3: jump/call
  
  // TODO: tail recursion

  if (!success) {
    // TODO: pop stack
    return 0;
  }

  if (recursion == 1) {
    printf("++ recursion\r\n");
    //jit_note("jump to lambda as recursion\n",__LINE__);
    // get jump address at runtime
    jit_movi(JIT_R0, (jit_word_t)currently_compiling_fn_op);
    jit_ldxi(JIT_R0, JIT_R0, sizeof(jit_word_t)); // *(r0 + 1) -> r0

    jit_prepare();
    jit_finishr(JIT_R0);
    jit_retval(retreg);
    
    //jit_node_t* rec_jump = jit_calli(currently_compiling_fn_label);
    //jit_patch_at(rec_jump, );
  } else {
    //jit_note("call lambda as function\n",__LINE__);
    jit_prepare();
    jit_finishi(lbd->next);
    jit_retval(retreg);
  }

  // pass 4: restore environment

  if (recursion<2) {
    jit_movr(JIT_V1, retreg); // fixme: how to ensure this is a clobber-free reg?

    // after call, restore old symbol values from the stack (in reverse order)
    for (int j=i-1; j>=0; j--) {
      // pop value
      env_entry* arge = arges[j];
      stack_pop(JIT_R0, &stack_ptr);
      jit_sti(&arge->cell, JIT_R0); // restore any overwritten value
    }
    
    jit_movr(retreg, JIT_V1);
  }

  if (requires == TAG_PURE_INT) {
    return unbox_int(retreg);
  }

  return 1;
}

int compile_if(int retreg, Cell* args, tag_t requires) {
  if (!car(args) || !car(cdr(args)) || !cdr(cdr(args))) return argnum_error("(if condition then-body [else-body])");
  
  jit_node_t *jump, *jump2, *else_label, *exit_label;
  
  // lbl1:

  int r = compile_arg(retreg, car(args), TAG_PURE_INT);
  if (!r) {
    printf("<could not compile if's condition>\n");
    return 0;
  }
  
  // cmp r0, 1
  // beq lbl2
  jump = jit_beqi(retreg, 0);

  // then
  r = compile_arg(retreg, car(cdr(args)), requires);
  if (!r) {
    printf("<could not compile if's then-branch>\n");
    return 0;
  }

  // exit
  jump2 = jit_jmpi();
  
  else_label = jit_label();

  // else
  if (car(cdr(cdr(args)))) {
    r = compile_arg(retreg, car(cdr(cdr(args))), requires);
    if (!r) {
      printf("<could not compile if's else-branch>\n");
      return 0;
    }
  }
  
  exit_label = jit_label();
  
  jit_patch_at(jump, else_label);
  jit_patch_at(jump2, exit_label);

  return 1;
}

int compile_while(int retreg, Cell* args, tag_t requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(while condition (body))");
  
  jit_node_t *jump, *jump2, *loop_label, *exit_label;
  
  // lbl1:

  loop_label = jit_label();

  int r = compile_arg(retreg, car(args), TAG_PURE_INT);
  if (!r) {
    printf("<could not compile while's condition>\n");
    return 0;
  }

  // cmp r0, 1
  // beq lbl2
  jump = jit_beqi(retreg, 0);
  
  r = compile_arg(retreg, car(cdr(args)), requires);
  if (!r) {
    printf("<could not compile while's body>\n");
    return 0;
  }

  // lbl2:
  jump2 = jit_jmpi();

  exit_label = jit_label();
  
  jit_patch_at(jump, exit_label);
  jit_patch_at(jump2, loop_label);

  return 1;
}

int compile_quote(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(quote arg)");
  jit_movi(JIT_R0, (jit_word_t)car(args));
  return 1;
}

jit_word_t do_car(Cell* cell) {
  if (!cell) return 0;
  if (cell->tag != TAG_CONS) return 0;
  return (jit_word_t)(cell->addr?cell->addr:0);
}

jit_word_t do_car_int(Cell* cell) {
  if (!cell) return 0;
  if (cell->tag != TAG_CONS) return 0;
  Cell* carc = cell->addr;
  if (!carc) return 0;
  return carc->value;
}

int compile_car(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(car list)");
  Cell* arg = car(args);

  // TODO check success
  
  int success = compile_arg(retreg, arg, TAG_ANY);
  if (success) {
    jit_prepare();
    jit_pushargr(retreg);
    if (requires == TAG_PURE_INT) {
      jit_finishi(do_car_int);
    } else {
      jit_finishi(do_car);
    }
    jit_retval(retreg);
    //jit_ldr(JIT_R0, JIT_R0); // car r0 = r0->addr
  } else {
    printf("<non-cons argument to cdr>\n");
    return 0;
  }

  return 1;
}

jit_word_t do_cdr(Cell* cell) {
  if (!cell) return 0;
  if (cell->tag != TAG_CONS) return 0;
  return (jit_word_t)(cell->next?cell->next:0);
}

int compile_cdr(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(cdr list)");
  Cell* arg = car(args);
  
  int success = compile_arg(retreg, arg, TAG_ANY);
  if (success) {
    jit_prepare();
    jit_pushargr(retreg);
    jit_finishi(do_cdr);
    jit_retval(retreg);
  } else {
    printf("<non-cons argument to cdr>\n");
    return 0;
  //jit_ldxi(JIT_R0, JIT_R0, sizeof(jit_word_t)); // cdr r0 = r0 + one word = r0->next
  }
  return 1;
}

int compile_cons(int retreg, Cell* args, tag_t requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(cons new-item list)");
  
  compile_arg(JIT_R0, car(args), TAG_ANY);  // FIXME check success
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R1, car(cdr(args)), TAG_ANY);  // FIXME check success
  stack_pop(JIT_R0, &stack_ptr);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(alloc_cons);
  jit_retval(retreg);

  return 1;
}

int compile_list(int retreg, Cell* args, tag_t requires) {
  // (list 1)
  // compile_arg -> R0
  // nil -> R1

  int num_items = 0;
  while (car(args)) {
    int success = compile_arg(JIT_R0, car(args), TAG_ANY);
    
    if (success) {
      stack_push(JIT_R0, &stack_ptr);
    } else {
      while (num_items--) {
        // unwind stack
        stack_pop(JIT_R1, &stack_ptr);
      }
      printf("<error compiling list item %d>\r\n",num_items);
      jit_movi(retreg, 0);
      return 0;
    }
    
    args = cdr(args);
    num_items++;
  }

  jit_prepare();
  jit_finishi(alloc_nil);
  jit_retval(JIT_R1);
  jit_movr(JIT_R0, JIT_R1);
  
  while (num_items--) {
    jit_prepare();
    stack_pop(JIT_R0, &stack_ptr);
    jit_pushargr(JIT_R0);
    jit_pushargr(JIT_R1);
    jit_finishi(alloc_cons);
    jit_retval(JIT_R1);
  }
  
  jit_movr(retreg, JIT_R1);
  return 1;
}

Cell* do_map(Cell* fn, Cell* list) {
  // map is special: save only 1 arg

  /*char buf[512];
  lisp_write(fn,buf,511);
  printf("~~ map fn: %p %s\r\n",fn,buf);*/

  Cell* argsym = car(((Cell*)fn->addr));
  env_entry* e = intern_symbol(argsym, &global_env);
  Cell* saved = e->cell;

  Cell* result = alloc_nil();

  while (car(list)) {
    e->cell = car(list);
    Cell* res = (Cell*)((funcptr)fn->next)();
    list = cdr(list);
    result = alloc_cons(res, result);
  }

  e->cell = saved;

  // TODO: build result list
  return result;
}

// test: (map (fn x (+ x 1)) (list 1 2 3))

int compile_map(int retreg, Cell* args, tag_t requires) {

  // (map fn list)

  compile_arg(JIT_R0, car(args), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R1, car(cdr(args)), TAG_ANY);
  stack_pop(JIT_R0, &stack_ptr);

  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(do_map);
  jit_retval(retreg);
  
  return 1;
}

// alloc allocates a bytes object with specified size
// will contain zeroes
int compile_alloc(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(alloc size)");
  Cell* size_arg = car(args);
  compile_arg(retreg, size_arg, TAG_PURE_INT); // FIXME check success
  
  jit_prepare();
  jit_pushargr(retreg);
  jit_finishi(alloc_num_bytes);
  jit_retval(retreg); // returns fresh cell

  return 1;
}

// alloc_str allocates a string object with specified bytes size
// will contain zeroes
int compile_alloc_str(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(alloc-str size)");
  Cell* size_arg = car(args);
  compile_arg(retreg, size_arg, TAG_PURE_INT);
  
  jit_prepare();
  jit_pushargr(retreg);
  jit_finishi(alloc_num_string);
  jit_retval(retreg); // returns fresh cell

  return 1;
}

// concat allocates a new string combining two strings or buffers
int compile_concat(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(concat str1 str2)");
  if (!car(cdr(args))) return argnum_error("(concat str1 str2)");
  
  Cell* arg1 = car(args);
  compile_arg(JIT_R0, arg1, TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  Cell* arg2 = car(cdr(args));
  compile_arg(JIT_R1, arg2, TAG_ANY);
  stack_pop(JIT_R0, &stack_ptr);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(alloc_concat);
  jit_retval(retreg); // returns fresh cell

  return 1;
}

int compile_substr(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(substr str1 from len)");
  if (!car(cdr(args))) return argnum_error("(substr str1 from len)");
  if (!car(cdr(cdr(args)))) return argnum_error("(substr str1 from len)");
  
  Cell* arg3 = car(cdr(cdr(args)));
  int success = compile_arg(JIT_R0, arg3, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  
  Cell* arg2 = car(cdr(args));
  success = compile_arg(JIT_R0, arg2, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  
  Cell* arg1 = car(args);
  success = compile_arg(JIT_R0, arg1, TAG_ANY);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(alloc_substr);
  jit_retval(retreg); // returns fresh cell

  return 1;
}

// write
// allocates a string object and writes s-expression dump of object
// into it
int compile_write(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(write buffer object)");
  
  Cell* buf_arg = car(args);
  compile_arg(JIT_R0, buf_arg, TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  Cell* obj_arg = car(cdr(args));
  compile_arg(JIT_R1, obj_arg, TAG_ANY);
  stack_pop(JIT_R0, &stack_ptr);

  jit_prepare();
  jit_pushargr(JIT_R1); // object Cell*
  //jit_ldxi(JIT_R1, JIT_R0, sizeof(jit_word_t)); // buffer size
  jit_movi(JIT_R1, 1023);
  jit_ldr(JIT_R0, JIT_R0);
  jit_pushargr(JIT_R0); // buffer char*
  jit_pushargr(JIT_R1); // buffer size 
  jit_finishi(lisp_write);

  // FIXME
  //jit_retval(retreg); // return target buffer cell
  jit_movi(retreg, 0);
  return 1;
}

#include "utf8.c"
#include "compile_vector.c"
#include "compile_file_io.c"
#include "compile_input.c"
#include "compile_eval.c"
#include "compile_net.c"
#include "compile_gfx.c"
#include "compile_math.c"

// 0 = failure
// 1 = success
int compile_applic(int retreg, Cell* list, tag_t required) {
  jit_note("compile_applic",__LINE__);

  debug_current_expr = list;
  
  Cell* op_cell = car(list);
  char* fn_name = NULL;

  if (!op_cell) {
    printf("-- apply empty list\n");
    jit_movi(JIT_R0, 0); // will it crash? :3
    return 0;
  }
  
  int recursion = 0;

  if (op_cell->tag == TAG_SYM) {
    fn_name = op_cell->addr;

    if (fn_name && currently_compiling_fn_sym) {
      //printf("-- fn: %s currently_compiling_fn_sym: %s\n",fn_name,currently_compiling_fn_sym->addr);
    
      if (strcmp(currently_compiling_fn_sym->addr, fn_name) == 0) {
        // recursion!
        op_cell = currently_compiling_fn_op;
        recursion = 1;
      }
    }

    if (!recursion) {
      op_cell = lookup_symbol(fn_name, &global_env);
    }
    
    if (!op_cell) {
      // dynamic call?
      //return compile_dynamic_lambda(retreg, car(list), cdr(list), required, &global_env);

      char buf[128];
      lisp_write(list,buf,127);
      printf("<compile_applic: undefined symbol %s near '%s'>\n",fn_name,buf);
      jit_movi(JIT_R0, 0);
      return 0;
    }
  }
  else if (op_cell->tag == TAG_LAMBDA) {
    // direct lambda
    printf("~~ direct lambda\r\n");
  }
  else if (op_cell->tag == TAG_CONS) {
    printf("~~ cons\r\n");
    if (compile_applic(JIT_R0, op_cell, required)) {
      return compile_dynamic_lambda(retreg, JIT_R0, cdr(list), required, &global_env);
    } else {
      return 0;
    }
  }
  else {
    printf("<error:can only apply sym or lambda, got (tag:%d)>\n",op_cell->tag);
    jit_movi(JIT_R0, 0);
    return 0;
  }
  
  jit_word_t op = op_cell->value;
  
  if (op_cell->tag == TAG_LAMBDA) {
    if (recursion) {
      printf("-- compile lambda recursion %p\n",op_cell);
    }
    return compile_lambda(retreg, op_cell, cdr(list), required, &global_env, recursion);
  }

  Cell* args = cdr(list);
  
  switch (op) {
  case BUILTIN_ADD:
    return compile_add(retreg, args, required);
    break;
  case BUILTIN_SUB:
    return compile_sub(retreg, args, required);
    break;
  case BUILTIN_MUL:
    return compile_mul(retreg, args, required);
    break;
  case BUILTIN_DIV:
    return compile_div(retreg, args, required);
    break;
  case BUILTIN_MOD:
    return compile_mod(retreg, args, required);
    break;
  case BUILTIN_LT:
    return compile_lt(retreg, args, required);
    break;
  case BUILTIN_GT:
    return compile_gt(retreg, args, required);
    break;
    
  case BUILTIN_IF:
    return compile_if(retreg, args, required);
    break;
  case BUILTIN_WHILE:
    return compile_while(retreg, args, required);
    break;
  case BUILTIN_DO:
    return compile_do(retreg, args, required);
    break;
    
  case BUILTIN_FN:
    return compile_fn(retreg, args, required);
    break;

  case BUILTIN_DEF:
    return compile_def(retreg, args, required);
    break;
    
  case BUILTIN_QUOTE:
    return compile_quote(retreg, args, required);
    break;
    
  case BUILTIN_CAR:
    return compile_car(retreg, args, required);
    break;
  case BUILTIN_CDR:
    return compile_cdr(retreg, args, required);
    break;
  case BUILTIN_CONS:
    return compile_cons(retreg, args, required);
    break;
  case BUILTIN_LIST:
    return compile_list(retreg, args, required);
    break;
  case BUILTIN_MAP:
    return compile_map(retreg, args, required);
    break;
    
  case BUILTIN_ALLOC:
    return compile_alloc(retreg, args, required);
    break;
  case BUILTIN_ALLOC_STR:
    return compile_alloc_str(retreg, args, required);
    break;
  case BUILTIN_CONCAT:
    return compile_concat(retreg, args, required);
    break;
  case BUILTIN_SUBSTR:
    return compile_substr(retreg, args, required);
    break;

  case BUILTIN_WRITE:
    return compile_write(retreg, args, required);
    break;
    
  case BUILTIN_EVAL:
    return compile_eval(retreg, args, required);
    break;
    
  case BUILTIN_GET:
    return compile_get(retreg, args, required);
    break;
  case BUILTIN_PUT:
    return compile_put(retreg, args, required);
    break;
  case BUILTIN_SIZE:
    return compile_size(retreg, args, required);
    break;

  case BUILTIN_UGET:
    return compile_uget(retreg, args, required);
    break;
  case BUILTIN_UPUT:
    return compile_uput(retreg, args, required);
    break;
  case BUILTIN_USIZE:
    return compile_usize(retreg, args, required);
    break;
  
  case BUILTIN_PRINT:
    return compile_print(retreg, args, required);
    break;

  case BUILTIN_PIXEL:
    return compile_pixel(retreg, args);
    break;
  case BUILTIN_RECTFILL:
    return compile_rect_fill(retreg, args);
    break;
  case BUILTIN_FLIP:
    return compile_flip(retreg);
    break;
  case BUILTIN_BLIT:
    return compile_blit(retreg, args);
    break;
  case BUILTIN_BLIT_MONO:
    return compile_blit_mono(retreg, args);
    break;
  case BUILTIN_BLIT_MONO_INV:
    return compile_blit_mono_inv(retreg, args);
    break;
  case BUILTIN_BLIT_STRING:
    return compile_blit_string(retreg, args, required);
    break;

  case BUILTIN_SIN:
    return compile_sin(retreg, args, required);
    break;
  case BUILTIN_COS:
    return compile_cos(retreg, args, required);
    break;
  case BUILTIN_SQRT:
    return compile_sqrt(retreg, args, required);
    break;

  case BUILTIN_INKEY:
    return compile_get_key(retreg, args, required);
    break;
  case BUILTIN_GC:
    return compile_gc(retreg);
    break;
  case BUILTIN_SYMBOLS:
    return compile_symbol_list(retreg);
    break;
  case BUILTIN_LOAD:
    return compile_load(retreg, args, required);
    break;
  case BUILTIN_SAVE:
    return compile_save(retreg, args, required);
    break;

    // TODO refactor networking
    
  case BUILTIN_UDP_POLL:
    return compile_udp_poll(retreg, args);
    break;
  case BUILTIN_UDP_SEND:
    return compile_udp_send(retreg, args);
    break;
    
  case BUILTIN_TCP_CONNECT:
    return compile_tcp_connect(retreg, args);
    break;
  case BUILTIN_TCP_SEND:
    return compile_tcp_send(retreg, args);
    break;
  case BUILTIN_TCP_BIND:
    return compile_tcp_bind(retreg, args);
    break;
  }
  return 0;
}

void memdump(jit_word_t start,uint32_t len,int raw) {
  for (uint32_t i=0; i<len;) {
    if (!raw) printf("%08x | ",start+i);
    for (uint32_t x=0; x<16; x++) {
      printf("%02x ",*((uint8_t*)start+i+x));
    }
    if (!raw)
    for (uint32_t x=0; x<16; x++) {
      uint8_t c = *((uint8_t*)start+i+x);
      if (c>=32 && c<=128) {
        printf("%c",c);
      } else {
        printf(".");
      }
    }
    printf("\r\n");
    i+=16;
  }
  printf("\r\n\r\n");
}

void init_compiler() {

  //memdump(0x6f460,0x200,0);
  //uart_getc();
  
  //printf("malloc test: %p\r\n",malloc(1024));

  printf("[compiler] creating global env hash table…\r\n");
  global_env = sm_new(1000);

  printf("[compiler] init_allocator…\r\n");
  init_allocator();

  int_cell_regs = (Cell*)malloc(10*sizeof(Cell));
  for (int i=0; i<10; i++) {
    int_cell_regs[i].tag = TAG_INT;
    int_cell_regs[i].value = 0;
  }

  jit_state_stack = (void*)malloc(3*50*sizeof(void*));
  
  error_cell = alloc_error(0);

  printf("[compiler] inserting symbols…\r\n");
  
  insert_symbol(alloc_sym("+"), alloc_builtin(BUILTIN_ADD), &global_env);
  insert_symbol(alloc_sym("-"), alloc_builtin(BUILTIN_SUB), &global_env);
  insert_symbol(alloc_sym("*"), alloc_builtin(BUILTIN_MUL), &global_env);
  insert_symbol(alloc_sym("/"), alloc_builtin(BUILTIN_DIV), &global_env);
  insert_symbol(alloc_sym("%"), alloc_builtin(BUILTIN_MOD), &global_env);
  
  printf("[compiler] arithmetic…\r\n");
  
  insert_symbol(alloc_sym("lt"), alloc_builtin(BUILTIN_LT), &global_env);
  insert_symbol(alloc_sym("gt"), alloc_builtin(BUILTIN_GT), &global_env);
  
  printf("[compiler] compare…\r\n");
  
  insert_symbol(alloc_sym("if"), alloc_builtin(BUILTIN_IF), &global_env);
  insert_symbol(alloc_sym("while"), alloc_builtin(BUILTIN_WHILE), &global_env);
  insert_symbol(alloc_sym("def"), alloc_builtin(BUILTIN_DEF), &global_env);
  insert_symbol(alloc_sym("print"), alloc_builtin(BUILTIN_PRINT), &global_env);
  insert_symbol(alloc_sym("do"), alloc_builtin(BUILTIN_DO), &global_env);
  insert_symbol(alloc_sym("fn"), alloc_builtin(BUILTIN_FN), &global_env);
  
  printf("[compiler] flow…\r\n");
  
  insert_symbol(alloc_sym("quote"), alloc_builtin(BUILTIN_QUOTE), &global_env);
  insert_symbol(alloc_sym("car"), alloc_builtin(BUILTIN_CAR), &global_env);
  insert_symbol(alloc_sym("cdr"), alloc_builtin(BUILTIN_CDR), &global_env);
  insert_symbol(alloc_sym("cons"), alloc_builtin(BUILTIN_CONS), &global_env);
  insert_symbol(alloc_sym("list"), alloc_builtin(BUILTIN_LIST), &global_env);
  insert_symbol(alloc_sym("map"), alloc_builtin(BUILTIN_MAP), &global_env);

  printf("[compiler] lists…\r\n");
  
  insert_symbol(alloc_sym("concat"), alloc_builtin(BUILTIN_CONCAT), &global_env);
  insert_symbol(alloc_sym("substr"), alloc_builtin(BUILTIN_SUBSTR), &global_env);
  insert_symbol(alloc_sym("alloc"), alloc_builtin(BUILTIN_ALLOC), &global_env);
  insert_symbol(alloc_sym("alloc-str"), alloc_builtin(BUILTIN_ALLOC_STR), &global_env);

  printf("[compiler] strings…\r\n");
  
  insert_symbol(alloc_sym("get"), alloc_builtin(BUILTIN_GET), &global_env);
  insert_symbol(alloc_sym("uget"), alloc_builtin(BUILTIN_UGET), &global_env);
  insert_symbol(alloc_sym("put"), alloc_builtin(BUILTIN_PUT), &global_env);
  insert_symbol(alloc_sym("uput"), alloc_builtin(BUILTIN_UPUT), &global_env);
  insert_symbol(alloc_sym("size"), alloc_builtin(BUILTIN_SIZE), &global_env);
  insert_symbol(alloc_sym("usize"), alloc_builtin(BUILTIN_USIZE), &global_env);

  printf("[compiler] get/put…\r\n");
  
  insert_symbol(alloc_sym("write"), alloc_builtin(BUILTIN_WRITE), &global_env);
  insert_symbol(alloc_sym("eval"), alloc_builtin(BUILTIN_EVAL), &global_env);
  
  printf("[compiler] write/eval…\r\n");
  
  insert_symbol(alloc_sym("pixel"), alloc_builtin(BUILTIN_PIXEL), &global_env);
  insert_symbol(alloc_sym("rectfill"), alloc_builtin(BUILTIN_RECTFILL), &global_env);
  insert_symbol(alloc_sym("flip"), alloc_builtin(BUILTIN_FLIP), &global_env);
  insert_symbol(alloc_sym("blit"), alloc_builtin(BUILTIN_BLIT), &global_env);
  insert_symbol(alloc_sym("blit-mono"), alloc_builtin(BUILTIN_BLIT_MONO), &global_env);
  insert_symbol(alloc_sym("blit-mono-inv"), alloc_builtin(BUILTIN_BLIT_MONO_INV), &global_env);
  insert_symbol(alloc_sym("blit-string"), alloc_builtin(BUILTIN_BLIT_STRING), &global_env);
  insert_symbol(alloc_sym("inkey"), alloc_builtin(BUILTIN_INKEY), &global_env);
  
  printf("[compiler] graphics…\r\n");
  
  insert_symbol(alloc_sym("gc"), alloc_builtin(BUILTIN_GC), &global_env);
  insert_symbol(alloc_sym("symbols"), alloc_builtin(BUILTIN_SYMBOLS), &global_env);
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

  printf("[compiler] math.\r\n");
  
  //int num_syms=HASH_COUNT(global_env);
  int num_syms = sm_get_count(global_env);
  printf("sledge knows %u symbols. enter (symbols) to see them.\r\n", num_syms);
}
