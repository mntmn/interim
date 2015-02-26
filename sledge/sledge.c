#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <lightning.h>
#include <stdint.h>
#include <stdlib.h>

#include "sdl.h"

#define MAX_INT 4294967296
//#define DEBUG 1

typedef jit_word_t (*funcptr)();
static jit_state_t *_jit;
static jit_state_t *_jit_saved;
static jit_word_t stack_ptr, stack_base;

#include "compiler.c"

inline int machine_video_set_pixel(uint32_t x, uint32_t y, uint32_t color) {
  sdl_setpixel(x,y,color);
  return 1;
}

inline int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  
  for (; y1<y2; y1++) {
    uint32_t x1=x;
    for (; x1<x2; x1++) {
      sdl_setpixel(x1,y1,color);
    }
  }
}

inline int machine_video_flip() {
  sdl_mainloop();
  return 1;
}

int machine_get_key(int modifiers) {
  if (modifiers) return sdl_get_modifiers();
  return sdl_get_key();
}

Cell* machine_save_file(Cell* cell, char* path) {
  printf("about to save: %s\n",path);
  if (cell->tag == TAG_STR || cell->tag == TAG_BYTES) {
    FILE* f = fopen(path,"wb");
    if (!f) return alloc_error(ERR_FORBIDDEN);
    
    fwrite(cell->addr, 1, cell->size, f);
    fclose(f);
    return alloc_int(1);
  } else {
    printf("error: cannot save tag %d\n",cell->tag);
  }
  return alloc_int(0);
}

Cell* machine_load_file(char* path) {
  printf("about to load: %s\n",path);
  struct stat st;
  if (stat(path, &st)) return alloc_error(ERR_NOT_FOUND);

  if (st.st_size < 1) return alloc_bytes(); // zero-byte file

  FILE* f = fopen(path,"rb");
  if (!f) return alloc_error(ERR_FORBIDDEN);

  Cell* result_cell = alloc_num_bytes(st.st_size);
  fread(result_cell->addr, 1, st.st_size, f);
  fclose(f);

  return result_cell;
}

Cell* machine_poll_udp() {
  return alloc_nil();
}

static struct timeval tm1;

static inline void start_clock()
{
  gettimeofday(&tm1, NULL);
}

static inline void stop_clock()
{
  struct timeval tm2;
  gettimeofday(&tm2, NULL);

  unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
  printf("%llu ms\n", t);
}

int main(int argc, char *argv[])
{
  Cell* ptest = NULL;
  char* in_line = NULL;
  char in_buffer[100*1024];
  int len = 0;

  init_compiler();
  
  int fullscreen = 0;

  if (argc>1 && !strcmp(argv[1],"-f")) {
    fullscreen = 1;
  }

  sdl_init(fullscreen);
  
  int in_offset = 0;
  int parens = 0;

  int jit_inited = 0;
  int sdl_inited = 0;

  while (1) {
    ptest = NULL;
    
    printf("sledge> ");
    int r = getline(&in_line, &len, stdin);

    if (r<1) exit(0);

    // recognize parens
    int l = strlen(in_line);
    
    for (int i=0; i<l; i++) {
      if (in_line[i] == ';') break;
      if (in_line[i] == '(') {
        parens++;
      } else if (in_line[i] == ')') {
        parens--;
      }
    }
    
    strcpy(in_buffer+in_offset, in_line);
    
    if (parens>0) {
      printf("…\n");
      if (l>0) {
        in_buffer[in_offset+l-1] = '\n';
      }
      in_offset+=l;
    } else {
      ptest = read_string(in_buffer);
      in_offset=0;
    }
    //printf("parens: %d offset: %d\n",parens,in_offset);
    
    jit_node_t  *in;
    funcptr     compiled;
    
    //printf("stack_ptr: %x\n",stack_ptr);
    //printf("ptest: %p\n",ptest);

    if (ptest) {

      if (!jit_inited) { 
        init_jit(argv[0]);
        jit_inited = 1;
      }

      _jit = jit_new_state();
      jit_prolog();
      stack_ptr = stack_base = jit_allocai(1024 * sizeof(int));

      compile_arg(JIT_R0, ptest, 0, 0);
  
      compiled = jit_emit();
      
#ifdef DEBUG
      jit_disassemble();
      start_clock();
#endif

      Cell* res = (Cell*)compiled(0);

      // TODO: move to write op
      if (!res) {
        printf("null\n");
      } else {
        char out_buf[1024*10];
        lisp_write(res, out_buf, 1024*10);
        printf("%s\n",out_buf);
      }
      
#ifdef DEBUG
      stop_clock();
#endif
      
      jit_clear_state();
      jit_destroy_state();
    } else {
    }

    sdl_mainloop();
  }
  finish_jit();
  return 0;
}
