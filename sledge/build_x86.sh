
gcc -m32 -g -o sledge --std=gnu99 -I. sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/posixfs.c ../devices/sdl2.c -lSDL2 -DDEV_SDL -lm -DCPU_X86 -DDEV_POSIXFS

