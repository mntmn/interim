#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

Cell* _file_cell;

Cell* posixfs_open(Cell* cpath) {
  _file_cell = alloc_nil();

  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[posixfs] open error: non-string path given\r\n");
    return _file_cell;
  }

  char* path = cpath->addr;
  
  if (!strncmp(path,"/sd/",4)) {
    char* name = NULL;
    char* filename = NULL;

    if (strlen(path)>4) {
      filename = path+4;
    }
    
    if (filename) {
      FILE* f = fopen(filename, "rb");
      if (f) {
        fseek(f, 0L, SEEK_END);
        int len = ftell(f);
        fseek(f, 0L, SEEK_SET);
        
        printf("[posixfs] trying to read file of len %d…\r\n",len);
        Cell* res = alloc_num_string(len);
        int read_len = fread(res->addr, len, 1, f);
        // TODO: close?
        _file_cell = res;
        return res;
      } else {
        // TODO should return error
        printf("FAT could not open file :(\r\n");
        _file_cell = alloc_string_copy("<error: couldn't open file.>"); // FIXME hack
        return _file_cell;
      }
      _file_cell = alloc_string_copy("<error: file not found.>");
      return _file_cell;
    } else {
      // TODO dir
    }
  }

  return _file_cell;
}

Cell* posixfs_read(Cell* stream) {
  return _file_cell;
}

Cell* posixfs_write(Cell* arg) {
  return NULL;
}

Cell* posixfs_mmap(Cell* arg) {
  return alloc_nil();
}

void mount_posixfs() {
  fs_mount_builtin("/sd", posixfs_open, posixfs_read, posixfs_write, 0, posixfs_mmap);
}

