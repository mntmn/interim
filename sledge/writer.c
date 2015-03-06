#include "minilisp.h"
#include <stdio.h>

#define TMP_BUF_SIZE 4096

char* write_(Cell* cell, char* buffer, int in_list, int bufsize) {
  //printf("writing %p (%d) to %p, size: %d\n",cell,cell->tag,buffer,bufsize);

  buffer[0]=0;
  if (cell == 0) {
    sprintf(buffer, "null");
  } else if (cell->tag == TAG_INT) {
    snprintf(buffer, bufsize, "%lld", cell->value);
  } else if (cell->tag == TAG_CONS) {
    if (cell->addr == 0 && cell->next == 0) {
      if (!in_list) {
        sprintf(buffer, "nil");
      }
    } else {
      char tmpl[TMP_BUF_SIZE];
      char tmpr[TMP_BUF_SIZE];
      write_((Cell*)cell->addr, tmpl, 0, TMP_BUF_SIZE);

      if (cell->next && ((Cell*)cell->next)->tag==TAG_CONS) {
        write_((Cell*)cell->next, tmpr, 1, TMP_BUF_SIZE);
        if (in_list) {
          if (tmpr[0]) {
            snprintf(buffer, bufsize, "%s %s", tmpl, tmpr);
          } else {
            snprintf(buffer, bufsize, "%s", tmpl); // skip nil at end of list
          }
        } else {
          // we're head of a list
          snprintf(buffer, bufsize, "(%s %s)", tmpl, tmpr);
        }
      } else {
        write_((Cell*)cell->next, tmpr, 0, TMP_BUF_SIZE);
        // improper list
        snprintf(buffer, bufsize, "(%s.%s)", tmpl, tmpr);
      }
    }
  } else if (cell->tag == TAG_SYM) {
    snprintf(buffer, bufsize, "%s", (char*)cell->addr);
  } else if (cell->tag == TAG_STR) {
    snprintf(buffer, min(bufsize,cell->size), "\"%s\"", (char*)cell->addr);
  } else if (cell->tag == TAG_BIGNUM) {
    snprintf(buffer, bufsize, "%s", (char*)cell->addr);
  } else if (cell->tag == TAG_LAMBDA) {
    char tmpr[TMP_BUF_SIZE];
    snprintf(buffer, bufsize, "(proc %p)", cell->next);
  } else if (cell->tag == TAG_BUILTIN) {
    snprintf(buffer, bufsize, "(op %lld)", cell->value);
  } else if (cell->tag == TAG_ERROR) {
    switch (cell->value) {
      case ERR_SYNTAX: sprintf(buffer, "<e0:syntax error.>"); break;
      case ERR_MAX_EVAL_DEPTH: sprintf(buffer, "<e1:deepest level of evaluation reached.>"); break;
      case ERR_UNKNOWN_OP: sprintf(buffer, "<e2:unknown operation.>"); break;
      case ERR_APPLY_NIL: sprintf(buffer, "<e3:cannot apply nil.>"); break;
      case ERR_INVALID_PARAM_TYPE: sprintf(buffer, "<e4:invalid or no parameter given.>"); break;
      case ERR_OUT_OF_BOUNDS: sprintf(buffer, "<e5:out of bounds.>"); break;
      default: sprintf(buffer, "<e%lld:unknown>", cell->value); break;
    }
  } else if (cell->tag == TAG_BYTES) {
    char* hex_buffer = malloc(cell->size*2+1);
    unsigned int i;
    for (i=0; i<cell->size; i++) {
      // FIXME: buffer overflow?
      snprintf(hex_buffer+i*2, bufsize-i*2, "%02x",((byte*)cell->addr)[i]);
    }
    snprintf(buffer, bufsize, "\[%s]", hex_buffer);
    free(hex_buffer);
  } else {
    snprintf(buffer, bufsize, "<tag:%i>", cell->tag);
  }
  return buffer;
}

char* lisp_write(Cell* cell, char* buffer, int bufsize) {
  return write_(cell, buffer, 0, bufsize);
}
