#include <stdint.h>
#include <stdio.h>

void uart_putc(unsigned char byte);
void uart_puts(const char* str);

void memdump(void* start,uint32_t len,int raw) {
  for (uint32_t i=0; i<len;) {
    if (!raw) printf("%08x | ",start+i);
    for (uint32_t x=0; x<16; x++) {
      printf("%02x ",*((uint8_t*)start+i+x));
    }
    if (!raw)
      for (uint32_t x=0; x<16; x++) {
        uint8_t c = *((uint8_t*)start+i+x);
        if (c>=32 && c<=128) {
          printf("%c",c);
        } else {
          printf(".");
        }
      }
    printf("\r\n");
    i+=16;
  }
  printf("\r\n\r\n");
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
