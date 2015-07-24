static struct fs* fat_fs;

void vfs_register(struct fs *fs) {
  printf("~~ vfs_register: %s/%s block_size: %d\r\n",fs->parent->device_name,fs->fs_name,fs->block_size);
  printf("~~ read_directory: %p fopen: %p\r\n",fs->read_directory,fs->fopen);

  //char* name = "/";

  //struct dirent* dir = fs->read_directory(fs,&name);

  //printf("~~ dirent: %p name: %s\r\n",dir,dir->name);
  fat_fs = fs;
}

static Cell* _fatfs_stream;

Cell* fatfs_open(Cell* cpath) {
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[fatfs_open] error: non-string path given\r\n");
    _fatfs_stream = alloc_string_copy("404");
    return alloc_nil();
  }

  char* path = cpath->addr;
  
  if (!strncmp(path,"/sd/",4) && fat_fs) {
    char* name = NULL;
    struct dirent* dir = fat_fs->read_directory(fat_fs,&name);

    char* filename = NULL;
    if (strlen(path)>4) {
      filename = path+4;
    }
    
    printf("~~ dirent: %p name: %s\r\n",dir,dir->name);

    if (filename) {
      // look for the file
      printf("FAT looking for %s...\r\n",filename);
      while (dir) {
        if (!strcmp(filename, dir->name)) {
          // found it
          printf("FAT found file. opening...\r\n");
          fs_file* f = fat_fs->fopen(fat_fs, dir, "r");
          if (f) {
            printf("FAT trying to read file of len %d...\r\n",f->len);
            Cell* res = alloc_num_string(f->len);
            int len = fat_fs->fread(fat_fs, res->addr, f->len, f);
            printf("FAT bytes read: %d\r\n",len);
            // TODO: close?
            _fatfs_stream = res;
            return res;
          } else {
            // TODO should return error
            printf("FAT could not open file :(\r\n");
            _fatfs_stream = alloc_string_copy("<error: couldn't open file.>"); // FIXME hack
            return _fatfs_stream;
          }
        }
        dir = dir->next;
      }
      _fatfs_stream = alloc_string_copy("<error: file not found.>");
      return _fatfs_stream;
    } else {
      // directory
      
      Cell* res = alloc_num_string(4096);
      char* ptr = (char*)res->addr;
      while (dir) {
        int len = sprintf(ptr,"%s",dir->name);
        ptr[len] = '\n';
        ptr+=len+1;
        dir = dir->next;
      }
      _fatfs_stream = res; // FIXME hack
      return res;
    }
  }

  Cell* result_cell = alloc_int(0);
  return result_cell;
}

Cell* fatfs_read(Cell* stream) {
  return _fatfs_stream;
}

Cell* fatfs_mmap(Cell* stream) {
  return _fatfs_stream;
}
  
void mount_fatfs() {
  fs_mount_builtin("/sd", fatfs_open, fatfs_read, 0, 0, fatfs_mmap);
}

