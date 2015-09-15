#include "fat/ff.h"

static FATFS fat_fs;

void fatfs_debug() {
}

Cell* fatfs_open(Cell* cpath) {
  printf("[fatfs_open] called\r\n");
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[fatfs_open] error: non-string path given\r\n");
    return alloc_nil();
  }

  char* path = cpath->ar.addr;
  if (!strncmp(path,"/sd/",4)) {
    char* filename = NULL;
    if (strlen(path)>4) {
      filename = path+4;
    }
    
    if (filename) {
      // look for the file
      printf("FAT looking for %s...\r\n",filename);

      FILINFO nfo;
      FRESULT rc = f_stat(filename, &nfo);
      if (rc) {
        printf("Failed to stat file %s: %u\r\n", filename, rc);
        return alloc_int(0);
      }
      
      FIL fp;
      rc = f_open(&fp, filename, FA_READ);
      if (rc) {
        printf("Failed to open file %s: %u\r\n", filename, rc);
        return alloc_int(0);
      }

      printf("filesize: %d\r\n",nfo.fsize);
      
      return alloc_int(nfo.fsize);
    } else {
      // directory

      return alloc_int(1);
    }
  }

  return alloc_int(0);
}

Cell* fatfs_read(Cell* stream) {
  Stream* s = (Stream*)stream->ar.addr;
  char* path = ((char*)s->path->ar.addr);

  if (!strncmp(path,"/sd/",4)) {
    char* filename = NULL;
    if (strlen(path)>4) {
      filename = path+4;
    }
    
    if (filename) {
      printf("FAT reading %s...\r\n",filename);

      FILINFO nfo;
      FRESULT rc = f_stat(filename, &nfo);
      if (rc) {
        printf("Failed to stat file %s: %u\r\n", filename, rc);
        return alloc_num_bytes(0);
      }
      
      FIL fp;
      rc = f_open(&fp, filename, FA_READ);
      if (rc) {
        printf("Failed to open file %s: %u\r\n", filename, rc);
        return alloc_num_bytes(0);
      }

      printf("filesize: %d\r\n",nfo.fsize);

      uint32_t buf_sz = nfo.fsize;
      Cell* result = alloc_num_bytes(buf_sz+1);
      UINT bytes_read;

      rc = f_read(&fp, result->ar.addr, buf_sz, &bytes_read);
      if (rc) printf("Read failed: %u\r\n", rc);
      
      rc = f_close(&fp);
      
      return result;
    } else {
      // directory
      
      Cell* res = alloc_num_string(4096);
      char* ptr = (char*)res->ar.addr;

      FRESULT rc;
      DIR dj;			/* Pointer to the open directory object */
      FILINFO fno;

      rc = f_opendir(&dj, "/");

      printf("opendir: %d\r\n",rc);

      if (!rc) do {
        rc = f_readdir(&dj, &fno);
        printf("file: %s\r\n",fno.fname);
        int len = sprintf(ptr,"%s",fno.fname);
        ptr[len] = '\n';
        ptr+=len+1;
      } while (!rc && dj.sect>0);
      
      return res;
    }
  }

}

Cell* fatfs_write(Cell* stream, Cell* packet) {
  FIL fp;
  Stream* s = (Stream*)stream->ar.addr;
  char* path = ((char*)s->path->ar.addr)+3;
  printf("writing to stream with path %s\r\n",path);
  FRESULT rc = f_open(&fp, path, FA_WRITE|FA_CREATE_ALWAYS);
  UINT bytes_written = 0;
  if (!rc) {
    printf("opened for writing!\r\n");
    rc = f_write(&fp, packet->ar.addr, packet->dr.size, &bytes_written);
    printf("rc: %d bytes_written: %d\r\n",rc,bytes_written);
    rc = f_close(&fp);
  } else {
    printf("fatfs cannot write: rc %d\r\n",rc);
  }
  return alloc_int(bytes_written);
}
  
void mount_fatfs() {
  f_mount(0, &fat_fs);
  fs_mount_builtin("/sd", fatfs_open, fatfs_read, fatfs_write, 0, 0);
}

