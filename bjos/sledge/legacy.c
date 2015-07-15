
int machine_video_set_pixel(uint32_t x, uint32_t y, COLOR_TYPE color) {
  //sdl_setpixel(x,y,color);
  return 1;
}

int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, COLOR_TYPE color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  
  for (; y1<y2; y1++) {
    uint32_t x1=x;
    for (; x1<x2; x1++) {
      //sdl_setpixel(x1,y1,color);
    }
  }
}

inline int machine_video_flip() {
  //sdl_mainloop();
  machine_video_rect(0,0,1920,1080,0xff);
  return 1;
}

int machine_get_key(int modifiers) {
  //if (modifiers) return sdl_get_modifiers();
  //int k = sdl_get_key();
  int k = 0;
  //if (k) printf("k: %d\n",k);
  if (k==43) k=134;
  if (k==80) k=130;
  if (k==79) k=131;
  if (k==82) k=132;
  if (k==81) k=133;
  return k;
}

Cell* machine_save_file(Cell* cell, char* path) {
  printf("about to save: %s\n",path);
  if (cell->tag == TAG_STR || cell->tag == TAG_BYTES) {
    FILE* f = fopen(path,"wb");
    if (!f) return alloc_error(ERR_FORBIDDEN);
    
    fwrite(cell->addr, 1, cell->size, f);
    fclose(f);
    return alloc_int(1);
  } else {
    printf("error: cannot save tag %d\n",cell->tag);
  }
  return alloc_int(0);
}

static char sysfs_tmp[1024];

Cell* machine_load_file(char* path) {
  // sysfs
  if (!strcmp(path,"/sys/mem")) {
    MemStats* mst = alloc_stats();
    sprintf(sysfs_tmp, "(%d %d %d %d)", mst->byte_heap_used, mst->byte_heap_max, mst->cells_used, mst->cells_max);
    return read_string(sysfs_tmp);
  }
  
  char buf[512];
  sprintf(buf,"../rootfs/%s",path);
  path = buf;
  
  printf("about to load: %s\n",path);
  struct stat st;
  if (stat(path, &st)) return alloc_error(ERR_NOT_FOUND);

  if (st.st_size < 1) return alloc_bytes(); // zero-byte file

  FILE* f = fopen(path,"rb");
  if (!f) return alloc_error(ERR_FORBIDDEN);

  Cell* result_cell = alloc_num_bytes(st.st_size);
  fread(result_cell->addr, 1, st.st_size, f);
  fclose(f);

  return result_cell;
}


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// TODO: create socket handle and return it

static int tcp_outbound_sockfd = 0;

Cell* my_tcp_connected_callback;
Cell* my_tcp_data_callback;
Cell* network_cell;

Cell* machine_poll_udp() {
  if (my_tcp_data_callback) {
    int len = read(tcp_outbound_sockfd,network_cell->addr,1024*64);
    if (len>0) {
      printf("-- received tcp packet of len: %d\n",len);
      ((uint8_t*)network_cell->addr)[len] = 0;
      network_cell->size = len;
      funcptr fn = (funcptr)my_tcp_data_callback->next;
      fn();
      
      return NULL;
    }
  }
  return NULL;
}

Cell* machine_send_udp(Cell* data_cell) {
  return data_cell;
}


Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell) {
  my_tcp_connected_callback = connected_fn_cell;
  my_tcp_data_callback = data_fn_cell;

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  
  bcopy((char *)host_cell->addr,
        (char *)&serv_addr.sin_addr.s_addr,
        4); // ipv4 only
  
  tcp_outbound_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  struct timeval tv;
  tv.tv_sec = 1;  /* 1 Sec Timeout */
  tv.tv_usec = 0;  // Not init'ing this can cause strange errors

  setsockopt(tcp_outbound_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
  
  serv_addr.sin_port = htons(port_cell->value);

  printf("[machine_connect_tcp] trying to connect to %x:%d\r\n",serv_addr.sin_addr.s_addr,port_cell->value);
  if (connect(tcp_outbound_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    printf("[machine_connect_tcp] couldn't connect.\r\n");
  } else {
    // call connected callback
    printf("[machine_connect_tcp] connected. calling %p…\r\n",my_tcp_connected_callback->next);
    funcptr fn = (funcptr)my_tcp_connected_callback->next;
    fn();
  }
  
  return port_cell;
}

Cell* machine_send_tcp(Cell* data_cell) {
  write(tcp_outbound_sockfd,data_cell->addr,data_cell->size);
  return data_cell;
}

Cell* machine_bind_tcp(Cell* port_cell, Cell* fn_cell) {
  return fn_cell;
}
