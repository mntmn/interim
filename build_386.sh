gcc -g -o build/interim.o --std=gnu99 -m32 -nostartfiles -nostdlib -ffreestanding devices/bios.S devices/bios.c -DCPU_X86 -lgcc

#gcc -g -o build/interim.o --std=gnu99 -m32 -nostartfiles -nostdlib -ffreestanding -Isledge sledge/sledge.c sledge/reader.c sledge/writer.c sledge/alloc.c sledge/strmap.c sledge/stream.c devices/bios.c devices/bios.S -DCPU_X86 ../dietlibc/bin-i386/dietlibc.a ../dietlibc/bin-i386/libm.a -lgcc

ld -T devices/bios.ld -m elf_i386 --build-id=none -o build/interim.bin -nostdlib build/interim.o

