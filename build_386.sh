gcc -T devices/bios.ld -o build/interim.bin --std=gnu99 -m32  -fno-asynchronous-unwind-tables -nostartfiles -nostdlib -ffreestanding -I/usr/include/newlib devices/bios.S devices/bios.c -DCPU_X86 -Isledge sledge/sledge.c sledge/reader.c sledge/writer.c sledge/alloc.c sledge/strmap.c sledge/stream.c -lgcc /home/mntmn/code/newlib/newlib/i686-elf/32/newlib/libc.a /home/mntmn/code/newlib/newlib/i686-elf/32/newlib/libm.a

#gcc -T devices/bios.ld -m32 -o build/interim.bin -ffreestanding -nostdlib build/interim.o

