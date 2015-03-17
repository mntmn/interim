
#define compile_int_arg(); args = cdr(args);\
  if (!car(args)) { printf("error: missing arguments.\n"); return 0; }\
  compile_arg(JIT_R0, car(args), TAG_PURE_INT);\
  stack_push(JIT_R0, &stack_ptr);

#define push_stack_arg(); stack_pop(JIT_R0, &stack_ptr);\
  jit_pushargr(JIT_R0);

int compile_rect_fill(Cell* args) {
  Cell* arg_x = car(args);
  if (!arg_x) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_y = car(args);
  if (!arg_y) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_w = car(args);
  if (!arg_w) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_h = car(args);
  if (!arg_h) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_c = car(args);
  if (!arg_c) return argnum_error("(rectfill x y w h color)");
  
  compile_arg(JIT_R0, arg_c, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_h, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_w, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_y, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_x, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);

  jit_prepare();
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_video_rect);

  return 1;
}

int compile_pixel(Cell* args) {
  Cell* arg_x = car(args);
  if (!arg_x) return argnum_error("(pixel x y color)");
  args = cdr(args);
  Cell* arg_y = car(args);
  if (!arg_y) return argnum_error("(pixel x y color)");
  args = cdr(args);
  Cell* arg_c = car(args);
  if (!arg_c) return argnum_error("(pixel x y color)");
  
  compile_arg(JIT_R0, arg_c, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_y, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_x, TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  
  jit_prepare();
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_video_set_pixel);

  return 1;
}

void compile_flip() {
  jit_prepare();
  jit_finishi(machine_video_flip);
}

void compile_blit(Cell* args) {
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(blit_vector32);
}

int compile_blit_mono(Cell* args) {
  compile_arg(JIT_R0, car(args), TAG_BYTES);
  jit_ldr(JIT_R0, JIT_R0); // load bytes addr
  stack_push(JIT_R0, &stack_ptr);
  
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();

  jit_prepare();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg(); // pop bytes addr
  jit_finishi(blit_vector1);
  jit_retval(JIT_R0);

  return 1;
}

int compile_blit_mono_inv(Cell* args) {
  compile_arg(JIT_R0, car(args), TAG_BYTES);
  jit_ldr(JIT_R0, JIT_R0); // load bytes addr
  stack_push(JIT_R0, &stack_ptr);
  
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();

  jit_prepare();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg(); // pop bytes addr
  jit_finishi(blit_vector1_invert);
  jit_retval(JIT_R0);

  return 1;
}
