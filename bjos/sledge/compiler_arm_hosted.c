#define CODESZ 4096

int compile_for_platform(Cell* expr, Cell** res) {
  code = mmap(0, CODESZ, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  memset(code, 0, CODESZ);
  jit_init();
  cpool_idx = 128; // 128 ops gap
  code_idx = 0;

  int tag = compile_expr(expr, NULL);
  jit_ret();

  FILE* f = fopen("/tmp/test","w");
  fwrite(code, CODESZ, 1, f);
  fclose(f);

  int mp_res = mprotect(code, CODESZ, PROT_EXEC|PROT_READ);
  
  funcptr fn = (funcptr)code;
  *res = (Cell*)fn();
  printf("pointer result: %p\n",*res);
  //printf("pointer value: %p\n",((Cell*)*res)->value);

  return 1;
}
