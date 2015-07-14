
void jit_movi(int reg, int imm) {
  fprintf(jit_out, "ldr r%d, =%d\n", reg, imm);
}

void jit_movr(int dreg, int sreg) {
  fprintf(jit_out, "mov r%d, r%d\n", dreg, sreg);
}

void jit_ldr(int reg, void* addr) {
  fprintf(jit_out, "ldr r%d, =%p\n",  reg, addr);
  fprintf(jit_out, "ldr r%d, [r%d]\n", reg, reg);
}

void jit_lea(int reg, void* addr) {
  fprintf(jit_out, "ldr r%d, =%p\n", reg, addr);
}

void jit_addr(int dreg, int sreg) {
  fprintf(jit_out, "add r%d, r%d, r%d\n", dreg, sreg, sreg);
}

void jit_call(void* func, char* note) {
  fprintf(jit_out, "ldr lr, =%p // %s\n",func,note);
  fprintf(jit_out, "bx lr\n");
}

void jit_push(int r1, int r2) {
  if (r1==r2) {
    fprintf(jit_out, "push {r%d}\n",r1);
  } else {
    fprintf(jit_out, "push {r%d-r%d}\n",r1,r2);
  }
}

void jit_pop(int r1, int r2) {
  if (r1==r2) {
    fprintf(jit_out, "pop {r%d}\n",r1);
  } else {
    fprintf(jit_out, "pop {r%d-r%d}\n",r2,r1);
  }
}
