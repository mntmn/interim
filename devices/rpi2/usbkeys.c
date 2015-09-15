#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <unistd.h>
#include <fcntl.h>

#define KBUFSZ 5
static char usb_key_in[KBUFSZ];
static int ki = 0;

void uspi_keypress_handler (const char *str)
{
  printf("[uspi-keyboard] pressed: '%s' (%d)\r\n",str,str[0]);

  char k=str[0];
  if (strlen(str)>1) {
    if (k==27) {
      k = str[2];
      printf("[uspi-keyboard] esc seq key2: %d",k);
    }
    if (k==68) k=19;
    if (k==67) k=20;
    if (k==65) k=17;
    if (k==66) k=18;
  }
  
  usb_key_in[ki] = k;
  ki++;
  if (ki>=KBUFSZ) ki = 0;
}

Cell* usbkeys_open(Cell* cpath) {
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[usbkeys] open error: non-string path given\r\n");
    return alloc_nil();
  }
  for (int i=0; i<KBUFSZ; i++) {
    usb_key_in[i]=0;
  }
  ki = 0;

  return alloc_int(1);
}

Cell* usbkeys_read(Cell* stream) {
  char c = 0;
  if (ki>0) {
    ki--;
    c = usb_key_in[0];
    // FIFO
    for (int i=0; i<KBUFSZ-1; i++) {
      usb_key_in[i]=usb_key_in[i+1];
    }
  }
  
  Cell* res = alloc_string_copy(" ");
  ((uint8_t*)res->ar.addr)[0] = c;
  
  return res;
}

Cell* usbkeys_write(Cell* arg) {
  // could be used to control LEDs
  return NULL;
}

Cell* usbkeys_mmap(Cell* arg) {
  return alloc_nil();
}

void mount_usbkeys() {
  fs_mount_builtin("/keyboard", usbkeys_open, usbkeys_read, usbkeys_write, 0, usbkeys_mmap);
}

