#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include "minilisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h> // mprotect

Cell* platform_eval(Cell* expr); // FIXME

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
  filesystems_init();

#ifdef DEV_SDL2
  void dev_sdl2_init();
  dev_sdl2_init();
#endif

#ifdef DEV_SDL
  void dev_sdl_init();
  dev_sdl_init();
#endif

#ifdef DEV_LINUXFB
  void mount_linux_fbfs();
  mount_linux_fbfs();
#endif

#ifdef DEV_POSIXFS
  void mount_posixfs();
  mount_posixfs();
#endif
  
#ifdef DEV_CONSOLEKEYS
  void mount_consolekeys();
  mount_consolekeys();
#endif
  
  while (1) {
    expr = NULL;
    
    printf(KWHT "sledge> ");
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
        if (res<cell_heap_start) {
          printf("invalid cell (%p)\n",res);
        } else {
          char out_buf[1024*10];
          lisp_write(res, out_buf, 1024*10);
          printf(KCYN "\n%s\n\n" KWHT,out_buf);
        }
      } else {
        printf("<compilation failed>\n");
        res = NULL;
      }
    }
  }
  return 0;
}

Cell* platform_eval(Cell* expr) {
  if (!expr || expr->tag!=TAG_CONS) {
    printf("[platform_eval] error: no expr given.\r\n");
    return NULL;
  }

  char buf[512];

  int i = 0;
  Cell* res = alloc_nil();
  Cell* c;
  while ((c = car(expr))) {
    i++;
    int tag = compile_for_platform(c, &res); 
  
    if (tag) {
      //printf("~~ expr %d res: %p\r\n",i,res);
      //lisp_write(res, buf, 512);
      //printf("~> %s\r\n",buf);
    } else {
      lisp_write(expr, buf, 512);
      printf("[platform_eval] stopped at expression %d: %s…\r\n",i,buf);
      break;
    }
    // when to free the code? -> when no bound lambdas involved
    
    expr = cdr(expr);
  }
  
  return res;
}
