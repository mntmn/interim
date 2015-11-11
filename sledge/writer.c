#include "minilisp.h"
#include "stream.h"
#include <stdio.h>

#define TMP_BUF_SIZE 512
#define INTFORMAT "%ld"

char* tag_to_str(int tag) {
  switch (tag) {
  case TAG_FREED: return "freed"; 
  case TAG_INT: return "int";
  case TAG_CONS: return "cons";
  case TAG_SYM: return "symbol";
  case TAG_LAMBDA: return "lambda";
  case TAG_BUILTIN: return "builtin";
  case TAG_BIGNUM: return "bignum";
  case TAG_STR: return "string";
  case TAG_BYTES: return "bytes";
  case TAG_VEC: return "vector";
  case TAG_ERROR: return "error";
  case TAG_ANY: return "any";
  case TAG_VOID: return "void";
  case TAG_STREAM: return "stream";
  case TAG_STRUCT: return "struct";
    //case TAG_FS: return "filesystem";
    //case TAG_MARK: return "gc_mark";
  default: return "unknown";
  }
}

char* write_(Cell* cell, char* buffer, int in_list, int bufsize) {
  //printf("writing %p (%d) to %p, size: %d\n",cell,cell->tag,buffer,bufsize);
  bufsize--;
  
  buffer[0]=0;
  if (cell == NULL) {
    snprintf(buffer, bufsize, "null");
  } else if (cell->tag == TAG_INT) {
    snprintf(buffer, bufsize, INTFORMAT, cell->ar.value);
  } else if (cell->tag == TAG_CONS) {
    if (cell->ar.addr == 0 && cell->dr.next == 0) {
      if (!in_list) {
        snprintf(buffer, bufsize, "nil");
      }
    } else {
      char* tmpl=malloc(TMP_BUF_SIZE);
      char* tmpr=malloc(TMP_BUF_SIZE);
      write_((Cell*)cell->ar.addr, tmpl, 0, TMP_BUF_SIZE);

      if (cell->dr.next && ((Cell*)cell->dr.next)->tag==TAG_CONS) {
        write_((Cell*)cell->dr.next, tmpr, 1, TMP_BUF_SIZE);
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
        write_((Cell*)cell->dr.next, tmpr, 0, TMP_BUF_SIZE);
        // improper list
        snprintf(buffer, bufsize, "(%s.%s)", tmpl, tmpr);
      }
      free(tmpl);
      free(tmpr);
    }
  } else if (cell->tag == TAG_SYM) {
    snprintf(buffer, bufsize, "%s", (char*)cell->ar.addr);
  } else if (cell->tag == TAG_STR) {
    snprintf(buffer, min(bufsize-1,cell->dr.size+3), "\"%s\"", (char*)cell->ar.addr);
  } else if (cell->tag == TAG_BIGNUM) {
    snprintf(buffer, bufsize, "%s", (char*)cell->ar.addr);
  } else if (cell->tag == TAG_LAMBDA) {
    char tmp_args[TMP_BUF_SIZE];
    char tmp_body[TMP_BUF_SIZE*2];
    Cell* args = car(cell->ar.addr);
    int ai = 0;
    tmp_args[0]=0;
    tmp_body[0]=0;
    /*char debug[256];
    write_(args,debug,0,256);
    printf("debug %s\n",debug);*/
    while (args && car(car(args))) {
      if (car(car(args))->tag == TAG_CONS) {
        Cell* arg_cell = car(car(args));
        // typed arg
        ai += snprintf(tmp_args+ai, TMP_BUF_SIZE-ai, "(%s %s) ", (char*)(car(arg_cell)->ar.addr), (char*)(car(cdr(arg_cell))->ar.addr));
      } else {
        // untyped arg
        ai += snprintf(tmp_args+ai, TMP_BUF_SIZE-ai, "%s ", (char*)(car(car(args)))->ar.addr);
      }
      args = cdr(args);
    }
    write_(cdr(cell->ar.addr), tmp_body, 0, TMP_BUF_SIZE);
    snprintf(buffer, bufsize, "(fn %s %s)", tmp_args, tmp_body);
  } else if (cell->tag == TAG_BUILTIN) {
    snprintf(buffer, bufsize, "(op "INTFORMAT")", cell->ar.value);
  } else if (cell->tag == TAG_ERROR) {
    switch (cell->ar.value) {
      case ERR_SYNTAX: snprintf(buffer, bufsize, "<e0:syntax error.>"); break;
      case ERR_MAX_EVAL_DEPTH: snprintf(buffer, bufsize, "<e1:deepest level of evaluation reached.>"); break;
      case ERR_UNKNOWN_OP: snprintf(buffer, bufsize, "<e2:unknown operation.>"); break;
      case ERR_APPLY_NIL: snprintf(buffer, bufsize, "<e3:cannot apply nil.>"); break;
      case ERR_INVALID_PARAM_TYPE: snprintf(buffer, bufsize, "<e4:invalid or no parameter given.>"); break;
      case ERR_OUT_OF_BOUNDS: snprintf(buffer, bufsize, "<e5:out of bounds.>"); break;
      default: snprintf(buffer, bufsize, "<e"INTFORMAT":unknown>", cell->ar.value); break;
    }
  } else if (cell->tag == TAG_BYTES) {
    int strsize = min(cell->dr.size*2+3, bufsize);
    int max_bytes = (strsize-3)/2;

    if (bufsize>5) {
      unsigned int i;
      buffer[0]='[';
      for (i=0; i<max_bytes; i++) {
        sprintf(buffer+1+i*2, "%02x", ((uint8_t*)cell->ar.addr)[i]);
      }
      buffer[i*2+1]=']';
      buffer[i*2+2]=0;
    }
  } else if (cell->tag == TAG_VEC || cell->tag == TAG_STRUCT || cell->tag == TAG_STRUCT_DEF) {
    Cell** vec = cell->ar.addr;
    int elements = cell->dr.size;
    int pos = 1;

    if (bufsize>12) {
      int i=0;
      buffer[0]='(';
      pos = 1;
      if (cell->tag == TAG_VEC) {
        sprintf(&buffer[1],"vec ");
        pos = 5;
      }
      else if (cell->tag == TAG_STRUCT && vec && vec[0]) {
        Cell** struct_def = (Cell**)(vec[0]->ar.addr);
        pos = 1+sprintf(&buffer[1],"%s ",(char*)struct_def[0]->ar.addr);
        i=1;
      }
      else if (cell->tag == TAG_STRUCT_DEF) {
        sprintf(&buffer[1],"struct ");
        pos = 8;
      }
      
      for (; i<elements && pos<bufsize-1; i++) {
        //printf("i: %d pos: %d vec[i]: %p\n",i,pos,vec[i]);
        write_(vec[i], buffer+pos, 0, bufsize-pos);
        //printf("-> %s\n",buffer);
        pos += strlen(buffer+pos);
        buffer[pos]=' ';
        pos++;
      }
      buffer[pos]=')';
      buffer[pos+1]=0;
    }
  } else if (cell->tag == TAG_STREAM) {
    Stream* s = (Stream*)cell->ar.addr;
    if (s) {
      snprintf(buffer, bufsize, "<stream:%d:%s:%s>", s->id, (char*)s->path->ar.addr, (char*)s->fs->mount_point->ar.addr);
    } else {
      snprintf(buffer, bufsize, "<stream:null>");
    }
  } else {
    snprintf(buffer, bufsize, "<tag:%ld>", cell->tag);
  }
  return buffer;
}

char* lisp_write(Cell* cell, char* buffer, int bufsize) {
  return write_(cell, buffer, 0, bufsize);
}

Cell* lisp_write_to_cell(Cell* cell, Cell* buffer_cell) {
  if (buffer_cell->tag == TAG_STR || buffer_cell->tag == TAG_BYTES) {
    lisp_write(cell, buffer_cell->ar.addr, buffer_cell->dr.size);
  }
  return buffer_cell;
}
