
int compile_udp_poll(int retreg, Cell* args) {
  jit_prepare();
  jit_finishi(machine_poll_udp);
  jit_retval(retreg);
  
  return 1;
}

int compile_udp_send(int retreg, Cell* args) {
  jit_prepare();
  compile_arg(JIT_R0, car(args), TAG_ANY);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_send_udp);
  jit_retval(retreg);
  
  return 1;
}

int compile_tcp_connect(int retreg, Cell* args) {
  // Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell);
  
  compile_arg(JIT_R0, car(cdr(cdr(cdr(args)))), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(cdr(args))), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(args)), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(args), TAG_ANY);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_connect_tcp);
  jit_retval(retreg);
  
  return 1;
}

int compile_tcp_bind(int retreg, Cell* args) {
  jit_prepare();
  compile_arg(JIT_R0, car(args), TAG_ANY);
  jit_pushargr(JIT_R0);
  compile_arg(JIT_R0, car(cdr(args)), TAG_ANY);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_bind_tcp);
  jit_retval(retreg);
  
  return 1;
}

int compile_tcp_send(int retreg, Cell* args) {
  compile_arg(JIT_R0, car(args), TAG_ANY);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(machine_send_tcp);
  jit_retval(retreg);

  return 1;
}
