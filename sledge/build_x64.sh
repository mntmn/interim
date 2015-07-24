gcc -g -o sledge --std=gnu99 -I. sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/sdl2.c -lm -lSDL2 -DCPU_X64 -DDEV_SDL2

