#include <stdio.h>
#include "sdl2.h"
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"

#include <time.h>
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 600
#define BPP 2
#define DEPTH 8*BPP
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

  win = SDL_CreateWindow("sledge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH*SCALE, HEIGHT*SCALE, (fullscreen?SDL_WINDOW_FULLSCREEN:0));
  
  win_surf = SDL_GetWindowSurface(win);

  if (0) {
    pixels_surf = win_surf;
  } else {
    pixels_surf = SDL_CreateRGBSurface(0,WIDTH,HEIGHT,DEPTH,0xf800,0x7e0,0x1f,0);
  }

  printf("pixels_surf: %p\r\n\r\n",pixels_surf);
  printf("win_surf: %p\r\n\r\n",win_surf);

  memset(pixels_surf->pixels,0xff,WIDTH*HEIGHT*BPP);

  atexit(sdl_cleanup);

  return pixels_surf->pixels;
}

static int sdl_key = 0;
static int sdl_modifiers = 0;
static int sdl_shifted = 0;

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

Cell* fbfs_read(Cell* stream) {
  Stream* s = (Stream*)stream->ar.addr;
  char* path = s->path->ar.addr;
  if (!strcmp(path+12,"/width")) {
    return alloc_int(WIDTH);
  }
  else if (!strcmp(path+12,"/height")) {
    return alloc_int(HEIGHT);
  }
  else if (!strcmp(path+12,"/depth")) {
    return alloc_int(BPP);
  }
  else if (!strcmp(path+12,"/")) {
    return
      alloc_cons(alloc_string_copy("/width"),
      alloc_cons(alloc_string_copy("/height"),
      alloc_cons(alloc_string_copy("/depth"),alloc_nil())));
  }
  else {
    return alloc_int(0);
  }
}

Cell* fbfs_write(Cell* arg) {
  sdl_init(0);
  SDL_Event event;
  if (SDL_PollEvent(&event))
  {
    if (event.type==SDL_QUIT) exit(0);
  }

  SDL_Rect sr = {0,0,WIDTH,HEIGHT};
  SDL_Rect dr = {0,0,WIDTH*SCALE,HEIGHT*SCALE};

  if (SCALE!=1) {
    SDL_BlitScaled(pixels_surf,&sr,win_surf,&dr);
  } else {
    SDL_BlitSurface(pixels_surf,NULL,win_surf,NULL);
  }

  // TODO only if changes happened
  SDL_UpdateWindowSurface(win);

  SDL_Delay(20);
  
  return arg;
}

Cell* fbfs_mmap(Cell* arg) {
  sdl_init(0);
  Cell* fbtest = alloc_num_bytes(0);
  fbtest->ar.addr = sdl_get_fb();
  fbtest->dr.size = sdl_get_fbsize();
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

static int mouse_buttons=0;
static int mouse_x=0;
static int mouse_y=0;
static int last_mouse_x=0;
static int last_mouse_y=0;

Cell* keyfs_read() {
  sdl_key = 0;
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
      sdl_shifted = sdl_modifiers & KMOD_SHIFT;
      sdl_key = event.key.keysym.scancode;
      //printf("key: %d, mod: %x\r\n",sdl_key,sdl_modifiers);

      switch (sdl_key) {
        case SDL_SCANCODE_A: sdl_key = sdl_shifted ? 'A' : 'a'; break;
        case SDL_SCANCODE_B: sdl_key = sdl_shifted ? 'B' : 'b'; break;
        case SDL_SCANCODE_C: sdl_key = sdl_shifted ? 'C' : 'c'; break;
        case SDL_SCANCODE_D: sdl_key = sdl_shifted ? 'D' : 'd'; break;
        case SDL_SCANCODE_E: sdl_key = sdl_shifted ? 'E' : 'e'; break;
        case SDL_SCANCODE_F: sdl_key = sdl_shifted ? 'F' : 'f'; break;
        case SDL_SCANCODE_G: sdl_key = sdl_shifted ? 'G' : 'g'; break;
        case SDL_SCANCODE_H: sdl_key = sdl_shifted ? 'H' : 'h'; break;
        case SDL_SCANCODE_I: sdl_key = sdl_shifted ? 'I' : 'i'; break;
        case SDL_SCANCODE_J: sdl_key = sdl_shifted ? 'J' : 'j'; break;
        case SDL_SCANCODE_K: sdl_key = sdl_shifted ? 'K' : 'k'; break;
        case SDL_SCANCODE_L: sdl_key = sdl_shifted ? 'L' : 'l'; break;
        case SDL_SCANCODE_M: sdl_key = sdl_shifted ? 'M' : 'm'; break;
        case SDL_SCANCODE_N: sdl_key = sdl_shifted ? 'N' : 'n'; break;
        case SDL_SCANCODE_O: sdl_key = sdl_shifted ? 'O' : 'o'; break;
        case SDL_SCANCODE_P: sdl_key = sdl_shifted ? 'P' : 'p'; break;
        case SDL_SCANCODE_Q: sdl_key = sdl_shifted ? 'Q' : 'q'; break;
        case SDL_SCANCODE_R: sdl_key = sdl_shifted ? 'R' : 'r'; break;
        case SDL_SCANCODE_S: sdl_key = sdl_shifted ? 'S' : 's'; break;
        case SDL_SCANCODE_T: sdl_key = sdl_shifted ? 'T' : 't'; break;
        case SDL_SCANCODE_U: sdl_key = sdl_shifted ? 'U' : 'u'; break;
        case SDL_SCANCODE_V: sdl_key = sdl_shifted ? 'V' : 'v'; break;
        case SDL_SCANCODE_W: sdl_key = sdl_shifted ? 'W' : 'w'; break;
        case SDL_SCANCODE_X: sdl_key = sdl_shifted ? 'X' : 'x'; break;
        case SDL_SCANCODE_Y: sdl_key = sdl_shifted ? 'Y' : 'y'; break;
        case SDL_SCANCODE_Z: sdl_key = sdl_shifted ? 'Z' : 'z'; break;

        case SDL_SCANCODE_0: sdl_key = sdl_shifted ? ')' : '0'; break;
        case SDL_SCANCODE_1: sdl_key = sdl_shifted ? '!' : '1'; break;
        case SDL_SCANCODE_2: sdl_key = sdl_shifted ? '@' : '2'; break;
        case SDL_SCANCODE_3: sdl_key = sdl_shifted ? '#' : '3'; break;
        case SDL_SCANCODE_4: sdl_key = sdl_shifted ? '$' : '4'; break;
        case SDL_SCANCODE_5: sdl_key = sdl_shifted ? '%' : '5'; break;
        case SDL_SCANCODE_6: sdl_key = sdl_shifted ? '^' : '6'; break;
        case SDL_SCANCODE_7: sdl_key = sdl_shifted ? '&' : '7'; break;
        case SDL_SCANCODE_8: sdl_key = sdl_shifted ? '*' : '8'; break;
        case SDL_SCANCODE_9: sdl_key = sdl_shifted ? '(' : '9'; break;

        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_RETURN2: sdl_key = 10; break;

        case SDL_SCANCODE_BACKSPACE:
        case SDL_SCANCODE_DELETE: sdl_key = 127; break;

        case SDL_SCANCODE_SPACE: sdl_key = ' '; break;
        case SDL_SCANCODE_MINUS: sdl_key = sdl_shifted ? '_' : '-'; break;
        case SDL_SCANCODE_EQUALS: sdl_key = sdl_shifted ? '+' : '='; break;
        case SDL_SCANCODE_LEFTBRACKET: sdl_key = sdl_shifted ? '{' : '['; break;
        case SDL_SCANCODE_RIGHTBRACKET: sdl_key = sdl_shifted ? '}' : ']'; break;
        case SDL_SCANCODE_BACKSLASH: sdl_key = sdl_shifted ? '|' : '\\'; break;
        case SDL_SCANCODE_SEMICOLON: sdl_key = sdl_shifted ? ':' : ';'; break;
        case SDL_SCANCODE_APOSTROPHE: sdl_key = sdl_shifted ? '"' : '\''; break;
        case SDL_SCANCODE_GRAVE: sdl_key = sdl_shifted ? '~' : '`'; break;
        case SDL_SCANCODE_COMMA: sdl_key = sdl_shifted ? '<' : ','; break;
        case SDL_SCANCODE_PERIOD: sdl_key = sdl_shifted ? '>' : '.'; break;
        case SDL_SCANCODE_SLASH: sdl_key = sdl_shifted ? '?' : '/'; break;

        case SDL_SCANCODE_UP: sdl_key = 17; break; // device control 1
        case SDL_SCANCODE_DOWN: sdl_key = 18; break; // device control 2
        case SDL_SCANCODE_LEFT: sdl_key = 19; break; // device control 3
        case SDL_SCANCODE_RIGHT: sdl_key = 20; break; // device control 4

        default: sdl_key = 0; break;
      }
      break;
    case SDL_MOUSEMOTION:
      mouse_x = event.motion.x;
      mouse_y = event.motion.y;
      mouse_buttons = event.motion.state;
      break;
    }
  }

  Cell* res = alloc_string_copy(" ");
  ((uint8_t*)res->ar.addr)[0] = sdl_key;
  sdl_key = 0;
  return res;
}

void sdl_mount_keyfs() {
  fs_mount_builtin("/keyboard", keyfs_open, keyfs_read, 0, 0, 0);
}


Cell* mouse_open(Cell* cpath) {
  if (!cpath || cpath->tag!=TAG_STR) {
    printf("[usbmouse] open error: non-string path given\r\n");
    return alloc_nil();
  }

  return alloc_int(1);
}

Cell* mouse_read(Cell* stream) {
  mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

  int mouse_dx = mouse_x - last_mouse_x;
  int mouse_dy = mouse_y - last_mouse_y;
  Cell* res = alloc_cons(alloc_cons(alloc_int(mouse_x/SCALE),alloc_int(mouse_y/SCALE)),alloc_int(mouse_buttons));
  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;
  return res;
}

Cell* mouse_write(Cell* arg) {
  return NULL;
}

Cell* mouse_mmap(Cell* arg) {
  return alloc_nil();
}

void sdl_mount_mousefs() {
  fs_mount_builtin("/mouse", mouse_open, mouse_read, mouse_write, 0, mouse_mmap);
}

void dev_sdl_init() {
  sdl_mount_fbfs();
  sdl_mount_keyfs();
  sdl_mount_mousefs();
}
