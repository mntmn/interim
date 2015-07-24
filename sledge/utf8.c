
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

int utf8_strlen(char *s, int len) {
  int i = 0, j = 0;
  while (s[i] && i<len) {
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
      if (idx == j) {
        return rune;
      }
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

int utf8_str_to_runestr(char* ustr, int len_bytes, uint32_t* dest) {
  uint32_t desti = 0;
  uint32_t rune = 0;
  int state = 0;
  for (int i=0; i<len_bytes; i++) {
    uint8_t b1 = ustr[i];

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
      dest[desti++] = rune;
    }
  }
  return desti;
}

/*
jit_word_t utf8_strlen_cell(Cell* cell) {
  if (!cell || (cell->tag!=TAG_STR && cell->tag!=TAG_BYTES) || !cell->addr) return 0;
  return utf8_strlen(cell->addr, cell->size);
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
  
  unsigned int result = utf8_rune_at(cell->addr, c_idx->value);

  return result;
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
  int existing_len = utf8_rune_len(s[i]);

  int rune_len = 0;
  char tmp[10];
  rune_to_utf8(rune, tmp, &rune_len);
  
  if ((i+rune_len)>=cell->size) return 0;

  //printf("-- existing rune length at %d: %d new rune length: %d\n",idx,j,rune_len);
  
  if (existing_len>rune_len) {
    // new rune is smaller
    int movelen = cell->size - (i+existing_len);
    if (movelen<rune_len) {
      //printf("-- utf8_put_rune_at error: rune %d doesn't fit into string at %d\n",rune,idx);
      return 0;
    }
    printf("move a: %d -> %d len %d / size %d\r\n",i+existing_len,i+rune_len,movelen,cell->size);
    memmove(cell->addr+i+rune_len, cell->addr+i+existing_len, movelen);
  } else if (j<rune_len) {
    // new rune is bigger
    int movelen = cell->size - (i+rune_len);
    if (movelen<rune_len) {
      //printf("-- utf8_put_rune_at error: rune %d doesn't fit into string at %d\n",rune,idx);
      return 0;
    }
    printf("move b: %d -> %d len %d / size %d\r\n",i+existing_len,i+rune_len,movelen,cell->size);
    memmove(cell->addr+i+rune_len, cell->addr+i+existing_len, movelen);
  }

  // write the new rune
  for (int m=0; m<rune_len; m++) {
    ((uint8_t*)cell->addr)[i+m] = tmp[m];
  }

  return i;
}
*/
