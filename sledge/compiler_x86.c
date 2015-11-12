
#ifndef WIN32
#include <sys/mman.h>
#endif

Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
}

//void memdump(void* start,uint32_t len,int raw);

Cell* compile_for_platform(Cell* expr, Cell** res) {
  int codesz = 8192;

#ifdef WIN32
  uint8_t* jit_binary = malloc(codesz);
#else
  uint8_t* jit_binary = mmap(0, codesz, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
#endif
  
  printf("jit_binary: %p\r\n",jit_binary);
  
  memset(jit_binary, 0, codesz);

  jit_init(jit_binary, codesz);
  
  register void* sp asm ("sp");
  Frame* empty_frame = malloc(sizeof(Frame)); // FIXME leak
  empty_frame->f=NULL;
  empty_frame->sp=0;
  empty_frame->locals=0;
  empty_frame->stack_end=sp;
  empty_frame->parent_frame=NULL;

  Cell* success = compile_expr(expr, empty_frame, prototype_any);
  
  jit_ret();

  if (success) {
    printf("<assembled at: %p>\r\n",jit_binary);

    //memdump(code,64,0);

    //FILE* f = fopen("/tmp/jit.x86","w+");
    //fwrite(code, 1, codesz, f);
    //fclose(f);
    
#ifndef WIN32
    int mp_res = mprotect(jit_binary, codesz, PROT_EXEC|PROT_READ);
#endif
    *res = execute_jitted(jit_binary);
  }
  return success;
}
