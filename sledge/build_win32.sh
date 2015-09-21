#!/bin/sh

i686-w64-mingw32-gcc -m32 -g -o sledge.exe --std=gnu99 -I. ${CFLAGS} sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/posixfs.c -lm -DCPU_X86 -DDEV_POSIXFS

