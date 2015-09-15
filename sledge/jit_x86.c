
char* regnames[] = {
  "%eax",
  "%edi",
  "%esi",
  "%edx",
  "%ecx",
  "%ebx",
  "%esp"
};

enum jit_reg {
  R0 = 0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6
};

enum arg_reg {
  ARGR0 = 1,
  ARGR1 = 2,
  ARGR2 = 3
};

uint8_t regi[] = {
  0,
  7,
  6,
  2,
  1,
  3,
  0
};

#define RSP R6

static uint8_t* code;
static uint32_t code_idx;

#define JIT_MAX_LABELS 32
static int label_idx = 0;
static Label jit_labels[JIT_MAX_LABELS];
static Label jit_labels_unres[JIT_MAX_LABELS]; // unresolved (forward) labels
static int unres_labels = 0;


void jit_init() {
  // cleans up jit state
  label_idx = 0;
  unres_labels = 0;
  code_idx = 0;
  for (int i=0; i<JIT_MAX_LABELS; i++) {
    if (jit_labels[i].name) free(jit_labels[i].name);
    jit_labels[i].name = NULL;
    jit_labels[i].idx = 0;
    if (jit_labels_unres[i].name) free(jit_labels_unres[i].name);
    jit_labels_unres[i].name = NULL;
    jit_labels_unres[i].idx = 0;
  }
}

void jit_movi(int reg, int imm) {
  /*if (reg==R0) {
    code[code_idx++] = 0xa1;
  } else {
    code[code_idx++] = 0x8b;
    code[code_idx++] = reg_code(reg);
    }*/

  code[code_idx++] = 0xb8 | regi[reg];
  
  code[code_idx++] = imm&0xff; imm>>=8;
  code[code_idx++] = imm&0xff; imm>>=8;
  code[code_idx++] = imm&0xff; imm>>=8;
  code[code_idx++] = imm&0xff;
}

void jit_movr(int dreg, int sreg) {
  code[code_idx++] = 0x89;
  code[code_idx++] = 0xc0 + (regi[sreg]<<3) + regi[dreg];
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
  code[code_idx++] = 0x8b;
  code[code_idx++] = (regi[reg]<<3) + regi[reg];
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
  /*fprintf(jit_out, "movb (%s), %%dl\n", regnames[reg]);
  fprintf(jit_out, "andq $0xff, %rdx\n", regnames[reg]);
  if (reg!=3) {
    fprintf(jit_out, "movq %%rdx, %s\n", regnames[reg]);
    }*/
}

// clobbers rdx!
void jit_ldrw(int reg) {
  /*fprintf(jit_out, "movl (%s), %%edx\n", regnames[reg]);
  if (reg!=3) {
    fprintf(jit_out, "movq %%rdx, %s\n", regnames[reg]);
    }*/
}

// 8 bit only from rdx!
void jit_strb(int reg) {
  //fprintf(jit_out, "movb %%dl, (%s)\n", regnames[reg]);
}

// 32 bit only from rdx!
void jit_strw(int reg) {
  //fprintf(jit_out, "movl %%edx, (%s)\n", regnames[reg]);
}

void jit_addr(int dreg, int sreg) {
  code[code_idx++] = 0x01;
  code[code_idx++] = 0xc0 + (regi[sreg]<<3) + regi[dreg];
}

void jit_addi(int dreg, int imm) {
}

void jit_andr(int dreg, int sreg) {
}

void jit_orr(int dreg, int sreg) {
}

void jit_xorr(int dreg, int sreg) {
}

void jit_shrr(int dreg, int sreg) {
  //fprintf(jit_out, "movq %s, %%rcx\n", regnames[sreg]);
  //fprintf(jit_out, "shr %%cl, %s\n", regnames[dreg]);
}

void jit_shlr(int dreg, int sreg) {
  //fprintf(jit_out, "movq %s, %%rcx\n", regnames[sreg]);
  //fprintf(jit_out, "shl %%cl, %s\n", regnames[dreg]);
}

void jit_subr(int dreg, int sreg) {
  //fprintf(jit_out, "subq %s, %s\n", regnames[sreg], regnames[dreg]);
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
  jit_lea(R0, func);
  code[code_idx++] = 0x57; // push edi
  code[code_idx++] = 0xff; // call *eax
  code[code_idx++] = 0xd0;

  code[code_idx++] = 0x83;
  code[code_idx++] = 0xc4;
  code[code_idx++] = 0x04; // add $4, esp
}

void jit_call2(void* func, char* note) {
  jit_lea(R0, func);
  code[code_idx++] = 0x56; // push esi
  code[code_idx++] = 0x57; // push edi
  code[code_idx++] = 0xff; // call *eax
  code[code_idx++] = 0xd0;

  code[code_idx++] = 0x83;
  code[code_idx++] = 0xc4;
  code[code_idx++] = 0x08; // add $8, esp
}

void jit_call3(void* func, char* note) {
  jit_lea(R0, func);
  code[code_idx++] = 0x52; // push edx
  code[code_idx++] = 0x56; // push esi
  code[code_idx++] = 0x57; // push edi
  code[code_idx++] = 0xff; // call *eax
  code[code_idx++] = 0xd0;

  code[code_idx++] = 0x83;
  code[code_idx++] = 0xc4;
  code[code_idx++] = 12; // add $12, esp
}

void jit_callr(int dreg) {
}

int inline_mod(int a, int b) {
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
  code[code_idx++] = 0xc3;
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
