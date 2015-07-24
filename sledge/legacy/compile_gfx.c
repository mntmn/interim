
#define compile_int_arg(); args = cdr(args);\
  if (!car(args)) { printf("error: missing arguments.\n"); return 0; }\
  compile_arg(JIT_R0, car(args), TAG_PURE_INT);\
  stack_push(JIT_R0, &stack_ptr);

#define push_stack_arg(); stack_pop(JIT_R0, &stack_ptr);\
  jit_pushargr(JIT_R0);

int compile_rect_fill(int retreg, Cell* args) {
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

  jit_movi(retreg, 0);

  return 1;
}

int compile_pixel(int retreg, Cell* args) {
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

  jit_movi(retreg, 0);
  
  return 1;
}

int compile_flip(int retreg) {
  jit_prepare();
  jit_finishi(machine_video_flip);
  
  jit_movi(retreg, 0);

  return 1;
}

int compile_blit(int retreg, Cell* args) {
  compile_arg(JIT_R0, car(args), TAG_BYTES);
  //jit_ldr(JIT_R0, JIT_R0); // load bytes addr
  stack_push(JIT_R0, &stack_ptr);
  
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();

  jit_prepare();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  
  push_stack_arg(); // pop bytes addr
  jit_finishi(blit_vector32);
  
  jit_movi(retreg, 0);

  return 1;
}

int compile_blit_mono(int retreg, Cell* args) {
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
  
  jit_movi(retreg, 0);

  return 1;
}

int compile_blit_mono_inv(int retreg, Cell* args) {
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
  
  jit_movi(retreg, 0);

  return 1;
}

int compile_blit_string(int retreg, Cell* args, int requires) {
  compile_arg(JIT_R0, car(args), TAG_BYTES); // font
  stack_push(JIT_R0, &stack_ptr);

  args = cdr(args);
  compile_arg(JIT_R0, car(args), TAG_ANY); // string
  stack_push(JIT_R0, &stack_ptr);
  
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();

  jit_prepare();
  push_stack_arg(); // color
  push_stack_arg(); // h
  push_stack_arg(); // w
  push_stack_arg(); // y
  push_stack_arg(); // x
  push_stack_arg(); // cursor_pos (or -1)
  
  push_stack_arg(); // string
  push_stack_arg(); // font
  jit_finishi(blit_string1);
  jit_retval(retreg);

  box_int(retreg, requires);
  
  return 1;
}
