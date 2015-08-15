#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <unistd.h>
#include <fcntl.h>

#define KBUFSZ 5
static char uart_key_in[KBUFSZ];
static int ki = 0;

void uspi_keypress_handler (const char *str)
{
  printf("[uspi-keyboard] pressed: '%s' (%d)\r\n",str,str[0]);
  uart_key_in[ki] = str[0];
  ki++;
  if (ki>=KBUFSZ) ki = 0;
}

Cell* uartkeys_open(Cell* cpath) {
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[uartkeys] open error: non-string path given\r\n");
    return alloc_nil();
  }
  for (int i=0; i<KBUFSZ; i++) {
    uart_key_in[i]=0;
  }
  ki = 0;

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
    if (k==91) {
      k = uart_getc();
      if (k==27) {
        // fast repetition
        k = uart_getc();
        k = uart_getc();
      }
      if (k==68) k=130;
      if (k==67) k=131;
      if (k==65) k=132;
      if (k==66) k=133;
      printf("~~ inkey unknown sequence: 91,%d\r\n",k);
      k = 0;
    }
  }
  
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
  ((uint8_t*)res->addr)[0] = k;
  
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

