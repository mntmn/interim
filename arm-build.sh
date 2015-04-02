#!/bin/sh

set -e
GCC_OPTS=" -g -O2 -nostartfiles -nostdlib -mhard-float -ffreestanding -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -ftree-vectorize -std=c11 -L../newlib/arm-none-eabi/lib/fpu -I../newlib/arm-none-eabi/include "

COMPILE="arm-none-eabi-gcc $GCC_OPTS -I. -I./lightning/lightning -I./lightning/"

if [ $1 ]
then
   mkdir -p obj
   $COMPILE -c -o obj/lightning.o  lightning/lightning.c 
   $COMPILE -c -o obj/jit_names.o  lightning/jit_names.c
   $COMPILE -c -o obj/jit_note.o   lightning/jit_note.c
   $COMPILE -c -o obj/jit_size.o   lightning/jit_size.c
   $COMPILE -c -o obj/jit_memory.o lightning/jit_memory.c

   $COMPILE -c -o obj/glue.o   lightning/glue.c
   $COMPILE -c -o obj/reader.o bjos/sledge/reader.c
   $COMPILE -c -o obj/alloc.o  bjos/sledge/alloc.c
   $COMPILE -c -o obj/blit.o   bjos/sledge/blit.c
   $COMPILE -c -o obj/writer.o bjos/sledge/writer.c
fi

arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/unifont  obj/unifont.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/editor.l obj/editor.o

$COMPILE -o build/bomberjacket-arm.elf bjos/rpi2/arm_start.S bjos/main_rpi2.c bjos/rpi2/rpi-boot/emmc.c bjos/rpi2/rpi-boot/block.c bjos/rpi2/rpi-boot/timer.c bjos/rpi2/rpi-boot/mbox.c bjos/rpi2/rpi-boot/fat.c bjos/rpi2/rpi-boot/libfs.c bjos/rpi2/rpi-boot/mbr.c   bjos/rpi2/raspi.c bjos/rpi2/r3d.c bjos/barrier.S -T bjos/arm.ld  obj/lightning.o obj/jit_names.o obj/jit_note.o obj/jit_size.o obj/jit_memory.o obj/glue.o obj/reader.o obj/alloc.o obj/blit.o obj/writer.o -lc -lgcc -lm obj/unifont.o obj/editor.o

arm-none-eabi-objcopy build/bomberjacket-arm.elf -O binary build/kernel7.img

