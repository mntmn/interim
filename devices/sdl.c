#include <stdio.h>
#include "sdl.h"
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"

#define WIDTH 1920
#define HEIGHT 1080
#define BPP 2
#define DEPTH 16
#define SCALE 2

SDL_Surface* screen;
uint8_t* pixels = NULL;

void sdl_cleanup() {
  SDL_Quit();
}

static int sdl_initialized = 0;

void* sdl_init(int fullscreen)
{
  if (sdl_initialized) return screen->pixels;

  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_SWSURFACE);

  //if (pixels) free(pixels);
  //pixels = malloc(WIDTH*HEIGHT*BPP);
  
  atexit(sdl_cleanup);

  return screen->pixels;
  return NULL;
}

static int sdl_key = 0;
static int sdl_modifiers = 0;
int sdl_get_key() {
  int k = sdl_key;
  sdl_key = 0;
  return k;
}

int sdl_get_modifiers() {
  int m = sdl_modifiers;
  return m;
}

void* sdl_get_fb() {
  if (screen) return screen->pixels;
  return NULL;
}

long sdl_get_fbsize() {
  return WIDTH*HEIGHT*BPP;
}


Cell* fbfs_open() {
  sdl_init(0);
  return alloc_int(1);
}

Cell* fbfs_read() {
  return alloc_int(0);
}

static int fb_state = 0;
static int cursor_x = 0;
static int cursor_y = 0;

static int fb_count = 0;

Cell* fbfs_write(Cell* arg) {
  //printf("[fbfs_write] %lx\n",arg->value);
  SDL_Event event;
  SDL_PollEvent(&event);
  //SDL_BlitScaled(pixels_surf,{0,0,WIDTH/SCALE,HEIGHT/SCALE},screen,{0,0,WIDTH,HEIGHT});
  SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);
  fb_count=0;
  return arg;
}

Cell* fbfs_mmap(Cell* arg) {
  Cell* fbtest = alloc_num_bytes(0);
  fbtest->addr = sdl_get_fb();
  fbtest->size = sdl_get_fbsize();
  printf("fbtest->addr: %p\n",fbtest->addr);
  printf("fbtest->size: %lx\n",fbtest->size);

  memset(fbtest->addr,0xff,WIDTH*HEIGHT*BPP);

  return fbtest;
}

void sdl_mount_fbfs() {
  fs_mount_builtin("/framebuffer", fbfs_open, fbfs_read, fbfs_write, 0, fbfs_mmap);
}

Cell* keyfs_open() {
  return alloc_int(1);
}

Cell* keyfs_read() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) 
  {
    //printf("sdl event! %d\n",event.type);
    
    switch (event.type) 
    {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_KEYDOWN:
      sdl_modifiers = event.key.keysym.mod;
      sdl_key = event.key.keysym.sym;
      break;
    }
  }

  switch (sdl_key) {
  case 13: sdl_key = 10; break;
  case 8: sdl_key = 127; break;
  }
  
  Cell* res = alloc_string_copy(" ");
  ((uint8_t*)res->addr)[0] = sdl_key;
  sdl_key = 0;
  return res;
}

void sdl_mount_keyfs() {
  fs_mount_builtin("/keyboard", keyfs_open, keyfs_read, 0, 0, 0);
}

void dev_sdl_init() {
  sdl_mount_fbfs();
  sdl_init(0);
  
  /*Cell* fbtest = alloc_num_bytes(0);
  fbtest->addr = sdl_get_fb();
  fbtest->size = sdl_get_fbsize();
  printf("fbtest->addr: %p\n",fbtest->addr);
  printf("fbtest->size: %lx\n",fbtest->size);
  
  insert_global_symbol(alloc_sym("fb"), fbtest);*/

  sdl_mount_keyfs();
}
