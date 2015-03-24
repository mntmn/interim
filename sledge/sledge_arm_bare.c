//#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include "minilisp.h"
#include "alloc.h"

#include <lightning.h>

#include "raspi.h"

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

void main()
{
  enable_mmu();
  InvalidateDataCache();
  
  uart_puts("-- BOMBERJACKET/PI kernel_main entered.\r\n");

  FB = init_rpi_gfx();
  FB_MEM = FB; //malloc(1920*1080*4);
  
  sprintf(buf, "-- framebuffer at %p\r\n",FB);
  uart_puts(buf);
  
  sprintf(buf, "-- heap starts at %p\r\n", heap_end);
  uart_puts(buf);
  
  sprintf(buf, "-- stack pointer at %p\r\n", _get_stack_pointer());
  uart_puts(buf);

  if (FB) {
    for (int x = 0; x<1920*1080; x++) {
      FB[x] = x;
    }
  }

	/*while (1) {
    int k = uart_getc();
		uart_putc(k);
    for (int x = 0; x<1920*1080; x++) {
      FB[x] = (k<<16) | k;
    }
    }*/

  uart_repl();
}

// TODO: read https://github.com/mrvn/test/blob/master/mmu.cc
// from http://stackoverflow.com/a/5800238

#define NUM_PAGE_TABLE_ENTRIES 4096 /* 1 entry per 1MB, so this covers 4G address space */
#define CACHE_DISABLED    0x12
#define SDRAM_START       0x80000000
#define SDRAM_END         0x8fffffff
#define CACHE_WRITEBACK   0x1e

static uint32_t __attribute__((aligned(16384))) page_table[NUM_PAGE_TABLE_ENTRIES];

void enable_mmu(void)
{
  int i;
  uint32_t reg;

  /* Set up an identity-mapping for all 4GB, rw for everyone */
  for (i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++)
    page_table[i] = i << 20 | (3 << 10) | CACHE_DISABLED;
  /* Then, enable cacheable and bufferable for RAM only */
  for (i = SDRAM_START >> 20; i < SDRAM_END >> 20; i++)
  {
    page_table[i] = i << 20 | (3 << 10) | CACHE_WRITEBACK;
  }

  /* Copy the page table address to cp15 */
  __asm("mcr p15, 0, %0, c2, c0, 0"
               : : "r" (page_table) : "memory");
  /* Set the access control to all-supervisor */
  __asm("mcr p15, 0, %0, c3, c0, 0" : : "r" (~0));

  /* Enable the MMU */
  __asm("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
  reg|=0x1;
  
  __asm("mcr p15, 0, %0, c1, c0, 0" : : "r" (reg) : "cc");
}

void _exit(int status) {
  uart_puts("-- clib exit called. hanging.\r\n");

  memcpy(FB, (uint32_t*)0x200000, 1920*1080*4);

  //uint32_t fp;
  //READ_REGISTER(fp, fp);
  //sprintf(buf, "-- FP: %x\n",fp);
  
	while (1)
		uart_putc(uart_getc());
}

void printhex(uint32_t num) {
  char buf[9];
  buf[8] = 0;
  for (int i=7; i>=0; i--) {
    int d = num&0xf;
    if (d<10) buf[i]='0'+d;
    else buf[i]='a'+d-10;
    num=num>>4;
  }
  uart_puts(buf);
}

void printhex_signed(int32_t num) {
  char buf[9];
  buf[8] = 0;
  if (num<0) {
    uart_putc('-');
    num=-num;
  }
  for (int i=7; i>=0; i--) {
    int d = num&0xf;
    if (d<10) buf[i]='0'+d;
    else buf[i]='a'+d-10;
    num=num/16;
  }
  uart_puts(buf);
}

void* _sbrk(int incr)
{
  uart_puts("-- sbrk: ");
  printhex((uint32_t)heap_end);
  uart_puts(" ");
  printhex_signed(incr);
  uart_puts("\r\n");
  
  uint8_t* prev_heap_end;

  prev_heap_end = heap_end;

  heap_end += incr;
  return (void*)prev_heap_end;
}

void _kill() {
  uart_puts("-- clib kill called. not implemented.\r\n");
}

int _getpid() {
  uart_puts("-- clib getpid_r called. stubbed.\r\n");
  return 1;
}

int _isatty_r() {
  uart_puts("-- clib isatty_r called. stubbed.\r\n");
  return 1;
}

int _close() {
  uart_puts("-- clib close called. stubbed.\r\n");
  return 1;
}

int _fstat() {
  //uart_puts("-- clib fstat called. stubbed.\n");
  return 0;
}

int _fseek() {
  //uart_puts("-- clib fseek called. stubbed.\n");
  return 0;
}

int _lseek() {
  //uart_puts("-- clib lseek called. stubbed.\n");
  return 0;
}

int _read() {
  //uart_puts("-- clib read called. stubbed.\n");
  return 0;
}

size_t _write(int fildes, const void *buf, size_t nbytes) {
  //uart_puts("-- clib write called:\n");
  for (int i=0; i<nbytes; i++) {
    uart_putc(((char*)buf)[i]);
  }
  return nbytes;
}

int _fini() {
  uart_puts("-- clib _fini called. stubbed.\n");
  return 0;
}

int machine_video_set_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (x>=1920 || y>=1080) return 0;
  FB_MEM[y*1920+x] = color;
  
  return 1;
}

int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  uint32_t off = y1*1920;
  
  for (; y1<y2; y1++) {
    for (uint32_t x1=x; x1<x2; x1++) {
      FB_MEM[off+x1] = color;
    }
    off+=1920;
  }
}

int machine_video_flip() {
  memset(FB_MEM, 0xffffff, 1920*1080*4);
  return 1;
}

int machine_get_key(int modifiers) {
  if (modifiers) return 0;
  return uart_getc();
}

Cell* machine_poll_udp() {
  return NULL;
}

Cell* machine_send_udp(Cell* data_cell) {
  return NULL;
}

Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell) {
  return NULL;
}

Cell* machine_bind_tcp(Cell* port_cell, Cell* fn_cell) {
  return NULL;
}

Cell* machine_send_tcp(Cell* data_cell) {
  return NULL;
}

Cell* machine_save_file(Cell* cell, char* path) {
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

  Cell* result_cell = alloc_num_bytes(2*1024*1024);
  return result_cell;
}

typedef jit_word_t (*funcptr)();
static jit_state_t *_jit;
static jit_state_t *_jit_saved;
static void *stack_ptr, *stack_base;

#include "compiler.c"

void uart_repl() {
  char* out_buf = malloc(1024*10);
  char* in_line = malloc(1024*2);
  char* in_buf = malloc(1024*10);
  uart_puts("\r\n\r\nwelcome to sledge arm/32 (c)2015 mntmn.\r\n");
  
  init_compiler();

  uart_puts("\r\n\r\ncompiler initialized.\r\n");
  
  memset(out_buf,0,1024*10);
  memset(in_line,0,1024*2);
  memset(in_buf,0,1024*10);

  stack_ptr = stack_base = malloc(4096 * sizeof(jit_word_t));

  long count = 0;  
  int fullscreen = 0;
  
  int in_offset = 0;
  int parens = 0;

  int linec = 0;

  Cell* expr;
  char c = 0;

  //strcpy(in_line,"(eval editor-source)\n");
  
  init_jit(NULL);

  uart_puts("\r\n\r\nJIT initialized.\r\n");
    
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
    
    jit_node_t  *in;
    funcptr     compiled;

    if (expr) {
      _jit = jit_new_state();
      
      jit_prolog();
      
      int success = compile_arg(JIT_R0, expr, TAG_ANY);

      jit_retr(JIT_R0);

      if (success) {
        compiled = jit_emit();

        //jit_disassemble();

        //start_clock();

        printf("-- compiled: %p\r\n",compiled);
        memdump((uint32_t)compiled,200,1);

        Cell* res = (Cell*)compiled();
        printf("-- res at: %p\r\n",res);

        // TODO: move to write op
        if (!res) {
          uart_puts("null\n");
        } else {
          lisp_write(res, out_buf, 1024*10);
          uart_puts(out_buf);
        }
      }
      
      uart_puts("\r\n");
      
      jit_clear_state();
      jit_destroy_state();
    }
  }
}
