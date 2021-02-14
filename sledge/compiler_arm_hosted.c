#define CODESZ 8192
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DEBUG

int compile_for_platform(Cell* expr, Cell** res) {
  code = mmap(0, CODESZ, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  memset(code, 0, CODESZ);
  jit_init(0x200); // FIXME
  //cpool_idx = CODESZ/2; // 128 ops gap
  //code_idx = 0;

  register void* sp asm ("sp");
  Frame* empty_frame = malloc(sizeof(Frame)); // FIXME leak
  empty_frame->f=NULL;
  empty_frame->sp=0;
  empty_frame->locals=0;
  empty_frame->stack_end=sp;
  empty_frame->parent_frame=NULL;
  
  int tag = compile_expr(expr, empty_frame, prototype_any);
  jit_ret();

  FILE* f = fopen("/tmp/test","w");
  fwrite(code, CODESZ, 1, f);
  fclose(f);

  // disassemble
#ifdef DEBUG
  system("objdump -D -b binary -m aarch64 /tmp/test >/tmp/disasm");
  int fd = open("/tmp/disasm",O_RDONLY);
  char buf[1024];
  int buflen;
  while((buflen = read(fd, buf, 1024)) > 0)
  {
    write(1, buf, buflen);
  }
  close(fd);
#endif

  int mp_res = mprotect(code, CODESZ, PROT_EXEC|PROT_READ);
  
  funcptr fn = (funcptr)code;
  *res = (Cell*)fn();
  //printf("pointer result: %p\n",*res);
  //printf("pointer value: %p\n",((Cell*)*res)->value);

  return 1;
}
