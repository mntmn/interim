#ifndef MINILISP_H
#define MINILISP_H

#define jit_word_t unsigned long

//#include "lightning.h" // for jit_word_t
#include "strmap.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define TAG_FREED 0
#define TAG_INT  1
#define TAG_CONS 2
#define TAG_SYM  4
#define TAG_LAMBDA  8
#define TAG_BUILTIN 16
#define TAG_BIGNUM 32
#define TAG_STR 64
#define TAG_BYTES 128
#define TAG_VEC 256
#define TAG_ERROR 512
#define TAG_LET 1024
#define TAG_ANY 2048
#define TAG_VOID 4096
#define TAG_STREAM 8192
#define TAG_FS 16384

#define TAG_MARK 65536

#define TAG_PURE_INT 0 // not a cell

#define tag_t jit_word_t

#define MAX_EVAL_DEPTH 10000
#define SYM_INIT_BUFFER_SIZE 32
#define BIGNUM_INIT_BUFFER_SIZE 32

#define ERR_SYNTAX 0
#define ERR_MAX_EVAL_DEPTH 1
#define ERR_UNKNOWN_OP 2
#define ERR_APPLY_NIL 3
#define ERR_INVALID_PARAM_TYPE 4
#define ERR_OUT_OF_BOUNDS 5
#define ERR_OUT_OF_MEMORY 666

#define ERR_NOT_FOUND 404
#define ERR_FORBIDDEN 403

#define FLAG_FREED 1

#define min(a,b) (a > b ? a : b)
#define max(a,b) (b > a ? a : b)

#define unpack_int_arg(var) int var; if (car(args)->tag!=TAG_INT) { return alloc_error(ERR_INVALID_PARAM_TYPE); } else { var = car(args)->value; args=cdr(args); };

typedef struct Cell {
  union {
    jit_word_t value;
    void* addr;
  };
  union {
    jit_word_t size;
    void* next;
  };
  jit_word_t tag;
} Cell;

int is_nil(Cell* c);

typedef struct env_entry {
  Cell* cell;
  char name[64];
} env_entry;

typedef Cell* (*alien_func)(Cell* args, Cell* env);

char* lisp_write(Cell* cell, char* buffer, int bufsize);
Cell* get_globals();
void  register_alien_func(char* symname, alien_func func);

extern Cell* read_string(char* in);

#define car(x) (x?(Cell*)((Cell*)x)->addr:NULL)
#define cdr(x) (x?(Cell*)((Cell*)x)->next:NULL)

#endif
