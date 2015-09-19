#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

Cell* consolefs_open() {
  system("stty raw -echo");
  return alloc_int(1);
}

Cell* consolefs_read() {
  int c = fgetc(stdin);
  if (c==13) c=10; // CR/LF
  Cell* str = alloc_string_copy(" ");
  ((char*)str->ar.addr)[0] = c;
  return str;
}

Cell* consolefs_write(Cell* a1,Cell* arg) {
  fputc(arg->ar.value, stdout);
  return arg;
}

void consolefs_cleanup() {
  system("stty sane");
}

void mount_consolekeys() {
  fs_mount_builtin("/keyboard", consolefs_open, consolefs_read, consolefs_write, 0, 0);
  atexit(consolefs_cleanup);
}
