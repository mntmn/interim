
#include <stdint.h>

char* regnames[] = {
  "r0",
  "r1",
  "r2",
  "r3",
  "r4",
  "r5",
  "r6"
};

static uint32_t* code;
static uint32_t code_idx;
static uint32_t cpool_idx; // constant pool

FILE* jit_out;

/*

   31-28 cond
   27: 0
   26: 1
   25: 0
   24: P (1 = offset addressing)
   23: U (1 = +, 0 = -)
   22: 0 (1 = byte access)
   21: W (0 = offset addressing)
   20: L (0 = store 1 = load)

 */


void jit_movi(int reg, uint32_t imm) {
  // load from constant pool
  // template: 0b11000101100100000000000000000000
  // 0x5900000
  uint32_t op = 0xe5900000;
  op |= (15<<16); // base reg = pc
  op |= (reg<<12); // dreg
  op |= ((cpool_idx-code_idx-2)*4);
  *(code+cpool_idx) = imm; // store in constant pool

  cpool_idx++;

  code[code_idx] = op;
  code_idx++;
  
  //fprintf(jit_out, "movq $%p, %s\n", imm, regnames[reg]);
}

void jit_movr(int dreg, int sreg) {
  uint32_t op = 0xe1a00000;
  op |= (sreg<<0); // base reg = pc
  op |= (dreg<<12); // dreg
  
  code[code_idx++] = op;
}

void jit_movneg(int dreg, int sreg) {
  
  uint32_t op = 0x41a00000;
  op |= (sreg<<0); // base reg = pc
  op |= (dreg<<12); // dreg
  
  code[code_idx++] = op;
  //fprintf(jit_out, "cmovs %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_movne(int dreg, int sreg) {
  
  uint32_t op = 0x11a00000;
  op |= (sreg<<0); // base reg = pc
  op |= (dreg<<12); // dreg
  
  code[code_idx++] = op;
  //fprintf(jit_out, "cmovne %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_lea(int reg, void* addr) {
  jit_movi(reg, (uint32_t)addr);
}

void jit_ldr(int reg) {
  uint32_t op = 0xe5900000;
  op |= (reg<<16); // base reg = pc
  op |= (reg<<12); // dreg
  code[code_idx++] = op;
}

// 8 bit only from rdx! (R3)
void jit_ldrb(int reg) {
  uint32_t op = 0xe5900000;
  op |= 1<<22; // byte access
  op |= (3<<16); // r3
  op |= (reg<<12); // dreg
  code[code_idx++] = op;
}

// 8 bit only from rdx! (R3)
void jit_strb(int reg) {
  uint32_t op = 0xe5800000;
  op |= 1<<22; // byte access
  op |= (3<<16); // r3
  op |= (reg<<12); // dreg
  code[code_idx++] = op;
}

// 32 bit only from rdx!
void jit_strw(int reg) {
  uint32_t op = 0xe5800000;
  op |= (3<<16); // r3
  op |= (reg<<12); // dreg
  code[code_idx++] = op;
}

void jit_addr(int dreg, int sreg) {
  uint32_t op = 0xe0900000;
  op |= (sreg<<0);
  op |= (dreg<<12);
  op |= (dreg<<16);
  code[code_idx++] = op;
}

void jit_addi(int dreg, int imm) {
  jit_movi(11,imm);
  jit_addr(dreg,11);
}

void jit_subr(int dreg, int sreg) {
  uint32_t op = 0xe0500000;
  op |= (sreg<<0);
  op |= (dreg<<12);
  op |= (dreg<<16);
  code[code_idx++] = op;
}

void jit_mulr(int dreg, int sreg) {
  uint32_t op = 0xe0100090;
  op |= (sreg<<8);
  op |= (dreg<<0);
  op |= (dreg<<16);
  code[code_idx++] = op;
}

void jit_divr(int dreg, int sreg) {
  // call the c lib function
  // later: http://thinkingeek.com/2013/08/11/arm-assembler-raspberry-pi-chapter-15/
}

void jit_call(void* func, char* note) {
  code[code_idx++] = 0xe92d4000; // stmfd	sp!, {lr}
  jit_movr(14,15);
  jit_lea(15,func);
  code[code_idx++] = 0xe8bd4000; // ldmfd	sp!, {lr}
  
  //fprintf(jit_out, "mov $%p, %%rax\n", func);
  //fprintf(jit_out, "callq *%%rax # %s\n", note);
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
  jit_movr(15,14); // lr -> pc
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
