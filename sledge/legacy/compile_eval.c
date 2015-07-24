jit_word_t compile_compile(Cell* expr) {
  if (!expr) return (jit_word_t)alloc_nil();
  if (expr->tag!=TAG_STR && expr->tag!=TAG_BYTES) return (jit_word_t)alloc_error(ERR_INVALID_PARAM_TYPE);
  
  Cell* read_expr = read_string(expr->addr);
  if (!read_expr) return (jit_word_t)alloc_error(ERR_APPLY_NIL);

  int done = 0;
  int step_by_step = 0;

  Cell* compile_expr = read_expr;
  
  if (read_expr->tag == TAG_CONS && car(read_expr)->tag == TAG_CONS) {
    printf("-- eval top-level list, compiling step-by-step\r\n");
    
    /*char* buf = malloc(20000);
    memset(buf,0,20000);
    lisp_write(compile_expr, buf, 19999);
    printf("~~ %s ~~\r\n",buf);*/
      
    step_by_step = 1;
  }

  Cell* exec_res = NULL;
    
  while (!done) {
    if (step_by_step) {
      compile_expr = car(read_expr);
      /*static char buf[1024];
      memset(buf,0,1024);
      lisp_write(compile_expr, buf, 1023);
      printf("~~ s-b-s compiling %s\r\n",buf);*/
    }
  
    if (compile_expr) {
      Cell* res = alloc_lambda(alloc_nil());
  
      push_jit_state();
      _jit = jit_new_state();
      jit_node_t* fn_label = jit_label();
      jit_prolog();
  
      //stack_ptr = stack_base = jit_allocai(32 * sizeof(jit_word_t));
      
      jit_node_t* fn_body_label = jit_label();

      int success = compile_arg(JIT_R0, compile_expr, TAG_ANY);

      if (success) {
        jit_retr(JIT_R0);
        jit_epilog();

        res->next = jit_emit();
      }
  
      jit_clear_state();
      //jit_destroy_state();
      pop_jit_state();

      if (success) {
        funcptr fn = (funcptr)res->next;
        if (fn) {
          //printf("would jump to fn %p\r\n",fn);
          exec_res = (Cell*)fn();
        }
      } else {
        return 0;
      }
      
    } else {
      //done = 1;
    }
    
    if (step_by_step) {
      read_expr = cdr(read_expr);
      if (!read_expr) done = 1;
    } else {
      done = 1;
    }
  }
  
  return (jit_word_t)exec_res;
}

typedef jit_word_t (*funcptr)();

// eval
int compile_eval(int retreg, Cell* args, tag_t requires) {
  Cell* arg = car(args);
  if (!arg) return argnum_error("(eval string)");
  
  compile_arg(JIT_R0, arg, TAG_ANY);

  // compile s-exp
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(compile_compile);
  jit_retval(retreg);

  return 1;
}

