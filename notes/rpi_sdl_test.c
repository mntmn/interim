#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "jit_arm_raw.c"
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "SDL/SDL.h"

#define CODESZ 1024*4

typedef uint32_t (*funcptr)();

void hello() {
  printf("hello!\n");
}

SDL_Surface* screen;

void setup_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
}

int main() {
  setup_sdl();

  code = mmap(0, CODESZ, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  memset(code, 0, CODESZ);
  cpool_idx = 128; // 128 ops gap

  uint32_t* fb = (uint32_t*)screen->pixels;

  jit_movi(1,0);
  jit_movi(5,0);
  jit_movi(4,4);
  jit_movi(6,1);
  jit_movi(7,640*480);
  jit_movi(2,0xff00ff);
  jit_lea(3,fb);
  jit_label("loop");
  
  jit_strw(2);
  jit_addr(3,4);
  jit_addr(5,6);
  jit_addr(2,6);
  jit_cmpr(5,7);
  
  jit_jne("loop");
  jit_ret();
  jit_ret();
  

  FILE* f = fopen("/tmp/test","w");
  fwrite(code, CODESZ, 1, f);
  fclose(f);

  int mp_res = mprotect(code, CODESZ, PROT_EXEC|PROT_READ);

  funcptr fn = (funcptr)code;
  uint32_t res = fn();
  printf("asm result: %lx\n",res);
  SDL_UpdateRect(screen, 0, 0, 640, 480);

  printf("done\n");
 
  char* foo=malloc(64); 
  gets(foo);

  //free(code);
  //munmap(code);
}
