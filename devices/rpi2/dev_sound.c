#include <stdio.h>
#include "minilisp.h"
#include "alloc.h"
#include "stream.h"
#include "compiler_new.h"
#include <unistd.h>
#include <fcntl.h>

typedef struct DMA_CB {
  uint32_t info;
  uint32_t source;
  uint32_t dest;
  uint32_t len;
  uint32_t stride;
  uint32_t next_block;
} DMA_CB;

uint32_t silence[] __attribute__((aligned(16))) = {0,0,0,0};

static DMA_CB dma_cb __attribute__((aligned(32)));

Cell* soundfs_open(Cell* path_cell) {
  return alloc_int(1);
}

Cell* soundfs_read() {
  return alloc_int(1);
}

Cell* soundfs_write(Cell* stream, Cell* packet) {
  //ethernet_tx(packet->ar.addr,packet->dr.size);
  uint32_t PBASE = PERIPHERAL_BASE;

  // enable phone jack (GPIO 40+45) PWM0/PWM1
  mmio_write(GPIO_BASE+GPIO_GPFSEL4, GPIO_FSEL0_ALT0 + GPIO_FSEL5_ALT0); 
  printf("[soundfs] gpios enabled\r\n");
  
  dma_cb.info = DMA_DEST_DREQ + DMA_PERMAP_5 + DMA_SRC_INC;
  dma_cb.source = (uint32_t)silence;
  dma_cb.dest = 0x7E000000 + PWM_BASE + PWM_FIF1;
  dma_cb.len = 2*4;
  dma_cb.next_block = (uint32_t)&dma_cb;

  // test
  dma_cb.source = (uint32_t)packet->ar.addr;
  dma_cb.len =  (uint32_t)packet->dr.size;
  printf("DMA block source changed to %p\r\n",dma_cb.source);

  printf("[soundfs] dma_cb created\r\n");
  
  mmio_write(PBASE+ CM_BASE+CM_PWMDIV, CM_PASSWORD + 0x2000); // Bits 0..11 Fractional Part Of Divisor = 0, Bits 12..23 Integer Part Of Divisor = 2);
  mmio_write(PBASE+ CM_BASE+CM_PWMCTL, CM_PASSWORD + CM_ENAB + CM_SRC_OSCILLATOR);
  
  printf("[soundfs] clock enabled\r\n");
  
  mmio_write(PBASE+ PWM_BASE+PWM_RNG1, 0x1b4);
  mmio_write(PBASE+ PWM_BASE+PWM_RNG2, 0x1b4);
  mmio_write(PBASE+ PWM_BASE+PWM_CTL, PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1);
  mmio_write(PBASE+ PWM_BASE+PWM_DMAC, PWM_ENAB+0x0001);
  
  printf("[soundfs] PWM enabled\r\n");

  mmio_write(PBASE+ DMA_ENABLE, DMA_EN0);

  printf("[soundfs] DMA enabled\r\n");

  mmio_write(PBASE+ DMA0_BASE+DMA_CONBLK_AD, (uint32_t)&dma_cb);
  
  mmio_write(PBASE+ DMA0_BASE+DMA_CS, DMA_ACTIVE);
  
  printf("[soundfs] DMA BASE enabled\r\n");

  return alloc_int(1);
}

void mount_soundfs() {
  fs_mount_builtin("/sound", soundfs_open, soundfs_read, soundfs_write, 0, 0);
  printf("[soundfs] mounted\r\n");
}
