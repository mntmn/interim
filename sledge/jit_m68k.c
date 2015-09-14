
char* regnames[] = {
  "d0",
  "d1",
  "d2",
  "d3",
  "d4",
  "d5",
  "d6",
  "d7",
  "a0",
  "a1",
  "a2",
  "a3",
  "a4",
  "a5",
  "a6",
  "a7"  // stack pointer
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
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15
};

enum arg_reg {
  ARGR0 = 0,
  ARGR1 = 1,
  ARGR2 = 2
};

uint8_t regi[] = {
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  14,
  15
};

#define RSP R15

static uint8_t* code;
static uint32_t code_idx;

typedef struct Label {
  char* name;
  uint32_t idx;
} Label;

#define JIT_MAX_LABELS 32
static int label_idx = 0;
static Label jit_labels[JIT_MAX_LABELS];
static Label jit_labels_unres[JIT_MAX_LABELS]; // unresolved (forward) labels
static int unres_labels = 0;


void jit_init() {
  int i;
  // cleans up jit state
  label_idx = 0;
  unres_labels = 0;
  code_idx = 0;
  
  for (i=0; i<JIT_MAX_LABELS; i++) {
    if (jit_labels[i].name) free(jit_labels[i].name);
    jit_labels[i].name = NULL;
    jit_labels[i].idx = 0;
    if (jit_labels_unres[i].name) free(jit_labels_unres[i].name);
    jit_labels_unres[i].name = NULL;
    jit_labels_unres[i].idx = 0;
  }
}

void jit_movi(int reg, int imm) {
  code[code_idx++] = 0x20 | regi[reg]<<1;
  code[code_idx++] = 0x3c;
  
  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;
}

void jit_movr(int dreg, int sreg) {
  code[code_idx++] = 0x20 | regi[dreg]<<1;
  code[code_idx++] = 0x00 | regi[sreg];
}

void jit_movneg(int dreg, int sreg) {
}

void jit_movne(int dreg, int sreg) {
}

void jit_moveq(int dreg, int sreg) {
}

void jit_lea(int reg, void* addr) {
  jit_movi(reg, (uint32_t)addr);
}

void jit_ldr(int reg) {
  code[code_idx++] = 0x20;
  code[code_idx++] = 0x40|regi[reg];
  code[code_idx++] = 0x20|(regi[reg]<<1);
  code[code_idx++] = 0x10;
}

void jit_ldr_stack(int dreg, int offset) {
}

void jit_str_stack(int sreg, int offset) {
}

void jit_inc_stack(int offset) {
}

void jit_dec_stack(int offset) {
}

// clobbers rdx!
void jit_ldrb(int reg) {
  code[code_idx++] = 0x10; // moveb @(00000000,%d0:l),%d0
  code[code_idx++] = 0x30;
  code[code_idx++] = 0x09;
  code[code_idx++] = 0x90;
}

void jit_ldrw(int reg) {

}

// 8 bit only from R3! (d3)
void jit_strb(int dreg) {
  code[code_idx++] = 0x11; //
  code[code_idx++] = 0x83; // the 3 is d3
  code[code_idx++] = 0x09 | regi[dreg]<<4;
  code[code_idx++] = 0x90;
  
  //fprintf(jit_out, "movb %%dl, (%s)\n", regnames[reg]);
}

// 32 bit only from rdx!
void jit_strw(int reg) {
  //fprintf(jit_out, "movl %%edx, (%s)\n", regnames[reg]);
}

void jit_addr(int dreg, int sreg) {
  code[code_idx++] = 0xd0|regi[dreg]<<1;
  code[code_idx++] = 0x80|regi[sreg];
}

void jit_addi(int dreg, int imm) {
  code[code_idx++] = 0x06;
  code[code_idx++] = 0x80;
  
  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;
}

void jit_andr(int dreg, int sreg) {
  code[code_idx++] = 0xc0|regi[dreg]<<1;
  code[code_idx++] = 0x80|regi[sreg];
}

void jit_orr(int dreg, int sreg) {
  code[code_idx++] = 0x80|regi[dreg]<<1;
  code[code_idx++] = 0x80|regi[sreg];
}

void jit_xorr(int dreg, int sreg) {
  code[code_idx++] = 0xb1|regi[sreg]<<1;
  code[code_idx++] = 0x80|regi[dreg];
}

void jit_shrr(int dreg, int sreg) {
  code[code_idx++] = 0xe1|regi[sreg]<<1;
  code[code_idx++] = 0xa8|regi[dreg];
}

void jit_shlr(int dreg, int sreg) {
  code[code_idx++] = 0xe0|regi[sreg]<<1;
  code[code_idx++] = 0xa8|regi[dreg];
}

void jit_subr(int dreg, int sreg) {
  code[code_idx++] = 0x90|regi[dreg]<<1;
  code[code_idx++] = 0x80|regi[sreg];
}

void jit_mulr(int dreg, int sreg) {
  //fprintf(jit_out, "imulq %s, %s\n", regnames[sreg], regnames[dreg]);
}

void jit_divr(int dreg, int sreg) {
  /*fprintf(jit_out, "movq %s, %%rax\n", regnames[dreg]);
  fprintf(jit_out, "cqto\n");
  fprintf(jit_out, "idivq %s\n", regnames[sreg]);
  fprintf(jit_out, "movq %%rax, %s\n", regnames[dreg]);*/
}

void jit_call(void* func, char* note) {
  uint32_t imm = (uint32_t)func;

  code[code_idx++] = 0x2f; // move d0, -(sp) 
  code[code_idx++] = 0x00; // d0
  
  code[code_idx++] = 0x20; // move function pointer to a0
  code[code_idx++] = 0x7c;

  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;

  code[code_idx++] = 0x4e; // jsr (a0)
  code[code_idx++] = 0x90;

  code[code_idx++] = 0x58; // addq.l #4, sp
  code[code_idx++] = 0x8f; 
}

void jit_call2(void* func, char* note) {
  uint32_t imm = (uint32_t)func;

  code[code_idx++] = 0x2f; // move d1, -(sp) 
  code[code_idx++] = 0x01; // d1
  code[code_idx++] = 0x2f; // move d0, -(sp) 
  code[code_idx++] = 0x00; // d0
  
  code[code_idx++] = 0x20; // move function pointer to a0
  code[code_idx++] = 0x7c;

  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;

  code[code_idx++] = 0x4e; // jsr (a0)
  code[code_idx++] = 0x90;
  
  code[code_idx++] = 0x50; // addq.l #8, sp
  code[code_idx++] = 0x8f; 
}

void jit_call3(void* func, char* note) {
  uint32_t imm = (uint32_t)func;

  code[code_idx++] = 0x2f; // move d2, -(sp) 
  code[code_idx++] = 0x02; // d2
  code[code_idx++] = 0x2f; // move d1, -(sp) 
  code[code_idx++] = 0x01; // d1
  code[code_idx++] = 0x2f; // move d0, -(sp) 
  code[code_idx++] = 0x00; // d0
  
  code[code_idx++] = 0x20; // move function pointer to a0
  code[code_idx++] = 0x7c;

  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;

  code[code_idx++] = 0x4e; // jsr (a0)
  code[code_idx++] = 0x90;
  
  code[code_idx++] = 0x50; // addq.l #8, sp
  code[code_idx++] = 0x8f; 
  code[code_idx++] = 0x58; // addq.l #4, sp
  code[code_idx++] = 0x8f; 
}

void jit_callr(int dreg) {
  code[code_idx++] = 0x20 | regi[dreg]; // move dx, a0
  code[code_idx++] = 0x40;

  code[code_idx++] = 0x4e; // jsr (a0)
  code[code_idx++] = 0x90;
}

int32_t inline_mod(int a, int b) {
  return a%b;
}
void jit_modr(int dreg, int sreg) {
  /*jit_movr(ARGR0,dreg);
  jit_movr(ARGR1,sreg);
  jit_call(inline_mod,"mod");
  if (dreg!=0) jit_movr(dreg,0);*/
}

void jit_cmpi(int sreg, int imm) {
  //fprintf(jit_out, "cmp $%d, %s\n", imm, regnames[sreg]);
}

void jit_cmpr(int sreg, int dreg) {
  //fprintf(jit_out, "cmp %s, %s\n", regnames[dreg], regnames[sreg]);
}

void jit_je(char* label) {
  //fprintf(jit_out, "je %s\n", label);
}

void jit_jneg(char* label) {
  //fprintf(jit_out, "js %s\n", label);
}

void jit_jmp(char* label) {
}

void jit_label(char* label) {
}

void jit_ret() {
  code[code_idx++] = 0x4e;
  code[code_idx++] = 0x75;
}

void jit_push(int r1, int r2) {
  /*for (int i=r1; i<=r2; i++) {
    fprintf(jit_out, "push %s\n",regnames[i]);
    }*/
}

void jit_pop(int r1, int r2) {
  /*for (int i=r2; i>=r1; i--) {
    fprintf(jit_out, "pop %s\n",regnames[i]);
    }*/
}

void debug_handler() {
}
