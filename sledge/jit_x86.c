
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
  4,
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
static int jit_max_size = 0;

void jit_init(uint8_t* dest, int max_size) {
  code = dest;
  
  // cleans up jit state
  label_idx = 0;
  unres_labels = 0;
  code_idx = 0;
  jit_max_size = max_size; // TODO enforce
  
  for (int i=0; i<JIT_MAX_LABELS; i++) {
    if (jit_labels[i].name) free(jit_labels[i].name);
    jit_labels[i].name = NULL;
    jit_labels[i].idx = 0;
    if (jit_labels_unres[i].name) free(jit_labels_unres[i].name);
    jit_labels_unres[i].name = NULL;
    jit_labels_unres[i].idx = 0;
  }
}

void jit_imm(int imm) { 
  code[code_idx++] = imm&0xff; imm>>=8;
  code[code_idx++] = imm&0xff; imm>>=8;
  code[code_idx++] = imm&0xff; imm>>=8;
  code[code_idx++] = imm&0xff;
}

Label* find_label(char* label) {
  int i;
  for (i=0; i<label_idx; i++) {
    if (jit_labels[i].name && (strcmp(jit_labels[i].name,label)==0)) {
      return &jit_labels[i];
    }
  }
  return NULL;
}

Label* find_unresolved_label(char* label) {
  int i;
  for (i=0; i<unres_labels; i++) {
    if (jit_labels_unres[i].name && (strcmp(jit_labels_unres[i].name,label)==0)) {
      return &jit_labels_unres[i];
    }
  }
  return NULL;
}

void jit_emit_branch(char* label) {
  Label* lbl = find_label(label);
  if (lbl) {
    int offset = (lbl->idx - code_idx);
    //printf("offset to %s: %d (*4)\r\n",label,offset);
    if (offset<0) {
      offset = 0xffffffff-(-offset)+1 - 4;
      jit_imm(offset);
    }
  } else {
    //printf("! label not found %s, adding unresolved.\r\n",label);
    jit_labels_unres[unres_labels].name = strdup(label);
    jit_labels_unres[unres_labels].idx  = code_idx;
    
    jit_imm(0);
    
    unres_labels++;
  }
}

void jit_movi(int reg, int imm) {
  code[code_idx++] = 0xb8 | regi[reg];
  jit_imm(imm);
}

void jit_movr(int dreg, int sreg) {
  if (dreg == sreg) return;
  code[code_idx++] = 0x89;
  code[code_idx++] = 0xc0 | (regi[sreg]<<3) | regi[dreg];
}

void jit_movneg(int dreg, int sreg) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0x48;
  code[code_idx++] = 0xc0 | (regi[dreg]<<3) | regi[sreg];
}

void jit_movne(int dreg, int sreg) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0x45;
  code[code_idx++] = 0xc0 | (regi[dreg]<<3) | regi[sreg];
}

void jit_moveq(int dreg, int sreg) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0x44;
  code[code_idx++] = 0xc0 | (regi[dreg]<<3) | regi[sreg];
}

void jit_lea(int reg, void* addr) {
  jit_movi(reg, (uint32_t)addr);
}

void jit_addr(int dreg, int sreg) {
  code[code_idx++] = 0x01;
  code[code_idx++] = 0xc0 + (regi[sreg]<<3) + regi[dreg];
}

// TODO: smaller immediate encodings
void jit_addi(int dreg, int imm) {
  if (dreg == R0) {
    code[code_idx++] = 0x05;
  } else {
    code[code_idx++] = 0x81;
    code[code_idx++] = 0xc0 | regi[dreg];
  }
  jit_imm(imm);
}

void jit_subi(int dreg, int imm) {
  if (dreg == R0) {
    code[code_idx++] = 0x2d;
  } else {
    code[code_idx++] = 0x81;
    code[code_idx++] = 0xe8 | regi[dreg];
  }
  jit_imm(imm);
}

void jit_andr(int dreg, int sreg) {
  code[code_idx++] = 0x21;
  code[code_idx++] = 0xc0 | (regi[sreg]<<3) | regi[dreg];
}

void jit_andi(int dreg, int imm) {
  if (dreg == R0) {
    code[code_idx++] = 0x25;
  } else {
    code[code_idx++] = 0x81;
    code[code_idx++] = 0xe0 | regi[dreg];
  }
  jit_imm(imm);
}

void jit_notr(int dreg) {
  code[code_idx++] = 0xf7;
  code[code_idx++] = 0xd0 | regi[dreg];
}

void jit_orr(int dreg, int sreg) {
  code[code_idx++] = 0x09;
  code[code_idx++] = 0xc0 | (regi[sreg]<<3) | regi[dreg];
}

void jit_xorr(int dreg, int sreg) {
  code[code_idx++] = 0x31;
  code[code_idx++] = 0xc0 | (regi[sreg]<<3) | regi[dreg];
}

void jit_shrr(int dreg, int sreg) {
  jit_movr(R3,R4);
  jit_movr(R4,sreg);
  code[code_idx++] = 0xd3; // shr %cl, dreg
  code[code_idx++] = 0xe8 | (regi[dreg]);
  jit_movr(R4,R3);
}

void jit_shlr(int dreg, int sreg) {
  jit_movr(R3,R4);
  jit_movr(R4,sreg);
  code[code_idx++] = 0xd3; // shr %cl, dreg
  code[code_idx++] = 0xe0 | (regi[dreg]);
  jit_movr(R4,R3);
}

void jit_subr(int dreg, int sreg) {
  code[code_idx++] = 0x29;
  code[code_idx++] = 0xc0 | (regi[sreg]<<3) | regi[dreg];
}

void jit_mulr(int dreg, int sreg) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0xaf;
  code[code_idx++] = 0xc0 | (regi[dreg]<<3) | regi[sreg];
}

void jit_divr(int dreg, int sreg) {
  jit_movr(R0, dreg);
  code[code_idx++] = 0x99; // sign-extend rax to edx:rax (cdq)
  code[code_idx++] = 0xf7;
  code[code_idx++] = 0xf8 | regi[sreg]; // idiv goes to %rax
  jit_movr(dreg, R0);
}


void jit_ldr(int reg) {
  code[code_idx++] = 0x8b;
  code[code_idx++] = (regi[reg]<<3) | regi[reg];
}

void jit_ldr_stack(int dreg, int offset) {
  code[code_idx++] = 0x8b;
  code[code_idx++] = 0x44 | (regi[dreg]<<3);
  code[code_idx++] = 0x24;
  code[code_idx++] = (char)offset;
}

void jit_str_stack(int sreg, int offset) {
  code[code_idx++] = 0x89;
  code[code_idx++] = 0x44 | (regi[sreg]<<3);
  code[code_idx++] = 0x24;
  code[code_idx++] = (char)offset;
}

// clobbers rdx!
void jit_ldrb(int reg) {
  code[code_idx++] = 0x8a; // movb (reg), %dl
  code[code_idx++] = 0x10 | regi[reg];

  jit_andi(R3, 0xff);
  jit_movr(reg, R3);
}

void jit_ldrs(int reg) {
  code[code_idx++] = 0x66; // movw (reg), %dx
  code[code_idx++] = 0x8b;
  code[code_idx++] = 0x10 | regi[reg];
  
  jit_andi(R3, 0xffff);
  jit_movr(reg, R3);
}

void jit_ldrw(int reg) {
  code[code_idx++] = 0x8b; // movl (reg), %dx
  code[code_idx++] = 0x10 | regi[reg];
}

// 8 bit only from rdx!
void jit_strb(int reg) {
  code[code_idx++] = 0x88; // movb %dl, (reg)
  code[code_idx++] = 0x10 | regi[reg];
}

void jit_strw(int reg) {
  code[code_idx++] = 0x89; // movl %edx, (reg)
  code[code_idx++] = 0x10 | regi[reg];
}

void jit_strs(int reg) {
  code[code_idx++] = 0x66;
  code[code_idx++] = 0x89; // movw %dx, (reg)
  code[code_idx++] = 0x10 | regi[reg];
}

#define jit_stra jit_strw

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
  code[code_idx++] = 0xff;
  code[code_idx++] = 0xd0 | regi[dreg];
}

int inline_mod(int a, int b) {
  return a%b;
}
void jit_modr(int dreg, int sreg) {
  jit_movr(ARGR0,dreg);
  jit_movr(ARGR1,sreg);
  jit_call2(inline_mod,"mod");
  if (dreg!=0) jit_movr(dreg,0);
}

void jit_cmpi(int dreg, int imm) {
  if (dreg == R0) {
    code[code_idx++] = 0x3d;
  } else {
    code[code_idx++] = 0x81;
    code[code_idx++] = 0xf8 | regi[dreg];
  }
  jit_imm(imm);
}

void jit_cmpr(int sreg, int dreg) {
  code[code_idx++] = 0x39;
  code[code_idx++] = 0xc0 | (regi[sreg]<<3) | regi[dreg];
}

void jit_je(char* label) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0x84;
  jit_emit_branch(label);
}

void jit_jne(char* label) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0x85;
  jit_emit_branch(label);
}

void jit_jneg(char* label) {
  code[code_idx++] = 0x0f;
  code[code_idx++] = 0x88;
  jit_emit_branch(label);
}

void jit_jmp(char* label) {
  code[code_idx++] = 0xe9;
  jit_emit_branch(label);
}

void jit_label(char* label) {
  Label* unres_lbl = NULL;
  jit_labels[label_idx].name = strdup(label);
  jit_labels[label_idx].idx = code_idx;

  while ((unres_lbl = find_unresolved_label(label))) {
    //printf("! forward label to %s at idx %d resolved.\r\n",label,unres_lbl->idx);
    int offset = (code_idx - unres_lbl->idx);
    int imm = offset-4;
    code[unres_lbl->idx]   = imm&0xff; imm>>=8;
    code[unres_lbl->idx+1] = imm&0xff; imm>>=8;
    code[unres_lbl->idx+2] = imm&0xff; imm>>=8;
    code[unres_lbl->idx+3] = imm&0xff; imm>>=8;
    
    free(unres_lbl->name);
    unres_lbl->name = NULL;
    unres_lbl->idx  = 0;
  }
  
  label_idx++;
}

void jit_ret() {
  code[code_idx++] = 0xc3;
}

void jit_push(int r1, int r2) {
  for (int i=r1; i<=r2; i++) {
    code[code_idx++] = 0x50|regi[i];
  }
}

void jit_pop(int r1, int r2) {
  for (int i=r2; i>=r1; i--) {
    code[code_idx++] = 0x58|regi[i];
  }
}

void jit_inc_stack(int offset) {
  jit_addi(RSP, offset);
}

void jit_dec_stack(int offset) {
  jit_subi(RSP, offset);
}

// do any needed stack alignment etc. here for host ABI
void jit_host_call_enter() {
}
void jit_host_call_exit() {
}

void jit_comment(char* comment) {
}

void debug_handler() {
}
