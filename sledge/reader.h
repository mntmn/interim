#ifndef READER_H
#define READER_H

#include "minilisp.h"

#define PST_ATOM 0
#define PST_NUM 2
#define PST_SYM 3
#define PST_STR 4
#define PST_BIGNUM 5
#define PST_NUM_NEG 6
#define PST_BYTES 7
#define PST_COMMENT 9
#define PST_ERR_UNEXP_CLOSING_BRACE 10
#define PST_ERR_UNEXP_JUNK_IN_NUMBER 11
#define PST_ERR_UNEXP_JUNK_IN_BYTES 12

#define uint unsigned int

typedef struct ReaderState {
  uint state;
  Cell* cell;
  uint sym_len;

  Cell** stack;
  uint level; // "stack pointer"
} ReaderState;

ReaderState* read_char(char c, ReaderState* rs);
Cell* read_string(char* in);

#endif
