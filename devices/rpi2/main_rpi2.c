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
//#include "devices/rpi2/rpi-boot/vfs.h"
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

//extern void libfs_init();
extern void uspi_keypress_handler(const char *str);

static int have_eth = 0;

//void init_mini_ip(Cell* buffer_cell);

/*
multicore:
> You need to write a physical ARM address to:
> 0x4000008C + 0x10 * core // core := 1..3
*/

void main()
{
  //arm_invalidate_data_caches();

  uart_init(); // gpio setup also affects emmc TODO: document
  
  uart_puts("-- INTERIM/PI kernel_main entered.\r\n");
  setbuf(stdout, NULL);

  arm_clear_data_caches();
  arm_invalidate_data_caches();
  
  printf("-- init page table… --\r\n");
  init_page_table();
  
  printf("-- syncing… -- \r\n");
  arm_dmb();
  arm_isb();
  arm_dsb();
  arm_clear_data_caches();
  arm_invalidate_data_caches();
  arm_dmb();
  arm_isb();
  arm_dsb();
  
  printf("-- enable MMU… --\r\n");
  mmu_init();
  printf("-- MMU enabled. --\r\n");

  arm_dmb();
  arm_isb();
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
  
  //libfs_init();
  printf("malloc4096 test 4: %p\r\n",malloc(4096));
  
  uart_repl();
}

#include "devices/fbfs.c"
#include "devices/rpi2/fatfs.c"
//#include "devices/rpi2/usbkeys.c"
#include "devices/rpi2/usbmouse.c"
#include "devices/rpi2/uartkeys.c"
#include "devices/rpi2/dev_ethernet.c"
#include "devices/rpi2/dev_sound.c"

#include <os/libc_glue.c>

typedef jit_word_t (*funcptr)();

Cell* platform_eval(Cell* expr); // FIXME

#include "compiler_new.c"

#define CODESZ 8192
#define REPLBUFSZ 1024*6

void fatfs_debug(); // FIXME

Cell* platform_debug() {
}

void uart_repl() {
  uart_puts("~~ trying to malloc repl buffers\r\n");
  char* out_buf = malloc(REPLBUFSZ);
  char* in_line = malloc(REPLBUFSZ);
  char* in_buf = malloc(REPLBUFSZ);
  sbrk(0);
  uart_puts("\r\n\r\n++ welcome to sledge arm/32 (c)2015 mntmn.\r\n");
  
  printf("malloc4096 test 1: %p\r\n",malloc(4096));
  init_compiler();
  printf("malloc4096 test 2: %p\r\n",malloc(4096));
  //insert_rootfs_symbols();
  mount_fbfs(FB);
  printf("malloc4096 test 3: %p\r\n",malloc(4096));
  //mount_usbkeys();
  mount_uartkeys();
  mount_fatfs();

  if (have_eth) {
    mount_ethernet();
  }

  if (USPiMouseAvailable()) {
    USPiMouseRegisterStatusHandler(uspi_mouse_handler);
    mount_mouse();
  }
  
  mount_soundfs();
  
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
  char eval_buf[512];

  int i = 0;
  Cell* res = alloc_nil();
  Cell* c;
  while ((c = car(expr))) {
    i++;
    arm_dmb();
    arm_isb();
    arm_dsb();

    lisp_write(c, eval_buf, 512);
    printf("<- compiling %d: %s\r\n",i,eval_buf);
    
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

      arm_dmb();
      arm_isb();
      arm_dsb();
      printf("~~ expr %d res: %p\r\n",i,res);
      lisp_write(res, eval_buf, 512);
      printf("~> %s\r\n",eval_buf);
    } else {
      lisp_write(expr, eval_buf, 512);
      printf("[platform_eval] stopped at expression %d: '%s'\r\n",i,eval_buf);
      break;
    }
    // when to free the code? -> when no bound lambdas involved
    
    expr = cdr(expr);
  }
  
  return res;
}
