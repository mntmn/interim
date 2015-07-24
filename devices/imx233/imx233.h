#define CONFIG_MX23

#include "regs-base.h"

#define HW_UARTDBGDR MXS_UARTDBG_BASE

#define UART0_DR HW_UARTDBGDR

void uart_putc(unsigned char byte);
unsigned char uart_getc();
void uart_write(const unsigned char* buffer, uint32_t size);
void uart_puts(const char* str);
void uart_init();
