#include "minilisp.h"
#ifndef SLEDGE_STREAM_H
#define SLEDGE_STREAM_H

typedef struct Filesystem {
  Cell* mount_point;
  Cell* open_fn;
  Cell* close_fn;
  Cell* read_fn;
  Cell* write_fn;
  Cell* delete_fn;
  Cell* mmap_fn;
} Filesystem;

typedef struct Stream {
  int id;
  Cell* path;
  long pos;
  long size;
  int mode; // read, write…
  // we could put here pointers to event handlers

  Filesystem* fs;
} Stream;

Cell* filesystems_init();
Cell* fs_open(Cell* path);
Cell* fs_mmap(Cell* path);
Cell* fs_mount(Cell* path, Cell* handlers);
Cell* stream_read(Cell* stream);
Cell* stream_write(Cell* stream, Cell* arg);
void fs_mount_builtin(char* path, void* open_handler, void* read_handler, void* write_handler, void* delete_handler, void* mmap_handler);
Cell* get_fs_list();

#endif
