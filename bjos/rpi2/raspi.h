#ifndef RASPI_H
#include <stdint.h>

#ifdef RPI_1
#define peripheral_base 0x20000000
#else
#define peripheral_base 0x3f000000

// Cache control
#define InvalidateInstructionCache() \
__asm volatile ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0) : "memory")
#define FlushPrefetchBuffer() __asm volatile ("isb" ::: "memory")
#define FlushBranchTargetCache() \
__asm volatile ("mcr p15, 0, %0, c7, c5, 6" : : "r" (0) : "memory")

void InvalidateDataCache();
void CleanDataCache();

// Barriers
#define DataSyncBarrier() __asm volatile ("dsb" ::: "memory")
#define DataMemBarrier() __asm volatile ("dmb" ::: "memory")
#define InstructionSyncBarrier() __asm volatile ("isb" ::: "memory")
#define InstructionMemBarrier() __asm volatile ("isb" ::: "memory")

#endif

#define mailbox peripheral_base+0xB880

#define GPU_CACHED_BASE	0x40000000
#define GPU_UNCACHED_BASE	0xC0000000

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
  GPIO_BASE = (peripheral_base + 0x200000),
 
  // The offsets for reach register.
 
  // Controls actuation of pull up/down to ALL GPIO pins.
  GPPUD = (GPIO_BASE + 0x94),
 
  // Controls actuation of pull up/down for specific GPIO pin.
  GPPUDCLK0 = (GPIO_BASE + 0x98),
 
  // The base address for UART.
  UART0_BASE = (peripheral_base + 0x201000),
 
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


#define SET_CLOCK_RATE 0x00038002 // Clocks: Set Clock Rate (Response: Clock ID, Rate In Hz)

#define CLK_EMMC_ID  0x1 // EMMC
#define CLK_UART_ID  0x2 // UART
#define CLK_ARM_ID   0x3 // ARM
#define CLK_CORE_ID  0x4 // CORE
#define CLK_V3D_ID   0x5 // V3D
#define CLK_H264_ID  0x6 // H264
#define CLK_ISP_ID   0x7 // ISP
#define CLK_SDRAM_ID 0x8 // SDRAM
#define CLK_PIXEL_ID 0x9 // PIXEL
#define CLK_PWM_ID   0xA // PWM

#define ENABLE_QPU 0x00030012 // QPU: Enables The QPU (Response: Enable State)

void mmio_write(uint32_t reg, uint32_t data);
uint32_t mmio_read(uint32_t reg);

void uart_putc(unsigned char byte);
void uart_write(const unsigned char* buffer, uint32_t size);
void uart_puts(const char* str);
unsigned char uart_getc();

uint32_t* init_rpi_gfx();
void init_rpi_qpu();

#endif
