#ifndef SLEDGE_COMPILER_H
#define SLEDGE_COMPILER_H

#include <string.h>

#define MAXARGS 8
#define MAXFRAME 64 // maximum MAXFRAME-MAXARGS local vars

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
  char* type_name;
} Arg;

typedef struct Frame Frame;

struct Frame {
  Arg* f;
  int sp;
  int locals;
  void* stack_end;
  Frame* parent_frame;
};

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
  BUILTIN_IF,
  BUILTIN_FN,

  BUILTIN_CAR,
  BUILTIN_CDR,
  BUILTIN_CONS,
  BUILTIN_LIST,

  BUILTIN_ALLOC,
  BUILTIN_ALLOC_STR,
  BUILTIN_NEW,
  BUILTIN_BYTES_TO_STR,
  BUILTIN_STR_TO_BYTES,
  BUILTIN_CONCAT,
  BUILTIN_SUBSTR,

  BUILTIN_GET8,
  BUILTIN_GET16,
  BUILTIN_GET32,
  BUILTIN_PUT8,
  BUILTIN_PUT16,
  BUILTIN_PUT32,
  BUILTIN_SGET,
  BUILTIN_SPUT,
  BUILTIN_SIZE,

  BUILTIN_UGET,
  BUILTIN_UPUT,
  BUILTIN_USIZE,

  BUILTIN_TYPE,
  BUILTIN_LET,
  BUILTIN_QUOTE,
  BUILTIN_STRUCT,
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
