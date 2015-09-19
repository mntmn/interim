#!/bin/sh

# stop on errors
set -e

# rebuild uspi USB lib
WD=$(pwd)
cd devices/rpi2/uspi/lib
make
cd $WD

NEWLIB="/usr/lib/arm-none-eabi/newlib"
GCC_OPTS=" -g -O2 -nostartfiles -nostdlib -mhard-float -ffreestanding -mno-unaligned-access -fno-toplevel-reorder -mcpu=cortex-a7 -mfpu=neon-vfpv4 -std=gnu11 -L$NEWLIB/fpu -I./sledge -I. -I/usr/include/newlib -Idevices/rpi2 -Idevices/rpi2/uspi/env/include/ -DCPU_ARM "

COMPILE="arm-none-eabi-gcc $GCC_OPTS"

mkdir -p obj
$COMPILE -c -o obj/alloc.o  sledge/alloc.c
$COMPILE -c -o obj/reader.o sledge/reader.c
$COMPILE -c -o obj/strmap.o sledge/strmap.c
$COMPILE -c -o obj/writer.o sledge/writer.c
$COMPILE -c -o obj/stream.o sledge/stream.c
$COMPILE -c -o obj/raspi.o  devices/rpi2/raspi.c
$COMPILE -c -o obj/r3d.o  devices/rpi2/r3d.c
$COMPILE -c -o obj/debug_util.o  devices/debug_util.c
$COMPILE -c -o obj/uspi_glue.o  devices/rpi2/uspi_glue.c

$COMPILE -c -o obj/timer2.o  devices/rpi2/rpi-boot/timer.c
$COMPILE -c -o obj/block.o  devices/rpi2/rpi-boot/block.c
$COMPILE -c -o obj/mbr.o  devices/rpi2/rpi-boot/mbr.c
$COMPILE -c -o obj/emmc.o  devices/rpi2/rpi-boot/emmc.c
$COMPILE -c -o obj/mbox.o  devices/rpi2/rpi-boot/mbox.c
$COMPILE -c -o obj/block_cache.o  devices/rpi2/rpi-boot/block_cache.c
$COMPILE -c -o obj/ccsbcs.o  devices/rpi2/fat/ccsbcs.c
$COMPILE -c -o obj/ff.o  devices/rpi2/fat/ff.c
$COMPILE -c -o obj/diskio.o  devices/rpi2/fat/diskio.c

$COMPILE -c -o obj/timer.o            devices/rpi2/uspi/env/lib/timer.c
$COMPILE -c -o obj/interrupt.o        devices/rpi2/uspi/env/lib/interrupt.c
$COMPILE -c -o obj/memio.o            devices/rpi2/uspi/env/lib/memio.c
$COMPILE -c -o obj/assert.o           devices/rpi2/uspi/env/lib/assert.c
$COMPILE -c -o obj/bcmpropertytags.o  devices/rpi2/uspi/env/lib/bcmpropertytags.c
$COMPILE -c -o obj/bcmmailbox.o       devices/rpi2/uspi/env/lib/bcmmailbox.c
$COMPILE -c -o obj/exceptionstub.o    devices/rpi2/uspi/env/lib/exceptionstub.S
$COMPILE -c -o obj/exceptionhandler.o devices/rpi2/uspi/env/lib/exceptionhandler.c

$COMPILE -o build/interim-arm.elf -T devices/rpi2/arm.ld devices/rpi2/arm_start.S devices/rpi2/main_rpi2.c\
         obj/raspi.o obj/r3d.o\
         obj/reader.o obj/strmap.o obj/alloc.o obj/writer.o obj/stream.o\
         obj/debug_util.o\
         devices/rpi2/uspi/lib/libuspi.a\
         \
         obj/timer2.o\
         obj/block.o\
         obj/emmc.o\
         obj/mbox.o\
         obj/ff.o\
         obj/diskio.o\
         obj/block_cache.o\
         obj/ccsbcs.o\
         \
         obj/timer.o\
         obj/interrupt.o\
         obj/memio.o\
         obj/assert.o\
         obj/bcmpropertytags.o\
         obj/bcmmailbox.o\
         obj/exceptionstub.o\
         obj/exceptionhandler.o\
         \
         obj/uspi_glue.o\
         -lc -lgcc -lm

# extract binary image from ELF executable
arm-none-eabi-objcopy build/interim-arm.elf -O binary build/kernel7.img
