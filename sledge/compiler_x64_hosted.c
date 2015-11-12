#if defined(__APPLE__) && defined(__MACH__)
#define MAP_ANONYMOUS MAP_ANON
#endif

//#define DEBUG

#include <sys/mman.h> // mprotect
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
}

int compile_for_platform(Cell* expr, Cell** res) {
  jit_out = fopen("/tmp/jit_out.s","w");
  
  jit_init();
  
  register void* sp asm ("sp");
  Frame* empty_frame = malloc(sizeof(Frame)); // FIXME leak
  empty_frame->f=NULL;
  empty_frame->sp=0;
  empty_frame->locals=0;
  empty_frame->stack_end=sp;
  empty_frame->parent_frame=NULL;

  Cell* success = compile_expr(expr, empty_frame, prototype_any);
  jit_ret();
  char* defsym = "anon";

  if (!success) {
    printf("<compile_expr failed: %p>\r\n",success);
  }

  if (success) {
    int codesz = 1024;
    fclose(jit_out);

    struct stat src_stat;
    stat("/tmp/jit_out.s", &src_stat);
    off_t generated_sz = src_stat.st_size;

    if (expr && expr->tag == TAG_CONS && car(expr)->tag == TAG_SYM) {
      if (!strcmp(car(expr)->ar.addr,"def")) {
        defsym = car(cdr(expr))->ar.addr;
        char cpcmd[256];
        printf("compiled def %s\r\n",defsym);
        snprintf(cpcmd,255,"cp /tmp/jit_out.s /tmp/jit_out_%s.s",defsym);
        system(cpcmd);
      }
    }

#ifdef DEBUG
    FILE* asm_f = fopen("/tmp/jit_out.s","r");
    char* jit_asm = malloc(generated_sz+1);
    memset(jit_asm,0,generated_sz+1);
    int read = fread(jit_asm,1,generated_sz,asm_f);
    fclose(asm_f);
    printf("read: %d\n",read);
    
    printf("\nJIT ---------------------\n%s\n-------------------------\n\n",jit_asm);
    free(jit_asm);
#endif
        
    // prefix with arm-none-eabi- on ARM  -mlittle-endian
    
#if defined(__APPLE__) && defined(__MACH__)
    system("clang -no-integrated-as -c /tmp/jit_out.s -o /tmp/jit_out.o -Xassembler -L");
    system("gobjcopy /tmp/jit_out.o -O binary /tmp/jit_out.bin");
#else
    system("as -L /tmp/jit_out.s -o /tmp/jit_out.o");
    system("objcopy /tmp/jit_out.o -O binary /tmp/jit_out.bin");
#endif

    stat("/tmp/jit_out.bin", &src_stat);
    
    generated_sz = src_stat.st_size;
    while (generated_sz>codesz) {
      codesz*=2;
      printf ("<compiler: doubling code block size to %d>\r\n",codesz);
    }
    
    FILE* binary_f = fopen("/tmp/jit_out.bin","r");
    
    uint32_t* jit_binary = mmap(0, codesz, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
    
    int bytes_read = fread(jit_binary,1,codesz,binary_f);
    fclose(binary_f);

#ifdef DEBUG
    printf("<assembled bytes: %d at: %p (%s)>\n",bytes_read,jit_binary,defsym);
    char cmd[256];
    sprintf(cmd,"cp /tmp/jit_out.o /tmp/jit_%p_%s.o",jit_binary,defsym);
    system(cmd);
    sprintf(cmd,"cp /tmp/jit_out.s /tmp/jit_%p_%s.s",jit_binary,defsym);
    system(cmd);
#endif

    if (bytes_read>codesz) {
      printf("<error: max assembly size of %d exhausted. aborting>\n",codesz);
      munmap(jit_binary,codesz);
      return 0;
    }

    // read symbols for linking lambdas
#if defined(__APPLE__) && defined(__MACH__)
    system("gnm /tmp/jit_out.o > /tmp/jit_out.syms 2> /dev/null");
#else
    system("nm /tmp/jit_out.o > /tmp/jit_out.syms");
#endif
    FILE* link_f = fopen("/tmp/jit_out.syms","r");
    if (link_f) {
      char* link_line=malloc(128);
      while(fgets(link_line, 128, link_f)) {

        if (strlen(link_line)>22) {
          char ida=link_line[19];
          char idb=link_line[20];
          char idc=link_line[21];
          //printf("link_line: %s %c %c %c\n",link_line,ida,idb,idc);

          if (ida=='L' && idc=='_') {
            Cell* lambda = (Cell*)strtoul(&link_line[24], NULL, 16);
            if (idb=='0') {
              // function entrypoint
              uint64_t offset = strtoul(link_line, NULL, 16);
              void* binary = ((uint8_t*)jit_binary) + offset;
              //printf("function %p entrypoint: %p (+%ld)\n",lambda,binary,offset);

              if (lambda->tag == TAG_LAMBDA) {
                lambda->dr.next = binary;
              } else {
                printf("fatal error: no lambda found at %p!\n",lambda);
              }
            }
            else if (idb=='1') {
              // function exit
              //unsigned long long offset = strtoul(link_line, NULL, 16);
              //printf("function exit point: %p\n",offset);
            }
          }
        }
      }
      free(link_line);
    }

    int mp_res = mprotect(jit_binary, codesz, PROT_EXEC|PROT_READ);
    
    if (!mp_res) {
      *res = execute_jitted(jit_binary);
    } else {
      printf("<mprotect result: %d\n>",mp_res);
      *res = NULL;
      success = 0;
    }
  }
  return !!success;
}
