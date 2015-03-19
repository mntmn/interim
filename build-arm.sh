#!/bin/sh

set -e
# 
#  -mfpu=vfp -mfloat-abi=hard
GCC_OPTS=" -O2 -g -nostartfiles -nostdlib -msoft-float -ffreestanding -march=armv6zk -mcpu=arm1176jzf-s -std=c99 -L/home/mntmn/code/newlib/arm-none-eabi/lib/armv6-m -I/home/mntmn/code/newlib/arm-none-eabi/include "
OPTIMIZE=""

COMPILE="arm-none-eabi-gcc $GCC_OPTS -I. -I./lightning/lightning -I./lightning/"

if [ $1 ]
then
   mkdir -p obj
   $COMPILE -c $OPTIMIZE -o obj/lightning.o  lightning/lightning.c 
   $COMPILE -c $OPTIMIZE -o obj/jit_names.o  lightning/jit_names.c
   $COMPILE -c $OPTIMIZE -o obj/jit_note.o   lightning/jit_note.c
   $COMPILE -c $OPTIMIZE -o obj/jit_size.o   lightning/jit_size.c
   $COMPILE -c $OPTIMIZE -o obj/jit_memory.o lightning/jit_memory.c

   $COMPILE -c $OPTIMIZE -o obj/glue.o   lightning/glue.c
   $COMPILE -c $OPTIMIZE -o obj/reader.o sledge/reader.c
   $COMPILE -c $OPTIMIZE -o obj/alloc.o  sledge/alloc.c
   $COMPILE -c $OPTIMIZE -o obj/blit.o   sledge/blit.c
   $COMPILE -c $OPTIMIZE -o obj/writer.o sledge/writer.c
fi

# arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  sledge/fs/unifont unifont.o

arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm  editor-arm.l editor.o

#NEWLIB="./newlib/newlib-2.2.0/arm-none-eabi/newlib"

#$COMPILE -c -o obj/sledge_arm_bare.o sledge/sledge_arm_bare.c

#$COMPILE -o bomberjacket-arm.elf sledge/sledge_arm_bare.c obj/lightning.o obj/jit_names.o obj/jit_note.o obj/jit_size.o obj/jit_memory.o obj/glue.o obj/reader.o obj/alloc.o obj/blit.o obj/writer.o

$COMPILE -o bomberjacket-arm.elf sledge/arm_start.S sledge/sledge_arm_bare.c raspi.c -T arm.ld  obj/lightning.o obj/jit_names.o obj/jit_note.o obj/jit_size.o obj/jit_memory.o obj/glue.o obj/reader.o obj/alloc.o obj/blit.o obj/writer.o -lc -lgcc unifont.o editor.o

arm-none-eabi-objcopy bomberjacket-arm.elf -O binary kernel.img

# (blit-mono unifont 0 0 516 516 1000 0 0 0xffffff)
