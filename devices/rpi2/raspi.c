#include "raspi.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "rpi-boot/timer.h"

// from https://github.com/rsta2/uspi/blob/ebc7cbe9cbc637c9cd54b5362764581990f41d00/env/lib/synchronize.c
//
// Cache maintenance operations for ARMv7-A
//
// See: ARMv7-A Architecture Reference Manual, Section B4.2.1
//
// NOTE: The following functions should hold all variables in CPU registers. Currently this will be
// ensured using the register keyword and maximum optimization (see uspienv/synchronize.h).
//
// The following numbers can be determined (dynamically) using CTR, CSSELR, CCSIDR and CLIDR.
// As long we use the Cortex-A7 implementation in the BCM2836 these static values will work:
//
#define SETWAY_LEVEL_SHIFT	1
#define L1_DATA_CACHE_SETS	128
#define L1_DATA_CACHE_WAYS	4
#define L1_SETWAY_WAY_SHIFT	30	// 32-Log2(L1_DATA_CACHE_WAYS)
#define L1_DATA_CACHE_LINE_LENGTH	64
#define L1_SETWAY_SET_SHIFT	6	// Log2(L1_DATA_CACHE_LINE_LENGTH)
#define L2_CACHE_SETS	1024
#define L2_CACHE_WAYS	8
#define L2_SETWAY_WAY_SHIFT	29	// 32-Log2(L2_CACHE_WAYS)
#define L2_CACHE_LINE_LENGTH	64
#define L2_SETWAY_SET_SHIFT	6	// Log2(L2_CACHE_LINE_LENGTH)
#define DATA_CACHE_LINE_LENGTH_MIN	64	// min(L1_DATA_CACHE_LINE_LENGTH, L2_CACHE_LINE_LENGTH)

void arm_invalidate_data_caches(void)
{
// invalidate L1 data cache
  for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
  {
    for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
    {
      register uint32_t nSetWayLevel = nWay << L1_SETWAY_WAY_SHIFT
        | nSet << L1_SETWAY_SET_SHIFT
        | 0 << SETWAY_LEVEL_SHIFT;
      __asm volatile ("mcr p15, 0, %0, c7, c6, 2" : : "r" (nSetWayLevel) : "memory"); // DCISW
    }
  }
// invalidate L2 unified cache
  for (register unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
  {
    for (register unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
    {
      register uint32_t nSetWayLevel = nWay << L2_SETWAY_WAY_SHIFT
        | nSet << L2_SETWAY_SET_SHIFT
        | 1 << SETWAY_LEVEL_SHIFT;
      __asm volatile ("mcr p15, 0, %0, c7, c6, 2" : : "r" (nSetWayLevel) : "memory"); // DCISW
    }
  }
}
void arm_clear_data_caches(void)
{
// clean L1 data cache
  for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
  {
    for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
    {
      register uint32_t nSetWayLevel = nWay << L1_SETWAY_WAY_SHIFT
        | nSet << L1_SETWAY_SET_SHIFT
        | 0 << SETWAY_LEVEL_SHIFT;
      __asm volatile ("mcr p15, 0, %0, c7, c10, 2" : : "r" (nSetWayLevel) : "memory"); // DCCSW
    }
  }
// clean L2 unified cache
  for (register unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
  {
    for (register unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
    {
      register uint32_t nSetWayLevel = nWay << L2_SETWAY_WAY_SHIFT
        | nSet << L2_SETWAY_SET_SHIFT
        | 1 << SETWAY_LEVEL_SHIFT;
      __asm volatile ("mcr p15, 0, %0, c7, c10, 2" : : "r" (nSetWayLevel) : "memory"); // DCCSW
    }
  }
}

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

/*
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
}*/


void uart_init()
{
	mmio_write(UART0_CR, 0x0);

	mmio_write(GPPUD, 0x0);
	usleep(150000);

	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	usleep(150000);

	mmio_write(GPPUDCLK0, 0x0);

	mmio_write(UART0_ICR, 0x7ff);

	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);

	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) |
				(1 << 8) | (1 << 9) | (1 << 10));

	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}



void mailbox_write(uint32_t* fbinfo_addr, unsigned int channel)
{
  while(1)
  {
    //uart_puts("write loop 1\n");
    if((mmio_read(mailbox+0x18)&0x80000000)==0) break;
  }
  mmio_write(mailbox+0x20, (uint32_t)fbinfo_addr+channel);
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

#define t_set_physical_display 0x00048003
#define t_set_virtual_buffer 0x00048004
#define t_set_depth 0x00048005
#define t_set_virtual_offset 0x00048009
#define t_allocate_buffer 0x00040001

static volatile uint32_t gfx_message[] __attribute__ ((aligned (16))) = {
  27*4, // size
  0,

  t_set_physical_display,
  8,
  8,
  1920,
  1080,

  t_set_virtual_buffer,
  8,
  8,
  1920,
  1080,

  t_set_depth,
  4,
  4,
  16,

  t_set_virtual_offset,
  8,
  8,
  0,
  0,

  t_allocate_buffer,
  8,
  8,
  0,
  0,

  0
};

uint32_t* init_rpi_gfx()
{
  unsigned int ra,rb;
  unsigned int ry,rx;

  arm_dsb();

  // https://github.com/PeterLemon/RaspberryPi/blob/master/HelloWorld/CPU/LIB/R_PI2.INC
  *((volatile uint32_t*)(PERIPHERAL_BASE + 0xb880 + 0x20 + 0x8)) = (uint32_t)gfx_message + 0x8; // MAIL_TAGS = 8
  
  uint32_t* framebuffer = 0;
  
  do {
    arm_dmb();
    arm_isb();
    framebuffer = (uint32_t*)gfx_message[24];
    printf("-- waiting for framebuffer…\r\n");
  } while (!framebuffer);
  
  return framebuffer;
}

static volatile uint32_t qpu_message[] __attribute__ ((aligned (16))) = {
  12*4, // size
  0,

  SET_CLOCK_RATE,
  8,
  8,
  CLK_V3D_ID,
  250*1000*1000, // 250 mhz

  ENABLE_QPU,
  4,
  4,
  1,
    
  0
};

void init_rpi_qpu() {
  

  *((volatile uint32_t*)(PERIPHERAL_BASE + 0xb880 + 0x20 + 0x8)) = (uint32_t)qpu_message + 0x8; // MAIL_TAGS = 8

  do {
    arm_dmb();
    arm_isb();
    printf("-- waiting for qpu ack…\r\n");
  } while (!qpu_message[1]);
}

#include "mmu.c"
