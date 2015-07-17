set -e
gcc --std=gnu99 rpi_sdl_test.c -lSDL
./a.out
objdump -D -b binary -m armv5 /tmp/test

