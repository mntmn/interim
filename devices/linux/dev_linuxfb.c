#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define WIDTH 1920
#define HEIGHT 1080
#define BPP 2
#define DEPTH 16

Cell* linux_fbfs_open() {
  return alloc_int(1);
}

Cell* linux_fbfs_read() {
  return alloc_int(0);
}

Cell* linux_fbfs_write(Cell* arg) {
  return NULL;
}

Cell* linux_fbfs_mmap(Cell* arg) {
  long sz = WIDTH*HEIGHT*BPP;

  int fd = open("/dev/fb0", O_RDWR);
  printf("[linux_fbfs_mmap] open fd: %d\n",fd);

  if (fd>-1) {
    Cell* buffer_cell = alloc_int(0);
    buffer_cell->ar.addr = mmap(NULL, sz, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    buffer_cell->dr.size = sz;
    buffer_cell->tag = TAG_BYTES;
    printf("[linux_fbfs_mmap] buffer_cell->addr: %p\n",buffer_cell->ar.addr);
  
    return buffer_cell;
  } else {
    return alloc_nil();
  }
}

void mount_linux_fbfs() {
  fs_mount_builtin("/framebuffer", linux_fbfs_open, linux_fbfs_read, linux_fbfs_write, 0, linux_fbfs_mmap);
}

