#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 1024
#define HEIGHT 768
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

void sdl_init(int fullscreen)
{
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("sledge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, (fullscreen?SDL_WINDOW_FULLSCREEN:0));
  
  screen = SDL_GetWindowSurface(win);
  atexit(sdl_cleanup);
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
