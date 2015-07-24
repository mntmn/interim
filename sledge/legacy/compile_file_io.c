
int compile_load(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(load \"/local/path\")");
  Cell* arg = car(args);
  
  int success = compile_arg(JIT_R0, arg, TAG_ANY);
  if (!success) return 0;
  
  jit_ldr(JIT_R0, JIT_R0); // load string addr
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(machine_load_file); // returns bytes cell
  jit_retval(retreg);

  return 1;
}

int compile_save(int retreg, Cell* args, tag_t requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(save value \"/local/path\")");
  Cell* arg = car(args);
  
  int success = compile_arg(JIT_R0, car(cdr(args)), TAG_ANY);
  if (!success) return 0;
  
  jit_ldr(JIT_R0, JIT_R0); // load path addr
  stack_push(JIT_R0, &stack_ptr);
  
  success = compile_arg(JIT_R0, arg, TAG_ANY);
  // FIXME
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_save_file); // returns bytes cell
  jit_retval(JIT_R0);
  
  return 1;
}
