
void compile_udp_poll() {
  jit_prepare();
  jit_finishi(machine_poll_udp);
  jit_retval(JIT_R0);
}

void compile_udp_send(Cell* args) {
  jit_prepare();
  compile_arg(JIT_R0, car(args), 0, 0);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_send_udp);
  jit_retval(JIT_R0);
}

void compile_tcp_connect(Cell* args) {
  // Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell);
  
  compile_arg(JIT_R0, car(cdr(cdr(cdr(args)))), 0, 0);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(cdr(args))), 0, 0);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(args)), 0, 0);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(args), 0, 0);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_connect_tcp);
  jit_retval(JIT_R0);
}

void compile_tcp_bind(Cell* args) {
  jit_prepare();
  compile_arg(JIT_R0, car(args), 0, 0);
  jit_pushargr(JIT_R0);
  compile_arg(JIT_R0, car(cdr(args)), 0, 0);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_bind_tcp);
  jit_retval(JIT_R0);
}

void compile_tcp_send(Cell* args) {
  compile_arg(JIT_R0, car(args), 0, 0);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(machine_send_tcp);
  jit_retval(JIT_R0);
}
