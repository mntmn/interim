#!/bin/sh

# -mtune=cortex-a7 -mfpu=neon-vfpv4  -ftree-vectorize

NEWLIB="/usr/lib/arm-none-eabi/newlib"
# -fcall-saved-r1 -fcall-saved-r2 -fcall-saved-r3 
set -e
GCC_OPTS=" -g -O2 -nostartfiles -nostdlib -mhard-float -ffreestanding -mno-unaligned-access -fno-toplevel-reorder -mcpu=cortex-a7 -mfpu=neon-vfpv4 -std=gnu11 -L$NEWLIB/fpu -I./sledge -I. -I/usr/include/newlib -Idevices/rpi2/uspi/env/include/ -DCPU_ARM "

COMPILE="arm-none-eabi-gcc $GCC_OPTS"

mkdir -p obj
$COMPILE -c -o obj/alloc.o  sledge/alloc.c
$COMPILE -c -o obj/reader.o sledge/reader.c
$COMPILE -c -o obj/strmap.o sledge/strmap.c
$COMPILE -c -o obj/writer.o sledge/writer.c
$COMPILE -c -o obj/stream.o sledge/stream.c
$COMPILE -c -o obj/raspi.o  devices/rpi2/raspi.c
$COMPILE -c -o obj/r3d.o  devices/rpi2/r3d.c
$COMPILE -c -o obj/debug_util.o  os/debug_util.c
$COMPILE -c -o obj/uspi_glue.o  devices/rpi2/uspi_glue.c

$COMPILE -c -o obj/timer2.o  devices/rpi2/rpi-boot/timer.c
#$COMPILE -c -o obj/vfs.o  devices/rpi2/rpi-boot/vfs.c
$COMPILE -c -o obj/libfs.o  devices/rpi2/rpi-boot/libfs.c
$COMPILE -c -o obj/block.o  devices/rpi2/rpi-boot/block.c
$COMPILE -c -o obj/mbr.o  devices/rpi2/rpi-boot/mbr.c
$COMPILE -c -o obj/fat.o  devices/rpi2/rpi-boot/fat.c
$COMPILE -c -o obj/emmc.o  devices/rpi2/rpi-boot/emmc.c
$COMPILE -c -o obj/mbox.o  devices/rpi2/rpi-boot/mbox.c

$COMPILE -c -o obj/timer.o            devices/rpi2/uspi/env/lib/timer.c
$COMPILE -c -o obj/interrupt.o        devices/rpi2/uspi/env/lib/interrupt.c
$COMPILE -c -o obj/memio.o            devices/rpi2/uspi/env/lib/memio.c
$COMPILE -c -o obj/assert.o           devices/rpi2/uspi/env/lib/assert.c
$COMPILE -c -o obj/bcmpropertytags.o  devices/rpi2/uspi/env/lib/bcmpropertytags.c
$COMPILE -c -o obj/bcmmailbox.o       devices/rpi2/uspi/env/lib/bcmmailbox.c
$COMPILE -c -o obj/exceptionstub.o    devices/rpi2/uspi/env/lib/exceptionstub.S
$COMPILE -c -o obj/exceptionhandler.o devices/rpi2/uspi/env/lib/exceptionhandler.c

#arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/unifont  obj/unifont.o
#arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  bjos/rootfs/editor.l obj/editor.o

$COMPILE -o build/interim-arm.elf -T devices/rpi2/arm.ld devices/rpi2/arm_start.S devices/rpi2/main_rpi2.c\
         obj/raspi.o obj/r3d.o\
         obj/reader.o obj/strmap.o obj/alloc.o obj/writer.o obj/stream.o\
         obj/debug_util.o\
         devices/rpi2/uspi/lib/libuspi.a\
         \
         obj/timer2.o\
         obj/libfs.o\
         obj/block.o\
         obj/mbr.o\
         obj/fat.o\
         obj/emmc.o\
         obj/mbox.o\
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

arm-none-eabi-objcopy build/interim-arm.elf -O binary build/kernel7.img
