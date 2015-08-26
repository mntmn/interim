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
  if (have_eth) {
    USPiReceiveFrame(packet, &frame_len);

    if (frame_len) {
      printf("[ethfs] frame received! len: %d\r\n",frame_len);   
      memdump(packet,frame_len,0);
    }
  }
  return frame_len;
}

void ethernet_tx(uint8_t* packet, int len) {
  USPiSendFrame(packet, len);
  printf("[ethfs] frame sent (%d)\r\n",len);
}


#include "devices/mini-ip.c"


Cell* ethfs_open(Cell* path) {
  return alloc_int(1);
}

Cell* ethfs_read() {
  /*int len = ethernet_rx(eth_rx_buffer);
  Cell* packet = alloc_num_bytes(len);
  memcpy(packet->addr, eth_rx_buffer, len);*/

  return eth_task();
  
  //return packet;
}

Cell* ethfs_write(Cell* packet) {
  ethernet_tx(packet->addr,packet->size);
  
  return alloc_int(1);
}

void mount_ethernet() {
  eth_rx_buffer=malloc(64*1024);
  init_mini_ip();
  fs_mount_builtin("/net", ethfs_open, ethfs_read, ethfs_write, 0, 0);
  printf("[ethfs] mounted\r\n");
}
