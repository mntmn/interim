#include <stdint.h>
#include <string.h>
#include "imx233.h"

// /code/u-boot/arch/arm/include/asm/arch-mxs

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
	//while (mmio_read(UART0_FR) & (1 << 5)) { }
  while (*((volatile uint32_t*)0x80070018) & 32) {}; // check flag register for TXFF
	*((volatile uint32_t *)UART0_DR) = byte;
}

unsigned char uart_getc()
{
  // Wait for UART to have received something.
  while (*((volatile uint32_t*)0x80070018) & 16) {};
  //while (mmio_read(UART0_FR) & (1 << 4)) { }
  return *((volatile uint32_t *)UART0_DR);
    //return 0;
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

void uart_init() {
}
