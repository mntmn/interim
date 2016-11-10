#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include "minilisp.h"
#include <stdlib.h>
#include "alloc.h"

#include "compiler_new.c"

#define BUFSZ 2048

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

#ifdef __APPLE__
#include "../devices/macosx.c"
#endif

void terminal_writestring(const char* data);

int main(int argc, char *argv[])
{
  Cell* expr = NULL;
  char* in_line = malloc(BUFSZ);
  char* in_buffer = malloc(64*BUFSZ);
  char* out_buf = malloc(BUFSZ);
  char* res;
  int in_offset = 0;
  int parens = 0;
  size_t len = 0;
  int i;
  int in_fd = 0;
  FILE* in_f;

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
    in_fd = open(argv[1],O_RDONLY);
    in_f = fdopen(in_fd,"r");
    if (!in_f) in_f = stdin;
  } else {
    in_f = stdin;
  }

  while (1) {
    if (in_f == stdin) printf("interim> ");
    expr = NULL;
    len = 0;

    res = fgets(in_line, BUFSZ, in_f);
    if (res) {
      len = strlen(in_line);
    }

    //printf("line: (%d) |%s|\r\n",len,in_line);
    
    if (len>0) {
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
      in_buffer[in_offset+i]='\n';
      in_buffer[in_offset+i+1]=0;
    
      if (parens>0) {
        if (in_f == stdin) printf("...\r\n");
        in_offset+=i;
      } else {
        in_offset=0;
        if (len>1) {
          expr = (Cell*)read_string(in_buffer);
        } else {
          //printf("\r\n");
        }
      }
    }

    if (feof(in_f) || len==0) {
      if (in_f!=stdin) close(in_fd);
      in_f = stdin;
      in_fd = 0;
      in_offset=0;
      if (feof(stdin)) {
        printf("\n");
        exit(0);
      }
    }
    
    if (expr) {      
      Cell* res;
      int success = compile_for_platform(expr, &res);
      
      if (success) {
        if (in_f == stdin) {
          if (!res) {
            printf("invalid cell (%p)\r\n",res);
          } else {
            lisp_write(res, out_buf, 1024);
            printf("%s\r\n",out_buf);
          }
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
  char* buf=malloc(BUFSZ);
  int i = 0;
  Cell* res = (Cell*)alloc_nil();
  Cell* c;
  int tag;
  
  if (!expr || expr->tag!=TAG_CONS) {
    printf("[platform_eval] error: no expr given.\r\n");
    return NULL;
  }

  while (expr && (c = car(expr))) {
    tag = compile_for_platform(c, &res); 
  
    if (tag) {
      /*printf("~~ expr %d res: %p\r\n",i,res);
      lisp_write(res, buf, 512);
      printf("~> %s\r\n",buf);*/
    } else {
      lisp_write(c, buf, BUFSZ);
      printf("[platform_eval] stopped at expression %d: %s\r\n",i,buf);
      break;
    }
    // TOOD: when to free the code blocks? -> when no bound lambdas involved
    
    i++;
    expr = cdr(expr);
  }
  free(buf);
  
  return res;
}
