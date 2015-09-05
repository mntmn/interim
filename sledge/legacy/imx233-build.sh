#!/bin/sh

NEWLIB="/usr/lib/arm-none-eabi/newlib"

set -e
GCC_OPTS=" -g -O2 -nostartfiles -nostdlib -msoft-float -ffreestanding -fno-toplevel-reorder -march=armv5 -std=gnu11 -L$NEWLIB -I/usr/include/newlib -I./sledge -I. -DCPU_ARM "

# -Wcast-align

COMPILE="arm-none-eabi-gcc $GCC_OPTS -I."

mkdir -p obj
$COMPILE -c -o obj/reader.o sledge/reader.c
$COMPILE -c -o obj/strmap.o sledge/strmap.c
$COMPILE -c -o obj/alloc.o  sledge/alloc.c
$COMPILE -c -o obj/writer.o sledge/writer.c
$COMPILE -c -o obj/stream.o sledge/stream.c
$COMPILE -c -o obj/debug_util.o os/debug_util.c

#arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/unifont  obj/unifont.o
#arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/editor.l obj/editor.o

$COMPILE -o build/interim-arm5.elf devices/imx233/arm_start.S devices/imx233/main_imx233.c devices/imx233/imx233.c -T devices/imx233/arm.ld obj/reader.o obj/strmap.o obj/alloc.o obj/writer.o obj/stream.o obj/debug_util.o -lc -lgcc -lm

arm-none-eabi-objcopy build/interim-arm5.elf -O binary build/interim.imx

