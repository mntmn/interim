gcc -g -o sledge --std=gnu99 -I. sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/sdl.c ../devices/posixfs.c -lm -lSDL -DCPU_X64 -DDEV_SDL -DDEV_POSIXFS

