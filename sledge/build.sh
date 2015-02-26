
gcc -g -o sledge --std=c99 -I ./lightning/include -L./lightning/build/lib/ sledge.c reader.c writer.c alloc.c blit.c sdl.c -llightning -lSDL2
