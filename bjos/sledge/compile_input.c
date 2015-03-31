int compile_get_key(int retreg, Cell* args, tag_t requires) {
  if (!car(args)) return argnum_error("(inkey 0=keycode|1=modifiers)");
  compile_arg(JIT_R0, car(args), TAG_PURE_INT);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(machine_get_key);
  jit_retval(retreg);

  if (requires != TAG_PURE_INT) {
    return box_int(retreg, requires);
  }

  return 1;
}
