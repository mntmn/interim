#include "minilisp.h"
#include "reader.h"
#include "writer.h"
#include "alloc.h"
#include "uthash.h"

#include "machine.h"
#include "blit.h"

#define BUILTIN_ADD 1
#define BUILTIN_SUB 2
#define BUILTIN_MUL 3
#define BUILTIN_DIV 4
#define BUILTIN_MOD 5

#define BUILTIN_LT 6
#define BUILTIN_GT 7
#define BUILTIN_EQ 8

#define BUILTIN_WHILE 9

#define BUILTIN_DEF 10
#define BUILTIN_IF  11
#define BUILTIN_FN  12

#define BUILTIN_CAR 14
#define BUILTIN_CDR 15
#define BUILTIN_CONS 16

#define BUILTIN_GET 17
#define BUILTIN_PUT 18
#define BUILTIN_SIZE 19

#define BUILTIN_UGET 50
#define BUILTIN_UPUT 51
#define BUILTIN_USIZE 52

#define BUILTIN_TYPE 20
#define BUILTIN_LET 21
#define BUILTIN_QUOTE 22
#define BUILTIN_MAP 23
#define BUILTIN_DO 24

#define BUILTIN_EVAL 25
#define BUILTIN_WRITE 26

#define BUILTIN_PRINT 30

#define BUILTIN_PIXEL 40
#define BUILTIN_FLIP 41
#define BUILTIN_RECTFILL 42
#define BUILTIN_BLIT_MONO 43
#define BUILTIN_BLIT_MONO_INV 44
#define BUILTIN_INKEY 60

#define BUILTIN_ALIEN 34
#define BUILTIN_HELP 35
#define BUILTIN_LOAD 36
#define BUILTIN_SAVE 37
#define BUILTIN_UDP_POLL 70
#define BUILTIN_UDP_SEND 71
#define BUILTIN_TCP_CONNECT 72
#define BUILTIN_TCP_BIND 73
#define BUILTIN_TCP_SEND 74

typedef struct env_entry {
  char name[128];
  Cell* cell;
  UT_hash_handle hh;
} env_entry;

static struct env_entry* global_env = NULL;

static Cell* coerce_int_cell; // recycled cell used to return coereced integers
static Cell* error_cell; // recycled cell used to return errors

// utf8
unsigned int utf8_rune_len(uint8_t b) {
  if ((b & 0x80)==0) { // ascii
    return 1;
  } else if ((b & 0xe0) == 0xc0) {
    return 2;
  } else if ((b & 0xf0) == 0xe0) {
    return 3;
  } else if ((b & 0xf8) == 0xf0) {
    return 4;
  }
  return 1;
}

int utf8_strlen(char *s) {
  int i = 0, j = 0;
  while (s[i]) {
    if ((s[i] & 0xc0) != 0x80) j++;
    i++;
  }
  return j;
}

unsigned int utf8_rune_at(char* s, int idx) {
  int i = 0, j = 0;
  unsigned int rune = 0;
  int state = 0;
  while (s[i]) {
    unsigned char b1 = s[i];

    if ((b1 & 0x80)==0) { // ascii
      rune = b1;
      state = 0;
    } else if (state>0) {
      rune=(rune<<6) | (b1 & 0x3fu);
      state--;
    } else if ((b1 & 0xe0) == 0xc0) {
      // 16 bit
      rune = b1 & 0x1f;
      state = 1;
    } else if ((b1 & 0xf0) == 0xe0) {
      // 24 bit
      rune = b1 & 0x0f;
      state = 2;
    } else if ((b1 & 0xf8) == 0xf0) {
      // 32 bit
      rune = b1 & 0x07;
      state = 3;
    }

    // next char
    if (state == 0) {
      if (idx == j) return rune;
      j++;
    }
    i++;
  }
  return 0;
}

// adapted from TidyLib (c) 1998-2004 (W3C) MIT, ERCIM, Keio University
int rune_to_utf8(jit_word_t c, void* tempbuf, int* count)
{
  uint8_t* buf = (uint8_t*)tempbuf;
  int bytes = 0;
  int has_error = 0;
  
  if (c <= 0x7F)  /* 0XXX XXXX one uint8_t */
  {
    buf[0] = (uint8_t) c;
    bytes = 1;
  }
  else if (c <= 0x7FF)  /* 110X XXXX  two bytes */
  {
    buf[0] = (uint8_t) (0xC0 | (c >> 6));
    buf[1] = (uint8_t) (0x80 | (c & 0x3F));
    bytes = 2;
  }
  else if (c <= 0xFFFF)  /* 1110 XXXX  three bytes */
  {
    buf[0] = (uint8_t) (0xE0 | (c >> 12));
    buf[1] = (uint8_t) (0x80 | ((c >> 6) & 0x3F));
    buf[2] = (uint8_t) (0x80 | (c & 0x3F));
    bytes = 3;
  }
  else if (c <= 0x1FFFFF)  /* 1111 0XXX  four bytes */
  {
    buf[0] = (uint8_t) (0xF0 | (c >> 18));
    buf[1] = (uint8_t) (0x80 | ((c >> 12) & 0x3F));
    buf[2] = (uint8_t) (0x80 | ((c >> 6) & 0x3F));
    buf[3] = (uint8_t) (0x80 | (c & 0x3F));
    bytes = 4;
  }
  else if (c <= 0x3FFFFFF)  /* 1111 10XX  five bytes */
  {
    buf[0] = (uint8_t) (0xF8 | (c >> 24));
    buf[1] = (uint8_t) (0x80 | (c >> 18));
    buf[2] = (uint8_t) (0x80 | ((c >> 12) & 0x3F));
    buf[3] = (uint8_t) (0x80 | ((c >> 6) & 0x3F));
    buf[4] = (uint8_t) (0x80 | (c & 0x3F));
    bytes = 5;
    has_error = 1;
  }
  else if (c <= 0x7FFFFFFF)  /* 1111 110X  six bytes */
  {
    buf[0] = (uint8_t) (0xFC | (c >> 30));
    buf[1] = (uint8_t) (0x80 | ((c >> 24) & 0x3F));
    buf[2] = (uint8_t) (0x80 | ((c >> 18) & 0x3F));
    buf[3] = (uint8_t) (0x80 | ((c >> 12) & 0x3F));
    buf[4] = (uint8_t) (0x80 | ((c >> 6) & 0x3F));
    buf[5] = (uint8_t) (0x80 | (c & 0x3F));
    bytes = 6;
    has_error = 1;
  }
  else {
    has_error = 1;
  }
 
  *count = bytes;
  if (has_error) return -1;
  return 0;
}

jit_word_t utf8_strlen_cell(Cell* cell) {
  if (!cell || (cell->tag!=TAG_STR && cell->tag!=TAG_BYTES) || !cell->addr) return 0;
  return utf8_strlen(cell->addr);
}

jit_word_t utf8_rune_at_cell(Cell* cell, Cell* c_idx) {
  if (!cell || (cell->tag!=TAG_STR && cell->tag!=TAG_BYTES)) return 0;
  if (!c_idx || c_idx->tag!=TAG_INT) return 0;
  if (c_idx->value >= cell->size) return 0;
  if (c_idx->value < 0) return 0;
  if (!cell->addr) {
    printf("error: string with NULL addr at %p!\n",cell);
    return 0;
  }
  return utf8_rune_at(cell->addr, c_idx->value);
}

jit_word_t utf8_put_rune_at(Cell* cell, Cell* c_idx, Cell* c_rune) {
  if (!cell || (cell->tag!=TAG_STR && cell->tag!=TAG_BYTES)) return 0;
  if (!c_idx || c_idx->tag!=TAG_INT) return 0;
  if (!c_rune || c_rune->tag!=TAG_INT) return 0;

  char* s = cell->addr;
  int idx = c_idx->value;
  int rune = c_rune->value;

  if (idx<0 || idx>=cell->size) return 0;
  
  // fast forward to the right place
  unsigned int i = 0, j = 0;
  while (i<cell->size && s[i]) {
    if (j==idx) break;
    i+=utf8_rune_len(s[i]);
    j++;
  }

  // how long is the existing rune at target spot?
  j = utf8_rune_len(s[i]);
  
  int rune_len = 0;
  char tmp[10];
  rune_to_utf8(rune, tmp, &rune_len);
  
  if ((i+rune_len)>=cell->size) return 0;

  //printf("-- existing rune length at %d: %d new rune length: %d\n",idx,j,rune_len);
  
  if (j>rune_len) {
    // new rune is smaller
    int movelen = cell->size - (i+rune_len);
    if (movelen<rune_len) {
      //printf("-- utf8_put_rune_at error: rune %d doesn't fit into string at %d\n",rune,idx);
      return 0;
    }
    memmove(cell->addr+i+rune_len, cell->addr+i+j, movelen);
  } else if (j<rune_len) {
    // new rune is bigger
    int movelen = cell->size - (i+rune_len);
    if (movelen<rune_len) {
      //printf("-- utf8_put_rune_at error: rune %d doesn't fit into string at %d\n",rune,idx);
      return 0;
    }
    memmove(cell->addr+i+rune_len, cell->addr+i+j, movelen);
  }

  //printf("writing rune %d at %d: %x\n",rune,i,tmp[0]);

  // write the new rune
  for (int m=0; m<rune_len; m++) {
    ((uint8_t*)cell->addr)[i+m] = tmp[m];
  }

  return i;
}

Cell* lookup_symbol(char* name, env_entry** env) {
  env_entry* res;
  HASH_FIND_STR(*env, name, res);
  if (!res) return NULL;
  return res->cell;
}

int lookup_symbol_int(char* name, env_entry** env) {
  env_entry* res;
  HASH_FIND_STR(*env, name, res);
  if (!res || !res->cell) return 0;
  return res->cell->value; // TODO typecheck
}

void insert_symbol(Cell* symbol, Cell* cell, env_entry** env) {
  env_entry* e;
  HASH_FIND_STR(*env, symbol->addr, e);

  if (e) {
#ifdef DEBUG
    if (cell) {
      printf("insert_symbol replacing %s <- %x\n",symbol->addr,cell->value);
    } else {
      printf("insert_symbol replacing %s <- NULL\n",symbol->addr);
    }  
#endif
    if (!cell) {
      e->cell->tag = TAG_INT;
      e->cell->value = 0;
    } else {
      if (cell->tag == TAG_INT) {
        if (e->cell->tag == TAG_INT) {
          e->cell->value = cell->value; // no new allocation, just copy int value
        } else {
          //printf("-- %s copy over %p (%d) <- %p (%d)\n",symbol->addr,e->cell,e->cell->tag,cell,cell->tag);
          memcpy(e->cell, cell, sizeof(struct Cell));
        }
      } else {
        //e->cell = cell;
        memcpy(e->cell, cell, sizeof(struct Cell));
      }
    }
  } else {

#ifdef DEBUG
    if (cell) {
      printf("insert_symbol %s <- %x\n",symbol->addr,cell->value);
    } else {
      printf("insert_symbol %s <- NULL\n",symbol->addr);
    }
#endif
    e = malloc(sizeof(env_entry));
    strcpy(e->name, (char*)symbol->addr);

    if (cell && cell->tag == TAG_INT) {
      e->cell = alloc_clone(cell);
    } else {
      if (!cell) {
        e->cell = alloc_nil();
      } else {
        e->cell = alloc_clone(cell);
      }
    }
    
    HASH_ADD_STR(*env, name, e);
  }
}

void insert_symbol_int(Cell* symbol, jit_word_t value, env_entry** env) {
  env_entry* e;
  HASH_FIND_STR(*env, symbol->addr, e);
  
  if (e) {
    if (e->cell) {
#ifdef DEBUG
      printf("insert_symbol_int replacing %s (%p) <- %d\n",e->name,e->cell,value);
#endif
      e->cell->value = value;
      return;
    } else {
      printf("broken environment entry for %s\n",e->name);
    }
  }

  Cell* cell = alloc_int(value);
  
  e = malloc(sizeof(env_entry));
  strcpy(e->name, (char*)symbol->addr);
  e->cell = cell;

#ifdef DEBUG
  printf("insert_symbol_int %s <- %d\n",e->name,value);
#endif

  HASH_ADD_STR(*env, name, e);
}

void stack_push(int reg, jit_word_t* sp)
{
  jit_stxi(*sp, JIT_FP, reg);
  *sp += sizeof(jit_word_t);
}

void stack_pop(int reg, jit_word_t* sp)
{
  *sp -= sizeof(jit_word_t);
  jit_ldxi(reg, JIT_FP, *sp);
}

int compile_applic(Cell* list, int wants_int);

void argnum_error(char* usage) {
  printf("argument error. correct usage: %s.\n",usage);
  jit_movi(JIT_R0, (jit_word_t)error_cell);
}

// returns 1 if returning coerced cell
int compile_arg(int reg, Cell* arg, int save_regs, int wants_integer) {
  if (!arg) {
    argnum_error("missing argument");
    return 0;
  }
  
  jit_word_t tag = arg->tag;

  //printf("compile arg: %d %d %d\n",arg->tag,save_regs,wants_integer);
  
  if (tag == TAG_INT) {
    if (wants_integer) {
      jit_movi(reg, arg->value);
      return 0;
    } else {
      jit_movi(reg, (jit_word_t)arg);
      return 1; // FIXME: think about this again
    }
  }
  else {
    int coerced = 0;
    
    if (save_regs) {
      stack_push(JIT_R0, &stack_ptr);
    }
    
    if (tag == TAG_SYM) {
      // look up the value at runtime
    
      jit_prepare();
      jit_pushargi((jit_word_t)arg->addr);
      jit_pushargi((jit_word_t)&global_env);
      jit_retval(reg);

      if (wants_integer) {
        jit_finishi(lookup_symbol_int);
      } else {
        jit_finishi(lookup_symbol);
      }
    }
    else if (tag == TAG_CONS) {
      coerced = compile_applic(arg, wants_integer);
    }
    else {
      if (save_regs) {
        stack_pop(JIT_R0, &stack_ptr);
      }
      jit_movi(reg, (jit_word_t)arg);
      
      return 0;
    }
    
    if (reg != JIT_R0) {
      jit_movr(reg, JIT_R0);
    }
    
    if (save_regs) {
      stack_pop(JIT_R0, &stack_ptr);
    }

    return coerced;
  }
}

void compile_add(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(+ a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);

  stack_push(JIT_R0, &stack_ptr);
  stack_push(JIT_R1, &stack_ptr);
  
#ifdef DEBUG
  jit_prepare();
  jit_ellipsis();
  jit_pushargi((jit_word_t)"-- add %d %d\n");
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(printf);
#endif

  stack_pop(JIT_R1, &stack_ptr);
  stack_pop(JIT_R0, &stack_ptr);
  
  jit_addr(JIT_R0, JIT_R0, JIT_R1);
}

void compile_sub(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(- a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  jit_subr(JIT_R0, JIT_R0, JIT_R1);
}

void compile_mul(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(* a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  jit_mulr(JIT_R0, JIT_R0, JIT_R1);
}

void compile_div(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(/ a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  jit_divr(JIT_R0, JIT_R0, JIT_R1);
}

void compile_mod(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(% a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  
  //stack_push(JIT_R2, &stack_ptr);
  jit_movr(JIT_R2, JIT_R0);
  jit_divr(JIT_R0, JIT_R0, JIT_R1);
  jit_mulr(JIT_R0, JIT_R0, JIT_R1);
  jit_subr(JIT_R0, JIT_R2, JIT_R0);
  //stack_pop(JIT_R2, &stack_ptr);
}

void compile_lt(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(lt a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  jit_ltr(JIT_R0, JIT_R0, JIT_R1);
}

void compile_gt(Cell* args) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(gt a b)");
  compile_arg(JIT_R0, car(args), 0, 1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  jit_gtr(JIT_R0, JIT_R0, JIT_R1);
}

// FIXME: cheap way of detecting (tail) recursion
// later, manage this as a part of compiler state
// that is passed around
static Cell* currently_compiling_fn_sym = NULL;
static Cell* currently_compiling_fn_op = NULL;
static jit_node_t* currently_compiling_fn_label = NULL;

void compile_def(Cell* args, int wants_int) {
  if (!car(args) || !cdr(args) || !car(cdr(args))) return argnum_error("(def symbol definition)");
  Cell* sym = car(args);
  Cell* value = car(cdr(args));

  // analysis of what we are defining
  if (value) {
    if (value->tag == TAG_CONS) {
      Cell* opsym = car(value);
      if (opsym && opsym->tag == TAG_SYM) {
        Cell* op = lookup_symbol(opsym->addr, &global_env);
        if (op && op->value == BUILTIN_FN) {
          // we are binding a function
          currently_compiling_fn_sym = sym;
          //printf("-- compiling bound fn: %s\n",currently_compiling_fn_sym->addr);
        }
      }
    }
  }
  
  int coerced = compile_arg(JIT_R0, value, 0, 0);
  stack_push(JIT_R0, &stack_ptr);
  
  jit_prepare();
  jit_pushargi((jit_word_t)sym);

  if (coerced) {
    // copy int out of recycled cell
    jit_ldr(JIT_R0, JIT_R0);
    jit_pushargr(JIT_R0);
    jit_pushargi((jit_word_t)&global_env);
    jit_finishi(insert_symbol_int);
  } else {
    // store a cell
    jit_pushargr(JIT_R0);
    jit_pushargi((jit_word_t)&global_env);
    jit_finishi(insert_symbol);
  }
  stack_pop(JIT_R0, &stack_ptr);
  if (wants_int) {
    jit_ldr(JIT_R0, JIT_R0); // load cell value
  }

  currently_compiling_fn_sym = NULL;
  currently_compiling_fn_label = NULL;
}

void compile_print(Cell* args) {
  if (!car(args)) return argnum_error("(print a)");
  Cell* arg = car(args);

  compile_arg(JIT_R0, arg, 0, 1);
  stack_push(JIT_R0, &stack_ptr);

  jit_prepare();
  jit_pushargi((jit_word_t)"%d\n");
  jit_ellipsis();
  jit_pushargr(JIT_R0);
  jit_finishi(printf);

  stack_pop(JIT_R0, &stack_ptr);
}

void compile_load(Cell* args) {
  if (!car(args)) return argnum_error("(load \"/local/path\")");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, arg, 0, 0);
  jit_ldr(JIT_R0, JIT_R0); // load string addr
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(machine_load_file); // returns bytes cell
  jit_retval(JIT_R0);
}

void compile_save(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(save value \"/local/path\")");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, car(cdr(args)), 0, 0);
  jit_ldr(JIT_R0, JIT_R0); // load path addr
  stack_push(JIT_R0, &stack_ptr);
  
  compile_arg(JIT_R0, arg, 0, 0);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_save_file); // returns bytes cell
  jit_retval(JIT_R0);
}

void compile_rect_fill(Cell* args) {
  Cell* arg_x = car(args);
  if (!arg_x) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_y = car(args);
  if (!arg_y) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_w = car(args);
  if (!arg_w) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_h = car(args);
  if (!arg_h) return argnum_error("(rectfill x y w h color)");
  args = cdr(args);
  Cell* arg_c = car(args);
  if (!arg_c) return argnum_error("(rectfill x y w h color)");
  
  compile_arg(JIT_R0, arg_c, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_h, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_w, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_y, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_x, 0, 1);
  stack_push(JIT_R0, &stack_ptr);

  jit_prepare();
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_video_rect);
}

void compile_pixel(Cell* args) {
  Cell* arg_x = car(args);
  if (!arg_x) return argnum_error("(pixel x y color)");
  args = cdr(args);
  Cell* arg_y = car(args);
  if (!arg_y) return argnum_error("(pixel x y color)");
  args = cdr(args);
  Cell* arg_c = car(args);
  if (!arg_c) return argnum_error("(pixel x y color)");
  
  compile_arg(JIT_R0, arg_c, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_y, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  compile_arg(JIT_R0, arg_x, 0, 1);
  stack_push(JIT_R0, &stack_ptr);
  
  jit_prepare();
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  stack_pop(JIT_R0, &stack_ptr);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_video_set_pixel);
}

void compile_flip() {
  jit_prepare();
  jit_finishi(machine_video_flip);
}

void compile_udp_poll() {
  jit_prepare();
  jit_finishi(machine_poll_udp);
  jit_retval(JIT_R0);
}

void compile_udp_send(Cell* args) {
  jit_prepare();
  compile_arg(JIT_R0, car(args), 0, 0);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_send_udp);
  jit_retval(JIT_R0);
}

void compile_tcp_connect(Cell* args) {
  compile_arg(JIT_R0, car(args), 0, 0);
  compile_arg(JIT_R1, car(cdr(args)), 1, 0);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(machine_connect_tcp);
  jit_retval(JIT_R0);
}

void compile_tcp_bind(Cell* args) {
  jit_prepare();
  compile_arg(JIT_R0, car(args), 0, 0);
  jit_pushargr(JIT_R0);
  compile_arg(JIT_R0, car(cdr(args)), 0, 0);
  jit_pushargr(JIT_R0);
  jit_finishi(machine_bind_tcp);
  jit_retval(JIT_R0);
}

void compile_tcp_send(Cell* args) {
  compile_arg(JIT_R0, car(args), 0, 0);
  compile_arg(JIT_R1, car(cdr(args)), 1, 0);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(machine_send_tcp);
  jit_retval(JIT_R0);
}

void compile_blit(Cell* args) {
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(blit_vector32);
}

#define compile_int_arg(); args = cdr(args);\
  if (!car(args)) { printf("error: missing arguments.\n"); return; }\
  compile_arg(JIT_R0, car(args), 0, 1);\
  stack_push(JIT_R0, &stack_ptr);

#define push_stack_arg(); stack_pop(JIT_R0, &stack_ptr);\
  jit_pushargr(JIT_R0);

void compile_blit_mono(Cell* args) {
  compile_arg(JIT_R0, car(args), 0, 0);
  jit_ldr(JIT_R0, JIT_R0); // load bytes addr
  stack_push(JIT_R0, &stack_ptr);
  
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();

  jit_prepare();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg(); // pop bytes addr
  jit_finishi(blit_vector1);
  jit_retval(JIT_R0);
}

void compile_blit_mono_inv(Cell* args) {
  compile_arg(JIT_R0, car(args), 0, 0);
  jit_ldr(JIT_R0, JIT_R0); // load bytes addr
  stack_push(JIT_R0, &stack_ptr);
  
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();
  compile_int_arg();

  jit_prepare();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg();
  push_stack_arg(); // pop bytes addr
  jit_finishi(blit_vector1_invert);
  jit_retval(JIT_R0);
}

void compile_get_key(Cell* args) {
  if (!car(args)) return argnum_error("(inkey 0=keycode|1=modifiers)");
  compile_arg(JIT_R0, car(args), 0, 1);
  
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(machine_get_key);
  jit_retval(JIT_R0);
}

Cell* make_symbol_list() {
  //printf("use (symbolname arg1 arg2…) to apply symbols. known symbols are:\n");
  Cell* end = alloc_nil();
  
  for (env_entry* e=global_env; e != NULL; e=e->hh.next) {
    //printf("%s\n", e->name);

    end = alloc_cons(alloc_string_copy(e->name), end);
  }
  return end;
}

void compile_help() {
  jit_prepare();
  jit_finishi(make_symbol_list);
  jit_retval(JIT_R0);
}

void compile_do(Cell* args, int wants_int) {
  if (!car(args)) return argnum_error("(do op1 op2 …)");
  compile_arg(JIT_R0, car(args), 0, 0);

  while ((args = cdr(args)) && car(args)) {
    int is_last = !(car(cdr(args)));
    compile_arg(JIT_R0, car(args), 0, (is_last && wants_int));
  }
}

static int num_funcs = 0;
static int stack_ptr_saved = 0;
static int stack_base_saved = 0;

Cell* compile_fn(Cell* args) {
  if (!car(args)) {
    argnum_error("(fn arg1 arg2 … (body))");
    return NULL;
  }
  
  // args 0..n-2 = parameter symbols
  // arg n-1 = body

  num_funcs++;

  Cell* args_saved = args;

#ifdef DEBUG
  if (currently_compiling_fn_sym) {
    printf("-- compile_fn %s\n",currently_compiling_fn_sym->addr);
  } else {
    printf("-- compile_fn (closure)\n");
  }
#endif
  
  // skip to the body
  //printf("args: %p %s %p\n",car(args),car(args)->addr,args->next);
  while (car(args) && cdr(args) && car(cdr(args))) {
    //printf("arg: %p %s %p\n",args,car(args)->addr,args->next);
    args = cdr(args);
  }
  //printf("body: %p %d %p\n",args,args->tag,args->addr);
  
  _jit_saved = _jit;
  stack_ptr_saved = stack_ptr;
  stack_base_saved = stack_base;
  
  _jit = jit_new_state();
  jit_node_t* fn_label = jit_note(__FILE__, __LINE__);
  jit_prolog();
  
  stack_ptr = stack_base = jit_allocai(1024 * sizeof(int));
  
  jit_node_t* fn_body_label = jit_label();

  Cell* res = alloc_lambda(args_saved);
  // store info for potential recursion
  // currently_compiling_fn_label = fn_body_label;
  currently_compiling_fn_label = fn_label;
  currently_compiling_fn_op = res;

  // compile body
  // TODO: save _jit_saved on a stack
  compile_arg(JIT_R0, car(args), 0, 0);
  
  jit_retr(JIT_R0);
  jit_epilog();

  // res->addr will point to the args
  res->next = jit_emit();

#ifdef DEBUG
  printf("-- emitted at %p in %p\n",res->next,res);
  printf("<assembled: %p>\n",res->next);
  jit_disassemble();
  printf("--------------------------------------\n");
#endif
  
  jit_clear_state();
  //jit_destroy_state();

  _jit = _jit_saved;
  stack_ptr = stack_ptr_saved;
  stack_base = stack_base_saved;

  // return the allocated lambda
  jit_movi(JIT_R0, (jit_word_t)res);

  return res;
}

// compile application of a compiled function
void compile_lambda(Cell* lbd, Cell* args, int recursion) {
  jit_node_t* ret_label = jit_note(__FILE__, __LINE__);

  //printf("<lambda: %p>\n",lbd->next);
  
  Cell* args_orig = args;
  Cell* pargs = (Cell*)lbd->addr;
  Cell* pargs_orig = pargs;

  Cell* argnames[10]; // max argnames 10
  int argtypes[10]; // 0 = cell, 1 = int

  int i = 0;

  // FIXME: this fails if prototype parameter name is used inside of an argument expression

  // pass 0: save old symbol values
  while (i<10 && car(args) && car(pargs)) {
    // ignore the last arg, which is the function body
    if (car(cdr(pargs))) {
      Cell* sym = car(pargs);
      
      char buffer[64];
      sprintf(buffer,"save arg: %d %s\n",i,sym->addr);
      jit_note(buffer, __LINE__);

      // fixme: problem: this stores pointers, not values
      
      if (recursion<2) {
        // lookup any old symbol value, push it to stack
        jit_prepare();
        jit_pushargi((jit_word_t)sym->addr);
        jit_pushargi((jit_word_t)&global_env);
        jit_finishi(lookup_symbol);
        // store pointer
        stack_push(JIT_R0, &stack_ptr);

        jit_node_t* if_nil = jit_beqi(JIT_R0, 0);
        jit_node_t* skip_load = jit_forward();
        jit_ldr(JIT_R0, JIT_R0);
        
        jit_patch_at(if_nil,skip_load);
        jit_link(skip_load);
        
        // store value
        stack_push(JIT_R0, &stack_ptr);
      }
      
      i++;
    }
    pargs = cdr(pargs);
    args = cdr(args);
  }
  
  // pass 1: compile new values to temp slots
  args = args_orig;
  pargs = pargs_orig;
  i = 0;
  while (i<10 && car(args) && car(pargs)) {
    // ignore the last arg, which is the function body
    if (car(cdr(pargs))) {
      Cell* sym = car(pargs);
      argnames[i] = sym;
      
      // compile the argument value
      int coerced = compile_arg(JIT_R0, car(args), 0, 0);
      
      char buffer[64];
      sprintf(buffer,"compile arg: %d (%s) %s\n",i,(coerced?"int":"cell"),sym->addr);
      jit_note(buffer, __LINE__);

      argtypes[i] = 0;
      if (coerced) {
        // copy int out of recycled cell
        jit_ldr(JIT_R0, JIT_R0);
        argtypes[i] = 1;
      }

      // push for bind in phase 2
      stack_push(JIT_R0, &stack_ptr);
      
#ifdef DEBUG
      jit_prepare();
      jit_ellipsis();
      jit_pushargi((jit_word_t)"pushed arg: %x\n");
      jit_pushargr(JIT_R0);
      jit_finishi(printf);
#endif
      
      i++;
    }
    pargs = cdr(pargs);
    args = cdr(args);
  }

  // pass 2: bind temp slots to symbol names for use in fn body
  for (int j=i-1; j>=0; j--) {
    Cell* sym = argnames[j];

    char buffer[64];
    sprintf(buffer,"bind arg: %d %s\n",j,sym->addr);
    jit_note(buffer, __LINE__);
    
    jit_prepare();
    jit_pushargi((jit_word_t)sym);
    stack_pop(JIT_R0, &stack_ptr);
    jit_pushargr(JIT_R0);
    jit_pushargi((jit_word_t)&global_env);
    if (argtypes[j] == 0) {
      jit_finishi(insert_symbol);
    } else {
      jit_finishi(insert_symbol_int);
    }
  }

  // pass 3: jump/call
  
  // TODO: tail recursion

  if (recursion == 1) {
    jit_note("jump to lambda as recursion\n",__LINE__);
    // get jump address at runtime
    jit_movi(JIT_R0, (jit_word_t)currently_compiling_fn_op);
    jit_ldxi(JIT_R0, JIT_R0, sizeof(jit_word_t)); // *(r0 + 1) -> r0

    /*jit_prepare();
    jit_ellipsis();
    jit_pushargi((jit_word_t)"---- would jump to: %llx\n");
    jit_pushargr(JIT_R0);
    jit_finishi(printf);*/

    jit_prepare();
    jit_finishr(JIT_R0);
    jit_retval(JIT_R0);
    
    //jit_node_t* rec_jump = jit_calli(currently_compiling_fn_label);
    //jit_patch_at(rec_jump, );
  } else {
    jit_note("call lambda as function\n",__LINE__);
    jit_prepare();
    jit_finishi(lbd->next);
  }

  // pass 4: restore environment
  
  if (recursion<2) {
    jit_movr(JIT_R3, JIT_R0); // fixme: how to ensure this is a clobber-free reg?

    // after call, restore old symbol values from the stack (in reverse order)
    for (int j=i-1; j>=0; j--) {
      char buffer[64];
      sprintf(buffer,"restore arg %d %s",j,argnames[j]->addr);
      jit_note(buffer, __LINE__);

      // pop value
      stack_pop(JIT_R2, &stack_ptr);
      
      jit_prepare();
      jit_pushargi((jit_word_t)argnames[j]);
      stack_pop(JIT_R0, &stack_ptr);
      
      jit_node_t* skip_restore = jit_forward();
      jit_node_t* if_nil = jit_beqi(JIT_R0, 0);
      jit_patch_at(if_nil,skip_restore);
      
      jit_str(JIT_R0, JIT_R2); // restore any overwritten value
      
      jit_link(skip_restore);
      
      jit_pushargr(JIT_R0);
      jit_pushargi((jit_word_t)&global_env);
      jit_finishi(insert_symbol);
      
    }
    
    jit_movr(JIT_R0, JIT_R3);
  }
}

void compile_if(Cell* args, int wants_int) {
  if (!car(args) || !car(cdr(args)) || !cdr(cdr(args)) || !car(cdr(cdr(args)))) return argnum_error("(if condition (then-body) (else-body))");
  
  jit_node_t *jump, *jump2, *else_label, *exit_label;
  
  // lbl1:

  compile_arg(JIT_R0, car(args), 0, 1);

  // cmp r0, 1
  // beq lbl2
  jump = jit_beqi(JIT_R0, 0);

  // then
  
  compile_arg(JIT_R0, car(cdr(args)), 0, wants_int);

  // exit
  jump2 = jit_jmpi();
  
  else_label = jit_label();

  compile_arg(JIT_R0, car(cdr(cdr(args))), 0, wants_int);
  
  exit_label = jit_label();
  
  jit_patch_at(jump, else_label);
  jit_patch_at(jump2, exit_label);
}

void compile_while(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(while condition (body))");
  
  jit_node_t *jump, *jump2, *loop_label, *exit_label;
  
  // lbl1:

  loop_label = jit_label();

  compile_arg(JIT_R0, car(args), 0, 1);

  // cmp r0, 1
  // beq lbl2
  jump = jit_beqi(JIT_R0, 0);
  
  compile_arg(JIT_R1, car(cdr(args)), 1, 0);

  // lbl2:
  jump2 = jit_jmpi();

  exit_label = jit_label();
  jit_movr(JIT_R0, JIT_R1);
  
  jit_patch_at(jump, exit_label);
  jit_patch_at(jump2, loop_label);
}

void compile_quote(Cell* args) {
  if (!car(args)) return argnum_error("(quote arg)");
  jit_movi(JIT_R0, (jit_word_t)car(args));
}

void compile_car(Cell* args) {
  if (!car(args)) return argnum_error("(car list)");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, arg, 0, 0);
  jit_ldr(JIT_R0, JIT_R0); // car r0 = r0->addr
}

void compile_cdr(Cell* args) {
  if (!car(args)) return argnum_error("(cdr list)");
  Cell* arg = car(args);
  
  compile_arg(JIT_R0, arg, 0, 0);
  jit_ldxi(JIT_R0, JIT_R0, sizeof(jit_word_t)); // cdr r0 = r0 + one word = r0->next
}

void compile_cons(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(cons new-item list)");
  
  compile_arg(JIT_R0, car(args), 0, 0);
  compile_arg(JIT_R1, car(cdr(args)), 1, 0);

  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(alloc_cons);
  jit_retval(JIT_R0);
}

// write
// allocates a string object and writes s-expression dump of object
// into it
void compile_write(Cell* args) {
  if (!car(args)) return argnum_error("(write buffer object)");
  Cell* buf_arg = car(args);
  compile_arg(JIT_R0, buf_arg, 0, 0);
  jit_movr(JIT_V0,JIT_R0); // back up the buffer cell in V0
  Cell* obj_arg = car(cdr(args));
  compile_arg(JIT_R1, obj_arg, 1, 0);

  jit_prepare();
  jit_pushargr(JIT_R1); // object Cell*
  jit_ldr(JIT_R0, JIT_V0);
  jit_pushargr(JIT_R0); // buffer char*
  jit_ldxi(JIT_R2, JIT_V0, sizeof(jit_word_t)); // buffer size
  jit_pushargr(JIT_R2); // buffer size 
  jit_finishi(lisp_write);
  
  jit_movr(JIT_R0,JIT_V0); // return target buffer cell
}

jit_word_t compile_compile(Cell* expr) {
  if (!expr) return (jit_word_t)alloc_nil();

  if (expr->tag!=TAG_STR && expr->tag!=TAG_BYTES) return (jit_word_t)alloc_error(ERR_INVALID_PARAM_TYPE);
  
  Cell* read_expr = read_string(expr->addr);

  if (!read_expr) return (jit_word_t)alloc_error(ERR_APPLY_NIL);

  //Cell* compiled = compile_fn(alloc_cons(read_expr,NULL));
  
  //funcptr fn = (funcptr)compiled->next;
  //printf("compiled fn: %p\n",fn);
  //return fn();

  _jit_saved = _jit;
  stack_ptr_saved = stack_ptr;
  stack_base_saved = stack_base;
  
  _jit = jit_new_state();
  jit_node_t* fn_label = jit_label();
  jit_prolog();
  
  stack_ptr = stack_base = jit_allocai(1024 * sizeof(int));
  
  jit_node_t* fn_body_label = jit_label();

  Cell* res = alloc_lambda(alloc_nil());
  
  compile_arg(JIT_R0, read_expr, 0, 0);
  
  jit_retr(JIT_R0);
  jit_epilog();

  res->next = jit_emit();

  //printf("-- emitted at %p in %p\n",res->next,res);
  //printf("<assembled: %p>\n",res->next);
  
  jit_clear_state();
  
  _jit = _jit_saved;
  stack_ptr = stack_ptr_saved;
  stack_base = stack_base_saved;

  funcptr fn = (funcptr)res->next;
  
  return fn();
}

typedef jit_word_t (*funcptr)();

// eval
void compile_eval(Cell* args) {
  Cell* arg = car(args);
  if (!arg) return argnum_error("(eval string)");
  
  compile_arg(JIT_R0, arg, 0, 0);

  // compile s-exp
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(compile_compile);
  jit_retval(JIT_R0);
}

// vectors/strings
void compile_get(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(get bytes-or-string index)");
  Cell* arg = car(args);

  // TODO: tag + bounds check
  
  compile_arg(JIT_R0, arg, 0, 0);
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);

  // fetch size
  // jit_ldxi(JIT_R2, JIT_R0, sizeof(jit_word_t));
  // jit_node_t* jump = jit_bltr(JIT_R2, JIT_R1);
  
  jit_ldr(JIT_R0, JIT_R0); // car r0 = r0->addr
  jit_ldxr_uc(JIT_R0, JIT_R0, JIT_R1); // *(r0 + r1) -> r0
}

// utf8 char get
void compile_uget(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(uget string index)");
  Cell* arg = car(args);

  // TODO: tag + bounds check

  compile_arg(JIT_R0, arg, 0, 0);
  compile_arg(JIT_R1, car(cdr(args)), 1, 0);
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_finishi(utf8_rune_at_cell);
  jit_retval(JIT_R0);
}

void compile_put(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(put bytes-or-string index value)");
  Cell* arg = car(args);

  // TODO: tag + bounds check
  // TODO: optimize
  
  compile_arg(JIT_R0, arg, 0, 0);
  jit_ldr(JIT_R0, JIT_R0); // car r0 = r0->addr
  compile_arg(JIT_R1, car(cdr(args)), 1, 1);
  jit_addr(JIT_R0, JIT_R0, JIT_R1);
  compile_arg(JIT_R1, car(cdr(cdr(args))), 1, 1);
  jit_str_c(JIT_R0, JIT_R1); // *(r0 + r1) -> r0
}

// utf8 char put
void compile_uput(Cell* args) {
  if (!car(args) || !car(cdr(args))) return argnum_error("(uput string index rune)");
  Cell* arg = car(args);

  compile_arg(JIT_R0, arg, 0, 0);
  compile_arg(JIT_R1, car(cdr(cdr(args))), 1, 0);
  jit_movr(JIT_V0, JIT_R1);
  compile_arg(JIT_R1, car(cdr(args)), 1, 0);

  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_pushargr(JIT_R1);
  jit_pushargr(JIT_V0);
  jit_finishi(utf8_put_rune_at); // checks tag + bounds
  jit_retval(JIT_R0);
}

void compile_size(Cell* args) {
  if (!car(args)) return argnum_error("(size bytes/string)");
  Cell* arg = car(args);

  // FIXME: will crash with NULL
  
  compile_arg(JIT_R0, arg, 0, 0);
  jit_ldxi(JIT_R0, JIT_R0, sizeof(jit_word_t)); // cdr r0 = r0 + one word = r0->next
}

// utf8 string length
void compile_usize(Cell* args) {
  if (!car(args)) return argnum_error("(usize string)");
  Cell* arg = car(args);

  compile_arg(JIT_R0, arg, 0, 0);
  jit_prepare();
  jit_pushargr(JIT_R0);
  jit_finishi(utf8_strlen_cell); // this checks the tag
  jit_retval(JIT_R0);
}

int coerce_int_to_cell() {
  //jit_note("coerce_int_to_cell",__LINE__);
  jit_sti(&(coerce_int_cell->value), JIT_R0);
  jit_movi(JIT_R0, (jit_word_t)coerce_int_cell);
  return 1;
}

int coerce_cell_to_int(Cell* cell) {
  if (cell) return cell->value;
  return 0;
}

// FIXME: lambda applications are compile-time bound
// returns 1 if result stored in recycled int cell
int compile_applic(Cell* list, int wants_int) {
  jit_note("compile_applic",__LINE__);
  
  if (!car(list)) {
    jit_movi(JIT_R0, 0);
    return 0;
  }

  if (car(list)->tag!=TAG_SYM) {
    if (wants_int) {
      jit_movi(JIT_R0, car(list)->value);
    } else {
      jit_movi(JIT_R0, (jit_word_t)car(list));
    }
    return 0;
  }

  char* fn_name = car(list)->addr;
  Cell* op_cell = NULL;
  int recursion = 0;

  if (currently_compiling_fn_sym) {
    if (strcmp(currently_compiling_fn_sym->addr, fn_name) == 0) {
      // recursion!

      //printf("-- recursion detected! label: %p\n", currently_compiling_fn_label);
      op_cell = currently_compiling_fn_op;
      recursion = 1;
    }
  }

  if (!op_cell) {
    op_cell = lookup_symbol(fn_name, &global_env);
  }
  
  //printf("lookup %s -> op cell %p\n",car(list)->addr,op_cell);

  if (!op_cell) {
    printf("<error:undefined symbol %s>\n",car(list)->addr);
    jit_movi(JIT_R0, 0);
    return 0;
  }
  
  jit_word_t op = op_cell->value;

  if (op_cell->tag == TAG_LAMBDA) {
    compile_lambda(op_cell, cdr(list), recursion);
    if (wants_int) {
      jit_prepare();
      jit_pushargr(JIT_R0);
      jit_finishi(coerce_cell_to_int);
      jit_retval(JIT_R0); // FIXME: rethink
      return 0;
    }
    return 1;
  }
  
  //printf("lookup %s -> op %d\n",car(list)->addr,op);
  
  switch (op) {
  case BUILTIN_ADD:
    compile_add(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_SUB:
    compile_sub(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_MUL:
    compile_mul(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_DIV:
    compile_div(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_MOD:
    compile_mod(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_LT:
    compile_lt(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_GT:
    compile_gt(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
    
  case BUILTIN_IF:
    compile_if(cdr(list), wants_int);
    break;
  case BUILTIN_WHILE:
    compile_while(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_DO:
    compile_do(cdr(list), wants_int);
    break;
  case BUILTIN_FN:
    compile_fn(cdr(list));
    break;
  case BUILTIN_DEF:
    compile_def(cdr(list), wants_int);
    break;
    
  case BUILTIN_QUOTE:
    compile_quote(cdr(list));
    break;
  case BUILTIN_CAR:
    compile_car(cdr(list));
    break;
  case BUILTIN_CDR:
    compile_cdr(cdr(list));
    break;
  case BUILTIN_CONS:
    compile_cons(cdr(list));
    break;

  case BUILTIN_EVAL:
    compile_eval(cdr(list));
    break;
  case BUILTIN_WRITE:
    compile_write(cdr(list));
    break;
    
  case BUILTIN_GET:
    compile_get(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_UGET:
    compile_uget(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_PUT:
    compile_put(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_UPUT:
    compile_uput(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_SIZE:
    compile_size(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_USIZE:
    compile_usize(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  
  case BUILTIN_PRINT:
    compile_print(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
    
  case BUILTIN_PIXEL:
    compile_pixel(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_RECTFILL:
    compile_rect_fill(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_FLIP:
    compile_flip();
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_BLIT_MONO:
    compile_blit_mono(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_BLIT_MONO_INV:
    compile_blit_mono_inv(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_INKEY:
    compile_get_key(cdr(list));
    if (!wants_int) return coerce_int_to_cell();
    break;
  case BUILTIN_HELP:
    compile_help();
    break;
  case BUILTIN_LOAD:
    compile_load(cdr(list));
    break;
  case BUILTIN_SAVE:
    compile_save(cdr(list));
    break;
    
  case BUILTIN_UDP_POLL:
    compile_udp_poll();
    break;
  case BUILTIN_UDP_SEND:
    compile_udp_send(cdr(list));
    break;
    
  case BUILTIN_TCP_CONNECT:
    compile_tcp_connect(cdr(list));
    break;
  case BUILTIN_TCP_SEND:
    compile_tcp_send(cdr(list));
    break;
  case BUILTIN_TCP_BIND:
    compile_tcp_bind(cdr(list));
    break;
  }
  return 0;
}

void init_compiler() {
  init_allocator();
  coerce_int_cell = alloc_int(0);
  error_cell = alloc_error(0);
  
  insert_symbol(alloc_sym("+"), alloc_builtin(BUILTIN_ADD), &global_env);
  insert_symbol(alloc_sym("-"), alloc_builtin(BUILTIN_SUB), &global_env);
  insert_symbol(alloc_sym("*"), alloc_builtin(BUILTIN_MUL), &global_env);
  insert_symbol(alloc_sym("/"), alloc_builtin(BUILTIN_DIV), &global_env);
  insert_symbol(alloc_sym("%"), alloc_builtin(BUILTIN_MOD), &global_env);
  
  insert_symbol(alloc_sym("lt"), alloc_builtin(BUILTIN_LT), &global_env);
  insert_symbol(alloc_sym("gt"), alloc_builtin(BUILTIN_GT), &global_env);
  
  insert_symbol(alloc_sym("if"), alloc_builtin(BUILTIN_IF), &global_env);
  insert_symbol(alloc_sym("while"), alloc_builtin(BUILTIN_WHILE), &global_env);
  insert_symbol(alloc_sym("def"), alloc_builtin(BUILTIN_DEF), &global_env);
  insert_symbol(alloc_sym("print"), alloc_builtin(BUILTIN_PRINT), &global_env);
  insert_symbol(alloc_sym("do"), alloc_builtin(BUILTIN_DO), &global_env);
  insert_symbol(alloc_sym("fn"), alloc_builtin(BUILTIN_FN), &global_env);
  
  insert_symbol(alloc_sym("quote"), alloc_builtin(BUILTIN_QUOTE), &global_env);
  insert_symbol(alloc_sym("car"), alloc_builtin(BUILTIN_CAR), &global_env);
  insert_symbol(alloc_sym("cdr"), alloc_builtin(BUILTIN_CDR), &global_env);
  insert_symbol(alloc_sym("cons"), alloc_builtin(BUILTIN_CONS), &global_env);

  insert_symbol(alloc_sym("get"), alloc_builtin(BUILTIN_GET), &global_env);
  insert_symbol(alloc_sym("uget"), alloc_builtin(BUILTIN_UGET), &global_env);
  insert_symbol(alloc_sym("put"), alloc_builtin(BUILTIN_PUT), &global_env);
  insert_symbol(alloc_sym("uput"), alloc_builtin(BUILTIN_UPUT), &global_env);
  insert_symbol(alloc_sym("size"), alloc_builtin(BUILTIN_SIZE), &global_env);
  insert_symbol(alloc_sym("usize"), alloc_builtin(BUILTIN_USIZE), &global_env);

  insert_symbol(alloc_sym("write"), alloc_builtin(BUILTIN_WRITE), &global_env);
  insert_symbol(alloc_sym("eval"), alloc_builtin(BUILTIN_EVAL), &global_env);
  
  insert_symbol(alloc_sym("pixel"), alloc_builtin(BUILTIN_PIXEL), &global_env);
  insert_symbol(alloc_sym("rectfill"), alloc_builtin(BUILTIN_RECTFILL), &global_env);
  insert_symbol(alloc_sym("flip"), alloc_builtin(BUILTIN_FLIP), &global_env);
  insert_symbol(alloc_sym("blit-mono"), alloc_builtin(BUILTIN_BLIT_MONO), &global_env);
  insert_symbol(alloc_sym("blit-mono-inv"), alloc_builtin(BUILTIN_BLIT_MONO_INV), &global_env);
  insert_symbol(alloc_sym("inkey"), alloc_builtin(BUILTIN_INKEY), &global_env);
  
  insert_symbol(alloc_sym("ls"), alloc_builtin(BUILTIN_HELP), &global_env);
  insert_symbol(alloc_sym("load"), alloc_builtin(BUILTIN_LOAD), &global_env);
  insert_symbol(alloc_sym("save"), alloc_builtin(BUILTIN_SAVE), &global_env);
  
  insert_symbol(alloc_sym("udp-poll"), alloc_builtin(BUILTIN_UDP_POLL), &global_env);
  insert_symbol(alloc_sym("udp-send"), alloc_builtin(BUILTIN_UDP_SEND), &global_env);

  insert_symbol(alloc_sym("tcp-bind"), alloc_builtin(BUILTIN_TCP_BIND), &global_env);
  insert_symbol(alloc_sym("tcp-connect"), alloc_builtin(BUILTIN_TCP_CONNECT), &global_env);
  insert_symbol(alloc_sym("tcp-send"), alloc_builtin(BUILTIN_TCP_SEND), &global_env);

  int num_syms=HASH_COUNT(global_env);
  printf("sledge knows %u symbols. enter (ls) to see them.\n\n", num_syms);
}
