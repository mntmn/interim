#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "imx233/imx233.h"
#include "sledge/machine.h"
#include "sledge/minilisp.h"
#include "sledge/alloc.h"
#include "sledge/blit.h"

#include <lightning.h>

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
  
  uart_puts("-- BOMBERJACKET/IMX233 kernel_main entered.\r\n");
  setbuf(stdout, NULL);
  
  //libfs_init();

  //init_rpi_qpu();
  //uart_puts("-- QPU enabled.\r\n");

  FB = (COLOR_TYPE*)0x40000000;
  FB_MEM = FB;

  init_blitter(FB);
  
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

//static struct fs* fat_fs;

/*void vfs_register(struct fs *fs) {
  printf("~~ vfs_register: %s/%s block_size: %d\r\n",fs->parent->device_name,fs->fs_name,fs->block_size);
  printf("~~ read_directory: %p fopen: %p\r\n",fs->read_directory,fs->fopen);

  //char* name = "/";

  //struct dirent* dir = fs->read_directory(fs,&name);

  //printf("~~ dirent: %p name: %s\r\n",dir,dir->name);
  fat_fs = fs;
  }*/

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

#include "libc_glue.c"

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

Cell* lookup_global_symbol(char* name);

void memdump(jit_word_t start,uint32_t len,int raw);

int ethernet_rx(uint8_t* packet) {
  int frame_len = 0;
  if (have_eth) {
    //USPiReceiveFrame(packet, &frame_len);

    if (frame_len) {
      printf("[eth] frame received! len: %d\r\n",frame_len);   
      memdump((uint32_t)packet,frame_len,0);
    }
  }
  return frame_len;
}

void ethernet_tx(uint8_t* packet, int len) {
  //USPiSendFrame(packet, len);
  printf("[eth] frame sent (%d)\r\n",len);
}

int machine_video_flip() {
  //memset(FB_MEM, 0xffffff, 1920*1080*4);
  return 0;
}

static char usb_key_in = 0;
static int usb_keyboard_enabled = 0;

int machine_get_key(int modifiers) {
  if (modifiers) return 0;
  int k = 0;
  if (usb_keyboard_enabled) {
    k = usb_key_in;
    usb_key_in = 0;
  } else {
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
  }
  return k;
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

  Cell* result_cell = alloc_int(0);
  return result_cell;
}

typedef jit_word_t (*funcptr)();
static jit_state_t *_jit;
static jit_state_t *_jit_saved;
static void *stack_ptr, *stack_base;

#include "sledge/compiler.c"

void insert_rootfs_symbols() {
  // until we have a file system, inject binaries that are compiled in the kernel
  // into the environment
  
  /*extern uint8_t _binary_bjos_rootfs_unifont_start;
  extern uint32_t _binary_bjos_rootfs_unifont_size;
  Cell* unif = alloc_bytes(16);
  unif->addr = &_binary_bjos_rootfs_unifont_start;
  unif->size = _binary_bjos_rootfs_unifont_size;

  printf("~~ unifont is at %p\r\n",unif->addr);
  
  extern uint8_t* blitter_speedtest(uint8_t* font);
  unif->addr = blitter_speedtest(unif->addr);

  insert_symbol(alloc_sym("unifont"), unif, &global_env);
  */
  
  /*extern uint8_t _binary_bjos_rootfs_editor_l_start;
  extern uint32_t _binary_bjos_rootfs_editor_l_size;
  Cell* editor = alloc_string("boot");
  editor->addr = &_binary_bjos_rootfs_editor_l_start;
  editor->size = read_word((uint8_t*)&_binary_bjos_rootfs_editor_l_size,0); //_binary_bjos_rootfs_editor_l_size;

  printf("~~ editor-source is at %p, size %d\r\n",editor->addr,editor->size);
  
  insert_symbol(alloc_sym("editor-source"), editor, &global_env);*/

  //Cell* boot = alloc_string("(eval (load \"/sd/boot.l\"))");
  //insert_symbol(alloc_sym("boot-source"), boot, &global_env);
  
  //Cell* udp_cell = alloc_num_bytes(65535);
  //insert_symbol(alloc_sym("network-input"), udp_cell, &global_env);

  //init_mini_ip(udp_cell);
}

void uart_repl() {
  uart_puts("~~ trying to malloc repl buffers\r\n");
  char* out_buf = malloc(1024*10);
  char* in_line = malloc(1024*2);
  char* in_buf = malloc(1024*10);
  uart_puts("\r\n\r\n++ welcome to sledge arm/32 (c)2015 mntmn.\r\n");
  
  init_compiler();
  insert_rootfs_symbols();

  uart_puts("\r\n~~ compiler initialized.\r\n");
  
  memset(out_buf,0,1024*10);
  memset(in_line,0,1024*2);
  memset(in_buf,0,1024*10);

  // jit stack
  stack_ptr = stack_base = malloc(4096 * sizeof(jit_word_t));

  long count = 0;  
  int fullscreen = 0;
  
  int in_offset = 0;
  int parens = 0;

  int linec = 0;

  Cell* expr;
  char c = 13;

  //strcpy(in_line,"(eval (load \"/sd/boot.l\"))\n");
  
  init_jit(NULL);
  uart_puts("\r\n\r\n~~ JIT initialized.\r\n");

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
    //funcptr     compiled;

    if (expr) {
      _jit = jit_new_state();
      
      //jit_prolog();
      
      int success = compile_arg(JIT_R0, expr, TAG_ANY);

      //jit_movi(JIT_R0, 0);
      
      jit_retr(JIT_R0);

      //int success = 1;

      if (success) {
        funcptr compiled = jit_emit();

        //jit_disassemble();

        //start_clock();

        /*uint32_t compiled[] = {
          0xe3a03005,  // mov     r3, #5
          0xe1a00003,  // mov     r0, r3
          0xe12fff1e   // bx      lr
          };*/
        
        printf("-- compiled: %p\r\n",compiled);
        memdump((uint32_t)compiled,200,1);

        printf("-- jumping to code…");
        //Cell* res = expr;
        Cell* res = (Cell*)((funcptr)compiled)();
        printf("-- res at: %p\r\n",res);

        // TODO: move to write op
        if (!res || res<heap_end) {
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
