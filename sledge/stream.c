#include <stdio.h>
#include "stream.h"
#include "alloc.h"
#include "writer.h"

static Cell* fs_list;
static int num_fs;
static int stream_id;

typedef Cell* (*funcptr2)(Cell* a1, Cell* a2);

// TODO: include fs_list in gc mark

Cell* get_fs_list() {
  return fs_list;
}

Cell* fs_open(Cell* path) {
  Cell* fsl = fs_list;
  Cell* fs_cell;
  
  if (!path || path->tag!=TAG_STR) {
    printf("[open] error: string required.");
    return alloc_nil();
  }
  
  while ((fs_cell = car(fsl))) {
    Filesystem* fs = (Filesystem*)fs_cell->dr.next;
    //printf("compare %s : %s : %p\n",fs->mount_point->ar.addr, path->ar.addr, strstr(fs->mount_point->ar.addr, path->ar.addr));
    if (path->ar.addr && strstr(path->ar.addr, fs->mount_point->ar.addr) == path->ar.addr) {
      Stream* s = malloc(sizeof(Stream));
      Cell* stream_cell;
      
      printf("[open] via %s: %s\r\n", (char*)fs->mount_point->ar.addr, (char*)path->ar.addr);
      s->fs = fs;
      s->path = path;
      s->id = stream_id;

      // TODO: integrate in GC

      stream_cell = alloc_int(stream_id);
      stream_cell->tag = TAG_STREAM;
      stream_cell->ar.addr = s;
      stream_id++;

      // open the filesystem
      if (s->fs->open_fn && s->fs->open_fn->dr.next) {
        Cell* open_fn = s->fs->open_fn;
        ((funcptr2)open_fn->dr.next)(path, NULL);
      }

      return stream_cell;
    }
    fsl = cdr(fsl);
  }
  return alloc_nil();
}

Cell* fs_mmap(Cell* path) {
  Cell* fsl = fs_list;
  Cell* fs_cell;
  
  if (!path || path->tag!=TAG_STR) {
    printf("[mmap] error: string required.");
    return alloc_nil();
  }
  
  while ((fs_cell = car(fsl))) {
    Filesystem* fs = (Filesystem*)fs_cell->dr.next;
    if (path->ar.addr && strstr(path->ar.addr, fs->mount_point->ar.addr) == path->ar.addr) {
      printf("[mmap] found matching fs: %s for path: %s\r\n", (char*)fs->mount_point->ar.addr, (char*)path->ar.addr);

      if (fs->mmap_fn && fs->mmap_fn->dr.next) {
        Cell* mmap_fn = fs->mmap_fn;
        return (Cell*)((funcptr2)mmap_fn->dr.next)(path, NULL);
      } else {
        printf("[mmap] error: fs has no mmap implementation.");
        return alloc_nil();
      }
    }
    fsl = cdr(fsl);
  }
  return alloc_nil();
}

Cell* fs_mount(Cell* path, Cell* handlers) {
  Filesystem* fs;
  Cell* fs_cell;
  
  if (!path || path->tag!=TAG_STR) {
    printf("[mount] error: string required.");
    return alloc_nil();
  }

  fs = malloc(sizeof(Filesystem));
  fs->open_fn = car(handlers);
  handlers = cdr(handlers);
  fs->read_fn = car(handlers);
  handlers = cdr(handlers);
  fs->write_fn = car(handlers);
  handlers = cdr(handlers);
  fs->delete_fn = car(handlers);
  handlers = cdr(handlers);
  fs->mmap_fn = car(handlers);
  handlers = cdr(handlers);
  fs->mount_point = path;
  fs->close_fn = NULL;

  fs_cell = alloc_int(num_fs++);
  fs_cell->dr.next = fs;
  fs_cell->tag = TAG_FS;
  
  printf("[fs] mounted: %s\r\n",(char*)path->ar.addr);
  fs_list = alloc_cons(fs_cell, fs_list);

  return fs_cell;
}

Cell* wrap_in_lambda(void* cfunc) {
  Cell* lbd = alloc_lambda(alloc_nil());
  lbd->dr.next = cfunc;
  return lbd;
}

// TODO: pass stream (context) to handlers

Cell* stream_read(Cell* stream) {
  Stream* s;
  Cell* read_fn;
  
  if (!stream || stream->tag!=TAG_STREAM) {
    printf("[fs] error: non-stream passed to recv\r\n");
    return alloc_nil();
  }
  s = (Stream*)stream->ar.addr;
  read_fn = s->fs->read_fn;
  //char debug_buf[256];
  //lisp_write(read_fn, debug_buf, 256);
  //printf("[stream_read] fn: %s ptr: %p\n",debug_buf,read_fn->dr.next);
  return (Cell*)((funcptr2)read_fn->dr.next)(stream,NULL);
}

Cell* stream_write(Cell* stream, Cell* arg) {
  Stream* s;
  Cell* write_fn;
  
  if (!stream || stream->tag!=TAG_STREAM) {
    printf("[fs] error: non-stream passed to send\r\n");
    return alloc_nil();
  }
  s = (Stream*)stream->ar.addr;
  write_fn = s->fs->write_fn;
  //char debug_buf[256];
  //lisp_write(arg, debug_buf, 256);
  //printf("[stream_write] fn: %s ptr: %p\n",debug_buf,write_fn->dr.next);
  //printf("[stream_write] arg: %s\n",debug_buf);
  return (Cell*)((funcptr2)write_fn->dr.next)(stream,arg);
}

void fs_mount_builtin(char* path, void* open_handler, void* read_handler, void* write_handler, void* delete_handler, void* mmap_handler) {
  Cell* items[] = {
    wrap_in_lambda(open_handler),
    wrap_in_lambda(read_handler),
    wrap_in_lambda(write_handler),
    wrap_in_lambda(delete_handler),
    wrap_in_lambda(mmap_handler),
  };
  Cell* handlers = alloc_list(items,5);
  fs_mount(alloc_string_copy(path), handlers);
}

Cell* filesystems_init() {
  fs_list = alloc_nil();
  return fs_list;
}
