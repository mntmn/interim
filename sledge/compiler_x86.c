#include <sys/mman.h>

Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
}

//void memdump(void* start,uint32_t len,int raw);

int compile_for_platform(Cell* expr, Cell** res) {
  int codesz = 8192;
  
  uint8_t* jit_binary = mmap(0, codesz, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);

  printf("jit_binary: %p\r\n",jit_binary);
  
  memset(jit_binary, 0, codesz);

  jit_init(jit_binary, codesz);
  
  register void* sp asm ("sp");
  Frame empty_frame = {NULL, 0, 0, sp};
  int success = compile_expr(expr, &empty_frame, TAG_ANY);
  jit_ret();

  if (success) {
    printf("<assembled at: %p>\r\n",jit_binary);

    //memdump(code,64,0);

    //FILE* f = fopen("/tmp/jit.x86","w+");
    //fwrite(code, 1, codesz, f);
    //fclose(f);
    
    int mp_res = mprotect(jit_binary, codesz, PROT_EXEC|PROT_READ);
    *res = execute_jitted(jit_binary);
    //printf("res: %p\r\n",res);
    success = 1;
    
  }
  return success;
}
