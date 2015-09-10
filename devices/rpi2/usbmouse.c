#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <unistd.h>
#include <fcntl.h>

static int mouse_dx;
static int mouse_dy;
static int mouse_buttons;

void uspi_mouse_handler(unsigned buttons, int dx, int dy)
{
  //printf("[uspi-mouse] dx: %d dy: %d buttons: %d\r\n",dx,dy,buttons);
  mouse_dx += dx;
  mouse_dy += dy;
  mouse_buttons = buttons;
}

Cell* usbmouse_open(Cell* cpath) {
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[usbmouse] open error: non-string path given\r\n");
    return alloc_nil();
  }

  return alloc_int(1);
}

Cell* usbmouse_read(Cell* stream) {
  Cell* res = alloc_cons(alloc_cons(alloc_int(mouse_dx),alloc_int(mouse_dy)),alloc_int(mouse_buttons));
  mouse_dx = 0;
  mouse_dy = 0;
  return res;
}

Cell* usbmouse_write(Cell* arg) {
  // could be used to control LEDs
  return NULL;
}

Cell* usbmouse_mmap(Cell* arg) {
  return alloc_nil();
}

void mount_mouse() {
  fs_mount_builtin("/mouse", usbmouse_open, usbmouse_read, usbmouse_write, 0, usbmouse_mmap);
}

