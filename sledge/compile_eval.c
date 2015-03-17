jit_word_t compile_compile(Cell* expr) {
  if (!expr) return (jit_word_t)alloc_nil();
  if (expr->tag!=TAG_STR && expr->tag!=TAG_BYTES) return (jit_word_t)alloc_error(ERR_INVALID_PARAM_TYPE);
  
  Cell* read_expr = read_string(expr->addr);
  if (!read_expr) return (jit_word_t)alloc_error(ERR_APPLY_NIL);

  push_jit_state();
  
  _jit = jit_new_state();
  jit_node_t* fn_label = jit_label();
  jit_prolog();
  
  stack_ptr = stack_base = jit_allocai(1024 * sizeof(int));
  
  jit_node_t* fn_body_label = jit_label();

  Cell* res = alloc_lambda(alloc_nil());
  
  compile_arg(JIT_R0, read_expr, TAG_ANY);
  
  jit_retr(JIT_R0);
  jit_epilog();

  res->next = jit_emit();
  
  jit_clear_state();
  
  pop_jit_state();

  funcptr fn = (funcptr)res->next;
  
  return fn();
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

