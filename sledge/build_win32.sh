#!/bin/sh

LIBS="-I /home/mntmn/code/sdlcross/SDL2-2.0.3/i686-w64-mingw32/include -L /home/mntmn/code/sdlcross/SDL2-2.0.3/i686-w64-mingw32/lib"

i686-w64-mingw32-gcc -m32 -g -o sledge.exe --std=gnu99 -I. ${CFLAGS} sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/posixfs.c ../devices/sdl2.c -lm $LIBS -lSDL2 -DCPU_X86 -DDEV_POSIXFS -DDEV_SDL

