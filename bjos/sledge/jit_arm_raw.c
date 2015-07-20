
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
  ARGR0 = 0,
  ARGR1,
  ARGR2
};

static uint32_t* code;
static uint32_t code_idx;
static uint32_t cpool_idx; // constant pool

typedef struct Label {
  char* name;
  uint32_t idx;
} Label;

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

void jit_init() {
  // cleans up jit state
  label_idx = 0;
  unres_labels = 0;
  code_idx = 0;
  cpool_idx = 128;
  for (int i=0; i<JIT_MAX_LABELS; i++) {
    jit_labels[i].name = NULL;
    jit_labels[i].idx = NULL;
    jit_labels_unres[i].name = NULL;
    jit_labels_unres[i].idx = NULL;
  }
}


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
  op |= (reg<<16); // r3
  op |= (3<<12); // dreg
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
  jit_movi(9,imm);
  jit_addr(dreg,9);
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

void jit_call(void* func, char* note) {
  code[code_idx++] = 0xe92d4000; // stmfd	sp!, {lr}
  jit_movr(14,15);
  jit_lea(15,func);
  code[code_idx++] = 0xe8bd4000; // ldmfd	sp!, {lr}
}

int32_t inline_div(int32_t a, int32_t b) {
  return a/b;
}
void jit_divr(int dreg, int sreg) {
  jit_movr(0,dreg);
  jit_movr(1,sreg);
  jit_call(inline_div,"div");
  if (dreg!=0) jit_movr(dreg,0);
  // call the c lib function
  // later: http://thinkingeek.com/2013/08/11/arm-assembler-raspberry-pi-chapter-15/
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
  for (int i=0; i<JIT_MAX_LABELS; i++) {
    if (jit_labels[i].name && strcmp(jit_labels[i].name,label)==0) {
      return &jit_labels[i];
    }
  }
  return NULL;
}

Label* find_unresolved_label(char* label) {
  for (int i=0; i<JIT_MAX_LABELS; i++) {
    if (jit_labels_unres[i].name && strcmp(jit_labels_unres[i].name,label)==0) {
      return &jit_labels_unres[i];
    }
  }
  return NULL;
}

void jit_emit_branch(uint32_t op, char* label) {
  Label* lbl = find_label(label);
  if (lbl) {
    int offset = (lbl->idx - code_idx) - 2;
    //printf("offset: %d (*4)\n",offset);
    if (offset<0) {
      offset = 0x1000000-(-offset);
      op|=offset;
      code[code_idx++] = op;
    }
  } else {
    printf("! label not found %s, adding unresolved.\n",label);
    jit_labels_unres[unres_labels].name = label;
    jit_labels_unres[unres_labels].idx  = code_idx;
    code[code_idx++] = op;
    
    unres_labels++;
  }
}

void jit_je(char* label) {
  uint32_t op = 0x0a000000; // beq
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
  uint32_t op = 0xea000000; // b
  jit_emit_branch(op, label);
}

void jit_label(char* label) {
  //fprintf(jit_out, "%s:\n", label);
  jit_labels[label_idx].name = label;
  jit_labels[label_idx].idx = code_idx;

  Label* unres_lbl = NULL;
  while ((unres_lbl = find_unresolved_label(label))) {
    printf("! forward label to %s at idx %d resolved.\n",label,unres_lbl->idx);
    code[unres_lbl->idx] |= (code_idx - unres_lbl->idx) - 2;

    unres_lbl->name = NULL;
    unres_lbl->idx  = 0;
  }
  
  label_idx++;
}

void jit_ret() {
  jit_movr(15,14); // lr -> pc
}

void jit_push(int r1, int r2) {
  uint32_t op = 0xe92d0000;
  
  for (int i=r1; i<=r2; i++) {
    op |= (1<<i); // build bit pattern of registers to push
  }
}

void jit_pop(int r1, int r2) {
  uint32_t op = 0xe8bd0000;
  
  for (int i=r1; i<=r2; i++) {
    op |= (1<<i); // build bit pattern of registers to pop
  }
}
