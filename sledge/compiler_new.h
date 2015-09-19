#ifndef SLEDGE_COMPILER_H
#define SLEDGE_COMPILER_H

#include <string.h>

#define MAXARGS 8
#define MAXFRAME 24 // maximum MAXFRAME-MAXARGS local vars

typedef void* (*funcptr)();

typedef enum arg_t {
  ARGT_CONST,
  ARGT_ENV,
  ARGT_LAMBDA,
  ARGT_REG,
  ARGT_REG_INT,
  ARGT_STACK,
  ARGT_STACK_INT
} arg_t;

typedef struct Arg {
  arg_t type;
  Cell* cell;
  env_entry* env;
  int slot;
  char* name;
} Arg;

typedef struct Frame {
  Arg* f;
  int sp;
  int locals;
  void* stack_end;
} Frame;

typedef struct Label {
  char* name;
  int idx;
} Label;

typedef enum builtin_t {
  BUILTIN_ADD = 1,
  BUILTIN_SUB,
  BUILTIN_MUL,
  BUILTIN_DIV,
  BUILTIN_MOD,
  
  BUILTIN_BITAND,
  BUILTIN_BITNOT,
  BUILTIN_BITOR,
  BUILTIN_BITXOR,
  BUILTIN_SHL,
  BUILTIN_SHR,

  BUILTIN_LT,
  BUILTIN_GT,
  BUILTIN_EQ,

  BUILTIN_WHILE,

  BUILTIN_DEF,
  BUILTIN_IF ,
  BUILTIN_FN ,

  BUILTIN_CAR,
  BUILTIN_CDR,
  BUILTIN_CONS,
  BUILTIN_LIST,

  BUILTIN_ALLOC,
  BUILTIN_ALLOC_STR,
  BUILTIN_BYTES_TO_STR,
  BUILTIN_STR_TO_BYTES,
  BUILTIN_CONCAT,
  BUILTIN_SUBSTR,

  BUILTIN_GET,
  BUILTIN_GET32,
  BUILTIN_PUT,
  BUILTIN_PUT32,
  BUILTIN_SIZE,

  BUILTIN_UGET,
  BUILTIN_UPUT,
  BUILTIN_USIZE,

  BUILTIN_TYPE,
  BUILTIN_LET,
  BUILTIN_QUOTE,
  BUILTIN_MAP,
  BUILTIN_DO,

  BUILTIN_EVAL,
  BUILTIN_READ,
  BUILTIN_WRITE,

  BUILTIN_PRINT,
  BUILTIN_MOUNT,
  BUILTIN_MMAP,
  BUILTIN_OPEN,
  BUILTIN_CLOSE,
  BUILTIN_RECV,
  BUILTIN_SEND,

  BUILTIN_PIXEL,
  BUILTIN_FLIP,
  BUILTIN_RECTFILL,
  BUILTIN_BLIT,
  BUILTIN_BLIT_MONO,
  BUILTIN_BLIT_MONO_INV,
  BUILTIN_BLIT_STRING,
  BUILTIN_INKEY,

  BUILTIN_GC,
  BUILTIN_SYMBOLS,
  BUILTIN_DEBUG,
  
  BUILTIN_LOAD,
  BUILTIN_SAVE,

  BUILTIN_SIN,
  BUILTIN_COS,
  BUILTIN_SQRT
} builtin_t;

Cell* insert_global_symbol(Cell* symbol, Cell* cell);
env_entry* lookup_global_symbol(char* name);

extern Cell* platform_debug();
Cell* platform_eval(Cell* expr);

#endif
