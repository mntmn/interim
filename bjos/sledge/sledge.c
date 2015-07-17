#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include "minilisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h> // mprotect

#include "sdl.h"

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

Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
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
  
  sdl_mount_fbfs();

  sdl_init(0);
  Cell* fbtest = alloc_num_bytes(0);
  fbtest->addr = sdl_get_fb();
  fbtest->size = sdl_get_fbsize();
  printf("fbtest->addr: %p\n",fbtest->addr);
  printf("fbtest->size: %lx\n",fbtest->size);
  
  insert_symbol(alloc_sym("fb"), fbtest, &global_env);

  int in_offset = 0;
  int parens = 0;

  int jit_inited = 0;
  int sdl_inited = 0;

  //stack_ptr = stack_base = malloc(4096 * sizeof(jit_word_t));

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
    
    funcptr     compiled;
    
    if (expr) {      
      Cell* res;
      int success = 1;
      
      jit_out = fopen("/tmp/jit_out.s","w");
  
      int tag = compile_expr(expr, NULL);
      jit_ret();

      if (success) {
        fclose(jit_out);

        FILE* asm_f = fopen("/tmp/jit_out.s","r");
        uint32_t* jit_asm = malloc(4096);
        memset(jit_asm, 0, 4096);
        fread(jit_asm,1,4096,asm_f);
        fclose(asm_f);
        
#ifdef DEBUG
        start_clock();
        printf("\nJIT ---------------------\n%s-------------------------\n\n",jit_asm);
#endif
        
        // prefix with arm-none-eabi- on ARM  -mlittle-endian
      
        system("as /tmp/jit_out.s -o /tmp/jit_out.o");
        system("objcopy /tmp/jit_out.o -O binary /tmp/jit_out.bin");

        FILE* binary_f = fopen("/tmp/jit_out.bin","r");

        uint32_t* jit_binary = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
        
        int bytes_read = fread(jit_binary,1,4096,binary_f);
        fclose(binary_f);

#ifdef DEBUG
        printf("<assembled bytes: %d at: %p>\n",bytes_read,jit_binary);
#endif

        // read symbols for linking lambdas
        system("nm /tmp/jit_out.o > /tmp/jit_out.syms");
        FILE* link_f = fopen("/tmp/jit_out.syms","r");
        if (link_f) {
          char link_line[128];
          while(fgets(link_line, sizeof(link_line), link_f)) {

            if (strlen(link_line)>22) {
              char ida=link_line[19];
              char idb=link_line[20];
              char idc=link_line[21];
              //printf("link_line: %s %c %c %c\n",link_line,ida,idb,idc);

              if (ida=='f' && idc=='_') {
                Cell* lambda = (Cell*)strtoul(&link_line[24], NULL, 16);
                if (idb=='0') {
                  // function entrypoint
                  // TODO: 64/32 bit
                  unsigned long long offset = strtoul(link_line, NULL, 16);
                  void* binary = ((uint8_t*)jit_binary) + offset;
                  //printf("function %p entrypoint: %p (+%ld)\n",lambda,binary,offset);

                  if (lambda->tag == TAG_LAMBDA) {
                    lambda->next = binary;
                  } else {
                    printf("fatal error: no lambda found at %p!\n",lambda);
                  }
                }
                else if (idb=='1') {
                  // function exit
                  unsigned long long offset = strtoul(link_line, NULL, 16);
                  //printf("function exit point: %p\n",offset);
                }
              }
            }
          }
        }
      
        //arm_emulate(jit_binary);
        int mp_res = mprotect(jit_binary, 4096, PROT_EXEC|PROT_READ);

        if (!mp_res) {
          res = execute_jitted(jit_binary);
        } else {
          printf("<mprotect result: %d\n>",mp_res);
          res = NULL;
        }
        
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
      
#ifdef DEBUG
      stop_clock();
#endif

      //MemStats* mst = alloc_stats();
      //printf("%lu heap bytes, %lu/%lu stack bytes used\n",mst->heap_bytes_used,mst->stack_bytes_used,mst->stack_bytes_max);
    }

    //sdl_mainloop();
  }
  return 0;
}
