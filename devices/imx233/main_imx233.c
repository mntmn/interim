#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "devices/imx233/imx233.h"
//#include "sledge/machine.h"
#include "sledge/minilisp.h"
#include "sledge/alloc.h"
#include "sledge/compiler_new.h"
#include "os/debug_util.h"
//#include "sledge/blit.h"

#define COLOR_TYPE uint8_t
#define SCREEN_W 640
#define SCREEN_H 480
#define SCREEN_BPP 1

void main();

extern uint32_t _bss_end;
uint8_t* heap_end;

void _cstartup(unsigned int r0, unsigned int r1, unsigned int r2)
{
  int i;
  for (i=0;i<2000;i++) {
    while (*((volatile uint32_t*)(0x80070018)) & 32) {}; // UART_PL01x_FR_TXFF
    *((volatile uint32_t*)0x80070000) = '@';
  }
  
  uart_puts("[interim] _cstartup\n");
  heap_end = (uint8_t*)0x42300000; // 0x300000 reserved for kernel binary + stack
  //memset(heap_end,0,1024*1024*16); // clear 16 MB of memory
  main();
  while(1) {};
}

// GPIO Register set
volatile unsigned int* gpio;

COLOR_TYPE* FB;
COLOR_TYPE* FB_MEM;

char buf[128];

void enable_mmu(void);
extern void* _get_stack_pointer();
void uart_repl();

//extern void libfs_init();
//extern void uspi_keypress_handler(const char *str);

static int have_eth = 0;
uint8_t* eth_rx_buffer;

void init_mini_ip(Cell* buffer_cell);

void main()
{
  //enable_mmu();
  //arm_invalidate_data_caches();

  //uart_init(); // gpio setup also affects emmc TODO: document
  
  uart_puts("-- INTERIM/IMX233 kernel_main entered.\r\n");
  setbuf(stdout, NULL);
  
  //libfs_init();

  //init_rpi_qpu();
  //uart_puts("-- QPU enabled.\r\n");

  FB = (COLOR_TYPE*)0x40000000;
  FB_MEM = FB;

  //init_blitter(FB);
  
  sprintf(buf, "-- framebuffer at %p.\r\n",FB);
  uart_puts(buf);
  
  sprintf(buf, "-- heap starts at %p.\r\n", heap_end);
  uart_puts(buf);
  
  sprintf(buf, "-- stack pointer at %p.\r\n", _get_stack_pointer());
  uart_puts(buf);

  memset(FB, 0x88, SCREEN_W*SCREEN_H*SCREEN_BPP);
  
  //eth_rx_buffer=malloc(64*1024);
  
  uart_repl();
}

#include <os/libc_glue.c>

int machine_video_set_pixel(uint32_t x, uint32_t y, COLOR_TYPE color) {
  if (x>=SCREEN_W || y>=SCREEN_H) return 0;
  FB_MEM[y*1920+x] = color;
  
  return 0;
}

int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, COLOR_TYPE color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  uint32_t off = y1*SCREEN_W;

  // FIXME: clip!
  
  for (; y1<y2; y1++) {
    for (uint32_t x1=x; x1<x2; x1++) {
      FB_MEM[off+x1] = color;
    }
    off+=SCREEN_W;
  }

  return 0;
}

int machine_get_key(int modifiers) {
  if (modifiers) return 0;
  int k = 0;
  
  k = uart_getc();
  if (k==27) {
    k = uart_getc();
    if (k==91) {
      k = uart_getc();
      if (k==27) {
        // fast repetition
        k = uart_getc();
        k = uart_getc();
      }
      if (k==68) return 130;
      if (k==67) return 131;
      if (k==65) return 132;
      if (k==66) return 133;
      printf("~~ inkey unknown sequence: 91,%d\r\n",k);
      return 0;
    }
  }
  return k;
}

#include "sledge/compiler_new.c"

void uart_repl() {
  uart_puts("~~ trying to malloc repl buffers\r\n");
  char* out_buf = malloc(1024*10);
  char* in_line = malloc(1024*2);
  char* in_buf = malloc(1024*10);
  uart_puts("\r\n\r\n++ welcome to sledge arm/32 (c)2015 mntmn.\r\n");
  
  init_compiler();
  
  uart_puts("\r\n~~ compiler initialized.\r\n");
  
  memset(out_buf,0,1024*10);
  memset(in_line,0,1024*2);
  memset(in_buf,0,1024*10);

  long count = 0;  
  int fullscreen = 0;
  
  int in_offset = 0;
  int parens = 0;

  int linec = 0;

  Cell* expr;
  char c = 0; //13;

  //strcpy(in_line,"(eval (load \"/sd/boot.l\"))\n");
  
  while (1) {
    expr = NULL;
    
    uart_puts("sledge> ");

    int i = 0;

    while (c!=13) {
      c = uart_getc();
      uart_putc(c);
      in_line[i++] = c;
      in_line[i] = 0;
    }
    c = 0;
    
    int len = strlen(in_line);

    // recognize parens
    
    for (i=0; i<len; i++) {
      if (in_line[i] == '(') {
        parens++;
      } else if (in_line[i] == ')') {
        parens--;
      }
    }

    //printf("parens: %d in_offset: %d\n",parens,in_offset);

    if (len>1) {
      strncpy(in_buf+in_offset, in_line, len-1);
      in_buf[in_offset+len-1] = 0;
      
      //printf("line: '%s' (%d)\n",in_buf,strlen(in_buf));
      
      linec++;
      //if (linec>10) while (1) {};
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
    //printf("parens: %d offset: %d\n",parens,in_offset);
    
    //funcptr     compiled;

    if (expr) {
      
      //jit_prolog();
      
      int success = 0; //compile_arg(JIT_R0, expr, TAG_ANY);

      if (success) {
        funcptr compiled = NULL; //jit_emit();

        //jit_disassemble();

        //start_clock();

        /*uint32_t compiled[] = {
          0xe3a03005,  // mov     r3, #5
          0xe1a00003,  // mov     r0, r3
          0xe12fff1e   // bx      lr
          };*/
        
        printf("-- compiled: %p\r\n",compiled);
        memdump(compiled,200,1);

        printf("-- jumping to code…");
        //Cell* res = expr;
        Cell* res = (Cell*)((funcptr)compiled)();
        printf("-- res at: %p\r\n",res);

        // TODO: move to write op
        if (!res || res<(Cell*)heap_end) {
          uart_puts("null\n");
        } else {
          lisp_write(res, out_buf, 1024*10);
          uart_puts(out_buf);
        }
      }
      
      uart_puts("\r\n");
    }
  }
}
