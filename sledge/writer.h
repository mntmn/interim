#include "minilisp.h"

char* lisp_write(Cell* cell, char* buffer, int bufsize);
Cell* lisp_write_to_cell(Cell* cell, Cell* buffer_cell);
char* tag_to_str(int tag);
