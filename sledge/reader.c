#include "reader.h"
#include "alloc.h"
#include <string.h>

Cell* reader_next_list_cell(Cell* cell, ReaderState* rs) {
  cell->dr.next = alloc_nil();
  cell = cell->dr.next;
  rs->state = PST_ATOM;
  return cell;
}

Cell* reader_end_list(Cell* cell, ReaderState* rs) {
  if (rs->level<1) {
    rs->state = PST_ERR_UNEXP_CLOSING_BRACE;
    return cell;
  }
  rs->level--;
  rs->stack--;
  if (cell->ar.addr) cell->dr.next = alloc_nil();
  cell = *rs->stack;
  
  cell = reader_next_list_cell(cell, rs);
  rs->state = PST_ATOM;
  return cell;
}

ReaderState* read_char(char c, ReaderState* rs) {
  Cell* cell = rs->cell;
  Cell* new_cell;

  if (!cell) {
    // make a root
    cell = alloc_nil();
    cell->dr.next = alloc_nil();
    *rs->stack = cell;
  }

  if (rs->state == PST_ATOM) {
    if (c==' ' || c==13 || c==10) {
      // skip whitespace
    } else if (c==';') {
      // comment
      rs->state = PST_COMMENT;
    } else if (c>='0' && c<='9') {
      rs->state = PST_NUM;
      rs->valuestate = VST_DEFAULT;
      new_cell = alloc_int(0);
      new_cell->ar.value = c-'0';
      cell->ar.addr = new_cell;

    } else if (c=='(') {
      // start list
      new_cell = alloc_nil();
      cell->ar.addr = new_cell;
      *rs->stack = cell;

      cell = new_cell;
      rs->stack++;
      rs->level++;
      rs->state = PST_ATOM;
    } else if (c==')') {
      // end of list
      cell = reader_end_list(cell, rs);
    } else if (c=='[') { 
      // bytes (hex notation)
      rs->state = PST_BYTES;
      rs->sym_len = 0;
      new_cell = alloc_bytes();
      cell->ar.addr = new_cell;
    } else if (c=='"') {
      // string
      rs->state = PST_STR;
      rs->sym_len = 0;
      new_cell = alloc_string();
      cell->ar.addr = new_cell;
    } else {
      // symbol
      rs->state = PST_SYM;
      rs->sym_len = 1;
      new_cell = alloc_num_bytes(SYM_INIT_BUFFER_SIZE);
      new_cell->tag = TAG_SYM;
      memset(new_cell->ar.addr, 0, SYM_INIT_BUFFER_SIZE);
      ((char*)new_cell->ar.addr)[0] = c;
      new_cell->dr.size = SYM_INIT_BUFFER_SIZE; // buffer space
      cell->ar.addr = new_cell;
    }

  } else if (rs->state == PST_COMMENT) {
    if (c=='\n' || c==0) {
      rs->state = PST_ATOM;
    }
  } else if (rs->state == PST_NUM || rs->state == PST_NUM_NEG) {
    if ((c>='0' && c<='9') || ((rs->valuestate == VST_HEX && c>='a' && c<='f'))) {
      // build number
      Cell* vcell = (Cell*)cell->ar.addr;
      int mul = 10, d = 0;
      if (rs->valuestate == VST_HEX) mul = 16;
      if (c>='a') {
        d = 10+(c-'a');
      } else {
        d = c-'0';
      }
      
      if (rs->state == PST_NUM_NEG) {
        vcell->ar.value = vcell->ar.value*mul - d;
      } else {
        vcell->ar.value = vcell->ar.value*mul + d;
      }
    } else if (c==' ' || c==13 || c==10) {
      cell = reader_next_list_cell(cell, rs);
    } else if (c==')') {
      cell = reader_end_list(cell, rs);
    } else if (c=='x') {
      rs->valuestate = VST_HEX;
    } else {
      rs->state = PST_ERR_UNEXP_JUNK_IN_NUMBER;
    }
  } else if (rs->state == PST_SYM || rs->state == PST_STR) {

    int append = 0;

    if (rs->state == PST_STR) {
      if (c=='"') {
        // string is over
        Cell* vcell = (Cell*)cell->ar.addr;
        vcell->dr.size = (rs->sym_len);
        cell = reader_next_list_cell(cell, rs);
      } else {
        append = 1;
      }
    }
    else {
      if (c==')') {
        cell = reader_end_list(cell, rs);
      } else if (c==' ' || c==13 || c==10) {
        cell = reader_next_list_cell(cell, rs);
      } else if (rs->state == PST_SYM && (c>='0' && c<='9')) {
        Cell* vcell = (Cell*)cell->ar.addr;
        // detect negative number
        if (((char*)vcell->ar.addr)[0] == '-') {
          // we're actually not a symbol, correct the cell.
          rs->state = PST_NUM_NEG;
          vcell->tag = TAG_INT;
          vcell->ar.value = -(c-'0');
        } else {
          append = 1;
        }
      } else {
        append = 1;
      }
    }

    if (append) {
      // build symbol/string
      Cell* vcell = (Cell*)cell->ar.addr;
      int idx = rs->sym_len;
      rs->sym_len++;
      if (rs->sym_len>=vcell->dr.size-1) {
        // grow buffer
        vcell->ar.addr = cell_realloc(vcell->ar.addr, vcell->dr.size, 2*vcell->dr.size);
        memset((char*)vcell->ar.addr+vcell->dr.size, 0, vcell->dr.size);
        vcell->dr.size = 2*vcell->dr.size;
      }
      ((char*)vcell->ar.addr)[idx] = c;
    }

  } else if (rs->state == PST_BYTES) {
    if (c==']') {
      Cell* vcell = (Cell*)cell->ar.addr;
      vcell->dr.size = (rs->sym_len)/2;
      cell = reader_next_list_cell(cell, rs);
    } else if ((c>='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')) {
      int n = c, idx;
      Cell* vcell;
      
      if (n>='a') n-=('a'-'9'-1); // hex 'a' to 10 offset
      if (n>='A') n-=('A'-'9'-1); // hex 'a' to 10 offset
      n-='0'; // char to value

      vcell = (Cell*)cell->ar.addr;
      idx = rs->sym_len;
      rs->sym_len++;
      if (rs->sym_len>=(vcell->dr.size/2)-1) {
        // grow buffer
        vcell->ar.addr = cell_realloc(vcell->ar.addr, vcell->dr.size, 2*vcell->dr.size); // TODO: check the math
        memset((char*)vcell->ar.addr+vcell->dr.size, 0, vcell->dr.size);
        vcell->dr.size = 2*vcell->dr.size;
      }
      if (idx%2==0) { // even digit
        ((uint8_t*)vcell->ar.addr)[idx/2] = n<<4; // high nybble
      } else { // odd digit
        ((uint8_t*)vcell->ar.addr)[idx/2] |= n;
      }
      
    } else if (c==' ' || c==13 || c==10) {
      // skip
    } else {
      rs->state = PST_ERR_UNEXP_JUNK_IN_BYTES;
    }
  }
  rs->cell = cell;
  return rs;
}

Cell* read_string(char* in) {
  ReaderState rs;
  int i, len;
  Cell stack_root[32];
  Cell* root;
  Cell* ret_cell;

  rs.state = PST_ATOM;
  rs.cell = 0;
  rs.level = 0;
  rs.stack = (void*)&stack_root;

  i=0;
  len = strlen(in);
  for (i=0; i<len; i++) {
    read_char(in[i], &rs);
    if (rs.state>=10) {
      //print("<read error %d at %d.>\n",rs.state,i);
      return alloc_error(ERR_SYNTAX);
    }
    //printf("rs %c: %d\n", in[i], rs.state);
  }
  if (rs.level!=0) {
    //print("<missing %d closing parens.>\r\n",rs.level);
    return alloc_error(ERR_SYNTAX);
  }
  if (rs.state!=PST_ATOM) {
    //printf("<read error: unexpected end of input.>\n");
    //return alloc_error(ERR_SYNTAX);
  }

  root = *rs.stack;
  
  if (root) {
    ret_cell = car(root);
    //if (root->dr.next) free(root->dr.next);
    //free(root);
    return ret_cell;
  }
  return alloc_error(ERR_SYNTAX);
}

Cell* read_string_cell(Cell* in) {
  char* str;
  
  if (!in) return alloc_nil();
  if (!in->dr.size) return alloc_nil();
  str = (char*)in->ar.addr;
  str[in->dr.size]=0;
  //printf("read[%s]\r\n",str);
  return read_string(str);
}
