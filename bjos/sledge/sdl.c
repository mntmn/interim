#include <stdio.h>
#include "sdl.h"
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"

#define WIDTH 1920
#define HEIGHT 1080
#define BPP 4
#define DEPTH 32

SDL_Surface* screen;
SDL_Window* win;

void sdl_setpixel(Uint32 x, Uint32 y, Uint32 color)
{
  //printf("sdl_setpixel: %ld %ld %ld\n",x,y,color);
  if (x>=WIDTH || y>=HEIGHT) return;
  Uint32 *pixmem32;
  
  pixmem32 = (Uint32*)screen->pixels + (y*WIDTH) + x;
  *pixmem32 = color;
}

void sdl_cleanup() {
  SDL_Quit();
}

static int sdl_initialized = 0;

void* sdl_init(int fullscreen)
{
  if (sdl_initialized) return screen->pixels;

  sdl_initialized = 1;
  
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("sledge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, (fullscreen?SDL_WINDOW_FULLSCREEN:0));
  
  screen = SDL_GetWindowSurface(win);
  atexit(sdl_cleanup);

  return screen->pixels;
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

int sdl_mainloop()
{
  SDL_Event event;
  
  if (SDL_PollEvent(&event)) 
  {      
    switch (event.type) 
    {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_KEYDOWN:
      sdl_modifiers = event.key.keysym.mod;
      sdl_key = event.key.keysym.scancode;
      break;
    }
  }
  
  SDL_UpdateWindowSurface(win);
  
  return 0;
}

void* sdl_get_fb() {
  return screen->pixels;
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
  
    //if (fb_count==1920) {
      SDL_Event event;
      SDL_PollEvent(&event);
      SDL_UpdateWindowSurface(win);
      fb_count=0;
      //}
  return arg;
}



void sdl_mount_fbfs() {
  fs_mount_builtin("/framebuffer", fbfs_open, fbfs_read, fbfs_write, 0);
}
