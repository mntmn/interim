#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "reader.h"
#include "compiler_new.h"
#include "os/debug_util.h"

#include "devices/rpi2/raspi.h"
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

void main()
{
  enable_mmu();
  arm_invalidate_data_caches();

  uart_init(); // gpio setup also affects emmc TODO: document
  
  uart_puts("-- INTERIM/PI kernel_main entered.\r\n");
  setbuf(stdout, NULL);
  
  //libfs_init();

  init_rpi_qpu();
  uart_puts("-- QPU enabled.\r\n");

  FB = init_rpi_gfx();
  FB_MEM = FB;

  //init_blitter(FB);
  
  sprintf(buf, "-- framebuffer at %p.\r\n",FB);
  uart_puts(buf);
  
  sprintf(buf, "-- heap starts at %p.\r\n", heap_end);
  uart_puts(buf);
  
  sprintf(buf, "-- stack pointer at %p.\r\n", _get_stack_pointer());
  uart_puts(buf);

  memset(FB, 0x88, 1920*1080*4);
  
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

  eth_rx_buffer=malloc(64*1024);
  
  memset(FB, 0x44, 1920*1080*4);
  
  uart_repl();
}

static struct fs* fat_fs;

void vfs_register(struct fs *fs) {
  printf("~~ vfs_register: %s/%s block_size: %d\r\n",fs->parent->device_name,fs->fs_name,fs->block_size);
  printf("~~ read_directory: %p fopen: %p\r\n",fs->read_directory,fs->fopen);

  //char* name = "/";

  //struct dirent* dir = fs->read_directory(fs,&name);

  //printf("~~ dirent: %p name: %s\r\n",dir,dir->name);
  fat_fs = fs;
}

#include <os/libc_glue.c>

/*
int machine_video_set_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (x>=1920 || y>=1080) return 0;
  FB_MEM[y*1920+x] = color;
  
  return 0;
}

int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  uint32_t off = y1*1920;

  // FIXME: clip!
  
  for (; y1<y2; y1++) {
    for (uint32_t x1=x; x1<x2; x1++) {
      FB_MEM[off+x1] = color;
    }
    off+=1920;
  }

  return 0;
}
*/

//Cell* lookup_global_symbol(char* name);

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

static char usb_key_in = 0;
static int usb_keyboard_enabled = 0;

void uspi_keypress_handler (const char *str)
{
  printf("[uspi-keyboard] pressed: '%s' (%d)\r\n",str,str[0]);
  usb_key_in = str[0];
  usb_keyboard_enabled = 1;
}

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

/*
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
  }*/

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

  if (!strncmp(path,"/sd/",4) && fat_fs) {
    char* name = NULL;
    struct dirent* dir = fat_fs->read_directory(fat_fs,&name);

    char* filename = NULL;
    if (strlen(path)>4) {
      filename = path+4;
    }
    
    printf("~~ dirent: %p name: %s\r\n",dir,dir->name);

    if (filename) {
      // look for the file
      printf("FAT looking for %s...\r\n",filename);
      while (dir) {
        if (!strcmp(filename, dir->name)) {
          // found it
          printf("FAT found file. opening...\r\n");
          FILE* f = fat_fs->fopen(fat_fs, dir, "r");
          if (f) {
            printf("FAT trying to read file of len %d...\r\n",f->len);
            Cell* res = alloc_num_string(f->len);
            int len = fat_fs->fread(fat_fs, res->addr, f->len, f);
            printf("FAT bytes read: %d\r\n",len);
            // TODO: close?
            return res;
          } else {
            // TODO should return error
            printf("FAT could not open file :(\r\n");
            return alloc_string_copy("<error: couldn't open file.>");
          }
        }
        dir = dir->next;
      }
      return alloc_string_copy("<error: file not found.>");
    } else {
      Cell* res = alloc_num_string(4096);
      char* ptr = (char*)res->addr;
      while (dir) {
        int len = sprintf(ptr,"%s",dir->name);
        ptr[len] = '\n';
        ptr+=len+1;
        dir = dir->next;
      }
      return res;
    }
  }

  Cell* result_cell = alloc_int(0);
  return result_cell;
}

typedef jit_word_t (*funcptr)();

#include "compiler_new.c"

/*void insert_rootfs_symbols() {
  // until we have a file system, inject binaries that are compiled in the kernel
  // into the environment
  
  extern uint8_t _binary_bjos_rootfs_unifont_start;
  extern uint32_t _binary_bjos_rootfs_unifont_size;
  Cell* unif = alloc_bytes(16);
  unif->addr = &_binary_bjos_rootfs_unifont_start;
  unif->size = _binary_bjos_rootfs_unifont_size;

  printf("~~ unifont is at %p\r\n",unif->addr);
  
  extern uint8_t* blitter_speedtest(uint8_t* font);
  unif->addr = blitter_speedtest(unif->addr);

  insert_symbol(alloc_sym("unifont"), unif, &global_env);

  extern uint8_t _binary_bjos_rootfs_editor_l_start;
  extern uint32_t _binary_bjos_rootfs_editor_l_size;
  Cell* editor = alloc_string("boot");
  editor->addr = &_binary_bjos_rootfs_editor_l_start;
  editor->size = read_word((uint8_t*)&_binary_bjos_rootfs_editor_l_size,0); //_binary_bjos_rootfs_editor_l_size;

  printf("~~ editor-source is at %p, size %d\r\n",editor->addr,editor->size);
  
  insert_symbol(alloc_sym("editor-source"), editor, &global_env);

  //Cell* boot = alloc_string("(eval (load \"/sd/boot.l\"))");
  //insert_symbol(alloc_sym("boot-source"), boot, &global_env);
  
  insert_symbol(alloc_sym("tx1"), alloc_int(1700), &global_env);
  insert_symbol(alloc_sym("tx2"), alloc_int(1732), &global_env);
  insert_symbol(alloc_sym("ty1"), alloc_int(32), &global_env);
  insert_symbol(alloc_sym("ty2"), alloc_int(64), &global_env);

  Cell* udp_cell = alloc_num_bytes(65535);
  insert_symbol(alloc_sym("network-input"), udp_cell, &global_env);

  init_mini_ip(udp_cell);
  }*/

void uart_repl() {
  uart_puts("~~ trying to malloc repl buffers\r\n");
  char* out_buf = malloc(1024*10);
  char* in_line = malloc(1024*2);
  char* in_buf = malloc(1024*10);
  uart_puts("\r\n\r\n++ welcome to sledge arm/32 (c)2015 mntmn.\r\n");
  
  init_compiler();
  //insert_rootfs_symbols();

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
  char c = 13;

  strcpy(in_line,"(eval (load \"/sd/boot.l\"))\n");
  
  r3d_init(FB);
  uart_puts("-- R3D initialized.\r\n");
  
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
    
    funcptr compiled;

    if (expr) {
      int success = 0;
      Cell* res = NULL;

      if (success) {
        if (!res) {
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
