#include <stdio.h>
#include "sdl2.h"
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"

#define WIDTH 1920
#define HEIGHT 1080
#define BPP 2
#define DEPTH 16
#define SCALE 1

SDL_Surface* win_surf;
SDL_Surface* pixels_surf;
SDL_Window* win;

void sdl_cleanup() {
  SDL_Quit();
}

static int sdl_initialized = 0;

void* sdl_init(int fullscreen)
{
  if (sdl_initialized) return pixels_surf->pixels;

  sdl_initialized = 1;
  
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("sledge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, (fullscreen?SDL_WINDOW_FULLSCREEN:0));
  
  win_surf = SDL_GetWindowSurface(win);

  pixels_surf = SDL_CreateRGBSurface(0,WIDTH,HEIGHT,DEPTH,0xf800,0x7e0,0x1f,0);

  printf("pixels_surf: %p\r\n\r\n",pixels_surf);
  printf("win_surf: %p\r\n\r\n",win_surf);

  memset(pixels_surf->pixels,0xff,WIDTH*HEIGHT*BPP);

  atexit(sdl_cleanup);

  return pixels_surf->pixels;
}

static int sdl_key = 0;
static int sdl_modifiers = 0;

void* sdl_get_fb() {
  return pixels_surf->pixels;
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

Cell* fbfs_write(Cell* arg) {
  sdl_init(0);
  SDL_Event event;
  SDL_PollEvent(&event);

  SDL_Rect sr = {0,0,WIDTH/SCALE,HEIGHT/SCALE};
  SDL_Rect dr = {0,0,WIDTH,HEIGHT};

  SDL_BlitScaled(pixels_surf,&sr,win_surf,&dr);
  
  SDL_UpdateWindowSurface(win);
  return arg;
}

Cell* fbfs_mmap(Cell* arg) {
  sdl_init(0);
  Cell* fbtest = alloc_num_bytes(0);
  fbtest->addr = sdl_get_fb();
  fbtest->size = sdl_get_fbsize();
  //printf("fbtest->addr: %p\n",fbtest->addr);
  //printf("fbtest->size: %lx\n",fbtest->size);

  return fbtest;
}

void sdl_mount_fbfs() {
  fs_mount_builtin("/framebuffer", fbfs_open, fbfs_read, fbfs_write, 0, fbfs_mmap);
}


Cell* keyfs_open() {
  return alloc_int(1);
}

#include <time.h>
#include <unistd.h>

Cell* keyfs_read() {
  sdl_key = 0;
  SDL_Event event;

  usleep(20000);
  
  if (SDL_PollEvent(&event))
  {
    //printf("sdl event! %d\n",event.type);
    
    switch (event.type) 
    {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_TEXTINPUT:
    case SDL_KEYDOWN:
      if (event.type == SDL_KEYDOWN) {
        sdl_modifiers = event.key.keysym.mod;
        //printf("key: %d, mod: %x\r\n",event.key.keysym.sym,event.key.keysym.mod);
        sdl_key = event.key.keysym.sym;
      } else {
        sdl_modifiers = 0;
        sdl_key = event.text.text[0];
      }
      
      if (sdl_key<200) {
      } else {
        switch (sdl_key) {
          case 1073741906: sdl_key = 17; break; // DC1 cursor up
          case 1073741905: sdl_key = 18; break; // DC2 cursor down
          case 1073741904: sdl_key = 19; break; // DC3 cursor left
          case 1073741903: sdl_key = 20; break; // DC4 cursor right
          default: sdl_key = 0;
        }
      }
      if (sdl_modifiers&1 || sdl_modifiers&2) {
        if (sdl_key>='a' && sdl_key<='z') {
          sdl_key+=('A'-'a');
        }
        else {
          switch (sdl_key) {
          case 223: sdl_key = '?'; break;
          case '1': sdl_key = '!'; break;
          case '2': sdl_key = '"'; break;
          case '3': sdl_key = '~'; break;
          case '4': sdl_key = '$'; break;
          case '5': sdl_key = '%'; break;
          case '6': sdl_key = '&'; break;
          case '7': sdl_key = '/'; break;
          case '8': sdl_key = '('; break;
          case '9': sdl_key = ')'; break;
          case '0': sdl_key = '='; break;
          case '<': sdl_key = '>'; break;
          case '+': sdl_key = '*'; break;
          case '#': sdl_key = '\''; break;
          case ',': sdl_key = ';'; break;
          case '.': sdl_key = ':'; break;
          case '-': sdl_key = '_'; break;
          default: break;
          }
        }
      }
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
  sdl_mount_keyfs();
}
