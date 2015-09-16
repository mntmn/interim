#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <unistd.h>
#include <fcntl.h>

Cell* uartkeys_open(Cell* cpath) {
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[uartkeys] open error: non-string path given\r\n");
    return alloc_nil();
  }

  return alloc_int(1);
}

Cell* uartkeys_read(Cell* stream) {
  int k = 0;
  if (mmio_read(UART0_FR) & (1 << 4)) {
    k = 0;
  } else {
    k = mmio_read(UART0_DR);
  }
  
  if (k==27) {
    k = uart_getc();
  }
  if (k==91) {
    k = uart_getc();
    if (k==27) {
      // fast repetition
      k = uart_getc();
      k = uart_getc();
    }
    if (k==68) k=19;
    if (k==67) k=20;
    if (k==65) k=17;
    if (k==66) k=18;
    //printf("~~ inkey unknown sequence: 91,%d\r\n",k);
  }
  
  if (k==13) k=10;
  
  /*char c = 0;
  if (ki>0) {
    ki--;
    c = uart_key_in[0];
    // FIFO
    for (int i=0; i<KBUFSZ-1; i++) {
      uart_key_in[i]=uart_key_in[i+1];
    }
  }*/
  
  Cell* res = alloc_string_copy(" ");
  ((uint8_t*)res->ar.addr)[0] = k;
  
  return res;
}

Cell* uartkeys_write(Cell* arg) {
  // could be used to control LEDs
  return NULL;
}

Cell* uartkeys_mmap(Cell* arg) {
  return alloc_nil();
}

void mount_uartkeys() {
  fs_mount_builtin("/keyboard", uartkeys_open, uartkeys_read, uartkeys_write, 0, uartkeys_mmap);
}

