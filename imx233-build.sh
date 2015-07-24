#!/bin/sh

# -mtune=cortex-a7 -mfpu=neon-vfpv4  -ftree-vectorize

set -e
GCC_OPTS=" -g -O2 -nostartfiles -nostdlib -msoft-float -ffreestanding -fno-toplevel-reorder -march=armv5 -std=c11 -L../newlib/arm-none-eabi/lib -I../newlib/arm-none-eabi/include"

# -Wcast-align

COMPILE="arm-none-eabi-gcc $GCC_OPTS -I."

if [ $1 ]
then
   mkdir -p obj
   $COMPILE -c -o obj/reader.o bjos/sledge/reader.c
   $COMPILE -c -o obj/strmap.o bjos/sledge/strmap.c
   $COMPILE -c -o obj/alloc.o  bjos/sledge/alloc.c
   $COMPILE -c -o obj/writer.o bjos/sledge/writer.c
fi

arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/unifont  obj/unifont.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/editor.l obj/editor.o

$COMPILE -o build/bomberjacket-arm.elf bjos/rpi2/arm_start.S bjos/main_imx233.c bjos/imx233/imx233.c -T bjos/arm.ld  obj/lightning.o obj/jit_names.o obj/jit_note.o obj/jit_size.o obj/jit_memory.o obj/glue.o obj/reader.o obj/strmap.o obj/alloc.o obj/blit.o obj/writer.o -lc -lgcc -lm obj/editor.o bjos/mini-ip.c

arm-none-eabi-objcopy build/bomberjacket-arm.elf -O binary build/interim.imx

