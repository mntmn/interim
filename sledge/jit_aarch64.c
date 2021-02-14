#include <stdint.h>

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
  R13, // SP Stack Pointer
  R14, // LR Link Register
  R15  // PC Program Counter
};
enum arg_reg {
  ARGR0 = 0,
  ARGR1,
  ARGR2
};

#define RSP R13

static uint32_t* code;
static uint32_t code_idx;
static uint32_t cpool_idx; // constant pool

#define JIT_MAX_LABELS 32
static int label_idx = 0;
static Label jit_labels[JIT_MAX_LABELS];
static Label jit_labels_unres[JIT_MAX_LABELS]; // unresolved (forward) labels
static int unres_labels = 0;

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

void jit_init(int gap) {
  // cleans up jit state
  label_idx = 0;
  unres_labels = 0;
  code_idx = 0;
  cpool_idx = gap;
  for (int i=0; i<JIT_MAX_LABELS; i++) {
    if (jit_labels[i].name) free(jit_labels[i].name);
    jit_labels[i].name = NULL;
    jit_labels[i].idx = 0;
    if (jit_labels_unres[i].name) free(jit_labels_unres[i].name);
    jit_labels_unres[i].name = NULL;
    jit_labels_unres[i].idx = 0;
  }
}

// why is this 32 bit?
void jit_movi(int reg, uint64_t imm) {
  // load from constant pool
  // adr x0, pc+0
  uint32_t op = 0x10000000;
  op |= reg; // dreg
  
  op |= (((cpool_idx*2)-code_idx))<<5;

  uint64_t* pool = (uint64_t*)(((uint64_t)code)+cpool_idx * 8);
  printf("code: %p pool: %p\n", code, pool);
  
  *pool = imm; // store in constant pool

  cpool_idx++;
  code[code_idx++] = op;

  // ldr x0, [x0]
  op = 0xf9400000;
  op |= reg; // dest
  op |= (reg<<5); // source
  code[code_idx++] = op;
}

void jit_movr(int dreg, int sreg) {
  uint32_t op = 0xaa0003e0;
  op |= (sreg<<16);
  op |= (dreg<<0);
  
  code[code_idx++] = op;
}

void jit_movneg(int dreg, int sreg) {  
  // TODO
  uint32_t op = 0x41a00000;
  op |= (sreg<<0); // base reg = pc
  op |= (dreg<<12); // dreg
  
  code[code_idx++] = op;
}

void jit_movne(int dreg, int sreg) {
  // TODO
  uint32_t op = 0x11a00000;
  op |= (sreg<<0); // base reg = pc
  op |= (dreg<<12); // dreg
  
  code[code_idx++] = op;
}

void jit_moveq(int dreg, int sreg) {
  // TODO
  uint32_t op = 0xaa0003e0;
  op |= (sreg<<16); // base reg = pc
  op |= (dreg<<0); // dreg
  
  code[code_idx++] = op;
}

void jit_lea(int reg, void* addr) {
  jit_movi(reg, (uint64_t)addr);
}

void jit_ldr(int reg) {
  uint32_t op = 0xf9400000;
  op |= (reg<<5); // sreg
  op |= reg; // dreg
  code[code_idx++] = op;
}

void jit_ldr_stack(int dreg, int offset) {
  // ldur
  uint32_t op = 0xf84003e0;
  if (offset == 0) {
    op = 0xf94003e0;
  }
  op |= ((offset)&0xff)<<12;
  op |= (dreg)&0xf; // dreg
  code[code_idx++] = op;
}

void jit_str_stack(int sreg, int offset) {
  // stur
  uint32_t op = 0xf80003e0;
  if (offset == 0) {
    // str
    op = 0xf90003e0;
  }
  if (offset<0) {
    offset = -1*offset;
  }
  op |= (offset)&0xfff;
  op |= (sreg<<12); // dreg
  code[code_idx++] = op;
}

void jit_inc_stack(int64_t offset) {
  // FIXME optimize
  jit_movi(9, offset*2); // FIXME
  uint32_t op = 0x8b2963ff;
  code[code_idx++] = op;
}

void jit_dec_stack(int64_t offset) {
  // FIXME optimize
  jit_movi(9, offset*2);
  uint32_t op = 0xcb2963ff;
  code[code_idx++] = op;
}

void jit_ldrw(int reg) {
  jit_ldr(reg);
}

// 8 bit only from rdx! (R3)
void jit_ldrb(int reg) {
  // TODO
  uint32_t op = 0;
  op |= 1<<22; // byte access
  op |= (reg<<16); // r3
  op |= (3<<12); // dreg
  code[code_idx++] = op;
}

// 8 bit only from rdx! (R3)
void jit_ldrs(int reg) {
  // TODO
  uint32_t op = 0; // ldrh
  op |= 1<<22; // byte access
  op |= (reg<<16); // r3
  op |= (3<<12); // dreg
  code[code_idx++] = op;
}

// 8 bit only from rdx! (R3)
void jit_strb(int reg) {
  // TODO
  uint32_t op = 0;
  op |= 1<<22; // byte access
  op |= (reg<<16); // r3
  op |= (3<<12); // dreg
  code[code_idx++] = op;
}

// 16 bit only from rdx! (R3)
void jit_strs(int reg) {
  // TODO
  uint32_t op = 0; // strh
  op |= 1<<22; // byte access
  op |= (reg<<16); // r3
  op |= (3<<12); // dreg
  code[code_idx++] = op;
}

// 32 bit only from rdx!
void jit_strw(int reg) {
  // TODO
  uint32_t op = 0;
  op |= (reg<<16); // r3
  op |= (3<<12); // dreg
  code[code_idx++] = op;
}

#define jit_stra jit_strw

void jit_addr(int dreg, int sreg) {
  uint32_t op = 0x8b000000;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

void jit_addi(int dreg, int imm) {
  // FIXME
  jit_movi(9,imm);
  jit_addr(dreg,9);
}

void jit_subr(int dreg, int sreg) {
  uint32_t op = 0xcb000000;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

// addi 0x11000000
// subi 0xd1000000

void jit_mulr(int dreg, int sreg) {
  uint32_t op = 0x1b007c00;
  op |= (sreg<<5);
  op |= (dreg<<0);
  op |= (dreg<<16);
  code[code_idx++] = op;
}

void jit_andr(int dreg, int sreg) {
  uint32_t op = 0x0a000000;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

void jit_notr(int dreg) {
  uint32_t op = 0x2a200000;
  op |= (dreg<<0);
  op |= (dreg<<12);
  code[code_idx++] = op;
}

void jit_orr(int dreg, int sreg) {
  uint32_t op = 0x2a000000;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

// FIXME
void jit_xorr(int dreg, int sreg) {
  uint32_t op = 0xe0200000;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

void jit_shlr(int dreg, int sreg) {
  uint32_t op = 0x1ac02000;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

void jit_shrr(int dreg, int sreg) {
  uint32_t op = 0x1ac02400;
  op |= (sreg<<16);
  op |= (dreg<<5);
  op |= (dreg);
  code[code_idx++] = op;
}

void jit_push(int r1, int r2) {
  for (int i=r1; i<=r2; i++) {
    uint32_t op = 0xf81f0fe0;
    op |= i;
    code[code_idx++] = op;
  }
}

void jit_pop(int r1, int r2) {
  for (int i=r1; i<=r2; i++) {
    uint32_t op = 0xf84107e0;
    op |= i;
    code[code_idx++] = op;
  }
}

void jit_call(void* func, char* note) {
  uint32_t op;
  uint32_t reg = 10;
  jit_push(30,30); // push LR
  //jit_movr(4,30);
  jit_movi(reg, (uint64_t)func); // FIXME
  //code[code_idx++] = 0x1000005e; // ADR x30 + 4
  op = 0xd63f0000; // BLR
  op |= (reg<<5);
  code[code_idx++] = op;
  jit_pop(30,30); // pop LR
  //jit_movr(30,4);
}

#define jit_call2 jit_call
#define jit_call3 jit_call

void jit_callr(int reg) {
  uint32_t op;
  
  jit_push(30,30); // push LR
  //code[code_idx++] = 0x1000005e; // ADR x30 + 4
  op = 0xd63f0000; // BLR
  op |= (reg<<5);
  code[code_idx++] = op;
  jit_pop(30,30); // pop LR
}

int64_t inline_div(int64_t a, int64_t b) {
  return a/b;
}
// FIXME
void jit_divr(int dreg, int sreg) {
  jit_movr(0,dreg);
  jit_movr(1,sreg);
  jit_call(inline_div,"div");
  if (dreg!=0) jit_movr(dreg,0);
  // call the c lib function
  // later: http://thinkingeek.com/2013/08/11/arm-assembler-raspberry-pi-chapter-15/
}

int64_t inline_mod(int64_t a, int64_t b) {
  return a%b;
}
void jit_modr(int dreg, int sreg) {
  jit_movr(0,dreg);
  jit_movr(1,sreg);
  jit_call(inline_mod,"mod");
  if (dreg!=0) jit_movr(dreg,0);
}

void jit_cmpr(int sreg, int dreg) {
  uint32_t op = 0xe1500000;
  op|=(sreg<<0);
  op|=(dreg<<16);
  code[code_idx++] = op;
}

void jit_cmpi(int sreg, int imm) {
  uint32_t op = 0xe3500000;
  op|=imm&0xffff; // TODO double check
  op|=(sreg<<16);
  code[code_idx++] = op;
}

Label* find_label(char* label) {
  for (int i=0; i<label_idx; i++) {
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
  for (int i=0; i<unres_labels; i++) {
    if (jit_labels_unres[i].name) {
      //printf("find_unres_label %s label vs %s\r\n",label,jit_labels_unres[i].name);
    }
    if (jit_labels_unres[i].name && (strcmp(jit_labels_unres[i].name,label)==0)) {
      return &jit_labels_unres[i];
    }
  }
  return NULL;
}

void jit_emit_branch(uint32_t op, char* label) {
  Label* lbl = find_label(label);
  if (lbl) {
    int offset = (lbl->idx - code_idx) - 2;
    //printf("offset to %s: %d (*4)\r\n",label,offset);
    if (offset<0) {
      offset = 0x1000000-(-offset);
      op|=offset;
      code[code_idx++] = op;
    }
  } else {
    //printf("! label not found %s, adding unresolved.\r\n",label);
    jit_labels_unres[unres_labels].name = strdup(label);
    jit_labels_unres[unres_labels].idx  = code_idx;
    code[code_idx++] = op;
    
    unres_labels++;
  }
}

void jit_je(char* label) {
  uint32_t op = 0x0a000000; // beq
  jit_emit_branch(op, label);
}

void jit_jge(char* label) {
  uint32_t op = 0xaa000000; // bge
  jit_emit_branch(op, label);
}

void jit_jne(char* label) {
  uint32_t op = 0x1a000000; // bne
  jit_emit_branch(op, label);
}

void jit_jneg(char* label) {
  uint32_t op = 0x4a000000; // bmi
  jit_emit_branch(op, label);
}

void jit_jmp(char* label) {
  //printf("jmp to label: %s\r\n",label);
  uint32_t op = 0xea000000; // b
  jit_emit_branch(op, label);
}

void jit_label(char* label) {
  jit_labels[label_idx].name = strdup(label);
  jit_labels[label_idx].idx = code_idx;

  Label* unres_lbl = NULL;
  while ((unres_lbl = find_unresolved_label(label))) {
    //printf("! forward label to %s at idx %d resolved.\r\n",label,unres_lbl->idx);
    code[unres_lbl->idx] |= (code_idx - unres_lbl->idx) - 2;

    free(unres_lbl->name);
    unres_lbl->name = NULL;
    unres_lbl->idx  = 0;
  }
  
  label_idx++;
}

void jit_ret() {
  uint32_t op = 0xd65f03c0;
  code[code_idx++] = op;
}

// do any needed stack alignment etc. here for host ABI
void jit_host_call_enter() {
}
void jit_host_call_exit() {
}

void jit_comment(char* comment) {
}

void debug_handler() {
  // NYI
}
