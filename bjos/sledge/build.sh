
gcc -g -o sledge --std=gnu99 sledge.c reader.c writer.c alloc.c strmap.c stream.c sdl2.c dev_linuxfb.c -lm -lSDL2 -DCPU_X64 -DDEV_SDL2

