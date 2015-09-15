#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void* heap_end;
 
/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
 
uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c) {
  if (c==10) {
    terminal_column=0;
    terminal_row++;
    if (terminal_row == VGA_HEIGHT) {
      terminal_initialize();
    }
    return;
  }
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
		}
	}
}

void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}

int bios_getc(void)
{
  uint16_t key_to_rune[] = {
    0,
    0,
    '1','2','3','4','5','6','7','8','9','0','/','"',9,
    '\t','q','w','e','r','t','z','u','i','o','p','-','+',10,
    0,'a','s','d','f','g','h','j','k','l','(',')',0,0,'*',
    'y','x','c','v','b','n','m',',','.','-',0,
    0,0,' ',
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0
  };

	int key;

  __asm("WaitLoop:   in     $0x64, %al");
  __asm(            "and    $1, %al");
  __asm(            "jz     WaitLoop");
  __asm(            "in     $0x60, %al");
  
  __asm("mov %%al,%0" : "=m" (key));
  if (key>0 && key<0x70) {
    return(key_to_rune[key]);
  } else {
    return 0;
  }
}

void uart_puts(char* str) {
  terminal_writestring(str);
}

void uart_putc(char c) {
  terminal_putchar(c);
}

char uart_getc() {
  return bios_getc();
}


int open() {
  uart_puts("-- clib open called. stubbed.\r\n");
  return 1;
}
int close() {
  uart_puts("-- clib close called. stubbed.\r\n");
  return 1;
}
int fstat() {
  //uart_puts("-- clib fstat called. stubbed.\n");
  return 0;
}
int lseek() {
  //uart_puts("-- clib lseek called. stubbed.\n");
  return 0;
}

ssize_t read(int fildes, void *buf, size_t nbyte) {
  //uart_puts("-- clib read called. stubbed.\n");
  printf("\r\n");
  int i = 0;
  int k = 0;
  while (k!=10 && i<nbyte) {
    k = uart_getc();
    if (k>0) {
      uart_putc(k);
      ((char*)buf)[i] = k;
      i++;
    }
  }
  return i;
}
size_t write(int fildes, const void *buf, size_t nbytes) {
  uart_puts("-- clib write called:\n");
  for (int i=0; i<nbytes; i++) {
    uart_putc(((char*)buf)[i]);
  }
  return nbytes;
}


#define GETLINE_MAX 256
static char getline_buf[GETLINE_MAX];

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
  printf("\r\n");
  int i = 0;
  int k = 0;
  while (k!=10 && i<GETLINE_MAX) {
    k = uart_getc();
    if (k>0) {
      uart_putc(k);
      getline_buf[i] = k;
      i++;
    }
  }
  *n = i;
  *lineptr = getline_buf;
  
  return i;
}

#include "debug_util.c"
#include "libc_glue.c"

int main(int argc, char *argv[]);

void kernel_main() {
  terminal_buffer = (uint16_t*)0xB8000;
  for (int i=0; i<80*23; i++) {
    terminal_buffer[i]='*'|(COLOR_WHITE << 8);
  }
  
	/* Initialize terminal interface */
	terminal_initialize();

  heap_end = (void*)0x200000;

  main(0,NULL);
}

/*extern _IO_ssize_t getline(char **__restrict __lineptr, size_t *__restrict __n, FILE *__restrict __stream) {
  return 0;
}
*/
