#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include "minilisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h> // mprotect

#include "sdl.h"

#define MAX_INT 4294967296
//#define DEBUG 0

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef jit_word_t (*funcptr)();
//static jit_state_t *_jit;
static void *stack_ptr, *stack_base;

//#include "compiler.c"

#include "compiler_new.c"

int machine_video_set_pixel(uint32_t x, uint32_t y, COLOR_TYPE color) {
  //sdl_setpixel(x,y,color);
  return 1;
}

int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, COLOR_TYPE color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  
  for (; y1<y2; y1++) {
    uint32_t x1=x;
    for (; x1<x2; x1++) {
      //sdl_setpixel(x1,y1,color);
    }
  }
}

inline int machine_video_flip() {
  //sdl_mainloop();
  machine_video_rect(0,0,1920,1080,0xff);
  return 1;
}

int machine_get_key(int modifiers) {
  //if (modifiers) return sdl_get_modifiers();
  //int k = sdl_get_key();
  int k = 0;
  //if (k) printf("k: %d\n",k);
  if (k==43) k=134;
  if (k==80) k=130;
  if (k==79) k=131;
  if (k==82) k=132;
  if (k==81) k=133;
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
  sprintf(buf,"../rootfs/%s",path);
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


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// TODO: create socket handle and return it

static int tcp_outbound_sockfd = 0;

Cell* my_tcp_connected_callback;
Cell* my_tcp_data_callback;
Cell* network_cell;

Cell* machine_poll_udp() {
  if (my_tcp_data_callback) {
    int len = read(tcp_outbound_sockfd,network_cell->addr,1024*64);
    if (len>0) {
      printf("-- received tcp packet of len: %d\n",len);
      ((uint8_t*)network_cell->addr)[len] = 0;
      network_cell->size = len;
      funcptr fn = (funcptr)my_tcp_data_callback->next;
      fn();
      
      return NULL;
    }
  }
  return NULL;
}

Cell* machine_send_udp(Cell* data_cell) {
  return data_cell;
}


Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell) {
  my_tcp_connected_callback = connected_fn_cell;
  my_tcp_data_callback = data_fn_cell;

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  
  bcopy((char *)host_cell->addr,
        (char *)&serv_addr.sin_addr.s_addr,
        4); // ipv4 only
  
  tcp_outbound_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  struct timeval tv;
  tv.tv_sec = 1;  /* 1 Sec Timeout */
  tv.tv_usec = 0;  // Not init'ing this can cause strange errors

  setsockopt(tcp_outbound_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
  
  serv_addr.sin_port = htons(port_cell->value);

  printf("[machine_connect_tcp] trying to connect to %x:%d\r\n",serv_addr.sin_addr.s_addr,port_cell->value);
  if (connect(tcp_outbound_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    printf("[machine_connect_tcp] couldn't connect.\r\n");
  } else {
    // call connected callback
    printf("[machine_connect_tcp] connected. calling %p…\r\n",my_tcp_connected_callback->next);
    funcptr fn = (funcptr)my_tcp_connected_callback->next;
    fn();
  }
  
  return port_cell;
}

Cell* machine_send_tcp(Cell* data_cell) {
  write(tcp_outbound_sockfd,data_cell->addr,data_cell->size);
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

  uint32_t* FB = 0; //(uint32_t*)sdl_init(fullscreen);
  //init_blitter(FB);
  
  Cell* unif = machine_load_file("unifont");
  printf("~~ unifont is at %p\r\n",unif->addr);
  
  extern uint8_t* blitter_speedtest(uint8_t* font);
  unif->addr = blitter_speedtest(unif->addr);
  insert_symbol(alloc_sym("unifont"), unif, &global_env);

  network_cell = alloc_num_bytes(1024*64+1);
  insert_symbol(alloc_sym("network-input"), network_cell, &global_env);
  
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
    
    //jit_node_t  *in;
    funcptr     compiled;
    
    if (expr) {
      if (!jit_inited) { 
        //init_jit(argv[0]);
        jit_inited = 1;
      }

      //stack_ptr = stack_base;
        
      //_jit = jit_new_state();
      //jit_prolog();
      
      Cell* res;
      //int success = compile_arg(JIT_R0, expr, TAG_ANY);
      int success = 1;
      
      jit_out = fopen("/tmp/jit_out.s","w");
  
      int tag = compile_expr(expr, NULL);
      jit_ret();

      if (success) {
        //compiled = jit_emit();
      
#ifdef DEBUG
        //jit_disassemble();
        start_clock();
#endif
        fclose(jit_out);

        FILE* asm_f = fopen("/tmp/jit_out.s","r");
        uint32_t* jit_asm = malloc(4096);
        memset(jit_asm, 0, 4096);
        fread(jit_asm,1,4096,asm_f);
        fclose(asm_f);
        printf("\nJIT ---------------------\n%s-------------------------\n\n",jit_asm);

        // prefix with arm-none-eabi- on ARM  -mlittle-endian
      
        system("as /tmp/jit_out.s -o /tmp/jit_out.o");
        system("objcopy /tmp/jit_out.o -O binary /tmp/jit_out.bin");

        FILE* binary_f = fopen("/tmp/jit_out.bin","r");
        //uint32_t* jit_binary = malloc(4096);

        uint32_t* jit_binary = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
        
        int bytes_read = fread(jit_binary,1,4096,binary_f);
        fclose(binary_f);

        printf("<assembled bytes: %d at: %p>\n",bytes_read,jit_binary);

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
                  printf("function %p entrypoint: %p (+%ld)\n",lambda,binary,offset);

                  if (lambda->tag == TAG_LAMBDA) {
                    lambda->next = binary;
                  } else {
                    printf("fatal error: no lambda found at %p!\n",lambda);
                  }
                }
                else if (idb=='1') {
                  // function exit
                  unsigned long long offset = strtoul(link_line, NULL, 16);
                  printf("function exit point: %p\n",offset);
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

      //collect_garbage(global_env);
      
      //jit_clear_state();
      //jit_destroy_state();
    } else {
    }

    //sdl_mainloop();
  }
  //finish_jit();
  return 0;
}
