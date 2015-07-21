
char* regnames[] = {
  "%rax",
  "%rdi",
  "%rsi",
  "%rdx",
  /*"%r8",
  "%r9",
  "%r10",
  "%r11",*/
  "%r12",
  "%r13",
  "%r14",
  "%r15",
};

enum jit_reg {
  R0 = 0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  R8
};
enum arg_reg {
  ARGR0 = 1,
  ARGR1 = 2,
  ARGR2 = 3
};

void jit_init() {
}

void jit_movi(int reg, void* imm) {
  fprintf(jit_out, "movq $%p, %s\n", imm, regnames[reg]);
}

void jit_movr(int dreg, int sreg) {
  fprintf(jit_out, "movq %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_movneg(int dreg, int sreg) {
  fprintf(jit_out, "cmovs %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_movne(int dreg, int sreg) {
  fprintf(jit_out, "cmovne %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_lea(int reg, void* addr) {
  fprintf(jit_out, "mov $%p, %s\n", addr, regnames[reg]);
}

void jit_ldr(int reg) {
  fprintf(jit_out, "mov (%s), %s\n", regnames[reg], regnames[reg]);
}

void jit_ldr_stack(int dreg, int offset) {
  fprintf(jit_out, "mov %d(%%rsp), %s\n", offset, regnames[dreg]);
}

// clobbers rdx!
void jit_ldrb(int reg) {
  fprintf(jit_out, "movb (%s), %%dl\n", regnames[reg]);
  fprintf(jit_out, "andq $0xff, %rdx\n", regnames[reg]);
  if (reg!=3) {
    fprintf(jit_out, "movq %%rdx, %s\n", regnames[reg]);
  }
}

// clobbers rdx!
void jit_ldrw(int reg) {
  fprintf(jit_out, "movl (%s), %%edx\n", regnames[reg]);
  //fprintf(jit_out, "andq $0xffffffff, %rdx\n", regnames[reg]);
  if (reg!=3) {
    fprintf(jit_out, "movq %%rdx, %s\n", regnames[reg]);
  }
}

// 8 bit only from rdx!
void jit_strb(int reg) {
  fprintf(jit_out, "movb %%dl, (%s)\n", regnames[reg]);
}

// 32 bit only from rdx!
void jit_strw(int reg) {
  fprintf(jit_out, "movl %%edx, (%s)\n", regnames[reg]);
}

void jit_addr(int dreg, int sreg) {
  fprintf(jit_out, "addq %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_addi(int dreg, int imm) {
  fprintf(jit_out, "addq $%d, %s\n", imm, regnames[dreg]);
}

void jit_subr(int dreg, int sreg) {
  fprintf(jit_out, "subq %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_mulr(int dreg, int sreg) {
  fprintf(jit_out, "imulq %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_divr(int dreg, int sreg) {
  fprintf(jit_out, "movq %s, %%rax\n", regnames[dreg]);
  fprintf(jit_out, "cqto\n");
  fprintf(jit_out, "idivq %s\n", regnames[sreg]);
  fprintf(jit_out, "movq %%rax, %s\n", regnames[dreg]);
}

void jit_call(void* func, char* note) {
  fprintf(jit_out, "mov $%p, %%rax\n", func);
  fprintf(jit_out, "callq *%%rax # %s\n", note);
}

void jit_cmpi(int sreg, int imm) {
  fprintf(jit_out, "cmp $%d, %s\n", imm, regnames[sreg]);
}

void jit_cmpr(int sreg, int dreg) {
  fprintf(jit_out, "cmp %s, %s\n", regnames[dreg], regnames[sreg]);
}

void jit_je(char* label) {
  fprintf(jit_out, "je %s\n", label);
}

void jit_jneg(char* label) {
  fprintf(jit_out, "js %s\n", label);
}

void jit_jmp(char* label) {
  fprintf(jit_out, "jmp %s\n", label);
}

void jit_label(char* label) {
  fprintf(jit_out, "%s:\n", label);
}

void jit_ret() {
  fprintf(jit_out, "ret\n");
}

void jit_push(int r1, int r2) {
  for (int i=r1; i<=r2; i++) {
    fprintf(jit_out, "push %s\n",regnames[i]);
  }
}

void jit_pop(int r1, int r2) {
  for (int i=r2; i>=r1; i--) {
    fprintf(jit_out, "pop %s\n",regnames[i]);
  }
}
