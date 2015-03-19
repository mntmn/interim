#include "raspi.h"
#include <stdint.h>
#include <string.h>

void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t *)reg = data;
}
 
uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t *)reg;
}

void uart_putc(unsigned char byte)
{
	// Wait for UART to become ready to transmit.
	while (mmio_read(UART0_FR) & (1 << 5)) { }
	mmio_write(UART0_DR, byte);
}

unsigned char uart_getc()
{
  // Wait for UART to have received something.
  while (mmio_read(UART0_FR) & (1 << 4)) { }
  return mmio_read(UART0_DR);
}

void delay(int32_t count)
{
	__asm("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : : [count]"r"(count) : "cc");
}

void uart_write(const unsigned char* buffer, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		uart_putc(buffer[i]);
  }
}

void uart_puts(const char* str)
{
	uart_write((const unsigned char*) str, strlen(str));
}

void uart_init()
{
	// Disable UART0.
	mmio_write(UART0_CR, 0x00000000);
	// Setup the GPIO pin 14 && 15.
 
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmio_write(GPPUD, 0x00000000);
	delay(150);
 
	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
 
	// Write 0 to GPPUDCLK0 to make it take effect.
	mmio_write(GPPUDCLK0, 0x00000000);
 
	// Clear pending interrupts.
	mmio_write(UART0_ICR, 0x7FF);
 
	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// UART_CLOCK = 3000000; Baud = 115200.
 
	// Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);
 
	// Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
	// Mask all interrupts.
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	// Enable UART0, receive & transfer part of UART.
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

#define mailbox 0x2000B880

void mailbox_write(unsigned int fbinfo_addr, unsigned int channel)
{
  while(1)
  {
    //uart_puts("write loop 1\n");
    if((mmio_read(mailbox+0x18)&0x80000000)==0) break;
  }
  mmio_write(mailbox+0x20, fbinfo_addr+channel);
}

unsigned int mailbox_read(unsigned int channel)
{
  uint32_t ra;
    
  while(1)
  {
    while(1)
    {
      //uart_puts("read loop 1\n");
      ra=mmio_read(mailbox+0x18);
      if((ra&0x40000000)==0) break;
    }
    //uart_puts("read loop 2\n");
    ra=mmio_read(mailbox);
    if((ra&0xf)==channel) break;
  }
  return ra;
}

uint32_t* init_rpi_gfx()
{
  unsigned int ra,rb;
  unsigned int ry,rx;

  mmio_write(0x40040000,1920); /* #0 Physical Width */
  mmio_write(0x40040004,1080); /* #4 Physical Height */
  mmio_write(0x40040008,1920); /* #8 Virtual Width */
  mmio_write(0x4004000C,1080); /* #12 Virtual Height */
  mmio_write(0x40040010,0); /* #16 GPU - Pitch */
  mmio_write(0x40040014,32); /* #20 Bit Depth */
  mmio_write(0x40040018,0); /* #24 X */
  mmio_write(0x4004001C,0); /* #28 Y */
  mmio_write(0x40040020,0); /* #32 GPU - Pointer */
  mmio_write(0x40040024,0); /* #36 GPU - Size */

  mailbox_write(0x40040000,1);
  mailbox_read(1);
  //rb=0x40040000;
    
  /*for(ra=0;ra<10;ra++)
    {
    hexstrings(rb); hexstring(GET32(rb));
    rb+=4;
    }*/

  uint32_t* framebuffer = (uint32_t*) mmio_read(0x40040020);

  return framebuffer;
}
