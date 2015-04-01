
gcc -g -o sledge --std=c99 -I ./lightning/include -L ./lightning/build/lib -I ./udis86/libudis86 sledge.c reader.c writer.c alloc.c blit.c sdl.c -llightning -lSDL2 -lm

#  ./udis86/libudis86/decode.c ./udis86/libudis86/itab.c  ./udis86/libudis86/udis86.c 
