
gcc -g -o sledge --std=gnu99 -I. sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/posixfs.c ../devices/sdl2.c -lm -lSDL2 -DCPU_X86 -DDEV_SDL -DDEV_POSIXFS

