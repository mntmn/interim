#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include "minilisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h> // mprotect

#define DEBUG

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#include "compiler_new.c"

#ifdef CPU_X64
#include "compiler_x64_hosted.c"
#endif

#ifdef CPU_ARM 
#include "compiler_arm_hosted.c"
#endif

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

int main(int argc, char *argv[])
{
  Cell* expr = NULL;
  char* in_line = NULL;
  char* in_buffer = malloc(100*1024);
  int in_offset = 0;
  int parens = 0;
  size_t len = 0;

  init_compiler();

#ifdef DEV_SDL2
  dev_sdl2_init();
#endif

#ifdef DEV_SDL
  dev_sdl_init();
#endif
  
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
    
    if (expr) {      
      Cell* res;
      
      int success = compile_for_platform(expr, &res); 

      if (success) {
        // OK
      } else {
        printf("<compilation failed>\n");
        res = NULL;
      }

      // TODO: move to write op
      if (res<cell_heap_start) {
        printf("invalid cell (%p)\n",res);
      } else {
        char out_buf[1024*10];
        lisp_write(res, out_buf, 1024*10);
        printf(KBLU "\n%s\n\n" KWHT,out_buf);
      }
      
      //MemStats* mst = alloc_stats();
      //printf("%lu heap bytes, %lu/%lu stack bytes used\n",mst->heap_bytes_used,mst->stack_bytes_used,mst->stack_bytes_max);
    }
  }
  return 0;
}
