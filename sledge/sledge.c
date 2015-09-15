#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include "minilisp.h"
#include <stdint.h>
#include <stdlib.h>
#include "alloc.h"
//#include <sys/mman.h> // mprotect

Cell* platform_eval(Cell* expr); // FIXME

#include "compiler_new.c"

#define BUFSZ 1024

#ifdef CPU_X64
#include "compiler_x64_hosted.c"
#endif

#ifdef CPU_ARM 
#include "compiler_arm_hosted.c"
#endif

#ifdef CPU_X86
#include "compiler_x86.c"
#endif

#ifdef __AMIGA
#include "compiler_m68k.c"
#endif

//ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void terminal_writestring(const char* data);

int main(int argc, char *argv[])
{
  Cell* expr = NULL;
  char* in_line = malloc(1024);
  char* in_buffer = malloc(10*1024);
  char* out_buf = malloc(1024);
  int in_offset = 0;
  int parens = 0;
  size_t len = 0;
  int i,l;
  FILE* in_file = stdin;

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

#ifdef DEV_BIOS
  void mount_bios();
  mount_bios();
#endif
  
#ifdef DEV_CONSOLEKEYS
  void mount_consolekeys();
  mount_consolekeys();
#endif
  
#ifdef __AMIGA
  mount_amiga();
#endif
  
  if (argc==2) {
    in_file = fopen(argv[1],"r");
  }

  while (1) {
    expr = NULL;
    
    printf("sledge> ");
    
    in_line = fgets(in_line, BUFSZ, in_file);
    if (in_line) {
      len = strlen(in_line);
    } else {
      len = 0;
    }

    //printf("line: (%d) |%s|\r\n",len,in_line);
    
    if (!in_line || feof(in_file)) {
      exit(0);
    }
    
    // recognize parens
    for (i=0; i<len; i++) {
      if (in_line[i] == ';') break;
      if (in_line[i] == '(') {
        parens++;
      } else if (in_line[i] == ')') {
        parens--;
      }
    }

    strncpy(in_buffer+in_offset, in_line, i);
    in_buffer[in_offset+i]=0;
    
    if (parens>0) {
      printf("...\n");
      if (len>0) {
        //in_buffer[in_offset+len-1] = '\n';
      }
      in_offset+=i;
    } else {
      if (len>1) {
        //printf("reading: |%s|\r\n",in_buffer);
        expr = (Cell*)read_string(in_buffer);
        in_offset=0;

        printf("compiling: %p\r\n",expr);
      }
    }
    
    if (expr) {      
      Cell* res;
      int success = compile_for_platform(expr, &res);
      
      if (success) {
        // OK
        if (!res) {
          printf("invalid cell (%p)\n",res);
        } else {
          lisp_write(res, out_buf, 1024);
          printf("\n%s\n\n",out_buf);
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
  char buf[512];
  int i = 0;
  Cell* res = (Cell*)alloc_nil();
  Cell* c;
  int tag;
  
  if (!expr || expr->tag!=TAG_CONS) {
    printf("[platform_eval] error: no expr given.\r\n");
    return NULL;
  }

  while (expr && (c = car(expr))) {
    i++;
    tag = compile_for_platform(c, &res); 
  
    if (tag) {
      //printf("~~ expr %d res: %p\r\n",i,res);
      //lisp_write(res, buf, 512);
      //printf("~> %s\r\n",buf);
    } else {
      lisp_write(expr, buf, 512);
      printf("[platform_eval] stopped at expression %d: %s...\r\n",i,buf);
      break;
    }
    // when to free the code? -> when no bound lambdas involved
    
    expr = cdr(expr);
  }
  
  return res;
}
