
int compile_sin(int retreg, Cell* args, int requires) {
  if (!car(args)) return argnum_error("(sin rad*10000)");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, arg, TAG_PURE_INT);
  jit_extr_d(JIT_F0, JIT_R0); // F0 = (float)R0
  jit_divi_d(JIT_F0, JIT_F0, 10000.0); // F0 = R0/10000

  jit_prepare();
  jit_pushargr_d(JIT_F0);
  jit_finishi(sin);
  jit_retval_d(JIT_F0);

  jit_muli_d(JIT_F0, JIT_F0, 10000.0); // F0 = 10000*F0
  jit_truncr_d_i(retreg, JIT_F0); // retreg = (float)F0

  return box_int(retreg, requires);
}

int compile_cos(int retreg, Cell* args, int requires) {
  if (!car(args)) return argnum_error("(cos rad*10000)");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, arg, TAG_PURE_INT);
  jit_extr_d(JIT_F0, JIT_R0); // F0 = (float)R0
  jit_divi_d(JIT_F0, JIT_F0, 10000.0); // F0 = R0/10000

  jit_prepare();
  jit_pushargr_d(JIT_F0);
  jit_finishi(cos);
  jit_retval_d(JIT_F0);

  jit_muli_d(JIT_F0, JIT_F0, 10000.0); // F0 = 10000*F0
  jit_truncr_d_i(retreg, JIT_F0); // retreg = (float)F0

  return box_int(retreg, requires);
}

int compile_sqrt(int retreg, Cell* args, int requires) {
  if (!car(args)) return argnum_error("(sqrt 36)");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, arg, TAG_PURE_INT);
  jit_extr_d(JIT_F0, JIT_R0); // F0 = (float)R0

  /*jit_prepare();
  jit_pushargr_d(JIT_F0);
  jit_finishi(sqrt);
  jit_retval_d(JIT_F0);*/

  jit_truncr_d_i(retreg, JIT_F0); // retreg = (float)F0

  return box_int(retreg, requires);
}
