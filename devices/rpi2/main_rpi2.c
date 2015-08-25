#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "reader.h"
#include "compiler_new.h"
#include "os/debug_util.h"

#include "devices/rpi2/raspi.h"
#include "devices/rpi2/mmu.h"
#include "devices/rpi2/r3d.h"
#include "devices/rpi2/rpi-boot/vfs.h"
#include "devices/rpi2/rpi-boot/util.h"
#include "devices/rpi2/uspi/include/uspi.h"

void main();

extern uint32_t _bss_end;
uint8_t* heap_end;

void _cstartup(unsigned int r0, unsigned int r1, unsigned int r2)
{
  heap_end = (uint8_t*)0x1000000; // start allocating at 16MB
  memset(heap_end,0,1024*1024*16); // clear 16 MB of memory
  main();
  while(1) {};
}

// GPIO Register set
volatile unsigned int* gpio;

uint32_t* FB;
uint32_t* FB_MEM;

char buf[128];

void enable_mmu(void);
extern void* _get_stack_pointer();
void uart_repl();

extern void libfs_init();
extern void uspi_keypress_handler(const char *str);

static int have_eth = 0;
uint8_t* eth_rx_buffer;

void init_mini_ip(Cell* buffer_cell);

/*
multicore:
> You only need to write a physical ARM address to:
> 0x4000008C + 0x10 * core // core := 1..3
*/

void main()
{
  //arm_invalidate_data_caches();

  uart_init(); // gpio setup also affects emmc TODO: document
  
  uart_puts("-- INTERIM/PI kernel_main entered.\r\n");
  setbuf(stdout, NULL);
  
  printf("-- init page table… --\r\n");
  init_page_table();
  
  printf("-- clear caches… -- \r\n");
  arm_invalidate_data_caches();
  arm_clear_data_caches();
  arm_dsb();
  
  printf("-- enable MMU… --\r\n");
  mmu_init();
  printf("-- MMU enabled. --\r\n");

  printf("-- clear caches… -- \r\n");
  arm_invalidate_data_caches();
  arm_clear_data_caches();
  arm_dsb();

  //printf("-- enable QPU… -- \r\n");
  //init_rpi_qpu();
  //uart_puts("-- QPU enabled.\r\n");
  
  FB = init_rpi_gfx();
  FB_MEM = FB;

  sprintf(buf, "-- framebuffer at %p.\r\n",FB);
  uart_puts(buf);

  sprintf(buf, "-- heap starts at %p.\r\n", heap_end);
  uart_puts(buf);
  
  sprintf(buf, "-- stack pointer at %p.\r\n", _get_stack_pointer());
  uart_puts(buf);

  memset(FB,0xff,1920*1080*2);
  
  printf("malloc4096 test 0: %p\r\n",malloc(4096));
  
  // uspi glue
  printf("uu uspi glue init…\r\n");
  extern void uspi_glue_init();
  uspi_glue_init();

  printf("uu USPiInitialize…\r\n");
  int res = USPiInitialize();
  printf("uu USPI initialization: %d\r\n", res);
  
  int have_kbd = USPiKeyboardAvailable();
  printf("uu USPI has keyboard: %d\r\n", have_kbd);
  if (have_kbd) {
    USPiKeyboardRegisterKeyPressedHandler(uspi_keypress_handler);
  }
  
  have_eth = USPiEthernetAvailable();
  printf("uu USPI has ethernet: %d\r\n", have_eth);

  //eth_rx_buffer=malloc(64*1024);*/
  
  libfs_init();
  printf("malloc4096 test 4: %p\r\n",malloc(4096));
  
  uart_repl();
}

#include "devices/fbfs.c"
#include "devices/rpi2/fatfs.c"
//#include "devices/rpi2/usbkeys.c"
#include "devices/rpi2/uartkeys.c"

#include <os/libc_glue.c>

int ethernet_rx(uint8_t* packet) {
  int frame_len = 0;
  if (have_eth) {
    USPiReceiveFrame(packet, &frame_len);

    if (frame_len) {
      printf("[eth] frame received! len: %d\r\n",frame_len);   
      memdump(packet,frame_len,0);
    }
  }
  return frame_len;
}

void ethernet_tx(uint8_t* packet, int len) {
  USPiSendFrame(packet, len);
  printf("[eth] frame sent (%d)\r\n",len);
}

/*
int machine_video_flip() {
  nv_vertex_t* triangles = r3d_init_frame();

  Cell* c_x1 = lookup_global_symbol("tx1");
  Cell* c_x2 = lookup_global_symbol("tx2");
  Cell* c_y1 = lookup_global_symbol("ty1");
  Cell* c_y2 = lookup_global_symbol("ty2");
  
  int x1 = c_x1->value*16;
  int y1 = c_y1->value*16;
  int x2 = c_x2->value*16;
  int y2 = c_y2->value*16;

  triangles[0].x = x1;
  triangles[0].y = y1;
  triangles[0].z = 1.0f;
  triangles[0].w = 1.0f;
  triangles[0].r = 1.0f;
  triangles[0].g = 0.0f;
  triangles[0].b = 1.0f;
  
  triangles[1].x = x2;
  triangles[1].y = y1;
  triangles[1].z = 1.0f;
  triangles[1].w = 1.0f;
  triangles[1].r = 1.0f;
  triangles[1].g = 1.0f;
  triangles[1].b = 1.0f;
  
  triangles[2].x = x1;
  triangles[2].y = y2;
  triangles[2].z = 1.0f;
  triangles[2].w = 1.0f;
  triangles[2].r = 1.0f;
  triangles[2].g = 1.0f;
  triangles[2].b = 1.0f;
  
  triangles[3].x = x2;
  triangles[3].y = y1;
  triangles[3].z = 1.0f;
  triangles[3].w = 1.0f;
  triangles[3].r = 1.0f;
  triangles[3].g = 1.0f;
  triangles[3].b = 1.0f;
  
  triangles[4].x = x2;
  triangles[4].y = y2;
  triangles[4].z = 1.0f;
  triangles[4].w = 1.0f;
  triangles[4].r = 1.0f;
  triangles[4].g = 0.0f;
  triangles[4].b = 1.0f;
  
  triangles[5].x = x1;
  triangles[5].y = y2;
  triangles[5].z = 1.0f;
  triangles[5].w = 1.0f;
  triangles[5].r = 1.0f;
  triangles[5].g = 1.0f;
  triangles[5].b = 1.0f;
  
  r3d_triangles(2, triangles);
  r3d_render_frame(0xffffffff);
  
  //memset(FB_MEM, 0xffffff, 1920*1080*4);
  //r3d_debug_gpu();
  
  return 0;
}
*/

typedef jit_word_t (*funcptr)();

Cell* platform_eval(Cell* expr); // FIXME

#include "compiler_new.c"

#define CODESZ 8192
#define REPLBUFSZ 1024*6

void fatfs_debug(); // FIXME

Cell* platform_debug() {

  for (int color=0; color<0xff; color+=5) {
    for (int y=0; y<500; y++) {
      int ofs = y*1920*2;
      for (int x=0; x<1000; x+=2) {
        ((char*)FB)[ofs+x]=color;
        ((char*)FB)[ofs+x+1]=color;
      }
    }
    printf("painted a.\r\n");
  }

  for (int j=0;j<100;j++) {
    for (int color=0; color<0xff; color+=5) {
      for (int y=0; y<500; y++) {
        memset(((char*)FB)+y*1920*2,color,1000);
      }
      printf("painted b.\r\n");
    }
  }
  
  return alloc_nil();
}

void uart_repl() {
  fatfs_debug();
  uart_puts("~~ trying to malloc repl buffers\r\n");
  sbrk(0);
  fatfs_debug();
  char* out_buf = malloc(REPLBUFSZ);
  char* in_line = malloc(REPLBUFSZ);
  char* in_buf = malloc(REPLBUFSZ);
  sbrk(0);
  uart_puts("\r\n\r\n++ welcome to sledge arm/32 (c)2015 mntmn.\r\n");
  fatfs_debug();
  
  printf("malloc4096 test 1: %p\r\n",malloc(4096));
  init_compiler();
  printf("malloc4096 test 2: %p\r\n",malloc(4096));
  fatfs_debug();
  //insert_rootfs_symbols();
  mount_fbfs(FB);
  printf("malloc4096 test 3: %p\r\n",malloc(4096));
  fatfs_debug();
  //mount_usbkeys();
  mount_uartkeys();
  mount_fatfs();
  fatfs_debug();
  
  uart_puts("\r\n~~ fs initialized.\r\n");

  printf("out_buf clear %p\r\n",out_buf);
  memset(out_buf,0,REPLBUFSZ);
  fatfs_debug();
  printf("in_line clear %p\r\n",in_line);
  memset(in_line,0,REPLBUFSZ);
  fatfs_debug();
  printf("in_buf clear %p\r\n",in_buf);
  memset(in_buf,0,REPLBUFSZ);
  fatfs_debug();
  
  long count = 0;
  
  int in_offset = 0;
  int parens = 0;

  int linec = 0;

  Cell* expr;
  char c = 0;

  strcpy(in_line,"(eval (read (recv (open \"/sd/shell.l\"))))\n");
  c=13;
  
  //r3d_init(FB);
  //uart_puts("-- R3D initialized.\r\n");
  
  while (1) {
    expr = NULL;
    
    uart_puts("sledge> ");

    int i = 0;

    while (c!=13 && i<(REPLBUFSZ-1)) {
      c = uart_getc();
      uart_putc(c);
      in_line[i++] = c;
      in_line[i] = 0;
    }
    c = 0;
    
    int len = strnlen(in_line,REPLBUFSZ);

    // recognize parens
    
    for (i=0; i<len; i++) {
      if (in_line[i] == '(') {
        parens++;
      } else if (in_line[i] == ')') {
        parens--;
      }
    }
    if (len>1) {
      strncpy(in_buf+in_offset, in_line, len-1);
      in_buf[in_offset+len-1] = 0;
      
      linec++;
    }
    printf("\r\n[%s]\r\n",in_buf);
    
    if (parens>0) {
      printf("\r\n...\r\n");
      in_offset+=len-1;
    } else {
      if (len>1) {
        expr = read_string(in_buf);
        in_offset=0;
      }
    }
    
    if (expr) {
      Cell* res = platform_eval(alloc_cons(expr, NULL));
      
      if (!res) {
        uart_puts("null\n");
      } else {
        lisp_write(res, out_buf, REPLBUFSZ);
        uart_puts(out_buf);
      }
      
      uart_puts("\r\n");
    }
  }
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
    code = malloc(CODESZ);
    memset(code, 0, CODESZ);
    jit_init(0x400);
    register void* sp asm ("sp"); // FIXME maybe unportable
    Frame empty_frame = {NULL, 0, 0, sp};
    int tag = compile_expr(c, &empty_frame, TAG_ANY);

    arm_dmb();
    arm_isb();
    arm_dsb();
    printf("compiled %d\r\n",i);
  
    if (tag) {
      jit_ret();
      funcptr fn = (funcptr)code;
      //printf("~~ fn at %p\r\n",fn);
      
      __asm("stmfd sp!, {r3-r12, lr}");
      (Cell*)fn();
      __asm("ldmfd sp!, {r3-r12, lr}");
      register Cell *retval asm ("r6");
      __asm("mov r6,r0");
      res = retval;

      //printf("~~ expr %d res: %p\r\n",i,res);
      lisp_write(res, buf, 512);
      printf("~> %s\r\n",buf);
    } else {
      lisp_write(expr, buf, 512);
      printf("[platform_eval] stopped at expression %d: '%s'\r\n",i,buf);
      break;
    }
    // when to free the code? -> when no bound lambdas involved
    
    expr = cdr(expr);
  }
  
  return res;
}
