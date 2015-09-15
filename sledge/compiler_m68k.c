Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
}

void memdump(void* start,uint32_t len,int raw);

int compile_for_platform(Cell* expr, Cell** res) {
  int codesz = 8192;
  int success = 0;
  register void* sp __asm ("sp");
  Frame empty_frame = {NULL, 0, 0, sp};
  
  code = malloc(codesz);
  
  memset(code, 0, codesz);

  jit_init();
  
  success = compile_expr(expr, &empty_frame, TAG_ANY);
  jit_ret();

  if (success) {
    printf("<assembled at: %p>\r\n",code);

    //memdump(code,64,0);

    /*if (bytes_read>codesz) {
      printf("<error: max assembly size of %d exhausted. aborting>\n",codesz);
      return 0;
      }*/

    *res = execute_jitted(code);
    //printf("res: %p\r\n",*res);
    success = 1;
    
  }
  return success;
}
