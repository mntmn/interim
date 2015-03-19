#ifndef RASPI_H
#include <stdint.h>

#define GPIO_GPFSEL0    0
#define GPIO_GPFSEL1    1
#define GPIO_GPFSEL2    2
#define GPIO_GPFSEL3    3
#define GPIO_GPFSEL4    4
#define GPIO_GPFSEL5    5
 
#define GPIO_GPSET0     7
#define GPIO_GPSET1     8
 
#define GPIO_GPCLR0     10
#define GPIO_GPCLR1     11
 
#define GPIO_GPLEV0     13
#define GPIO_GPLEV1     14
 
#define GPIO_GPEDS0     16
#define GPIO_GPEDS1     17
 
#define GPIO_GPREN0     19
#define GPIO_GPREN1     20
 
#define GPIO_GPFEN0     22
#define GPIO_GPFEN1     23
 
#define GPIO_GPHEN0     25
#define GPIO_GPHEN1     26
 
#define GPIO_GPLEN0     28
#define GPIO_GPLEN1     29
 
#define GPIO_GPAREN0    31
#define GPIO_GPAREN1    32
 
#define GPIO_GPAFEN0    34
#define GPIO_GPAFEN1    35
 
#define GPIO_GPPUD      37
#define GPIO_GPPUDCLK0  38
#define GPIO_GPPUDCLK1  39
 
#define LED_GPFSEL      GPIO_GPFSEL1
#define LED_GPFBIT      18
#define LED_GPSET       GPIO_GPSET0
#define LED_GPCLR       GPIO_GPCLR0
#define LED_GPIO_BIT    16

enum
{
    // The GPIO registers base address.
    GPIO_BASE = 0x20200000,
 
    // The offsets for reach register.
 
    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),
 
    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),
 
    // The base address for UART.
    UART0_BASE = 0x20201000,
 
    // The offsets for reach register for the UART.
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

void mmio_write(uint32_t reg, uint32_t data);
uint32_t mmio_read(uint32_t reg);

void uart_putc(unsigned char byte);
void uart_write(const unsigned char* buffer, uint32_t size);
void uart_puts(const char* str);
unsigned char uart_getc();

uint32_t* init_rpi_gfx();

#endif
