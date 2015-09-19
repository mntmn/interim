#include <stdio.h>
#include <exec/types.h>
#include <exec/io.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>

#include <devices/inputevent.h>
#include <devices/keyboard.h>

#include "minilisp.h"
#include "alloc.h"

#define WIDTH 512
#define HEIGHT 250
#define BPP 1
#define DEPTH 8

// http://cahirwpz.users.sourceforge.net/libnix/swapstack.html#swapstack
unsigned long __stack=128000; // stack requirements (bytes) for swapstack.o from libnix

struct Library *intuition_base;
struct Library *gfx_base;
struct Window *window;
struct Cell* buffer_cell;

Cell* amiga_fbfs_open() {
  return alloc_int(1);
}

Cell* amiga_fbfs_read() {
  return alloc_int(0);
}

Cell* amiga_fbfs_write(Cell* arg) {
  // TODO: render buffer to window rastport using chunky->planar conversion

  uint8_t* src  = (uint8_t*)buffer_cell->ar.addr;
  uint8_t* dest1 = (uint8_t*)window->RPort->BitMap->Planes[0];
  uint8_t* dest2 = (uint8_t*)window->RPort->BitMap->Planes[1];
  int i, j, x, y;

  uint32_t screenw = window->WScreen->Width;
  uint32_t pitch;
  uint32_t offset;
  int bitplanes;
  int bpr;

  bitplanes=window->RPort->BitMap->Depth;
  bpr=window->RPort->BitMap->BytesPerRow;
  offset=window->LeftEdge/8 + ((window->TopEdge)*bpr);
  pitch=bpr - window->Width/8;

  /*printf("topedge: %d barheight: %d\r\n",window->TopEdge,window->WScreen->BarLayer->Height);
  printf("screenw: %d winw: %d\r\n",screenw,window->Width);
  printf("pitch: %d\r\n",pitch);
  printf("offset: %d\r\n",offset);
  printf("src: %p\r\n",src);
  printf("bitplane: %p\r\n",dest1);*/

  dest1+=offset;
  //dest2+=offset;

  j=0;
  // 8 bytes become 1
  for (y=0; y<HEIGHT; y++) {
    for (i=0; i<WIDTH; i+=8) {
      uint8_t d = 0;
      d|=((*src++)&1)<<7;
      d|=((*src++)&1)<<6;
      d|=((*src++)&1)<<5;
      d|=((*src++)&1)<<4;
      d|=((*src++)&1)<<3;
      d|=((*src++)&1)<<2;
      d|=((*src++)&1)<<1;
      d|=((*src++)&1);
    
      *dest1++ = d;
      //*dest2++ = d;
    }
    dest1+=pitch;
    dest2+=pitch;
    //printf("line: %d\r\n",y);
  }
  
  return NULL;
}

Cell* amiga_fbfs_mmap(Cell* arg) {
  long sz = WIDTH*HEIGHT*BPP;
  int x,y;
  uint8_t* dest;
  buffer_cell = alloc_num_bytes(sz);
  printf("[amiga_fbfs_mmap] buffer_cell->addr: %p\n",buffer_cell->ar.addr);
  
  window = OpenWindowTags(NULL, WA_Title, (ULONG) "interim/amiga",
    WA_Left, 0,
    WA_Top, 0,
    WA_Width, WIDTH,
    WA_Height, HEIGHT,
    WA_Flags, WFLG_NOCAREREFRESH,
    WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_GADGETUP,
                          TAG_DONE);
  
  return buffer_cell;
}

Cell* amiga_keyfs_open() {
  return alloc_int(1);
}

char key_to_rune[] = {
  0,
  0,
  '1','2','3','4','5','6','7','8','9','0','/','"',9,9,
  '\t','q','w','e','r','t','z','u','i','o','p','-','+',9,0,0,
  0,'a','s','d','f','g','h','j','k','l','(',')',0,0,'*',0,0,'<',
  'y','x','c','v','b','n','m',',','.','-',0,0,0,0,
  0,' ',9,0,0,10,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0
};

Cell* amiga_keyfs_read() {
  
  Cell* key = alloc_string_copy(" ");
  uint8_t* magic = (void*)0xbfec01;
  uint8_t k = *magic;
  ((char*)key->ar.addr)[0] = 0;
  if (!(k&1)) {
    // keyup
    return key;
  }
  k>>=1;
  k=128-k;
  printf("keyin: %d\r\n",k);
  ((char*)key->ar.addr)[0] = key_to_rune[k];
  return key;
}

void mount_amiga_keyfs() {
  fs_mount_builtin("/keyboard", amiga_keyfs_open, amiga_keyfs_read, 0, 0, 0);
}

void mount_posixfs();

void mount_amiga_fbfs() {
  fs_mount_builtin("/framebuffer", amiga_fbfs_open, amiga_fbfs_read, amiga_fbfs_write, 0, amiga_fbfs_mmap);
  insert_global_symbol(alloc_sym("screen-width"),alloc_int(512));
  insert_global_symbol(alloc_sym("screen-height"),alloc_int(250));
  insert_global_symbol(alloc_sym("screen-bpp"),alloc_int(1));

  mount_amiga_keyfs();
  mount_posixfs();
}

void uart_puts(char* str) {
  printf(str);
}

void uart_putc(char c) {
  printf("%c",c);
}

void handle_window_events(struct Window *);

void cleanup_amiga() {
  if (window) CloseWindow(window);
  if (gfx_base) CloseLibrary(gfx_base);
  if (intuition_base) CloseLibrary(intuition_base);
}

void mount_amiga() {
  // TODO: exit if stack too small
  
  atexit(cleanup_amiga);
  intuition_base = OpenLibrary("intuition.library", 37);
  
  if (intuition_base == NULL) {
    printf("error: could not open intuition.library v37\r\n");
    return;
  }

  mount_amiga_fbfs();
  //amiga_fbfs_mmap(NULL);
}
