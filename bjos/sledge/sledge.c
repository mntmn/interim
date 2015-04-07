#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <lightning.h>
#include <stdint.h>
#include <stdlib.h>

#include "sdl.h"

#define MAX_INT 4294967296
#define DEBUG 1

typedef jit_word_t (*funcptr)();
static jit_state_t *_jit;
static void *stack_ptr, *stack_base;

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
  machine_video_rect(0,0,1920,1080,0xffffff);
  return 1;
}

int machine_get_key(int modifiers) {
  if (modifiers) return sdl_get_modifiers();
  int k = sdl_get_key();
  return k;
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

static char sysfs_tmp[1024];

Cell* machine_load_file(char* path) {
  // sysfs
  if (!strcmp(path,"/sys/mem")) {
    MemStats* mst = alloc_stats();
    sprintf(sysfs_tmp, "(%d %d %d %d)", mst->byte_heap_used, mst->byte_heap_max, mst->cells_used, mst->cells_max);
    return read_string(sysfs_tmp);
  }
  
  char buf[512];
  sprintf(buf,"fs/%s",path);
  path = buf;
  
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

Cell* my_tcp_connected_callback;
Cell* my_tcp_data_callback;

Cell* machine_poll_udp() {
  if (my_tcp_connected_callback) {
    printf("-- calling tcp connected callback\n");
    funcptr fn = (funcptr)my_tcp_connected_callback->next;
    return NULL;
  }
  return NULL;
}

Cell* machine_send_udp(Cell* data_cell) {
  return data_cell;
}

Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell) {
  my_tcp_connected_callback = connected_fn_cell;
  
  return port_cell;
}

Cell* machine_send_tcp(Cell* data_cell) {
  return data_cell;
}

Cell* machine_bind_tcp(Cell* port_cell, Cell* fn_cell) {
  return fn_cell;
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

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

int main(int argc, char *argv[])
{
  Cell* expr = NULL;
  char* in_line = NULL;
  char* in_buffer = malloc(100*1024);
  size_t len = 0;

  init_compiler();
  
  int fullscreen = 0;

  if (argc>1 && !strcmp(argv[1],"-f")) {
    fullscreen = 1;
  }

  uint32_t* FB = (uint32_t*)sdl_init(fullscreen);
  init_blitter(FB);
  
  Cell* unif = machine_load_file("unifont");
  printf("~~ unifont is at %p\r\n",unif->addr);
  
  extern uint8_t* blitter_speedtest(uint8_t* font);
  unif->addr = blitter_speedtest(unif->addr);
  insert_symbol(alloc_sym("unifont"), unif, &global_env);
  
  int in_offset = 0;
  int parens = 0;

  int jit_inited = 0;
  int sdl_inited = 0;

  stack_ptr = stack_base = malloc(4096 * sizeof(jit_word_t));

  while (1) {
    expr = NULL;
    
    printf("sledge> ");
    len = 0;
    int r = getline(&in_line, &len, stdin);

    if (r<1 || !in_line) exit(0);

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
      expr = read_string(in_buffer);
      in_offset=0;
    }
    
    jit_node_t  *in;
    funcptr     compiled;
    
    if (expr) {
      if (!jit_inited) { 
        init_jit(argv[0]);
        jit_inited = 1;
      }

      //stack_ptr = stack_base;
        
      _jit = jit_new_state();
      jit_prolog();
      
      Cell* res;
      int success = compile_arg(JIT_R0, expr, TAG_ANY);

      if (success) {
        compiled = jit_emit();
      
#ifdef DEBUG
        //jit_disassemble();
      start_clock();
#endif

        res = (Cell*)compiled(0);
      } else {
        printf("<compilation failed>\n");
        res = NULL;
      }

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

      //MemStats* mst = alloc_stats();
      //printf("%lu heap bytes, %lu/%lu stack bytes used\n",mst->heap_bytes_used,mst->stack_bytes_used,mst->stack_bytes_max);

      //collect_garbage(global_env);
      
      jit_clear_state();
      jit_destroy_state();
    } else {
    }

    sdl_mainloop();
  }
  finish_jit();
  return 0;
}
