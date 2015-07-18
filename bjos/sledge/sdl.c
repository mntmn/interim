	#include <stdio.h>
	#include "sdl.h"
	#include "minilisp.h"
	#include "alloc.h"
	#include "stream.h"
	#include "compiler_new.h"

	#define WIDTH 640
	#define HEIGHT 480
	#define BPP 4
	#define DEPTH 32

	SDL_Surface* screen;

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

	  SDL_Init(SDL_INIT_VIDEO);
	  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

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
	  SDL_Event event;
	  SDL_PollEvent(&event);
	  SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);
  fb_count=0;
  return arg;
}

void sdl_mount_fbfs() {
  fs_mount_builtin("/framebuffer", fbfs_open, fbfs_read, fbfs_write, 0);
}

void dev_sdl_init() {
  sdl_mount_fbfs();
  sdl_init(0);
  
  Cell* fbtest = alloc_num_bytes(0);
  fbtest->addr = sdl_get_fb();
  fbtest->size = sdl_get_fbsize();
  printf("fbtest->addr: %p\n",fbtest->addr);
  printf("fbtest->size: %lx\n",fbtest->size);
  
  insert_global_symbol(alloc_sym("fb"), fbtest);
}
