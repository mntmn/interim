
jit_word_t vec_get(Cell* vec, jit_word_t position) {
  if (!vec || (vec->tag!=TAG_STR && vec->tag!=TAG_BYTES)) return 0;
  if (position>=vec->size || position<0) return 0;
  return ((uint8_t*)vec->addr)[position];
}

jit_word_t vec_put(Cell* vec, jit_word_t position, jit_word_t value) {
  if (!vec || (vec->tag!=TAG_STR && vec->tag!=TAG_BYTES)) return 0;
  if (position>=vec->size || position<0) return 0;
  uint8_t v = (uint8_t)value;
  ((uint8_t*)vec->addr)[position] = v;

  return v;
}

// vectors/strings
int compile_get(int retreg, Cell* args, int requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(get bytes-or-string index)");
  Cell* arg = car(args);

  // TODO: tag + bounds check
  
  compile_arg(JIT_R0, arg, TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R1, car(cdr(args)), TAG_PURE_INT);
  stack_pop(JIT_R0, &stack_ptr);

  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(vec_get);
  jit_retval(retreg);

  // fetch size
  // jit_ldxi(JIT_R2, JIT_R0, sizeof(jit_word_t));
  // jit_node_t* jump = jit_bltr(JIT_R2, JIT_R1);
  
  //jit_ldr(JIT_R0, JIT_R0); // car r0 = r0->addr
  //jit_ldxr_uc(JIT_R0, JIT_R0, JIT_R1); // *(r0 + r1) -> r0
  
  return box_int(retreg, requires);
}

int compile_put(int retreg, Cell* args, int requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(put bytes-or-string index value)");
  Cell* arg = car(args);

  // TODO: tag + bounds check
  // TODO: optimize
  
  compile_arg(JIT_R0, car(cdr(cdr(args))), TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(args)), TAG_PURE_INT);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg, TAG_ANY);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(vec_put);
  jit_retval(retreg);
  
  /*jit_ldr(JIT_R0, JIT_R0); // car r0 = r0->addr
  jit_addr(JIT_R0, JIT_R0, JIT_R1);
  compile_arg(JIT_R1, car(cdr(cdr(args))), 1, 1);
  jit_str_c(JIT_R0, JIT_R1); // *(r0 + r1) -> r0 */

  return box_int(retreg, requires);
}

int compile_size(int retreg, Cell* args, int requires) {
  if (!car(args)) return argnum_error("(size bytes/string)");
  Cell* arg = car(args);

  printf("++ potential crash: unsafe (size)\n");

  // FIXME: will crash with NULL
  
  compile_arg(JIT_R0, arg, TAG_ANY);
  jit_ldxi(retreg, JIT_R0, sizeof(jit_word_t)); // cdr r0 = r0 + one word = r0->next
  
  return box_int(retreg, requires);
}


// utf8 char get
int compile_uget(int retreg, Cell* args, int requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(uget string index)");
  Cell* arg = car(args);

  compile_arg(JIT_R0, arg, TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(args)), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);

  
  jit_prepare();
  stack_pop(JIT_R1, &stack_ptr);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(utf8_rune_at_cell);
  jit_retval(retreg);

  return box_int(retreg, requires);
}

// utf8 char put
int compile_uput(int retreg, Cell* args, int requires) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(uput string index rune)");
  Cell* arg = car(args);

  compile_arg(JIT_R0, car(cdr(cdr(args))), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, car(cdr(args)), TAG_ANY);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg, TAG_ANY);

  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(utf8_put_rune_at); // checks tag + bounds
  jit_retval(retreg);
  
  return box_int(retreg, requires);
}

// utf8 string length
int compile_usize(int retreg, Cell* args, int requires) {
  if (!car(args)) return argnum_error("(usize string)");
  Cell* arg = car(args);

  compile_arg(JIT_R0, arg, TAG_ANY);
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(utf8_strlen_cell); // this checks the tag
  jit_retval(retreg);
  
  return box_int(retreg, requires);
}
