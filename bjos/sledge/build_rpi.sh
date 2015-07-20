
gcc -g -o sledge --std=gnu99 sledge.c reader.c writer.c alloc.c strmap.c stream.c sdl.c dev_linuxfb.c -lm -lSDL -DCPU_ARM -DDEV_LINUXFB

