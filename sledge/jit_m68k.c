
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
  code[code_idx++] = 0x6a; // bpl
  code[code_idx++] = 0x00;
  code[code_idx++] = 0x00; // skip
  code[code_idx++] = 0x04;
  jit_movr(dreg,sreg);
}

void jit_movne(int dreg, int sreg) {
  code[code_idx++] = 0x67; // beq
  code[code_idx++] = 0x00;
  code[code_idx++] = 0x00; // skip
  code[code_idx++] = 0x04;
  jit_movr(dreg,sreg);
}

void jit_moveq(int dreg, int sreg) {
  code[code_idx++] = 0x66; // bne
  code[code_idx++] = 0x00;
  code[code_idx++] = 0x00; // skip
  code[code_idx++] = 0x04;
  jit_movr(dreg,sreg);
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
  code[code_idx++] = 0x20|(regi[dreg]<<1); // move from sp indexed
  code[code_idx++] = 0x2f;
  code[code_idx++] = (offset&0xff00)>>8;
  code[code_idx++] = offset&0xff;
}

void jit_str_stack(int sreg, int offset) {
  code[code_idx++] = 0x2f; // move to sp indexed
  code[code_idx++] = 0x40|regi[sreg];
  code[code_idx++] = (offset&0xff00)>>8;
  code[code_idx++] = offset&0xff;
}

void jit_inc_stack(int imm) {
  code[code_idx++] = 0xdf; // adda.l
  code[code_idx++] = 0xfc;
  
  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;
}

void jit_dec_stack(int imm) {
  code[code_idx++] = 0x9f; // adda.l
  code[code_idx++] = 0xfc;
  
  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;
}

// clobbers rdx!
void jit_ldrb(int reg) {
  code[code_idx++] = 0x16; // moveb @(00000000,%d3:l),%d0
  code[code_idx++] = 0x30;
  code[code_idx++] = 0x09 | regi[reg]<<4;
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
  code[code_idx++] = 0x80|regi[dreg];
  
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

void jit_notr(int dreg) {
  code[code_idx++] = 0x46;
  code[code_idx++] = 0x80|regi[dreg];
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
  code[code_idx++] = 0xe0|regi[sreg]<<1;
  code[code_idx++] = 0xa8|regi[dreg];
}

void jit_shlr(int dreg, int sreg) {
  code[code_idx++] = 0xe1|regi[sreg]<<1;
  code[code_idx++] = 0xa8|regi[dreg];
}

void jit_subr(int dreg, int sreg) {
  code[code_idx++] = 0x90|regi[dreg]<<1;
  code[code_idx++] = 0x80|regi[sreg];
}

void jit_mulr(int dreg, int sreg) {
  code[code_idx++] = 0x4c;
  code[code_idx++] = 0x00|regi[sreg];
  code[code_idx++] = 0x08|regi[dreg]<<4;
  code[code_idx++] = 0x00;
}

void jit_divr(int dreg, int sreg) {
  code[code_idx++] = 0x4c;
  code[code_idx++] = 0x40|regi[sreg];
  code[code_idx++] = 0x08|regi[dreg]<<4;
  code[code_idx++] = 0x00|regi[dreg];
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
  jit_movr(ARGR0,dreg);
  jit_movr(ARGR1,sreg);
  jit_call2(inline_mod,"mod");
  if (dreg!=0) jit_movr(dreg,0);
}

void jit_cmpi(int sreg, int imm) {
  code[code_idx++] = 0x0c;
  code[code_idx++] = 0x80|regi[sreg];
  
  code[code_idx+3] = imm&0xff; imm>>=8;
  code[code_idx+2] = imm&0xff; imm>>=8;
  code[code_idx+1] = imm&0xff; imm>>=8;
  code[code_idx] = imm&0xff;
  code_idx+=4;
}

void jit_cmpr(int sreg, int dreg) {
  code[code_idx++] = 0xb0|regi[dreg]<<1;
  code[code_idx++] = 0x80|regi[sreg];
}


Label* find_label(char* label) {
  int i;
  for (i=0; i<label_idx; i++) {
    if (jit_labels[i].name) {
      //printf("find_label %s label vs %s\r\n",label,jit_labels[i].name);
    }
    if (jit_labels[i].name && (strcmp(jit_labels[i].name,label)==0)) {
      return &jit_labels[i];
    }
  }
  return NULL;
}

Label* find_unresolved_label(char* label) {
  int i;
  for (i=0; i<unres_labels; i++) {
    if (jit_labels_unres[i].name) {
      //printf("find_unres_label %s label vs %s\r\n",label,jit_labels_unres[i].name);
    }
    if (jit_labels_unres[i].name && (strcmp(jit_labels_unres[i].name,label)==0)) {
      return &jit_labels_unres[i];
    }
  }
  return NULL;
}

// m68k offsets are 16 bit
void jit_emit_branch(char* label) {
  Label* lbl = find_label(label);
  if (lbl) {
    int offset = (lbl->idx - code_idx);
    //printf("offset to %s: %d (*4)\r\n",label,offset);
    if (offset<0) {
      offset = 0x10000-(-offset);
      code[code_idx++] = (offset&0xff00)>>8;
      code[code_idx++] = offset&0xff;
    }
  } else {
    //printf("! label not found %s, adding unresolved.\r\n",label);
    jit_labels_unres[unres_labels].name = strdup(label);
    jit_labels_unres[unres_labels].idx  = code_idx;
    
    code[code_idx++] = 0;
    code[code_idx++] = 0;
    
    unres_labels++;
  }
}

void jit_je(char* label) {
  code[code_idx++] = 0x67; // beq
  code[code_idx++] = 0x00;
  jit_emit_branch(label);
}

void jit_jne(char* label) {
  code[code_idx++] = 0x66; // bne
  code[code_idx++] = 0x00;
  jit_emit_branch(label);
}

void jit_jneg(char* label) {
  code[code_idx++] = 0x6b; // bmi
  code[code_idx++] = 0x00;
  jit_emit_branch(label);
}

void jit_jmp(char* label) {
  code[code_idx++] = 0x60; // bra
  code[code_idx++] = 0x00;
  jit_emit_branch(label);
}

void jit_label(char* label) {
  Label* unres_lbl = NULL;
  jit_labels[label_idx].name = strdup(label);
  jit_labels[label_idx].idx = code_idx;

  while ((unres_lbl = find_unresolved_label(label))) {
    //printf("! forward label to %s at idx %d resolved.\r\n",label,unres_lbl->idx);
    int offset = (code_idx - unres_lbl->idx);
    code[unres_lbl->idx] = (offset&0xff00)>>8;
    code[unres_lbl->idx+1] = (offset&0xff);
    
    free(unres_lbl->name);
    unres_lbl->name = NULL;
    unres_lbl->idx  = 0;
  }
  
  label_idx++;
}

void jit_ret() {
  code[code_idx++] = 0x4e;
  code[code_idx++] = 0x75;
}

void jit_push(int r1, int r2) {
  int i;
  for (i=r1; i<=r2; i++) {
    //fprintf(jit_out, "push %s\n",regnames[i]);
    
    code[code_idx++] = 0x2f; // move dx, -(sp) 
    code[code_idx++] = regi[i];
  }
}

void jit_pop(int r1, int r2) {
  int i;
  for (i=r2; i>=r1; i--) {
    //fprintf(jit_out, "pop %s\n",regnames[i]);
    
    code[code_idx++] = 0x20|(regi[i]<<1); // move (sp)+, dx 
    code[code_idx++] = 0x1f;
  }
}

// do any needed stack alignment etc. here for host ABI
void jit_host_call_enter() {
}
void jit_host_call_exit() {
}

void debug_handler() {
}
