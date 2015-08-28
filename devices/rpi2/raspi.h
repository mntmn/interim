#ifndef RASPI_H
#include <stdint.h>

#ifdef RPI_1
#define PERIPHERAL_BASE 0x20000000
#else
#define PERIPHERAL_BASE 0x3f000000

// Cache control
#define arm_invalidate_instruction_cache() __asm volatile ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0) : "memory")
#define arm_flush_branch_target_cache()     __asm volatile ("mcr p15, 0, %0, c7, c5, 6" : : "r" (0) : "memory")

void arm_invalidate_data_caches();
void arm_clear_data_caches();

// Barriers
#define arm_dsb() __asm volatile ("dsb" ::: "memory")
#define arm_dmb() __asm volatile ("dmb" ::: "memory")
#define arm_isb() __asm volatile ("isb" ::: "memory")

#endif

#define mailbox PERIPHERAL_BASE+0xB880

#define GPU_CACHED_BASE	0x40000000
#define GPU_UNCACHED_BASE	0xC0000000

#define GPIO_GPFSEL0         0x0 // GPIO Function Select 0
#define GPIO_GPFSEL1         0x4 // GPIO Function Select 1
#define GPIO_GPFSEL2         0x8 // GPIO Function Select 2
#define GPIO_GPFSEL3         0xC // GPIO Function Select 3
#define GPIO_GPFSEL4        0x10 // GPIO Function Select 4
#define GPIO_GPFSEL5        0x14 // GPIO Function Select 5
#define GPIO_GPSET0         0x1C // GPIO Pin Output Set 0
#define GPIO_GPSET1         0x20 // GPIO Pin Output Set 1
#define GPIO_GPCLR0         0x28 // GPIO Pin Output Clear 0
#define GPIO_GPCLR1         0x2C // GPIO Pin Output Clear 1
#define GPIO_GPLEV0         0x34 // GPIO Pin Level 0
#define GPIO_GPLEV1         0x38 // GPIO Pin Level 1
#define GPIO_GPEDS0         0x40 // GPIO Pin Event Detect Status 0
#define GPIO_GPEDS1         0x44 // GPIO Pin Event Detect Status 1
#define GPIO_GPREN0         0x4C // GPIO Pin Rising Edge Detect Enable 0
#define GPIO_GPREN1         0x50 // GPIO Pin Rising Edge Detect Enable 1
#define GPIO_GPFEN0         0x58 // GPIO Pin Falling Edge Detect Enable 0
#define GPIO_GPFEN1         0x5C // GPIO Pin Falling Edge Detect Enable 1
#define GPIO_GPHEN0         0x64 // GPIO Pin High Detect Enable 0
#define GPIO_GPHEN1         0x68 // GPIO Pin High Detect Enable 1
#define GPIO_GPLEN0         0x70 // GPIO Pin Low Detect Enable 0
#define GPIO_GPLEN1         0x74 // GPIO Pin Low Detect Enable 1
#define GPIO_GPAREN0        0x7C // GPIO Pin Async. Rising Edge Detect 0
#define GPIO_GPAREN1        0x80 // GPIO Pin Async. Rising Edge Detect 1
#define GPIO_GPAFEN0        0x88 // GPIO Pin Async. Falling Edge Detect 0
#define GPIO_GPAFEN1        0x8C // GPIO Pin Async. Falling Edge Detect 1
#define GPIO_GPPUD          0x94 // GPIO Pin Pull-up/down Enable
#define GPIO_GPPUDCLK0      0x98 // GPIO Pin Pull-up/down Enable Clock 0
#define GPIO_GPPUDCLK1      0x9C // GPIO Pin Pull-up/down Enable Clock 1
#define GPIO_TEST           0xB0 // GPIO Test
 
#define LED_GPFSEL      GPIO_GPFSEL1
#define LED_GPFBIT      18
#define LED_GPSET       GPIO_GPSET0
#define LED_GPCLR       GPIO_GPCLR0
#define LED_GPIO_BIT    16

#define GPIO_FSEL0_IN    0x0 // GPIO Function Select: GPIO Pin X0 Is An Input
#define GPIO_FSEL0_OUT   0x1 // GPIO Function Select: GPIO Pin X0 Is An Output
#define GPIO_FSEL0_ALT0  0x4 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 0
#define GPIO_FSEL0_ALT1  0x5 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 1
#define GPIO_FSEL0_ALT2  0x6 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 2
#define GPIO_FSEL0_ALT3  0x7 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 3
#define GPIO_FSEL0_ALT4  0x3 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 4
#define GPIO_FSEL0_ALT5  0x2 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 5
#define GPIO_FSEL0_CLR   0x7 // GPIO Function Select: GPIO Pin X0 Clear Bits

#define GPIO_FSEL1_IN     0x0 // GPIO Function Select: GPIO Pin X1 Is An Input
#define GPIO_FSEL1_OUT    0x8 // GPIO Function Select: GPIO Pin X1 Is An Output
#define GPIO_FSEL1_ALT0  0x20 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 0
#define GPIO_FSEL1_ALT1  0x28 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 1
#define GPIO_FSEL1_ALT2  0x30 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 2
#define GPIO_FSEL1_ALT3  0x38 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 3
#define GPIO_FSEL1_ALT4  0x18 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 4
#define GPIO_FSEL1_ALT5  0x10 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 5
#define GPIO_FSEL1_CLR   0x38 // GPIO Function Select: GPIO Pin X1 Clear Bits

#define GPIO_FSEL2_IN      0x0 // GPIO Function Select: GPIO Pin X2 Is An Input
#define GPIO_FSEL2_OUT    0x40 // GPIO Function Select: GPIO Pin X2 Is An Output
#define GPIO_FSEL2_ALT0  0x100 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 0
#define GPIO_FSEL2_ALT1  0x140 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 1
#define GPIO_FSEL2_ALT2  0x180 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 2
#define GPIO_FSEL2_ALT3  0x1C0 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 3
#define GPIO_FSEL2_ALT4   0xC0 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 4
#define GPIO_FSEL2_ALT5   0x80 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 5
#define GPIO_FSEL2_CLR   0x1C0 // GPIO Function Select: GPIO Pin X2 Clear Bits

#define GPIO_FSEL3_IN      0x0 // GPIO Function Select: GPIO Pin X3 Is An Input
#define GPIO_FSEL3_OUT   0x200 // GPIO Function Select: GPIO Pin X3 Is An Output
#define GPIO_FSEL3_ALT0  0x800 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 0
#define GPIO_FSEL3_ALT1  0xA00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 1
#define GPIO_FSEL3_ALT2  0xC00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 2
#define GPIO_FSEL3_ALT3  0xE00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 3
#define GPIO_FSEL3_ALT4  0x600 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 4
#define GPIO_FSEL3_ALT5  0x400 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 5
#define GPIO_FSEL3_CLR   0xE00 // GPIO Function Select: GPIO Pin X3 Clear Bits

#define GPIO_FSEL4_IN       0x0 // GPIO Function Select: GPIO Pin X4 Is An Input
#define GPIO_FSEL4_OUT   0x1000 // GPIO Function Select: GPIO Pin X4 Is An Output
#define GPIO_FSEL4_ALT0  0x4000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 0
#define GPIO_FSEL4_ALT1  0x5000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 1
#define GPIO_FSEL4_ALT2  0x6000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 2
#define GPIO_FSEL4_ALT3  0x7000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 3
#define GPIO_FSEL4_ALT4  0x3000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 4
#define GPIO_FSEL4_ALT5  0x2000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 5
#define GPIO_FSEL4_CLR   0x7000 // GPIO Function Select: GPIO Pin X4 Clear Bits

#define GPIO_FSEL5_IN        0x0 // GPIO Function Select: GPIO Pin X5 Is An Input
#define GPIO_FSEL5_OUT    0x8000 // GPIO Function Select: GPIO Pin X5 Is An Output
#define GPIO_FSEL5_ALT0  0x20000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 0
#define GPIO_FSEL5_ALT1  0x28000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 1
#define GPIO_FSEL5_ALT2  0x30000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 2
#define GPIO_FSEL5_ALT3  0x38000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 3
#define GPIO_FSEL5_ALT4  0x18000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 4
#define GPIO_FSEL5_ALT5  0x10000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 5
#define GPIO_FSEL5_CLR   0x38000 // GPIO Function Select: GPIO Pin X5 Clear Bits

#define GPIO_FSEL6_IN         0x0 // GPIO Function Select: GPIO Pin X6 Is An Input
#define GPIO_FSEL6_OUT    0x40000 // GPIO Function Select: GPIO Pin X6 Is An Output
#define GPIO_FSEL6_ALT0  0x100000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 0
#define GPIO_FSEL6_ALT1  0x140000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 1
#define GPIO_FSEL6_ALT2  0x180000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 2
#define GPIO_FSEL6_ALT3  0x1C0000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 3
#define GPIO_FSEL6_ALT4   0xC0000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 4
#define GPIO_FSEL6_ALT5   0x80000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 5
#define GPIO_FSEL6_CLR   0x1C0000 // GPIO Function Select: GPIO Pin X6 Clear Bits

#define GPIO_FSEL7_IN         0x0 // GPIO Function Select: GPIO Pin X7 Is An Input
#define GPIO_FSEL7_OUT   0x200000 // GPIO Function Select: GPIO Pin X7 Is An Output
#define GPIO_FSEL7_ALT0  0x800000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 0
#define GPIO_FSEL7_ALT1  0xA00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 1
#define GPIO_FSEL7_ALT2  0xC00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 2
#define GPIO_FSEL7_ALT3  0xE00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 3
#define GPIO_FSEL7_ALT4  0x600000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 4
#define GPIO_FSEL7_ALT5  0x400000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 5
#define GPIO_FSEL7_CLR   0xE00000 // GPIO Function Select: GPIO Pin X7 Clear Bits

#define GPIO_FSEL8_IN          0x0 // GPIO Function Select: GPIO Pin X8 Is An Input
#define GPIO_FSEL8_OUT   0x1000000 // GPIO Function Select: GPIO Pin X8 Is An Output
#define GPIO_FSEL8_ALT0  0x4000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 0
#define GPIO_FSEL8_ALT1  0x5000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 1
#define GPIO_FSEL8_ALT2  0x6000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 2
#define GPIO_FSEL8_ALT3  0x7000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 3
#define GPIO_FSEL8_ALT4  0x3000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 4
#define GPIO_FSEL8_ALT5  0x2000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 5
#define GPIO_FSEL8_CLR   0x7000000 // GPIO Function Select: GPIO Pin X8 Clear Bits

#define GPIO_FSEL9_IN           0x0 // GPIO Function Select: GPIO Pin X9 Is An Input
#define GPIO_FSEL9_OUT    0x8000000 // GPIO Function Select: GPIO Pin X9 Is An Output
#define GPIO_FSEL9_ALT0  0x20000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 0
#define GPIO_FSEL9_ALT1  0x28000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 1
#define GPIO_FSEL9_ALT2  0x30000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 2
#define GPIO_FSEL9_ALT3  0x38000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 3
#define GPIO_FSEL9_ALT4  0x18000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 4
#define GPIO_FSEL9_ALT5  0x10000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 5
#define GPIO_FSEL9_CLR   0x38000000 // GPIO Function Select: GPIO Pin X9 Clear Bits

enum
{
  // The GPIO registers base address.
  GPIO_BASE = (PERIPHERAL_BASE + 0x200000),
 
  // The offsets for reach register.
 
  // Controls actuation of pull up/down to ALL GPIO pins.
  GPPUD = (GPIO_BASE + 0x94),
 
  // Controls actuation of pull up/down for specific GPIO pin.
  GPPUDCLK0 = (GPIO_BASE + 0x98),
 
  // The base address for UART.
  UART0_BASE = (PERIPHERAL_BASE + 0x201000),
 
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

// CM / Clock Manager
#define CM_BASE    0x101000 // Clock Manager Base Address
#define CM_GNRICCTL   0x000 // Clock Manager Generic Clock Control
#define CM_GNRICDIV   0x004 // Clock Manager Generic Clock Divisor
#define CM_VPUCTL     0x008 // Clock Manager VPU Clock Control
#define CM_VPUDIV     0x00C // Clock Manager VPU Clock Divisor
#define CM_SYSCTL     0x010 // Clock Manager System Clock Control
#define CM_SYSDIV     0x014 // Clock Manager System Clock Divisor
#define CM_PERIACTL   0x018 // Clock Manager PERIA Clock Control
#define CM_PERIADIV   0x01C // Clock Manager PERIA Clock Divisor
#define CM_PERIICTL   0x020 // Clock Manager PERII Clock Control
#define CM_PERIIDIV   0x024 // Clock Manager PERII Clock Divisor
#define CM_H264CTL    0x028 // Clock Manager H264 Clock Control
#define CM_H264DIV    0x02C // Clock Manager H264 Clock Divisor
#define CM_ISPCTL     0x030 // Clock Manager ISP Clock Control
#define CM_ISPDIV     0x034 // Clock Manager ISP Clock Divisor
#define CM_V3DCTL     0x038 // Clock Manager V3D Clock Control
#define CM_V3DDIV     0x03C // Clock Manager V3D Clock Divisor
#define CM_CAM0CTL    0x040 // Clock Manager Camera 0 Clock Control
#define CM_CAM0DIV    0x044 // Clock Manager Camera 0 Clock Divisor
#define CM_CAM1CTL    0x048 // Clock Manager Camera 1 Clock Control
#define CM_CAM1DIV    0x04C // Clock Manager Camera 1 Clock Divisor
#define CM_CCP2CTL    0x050 // Clock Manager CCP2 Clock Control
#define CM_CCP2DIV    0x054 // Clock Manager CCP2 Clock Divisor
#define CM_DSI0ECTL   0x058 // Clock Manager DSI0E Clock Control
#define CM_DSI0EDIV   0x05C // Clock Manager DSI0E Clock Divisor
#define CM_DSI0PCTL   0x060 // Clock Manager DSI0P Clock Control
#define CM_DSI0PDIV   0x064 // Clock Manager DSI0P Clock Divisor
#define CM_DPICTL     0x068 // Clock Manager DPI Clock Control
#define CM_DPIDIV     0x06C // Clock Manager DPI Clock Divisor
#define CM_GP0CTL     0x070 // Clock Manager General Purpose 0 Clock Control
#define CM_GP0DIV     0x074 // Clock Manager General Purpose 0 Clock Divisor
#define CM_GP1CTL     0x078 // Clock Manager General Purpose 1 Clock Control
#define CM_GP1DIV     0x07C // Clock Manager General Purpose 1 Clock Divisor
#define CM_GP2CTL     0x080 // Clock Manager General Purpose 2 Clock Control
#define CM_GP2DIV     0x084 // Clock Manager General Purpose 2 Clock Divisor
#define CM_HSMCTL     0x088 // Clock Manager HSM Clock Control
#define CM_HSMDIV     0x08C // Clock Manager HSM Clock Divisor
#define CM_OTPCTL     0x090 // Clock Manager OTP Clock Control
#define CM_OTPDIV     0x094 // Clock Manager OTP Clock Divisor
#define CM_PCMCTL     0x098 // Clock Manager PCM / I2S Clock Control
#define CM_PCMDIV     0x09C // Clock Manager PCM / I2S Clock Divisor
#define CM_PWMCTL     0x0A0 // Clock Manager PWM Clock Control
#define CM_PWMDIV     0x0A4 // Clock Manager PWM Clock Divisor
#define CM_SLIMCTL    0x0A8 // Clock Manager SLIM Clock Control
#define CM_SLIMDIV    0x0AC // Clock Manager SLIM Clock Divisor
#define CM_SMICTL     0x0B0 // Clock Manager SMI Clock Control
#define CM_SMIDIV     0x0B4 // Clock Manager SMI Clock Divisor
#define CM_TCNTCTL    0x0C0 // Clock Manager TCNT Clock Control
#define CM_TCNTDIV    0x0C4 // Clock Manager TCNT Clock Divisor
#define CM_TECCTL     0x0C8 // Clock Manager TEC Clock Control
#define CM_TECDIV     0x0CC // Clock Manager TEC Clock Divisor
#define CM_TD0CTL     0x0D0 // Clock Manager TD0 Clock Control
#define CM_TD0DIV     0x0D4 // Clock Manager TD0 Clock Divisor
#define CM_TD1CTL     0x0D8 // Clock Manager TD1 Clock Control
#define CM_TD1DIV     0x0DC // Clock Manager TD1 Clock Divisor
#define CM_TSENSCTL   0x0E0 // Clock Manager TSENS Clock Control
#define CM_TSENSDIV   0x0E4 // Clock Manager TSENS Clock Divisor
#define CM_TIMERCTL   0x0E8 // Clock Manager Timer Clock Control
#define CM_TIMERDIV   0x0EC // Clock Manager Timer Clock Divisor
#define CM_UARTCTL    0x0F0 // Clock Manager UART Clock Control
#define CM_UARTDIV    0x0F4 // Clock Manager UART Clock Divisor
#define CM_VECCTL     0x0F8 // Clock Manager VEC Clock Control
#define CM_VECDIV     0x0FC // Clock Manager VEC Clock Divisor
#define CM_OSCCOUNT   0x100 // Clock Manager Oscillator Count
#define CM_PLLA       0x104 // Clock Manager PLLA
#define CM_PLLC       0x108 // Clock Manager PLLC
#define CM_PLLD       0x10C // Clock Manager PLLD
#define CM_PLLH       0x110 // Clock Manager PLLH
#define CM_LOCK       0x114 // Clock Manager Lock
#define CM_EVENT      0x118 // Clock Manager Event
#define CM_INTEN      0x118 // Clock Manager INTEN
#define CM_DSI0HSCK   0x120 // Clock Manager DSI0HSCK
#define CM_CKSM       0x124 // Clock Manager CKSM
#define CM_OSCFREQI   0x128 // Clock Manager Oscillator Frequency Integer
#define CM_OSCFREQF   0x12C // Clock Manager Oscillator Frequency Fraction
#define CM_PLLTCTL    0x130 // Clock Manager PLLT Control
#define CM_PLLTCNT0   0x134 // Clock Manager PLLT0
#define CM_PLLTCNT1   0x138 // Clock Manager PLLT1
#define CM_PLLTCNT2   0x13C // Clock Manager PLLT2
#define CM_PLLTCNT3   0x140 // Clock Manager PLLT3
#define CM_TDCLKEN    0x144 // Clock Manager TD Clock Enable
#define CM_BURSTCTL   0x148 // Clock Manager Burst Control
#define CM_BURSTCNT   0x14C // Clock Manager Burst
#define CM_DSI1ECTL   0x158 // Clock Manager DSI1E Clock Control
#define CM_DSI1EDIV   0x15C // Clock Manager DSI1E Clock Divisor
#define CM_DSI1PCTL   0x160 // Clock Manager DSI1P Clock Control
#define CM_DSI1PDIV   0x164 // Clock Manager DSI1P Clock Divisor
#define CM_DFTCTL     0x168 // Clock Manager DFT Clock Control
#define CM_DFTDIV     0x16C // Clock Manager DFT Clock Divisor
#define CM_PLLB       0x170 // Clock Manager PLLB
#define CM_PULSECTL   0x190 // Clock Manager Pulse Clock Control
#define CM_PULSEDIV   0x194 // Clock Manager Pulse Clock Divisor
#define CM_SDCCTL     0x1A8 // Clock Manager SDC Clock Control
#define CM_SDCDIV     0x1AC // Clock Manager SDC Clock Divisor
#define CM_ARMCTL     0x1B0 // Clock Manager ARM Clock Control
#define CM_ARMDIV     0x1B4 // Clock Manager ARM Clock Divisor
#define CM_AVEOCTL    0x1B8 // Clock Manager AVEO Clock Control
#define CM_AVEODIV    0x1BC // Clock Manager AVEO Clock Divisor
#define CM_EMMCCTL    0x1C0 // Clock Manager EMMC Clock Control
#define CM_EMMCDIV    0x1C4 // Clock Manager EMMC Clock Divisor
#define CM_SRC_OSCILLATOR        0x01 // Clock Control: Clock Source  Oscillator
#define CM_SRC_TESTDEBUG0        0x02 // Clock Control: Clock Source  Test Debug 0
#define CM_SRC_TESTDEBUG1        0x03 // Clock Control: Clock Source  Test Debug 1
#define CM_SRC_PLLAPER           0x04 // Clock Control: Clock Source  PLLA Per
#define CM_SRC_PLLCPER           0x05 // Clock Control: Clock Source  PLLC Per
#define CM_SRC_PLLDPER           0x06 // Clock Control: Clock Source  PLLD Per
#define CM_SRC_HDMIAUX           0x07 // Clock Control: Clock Source  HDMI Auxiliary
#define CM_SRC_GND               0x08 // Clock Control: Clock Source  GND
#define CM_ENAB                  0x10 // Clock Control: Enable The Clock Generator
#define CM_KILL                  0x20 // Clock Control: Kill The Clock Generator
#define CM_BUSY                  0x80 // Clock Control: Clock Generator Is Running
#define CM_FLIP                 0x100 // Clock Control: Invert The Clock Generator Output
#define CM_MASH_1               0x200 // Clock Control: MASH Control  1-Stage MASH (Equivalent To Non-MASH Dividers)
#define CM_MASH_2               0x400 // Clock Control: MASH Control  2-Stage MASH
#define CM_MASH_3               0x600 // Clock Control: MASH Control  3-Stage MASH
#define CM_PASSWORD        0x5A000000 // Clock Control: Password "5A"

// DMA Controller
#define DMA0_BASE   0x7000 // DMA Channel 0 Register Set
#define DMA1_BASE   0x7100 // DMA Channel 1 Register Set
#define DMA2_BASE   0x7200 // DMA Channel 2 Register Set
#define DMA3_BASE   0x7300 // DMA Channel 3 Register Set
#define DMA4_BASE   0x7400 // DMA Channel 4 Register Set
#define DMA5_BASE   0x7500 // DMA Channel 5 Register Set
#define DMA6_BASE   0x7600 // DMA Channel 6 Register Set
#define DMA7_BASE   0x7700 // DMA Channel 7 Register Set
#define DMA8_BASE   0x7800 // DMA Channel 8 Register Set
#define DMA9_BASE   0x7900 // DMA Channel 9 Register Set
#define DMA10_BASE  0x7A00 // DMA Channel 10 Register Set
#define DMA11_BASE  0x7B00 // DMA Channel 11 Register Set
#define DMA12_BASE  0x7C00 // DMA Channel 12 Register Set
#define DMA13_BASE  0x7D00 // DMA Channel 13 Register Set
#define DMA14_BASE  0x7E00 // DMA Channel 14 Register Set
#define DMA_INT_STATUS  0x7FE0 // Interrupt Status of each DMA Channel
#define DMA_ENABLE      0x7FF0 // Global Enable bits for each DMA Channel
#define DMA15_BASE  0xE05000 // DMA Channel 15 Register Set
#define DMA_CS          0x0 // DMA Channel 0..14 Control & Status
#define DMA_CONBLK_AD   0x4 // DMA Channel 0..14 Control Block Address
#define DMA_TI          0x8 // DMA Channel 0..14 CB Word 0 (Transfer Information)
#define DMA_SOURCE_AD   0xC // DMA Channel 0..14 CB Word 1 (Source Address)
#define DMA_DEST_AD    0x10 // DMA Channel 0..14 CB Word 2 (Destination Address)
#define DMA_TXFR_LEN   0x14 // DMA Channel 0..14 CB Word 3 (Transfer Length)
#define DMA_STRIDE     0x18 // DMA Channel 0..14 CB Word 4 (2D Stride)
#define DMA_NEXTCONBK  0x1C // DMA Channel 0..14 CB Word 5 (Next CB Address)
#define DMA_DEBUG      0x20 // DMA Channel 0..14 Debug
#define DMA_ACTIVE                                 0x1 // DMA Control & Status: Activate the DMA
#define DMA_END                                    0x2 // DMA Control & Status: DMA End Flag
#define DMA_INT                                    0x4 // DMA Control & Status: Interrupt Status
#define DMA_DREQ                                   0x8 // DMA Control & Status: DREQ State
#define DMA_PAUSED                                0x10 // DMA Control & Status: DMA Paused State
#define DMA_DREQ_STOPS_DMA                        0x20 // DMA Control & Status: DMA Paused by DREQ State
#define DMA_WAITING_FOR_OUTSTANDING_WRITES        0x40 // DMA Control & Status: DMA is Waiting for the Last Write to be Received
#define DMA_ERROR                                0x100 // DMA Control & Status: DMA Error
#define DMA_PRIORITY_0                             0x0 // DMA Control & Status: AXI Priority Level 0
#define DMA_PRIORITY_1                         0x10000 // DMA Control & Status: AXI Priority Level 1
#define DMA_PRIORITY_2                         0x20000 // DMA Control & Status: AXI Priority Level 2
#define DMA_PRIORITY_3                         0x30000 // DMA Control & Status: AXI Priority Level 3
#define DMA_PRIORITY_4                         0x40000 // DMA Control & Status: AXI Priority Level 4
#define DMA_PRIORITY_5                         0x50000 // DMA Control & Status: AXI Priority Level 5
#define DMA_PRIORITY_6                         0x60000 // DMA Control & Status: AXI Priority Level 6
#define DMA_PRIORITY_7                         0x70000 // DMA Control & Status: AXI Priority Level 7
#define DMA_PRIORITY_8                         0x80000 // DMA Control & Status: AXI Priority Level 8
#define DMA_PRIORITY_9                         0x90000 // DMA Control & Status: AXI Priority Level 9
#define DMA_PRIORITY_10                        0xA0000 // DMA Control & Status: AXI Priority Level 10
#define DMA_PRIORITY_11                        0xB0000 // DMA Control & Status: AXI Priority Level 11
#define DMA_PRIORITY_12                        0xC0000 // DMA Control & Status: AXI Priority Level 12
#define DMA_PRIORITY_13                        0xD0000 // DMA Control & Status: AXI Priority Level 13
#define DMA_PRIORITY_14                        0xE0000 // DMA Control & Status: AXI Priority Level 14
#define DMA_PRIORITY_15                        0xF0000 // DMA Control & Status: AXI Priority Level 15
#define DMA_PRIORITY                           0xF0000 // DMA Control & Status: AXI Priority Level
#define DMA_PANIC_PRIORITY_0                       0x0 // DMA Control & Status: AXI Panic Priority Level 0
#define DMA_PANIC_PRIORITY_1                  0x100000 // DMA Control & Status: AXI Panic Priority Level 1
#define DMA_PANIC_PRIORITY_2                  0x200000 // DMA Control & Status: AXI Panic Priority Level 2
#define DMA_PANIC_PRIORITY_3                  0x300000 // DMA Control & Status: AXI Panic Priority Level 3
#define DMA_PANIC_PRIORITY_4                  0x400000 // DMA Control & Status: AXI Panic Priority Level 4
#define DMA_PANIC_PRIORITY_5                  0x500000 // DMA Control & Status: AXI Panic Priority Level 5
#define DMA_PANIC_PRIORITY_6                  0x600000 // DMA Control & Status: AXI Panic Priority Level 6
#define DMA_PANIC_PRIORITY_7                  0x700000 // DMA Control & Status: AXI Panic Priority Level 7
#define DMA_PANIC_PRIORITY_8                  0x800000 // DMA Control & Status: AXI Panic Priority Level 8
#define DMA_PANIC_PRIORITY_9                  0x900000 // DMA Control & Status: AXI Panic Priority Level 9
#define DMA_PANIC_PRIORITY_10                 0xA00000 // DMA Control & Status: AXI Panic Priority Level 10
#define DMA_PANIC_PRIORITY_11                 0xB00000 // DMA Control & Status: AXI Panic Priority Level 11
#define DMA_PANIC_PRIORITY_12                 0xC00000 // DMA Control & Status: AXI Panic Priority Level 12
#define DMA_PANIC_PRIORITY_13                 0xD00000 // DMA Control & Status: AXI Panic Priority Level 13
#define DMA_PANIC_PRIORITY_14                 0xE00000 // DMA Control & Status: AXI Panic Priority Level 14
#define DMA_PANIC_PRIORITY_15                 0xF00000 // DMA Control & Status: AXI Panic Priority Level 14
#define DMA_PANIC_PRIORITY                    0xF00000 // DMA Control & Status: AXI Panic Priority Level
#define DMA_WAIT_FOR_OUTSTANDING_WRITES     0x10000000 // DMA Control & Status: Wait for Outstanding Writes
#define DMA_DISDEBUG                        0x20000000 // DMA Control & Status: Disable Debug Pause Signal
#define DMA_ABORT                           0x40000000 // DMA Control & Status: Abort DMA
#define DMA_RESET                           0x80000000 // DMA Control & Status: DMA Channel Reset
#define DMA_INTEN                  0x1 // DMA Transfer Information: Interrupt Enable
#define DMA_TDMODE                 0x2 // DMA Transfer Information: 2D Mode
#define DMA_WAIT_RESP              0x8 // DMA Transfer Information: Wait for a Write Response
#define DMA_DEST_INC              0x10 // DMA Transfer Information: Destination Address Increment
#define DMA_DEST_WIDTH            0x20 // DMA Transfer Information: Destination Transfer Width
#define DMA_DEST_DREQ             0x40 // DMA Transfer Information: Control Destination Writes with DREQ
#define DMA_DEST_IGNORE           0x80 // DMA Transfer Information: Ignore Writes
#define DMA_SRC_INC              0x100 // DMA Transfer Information: Source Address Increment
#define DMA_SRC_WIDTH            0x200 // DMA Transfer Information: Source Transfer Width
#define DMA_SRC_DREQ             0x400 // DMA Transfer Information: Control Source Reads with DREQ
#define DMA_SRC_IGNORE           0x800 // DMA Transfer Information: Ignore Reads
#define DMA_BURST_LENGTH_1         0x0 // DMA Transfer Information: Burst Transfer Length 1 Word
#define DMA_BURST_LENGTH_2      0x1000 // DMA Transfer Information: Burst Transfer Length 2 Words
#define DMA_BURST_LENGTH_3      0x2000 // DMA Transfer Information: Burst Transfer Length 3 Words
#define DMA_BURST_LENGTH_4      0x3000 // DMA Transfer Information: Burst Transfer Length 4 Words
#define DMA_BURST_LENGTH_5      0x4000 // DMA Transfer Information: Burst Transfer Length 5 Words
#define DMA_BURST_LENGTH_6      0x5000 // DMA Transfer Information: Burst Transfer Length 6 Words
#define DMA_BURST_LENGTH_7      0x6000 // DMA Transfer Information: Burst Transfer Length 7 Words
#define DMA_BURST_LENGTH_8      0x7000 // DMA Transfer Information: Burst Transfer Length 8 Words
#define DMA_BURST_LENGTH_9      0x8000 // DMA Transfer Information: Burst Transfer Length 9 Words
#define DMA_BURST_LENGTH_10     0x9000 // DMA Transfer Information: Burst Transfer Length 10 Words
#define DMA_BURST_LENGTH_11     0xA000 // DMA Transfer Information: Burst Transfer Length 11 Words
#define DMA_BURST_LENGTH_12     0xB000 // DMA Transfer Information: Burst Transfer Length 12 Words
#define DMA_BURST_LENGTH_13     0xC000 // DMA Transfer Information: Burst Transfer Length 13 Words
#define DMA_BURST_LENGTH_14     0xD000 // DMA Transfer Information: Burst Transfer Length 14 Words
#define DMA_BURST_LENGTH_15     0xE000 // DMA Transfer Information: Burst Transfer Length 15 Words
#define DMA_BURST_LENGTH_16     0xF000 // DMA Transfer Information: Burst Transfer Length 16 Words
#define DMA_BURST_LENGTH        0xF000 // DMA Transfer Information: Burst Transfer Length
#define DMA_PERMAP_0               0x0 // DMA Transfer Information: Peripheral Mapping Continuous Un-paced Transfer
#define DMA_PERMAP_1           0x10000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 1
#define DMA_PERMAP_2           0x20000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 2
#define DMA_PERMAP_3           0x30000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 3
#define DMA_PERMAP_4           0x40000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 4
#define DMA_PERMAP_5           0x50000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 5
#define DMA_PERMAP_6           0x60000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 6
#define DMA_PERMAP_7           0x70000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 7
#define DMA_PERMAP_8           0x80000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 8
#define DMA_PERMAP_9           0x90000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 9
#define DMA_PERMAP_10          0xA0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 10
#define DMA_PERMAP_11          0xB0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 11
#define DMA_PERMAP_12          0xC0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 12
#define DMA_PERMAP_13          0xD0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 13
#define DMA_PERMAP_14          0xE0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 14
#define DMA_PERMAP_15          0xF0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 15
#define DMA_PERMAP_16         0x100000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 16
#define DMA_PERMAP_17         0x110000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 17
#define DMA_PERMAP_18         0x120000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 18
#define DMA_PERMAP_19         0x130000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 19
#define DMA_PERMAP_20         0x140000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 20
#define DMA_PERMAP_21         0x150000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 21
#define DMA_PERMAP_22         0x160000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 22
#define DMA_PERMAP_23         0x170000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 23
#define DMA_PERMAP_24         0x180000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 24
#define DMA_PERMAP_25         0x190000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 25
#define DMA_PERMAP_26         0x1A0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 26
#define DMA_PERMAP_27         0x1B0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 27
#define DMA_PERMAP_28         0x1C0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 28
#define DMA_PERMAP_29         0x1D0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 29
#define DMA_PERMAP_30         0x1E0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 30
#define DMA_PERMAP_31         0x1F0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 31
#define DMA_PERMAP            0x1F0000 // DMA Transfer Information: Peripheral Mapping
#define DMA_WAITS_0                0x0 // DMA Transfer Information: Add No Wait Cycles
#define DMA_WAITS_1           0x200000 // DMA Transfer Information: Add 1 Wait Cycle
#define DMA_WAITS_2           0x400000 // DMA Transfer Information: Add 2 Wait Cycles
#define DMA_WAITS_3           0x600000 // DMA Transfer Information: Add 3 Wait Cycles
#define DMA_WAITS_4           0x800000 // DMA Transfer Information: Add 4 Wait Cycles
#define DMA_WAITS_5           0xA00000 // DMA Transfer Information: Add 5 Wait Cycles
#define DMA_WAITS_6           0xC00000 // DMA Transfer Information: Add 6 Wait Cycles
#define DMA_WAITS_7           0xE00000 // DMA Transfer Information: Add 7 Wait Cycles
#define DMA_WAITS_8          0x1000000 // DMA Transfer Information: Add 8 Wait Cycles
#define DMA_WAITS_9          0x1200000 // DMA Transfer Information: Add 9 Wait Cycles
#define DMA_WAITS_10         0x1400000 // DMA Transfer Information: Add 10 Wait Cycles
#define DMA_WAITS_11         0x1600000 // DMA Transfer Information: Add 11 Wait Cycles
#define DMA_WAITS_12         0x1800000 // DMA Transfer Information: Add 12 Wait Cycles
#define DMA_WAITS_13         0x1A00000 // DMA Transfer Information: Add 13 Wait Cycles
#define DMA_WAITS_14         0x1C00000 // DMA Transfer Information: Add 14 Wait Cycles
#define DMA_WAITS_15         0x1E00000 // DMA Transfer Information: Add 15 Wait Cycles
#define DMA_WAITS_16         0x2000000 // DMA Transfer Information: Add 16 Wait Cycles
#define DMA_WAITS_17         0x2200000 // DMA Transfer Information: Add 17 Wait Cycles
#define DMA_WAITS_18         0x2400000 // DMA Transfer Information: Add 18 Wait Cycles
#define DMA_WAITS_19         0x2600000 // DMA Transfer Information: Add 19 Wait Cycles
#define DMA_WAITS_20         0x2800000 // DMA Transfer Information: Add 20 Wait Cycles
#define DMA_WAITS_21         0x2A00000 // DMA Transfer Information: Add 21 Wait Cycles
#define DMA_WAITS_22         0x2C00000 // DMA Transfer Information: Add 22 Wait Cycles
#define DMA_WAITS_23         0x2E00000 // DMA Transfer Information: Add 23 Wait Cycles
#define DMA_WAITS_24         0x3000000 // DMA Transfer Information: Add 24 Wait Cycles
#define DMA_WAITS_25         0x3200000 // DMA Transfer Information: Add 25 Wait Cycles
#define DMA_WAITS_26         0x3400000 // DMA Transfer Information: Add 26 Wait Cycles
#define DMA_WAITS_27         0x3600000 // DMA Transfer Information: Add 27 Wait Cycles
#define DMA_WAITS_28         0x3800000 // DMA Transfer Information: Add 28 Wait Cycles
#define DMA_WAITS_29         0x3A00000 // DMA Transfer Information: Add 29 Wait Cycles
#define DMA_WAITS_30         0x3C00000 // DMA Transfer Information: Add 30 Wait Cycles
#define DMA_WAITS_31         0x3E00000 // DMA Transfer Information: Add 31 Wait Cycles
#define DMA_WAITS            0x3E00000 // DMA Transfer Information: Add Wait Cycles
#define DMA_NO_WIDE_BURSTS   0x4000000 // DMA Transfer Information: Don't Do Wide Writes as a 2 Beat Burst

#define DMA_XLENGTH      0xFFFF // DMA Transfer Length: Transfer Length in Bytes
#define DMA_YLENGTH  0x3FFF0000 // DMA Transfer Length: When in 2D Mode, This is the Y Transfer Length

#define DMA_S_STRIDE      0xFFFF // DMA 2D Stride: Source Stride (2D Mode)
#define DMA_D_STRIDE  0xFFFF0000 // DMA 2D Stride: Destination Stride (2D Mode)

#define DMA_READ_LAST_NOT_SET_ERROR         0x1 // DMA Debug: Read Last Not Set Error
#define DMA_FIFO_ERROR                      0x2 // DMA Debug: Fifo Error
#define DMA_READ_ERROR                      0x4 // DMA Debug: Slave Read Response Error
#define DMA_OUTSTANDING_WRITES             0xF0 // DMA Debug: DMA Outstanding Writes Counter
#define DMA_ID                           0xFF00 // DMA Debug: DMA ID
#define DMA_STATE                     0x1FF0000 // DMA Debug: DMA State Machine State
#define DMA_VERSION                   0xE000000 // DMA Debug: DMA Version
#define DMA_LITE                     0x10000000 // DMA Debug: DMA Lite

#define DMA_INT0      0x1 // DMA Interrupt Status: Interrupt Status of DMA Engine 0
#define DMA_INT1      0x2 // DMA Interrupt Status: Interrupt Status of DMA Engine 1
#define DMA_INT2      0x4 // DMA Interrupt Status: Interrupt Status of DMA Engine 2
#define DMA_INT3      0x8 // DMA Interrupt Status: Interrupt Status of DMA Engine 3
#define DMA_INT4     0x10 // DMA Interrupt Status: Interrupt Status of DMA Engine 4
#define DMA_INT5     0x20 // DMA Interrupt Status: Interrupt Status of DMA Engine 5
#define DMA_INT6     0x40 // DMA Interrupt Status: Interrupt Status of DMA Engine 6
#define DMA_INT7     0x80 // DMA Interrupt Status: Interrupt Status of DMA Engine 7
#define DMA_INT8    0x100 // DMA Interrupt Status: Interrupt Status of DMA Engine 8
#define DMA_INT9    0x200 // DMA Interrupt Status: Interrupt Status of DMA Engine 9
#define DMA_INT10   0x400 // DMA Interrupt Status: Interrupt Status of DMA Engine 10
#define DMA_INT11   0x800 // DMA Interrupt Status: Interrupt Status of DMA Engine 11
#define DMA_INT12  0x1000 // DMA Interrupt Status: Interrupt Status of DMA Engine 12
#define DMA_INT13  0x2000 // DMA Interrupt Status: Interrupt Status of DMA Engine 13
#define DMA_INT14  0x4000 // DMA Interrupt Status: Interrupt Status of DMA Engine 14
#define DMA_INT15  0x8000 // DMA Interrupt Status: Interrupt Status of DMA Engine 15

#define DMA_EN0      0x1 // DMA Enable: Enable DMA Engine 0
#define DMA_EN1      0x2 // DMA Enable: Enable DMA Engine 1
#define DMA_EN2      0x4 // DMA Enable: Enable DMA Engine 2
#define DMA_EN3      0x8 // DMA Enable: Enable DMA Engine 3
#define DMA_EN4     0x10 // DMA Enable: Enable DMA Engine 4
#define DMA_EN5     0x20 // DMA Enable: Enable DMA Engine 5
#define DMA_EN6     0x40 // DMA Enable: Enable DMA Engine 6
#define DMA_EN7     0x80 // DMA Enable: Enable DMA Engine 7
#define DMA_EN8    0x100 // DMA Enable: Enable DMA Engine 8
#define DMA_EN9    0x200 // DMA Enable: Enable DMA Engine 9
#define DMA_EN10   0x400 // DMA Enable: Enable DMA Engine 10
#define DMA_EN11   0x800 // DMA Enable: Enable DMA Engine 11
#define DMA_EN12  0x1000 // DMA Enable: Enable DMA Engine 12
#define DMA_EN13  0x2000 // DMA Enable: Enable DMA Engine 13
#define DMA_EN14  0x4000 // DMA Enable: Enable DMA Engine 14

// PWM / Pulse Width Modulator Interface
#define PWM_BASE  0x20C000 // PWM Base Address
#define PWM_CTL        0x0 // PWM Control
#define PWM_STA        0x4 // PWM Status
#define PWM_DMAC       0x8 // PWM DMA Configuration
#define PWM_RNG1      0x10 // PWM Channel 1 Range
#define PWM_DAT1      0x14 // PWM Channel 1 Data
#define PWM_FIF1      0x18 // PWM FIFO Input
#define PWM_RNG2      0x20 // PWM Channel 2 Range
#define PWM_DAT2      0x24 // PWM Channel 2 Data

#define PWM_PWEN1     0x1 // PWM Control: Channel 1 Enable
#define PWM_MODE1     0x2 // PWM Control: Channel 1 Mode
#define PWM_RPTL1     0x4 // PWM Control: Channel 1 Repeat Last Data
#define PWM_SBIT1     0x8 // PWM Control: Channel 1 Silence Bit
#define PWM_POLA1    0x10 // PWM Control: Channel 1 Polarity
#define PWM_USEF1    0x20 // PWM Control: Channel 1 Use Fifo
#define PWM_CLRF1    0x40 // PWM Control: Clear Fifo
#define PWM_MSEN1    0x80 // PWM Control: Channel 1 M/S Enable
#define PWM_PWEN2   0x100 // PWM Control: Channel 2 Enable
#define PWM_MODE2   0x200 // PWM Control: Channel 2 Mode
#define PWM_RPTL2   0x400 // PWM Control: Channel 2 Repeat Last Data
#define PWM_SBIT2   0x800 // PWM Control: Channel 2 Silence Bit
#define PWM_POLA2  0x1000 // PWM Control: Channel 2 Polarity
#define PWM_USEF2  0x2000 // PWM Control: Channel 2 Use Fifo
#define PWM_MSEN2  0x8000 // PWM Control: Channel 2 M/S Enable

#define PWM_FULL1     0x1 // PWM Status: Fifo Full Flag
#define PWM_EMPT1     0x2 // PWM Status: Fifo Empty Flag
#define PWM_WERR1     0x4 // PWM Status: Fifo Write Error Flag
#define PWM_RERR1     0x8 // PWM Status: Fifo Read Error Flag
#define PWM_GAPO1    0x10 // PWM Status: Channel 1 Gap Occurred Flag
#define PWM_GAPO2    0x20 // PWM Status: Channel 2 Gap Occurred Flag
#define PWM_GAPO3    0x40 // PWM Status: Channel 3 Gap Occurred Flag
#define PWM_GAPO4    0x80 // PWM Status: Channel 4 Gap Occurred Flag
#define PWM_BERR    0x100 // PWM Status: Bus Error Flag
#define PWM_STA1    0x200 // PWM Status: Channel 1 State
#define PWM_STA2    0x400 // PWM Status: Channel 2 State
#define PWM_STA3    0x800 // PWM Status: Channel 3 State
#define PWM_STA4   0x1000 // PWM Status: Channel 4 State

#define PWM_ENAB  0x80000000 // PWM DMA Configuration: DMA Enable


void mmio_write(uint32_t reg, uint32_t data);
uint32_t mmio_read(uint32_t reg);

void uart_init();
void uart_putc(unsigned char byte);
void uart_write(const unsigned char* buffer, uint32_t size);
void uart_puts(const char* str);
unsigned char uart_getc();

uint32_t* init_rpi_gfx();
void init_rpi_qpu();
void delay(int32_t count);

#endif
