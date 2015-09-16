#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <unistd.h>
#include <fcntl.h>

static uint8_t* eth_rx_buffer;

int ethernet_rx(uint8_t* packet) {
  int frame_len = 0;
  USPiReceiveFrame(packet, &frame_len);

  if (frame_len) {
    printf("[ethfs] frame received! len: %d\r\n",frame_len);   
    memdump(packet,frame_len,0);
  }
  return frame_len;
}

void ethernet_tx(uint8_t* packet, int len) {
  USPiSendFrame(packet, len);
  printf("[ethfs] frame sent (%d)\r\n",len);
}


#include "devices/mini-ip.c"


Cell* ethfs_open(Cell* path_cell) {
  // TODO typecheck
  char* path = path_cell->ar.addr;
  
  if (!strncmp((char*)(path+4),"/tcp/",5)) {
    printf("[tcp] %s",path+4);
    char ip[4] = {0,0,0,0};
    int len = 0;
    int j = 0;
    int dgt = 0;
    int i = 0;
    for (i=9; i<strlen(path) && j<4; i++) {
      if (path[i]=='/') break;
      if (path[i]=='.') {
        j++;
      } else {
        ip[j]=ip[j]*10+(path[i]-'0');
      }
      len++;
    }
    printf("[tcp] ip from path: %d.%d.%d.%d\r\n",ip[0],ip[1],ip[2],ip[3]);
    int port = 0;
    i++;
    for (; i<strlen(path); i++) {
      if (path[i]=='/') break;
      port=port*10+(path[i]-'0');
    }
    printf("[tcp] port from path: %d\r\n",port);
    
    connect_tcp(ip,port);
  }
  
  return alloc_int(1);
}

Cell* ethfs_read() {
  /*int len = ethernet_rx(eth_rx_buffer);
  Cell* packet = alloc_num_bytes(len);
  memcpy(packet->ar.addr, eth_rx_buffer, len);*/

  return eth_task();
  
  //return packet;
}

// FIXME differntiate connections, protocols
Cell* ethfs_write(Cell* stream, Cell* packet) {
  //ethernet_tx(packet->ar.addr,packet->size);
  return send_tcp(packet);
}

void mount_ethernet() {
  eth_rx_buffer=malloc(64*1024);
  init_mini_ip();
  fs_mount_builtin("/net", ethfs_open, ethfs_read, ethfs_write, 0, 0);
  printf("[ethfs] mounted\r\n");
}
