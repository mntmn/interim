#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "jit_arm_raw.c"

#define CODESZ 1024*4

int main() {
  code = malloc(CODESZ);
  memset(code, 0, CODESZ);
  cpool_idx = 128; // 128 ops gap

  jit_movi(0, 0xdeadbeef);
  jit_movr(1,3);
  jit_movneg(9,8);
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
  jit_call(printf,"printf");
  jit_movi(2, 0xaaaaffff);
  jit_movi(3, 0x12345678);

  FILE* f = fopen("/tmp/test","w");
  fwrite(code, CODESZ, 1, f);
  fclose(f);

  free(code);
}
