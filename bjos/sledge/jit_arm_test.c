#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "jit_arm_raw.c"
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>

#define CODESZ 1024*4

typedef uint32_t (*funcptr)();

void hello() {
  printf("hello!\n");
}

int main() {
  code = mmap(0, CODESZ, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  memset(code, 0, CODESZ);
  cpool_idx = 128; // 128 ops gap

  jit_movi(8, 0xdead0000);
  jit_call(hello,"printf");
  jit_movi(8, 0xdead0000);
  jit_movi(7, 0x0000beef);
  jit_addr(7,8);
  jit_movr(0,8);
  jit_movneg(0,7);
  jit_movr(15,14); // ret

  jit_movr(1,3);
  jit_movne(9,8);
  jit_movi(1, 0xcafebabe);
  jit_lea(1, code);
  jit_ldr(1);
  jit_ldrb(1);
  jit_strb(2);
  jit_strw(3);
  jit_addr(4,1);
  jit_subr(4,1);
  jit_mulr(4,1);
  jit_movi(2, 0xaaaaffff);
  jit_movi(3, 0x12345678);

  FILE* f = fopen("/tmp/test","w");
  fwrite(code, CODESZ, 1, f);
  fclose(f);

  int mp_res = mprotect(code, CODESZ, PROT_EXEC|PROT_READ);

  funcptr fn = (funcptr)code;
  uint32_t res = fn();
  printf("asm result: %lx\n",res);

  //free(code);
  //munmap(code);
}
