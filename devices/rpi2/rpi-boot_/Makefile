ARMCC ?= arm-none-eabi-gcc
MAKEFILE = Makefile.rpi-boot
MAKEFILE_IN = $(MAKEFILE).in

all: kernel.img

.PHONY: clean kernel.img libfs.a qemu qemu-gdb dump

$(MAKEFILE): $(MAKEFILE_IN) config.h Makefile
	$(ARMCC) -P -traditional-cpp -std=gnu99 -E -o $(MAKEFILE) -x c $(MAKEFILE_IN)

clean: $(MAKEFILE)
	$(MAKE) -f $(MAKEFILE) clean

kernel.img: $(MAKEFILE)
	$(MAKE) -f $(MAKEFILE) kernel.img

libfs.a: $(MAKEFILE)
	$(MAKE) -f $(MAKEFILE) libfs.a

qemu: $(MAKEFILE)
	$(MAKE) -f $(MAKEFILE) qemu

qemu-gdb: $(MAKEFILE)
	$(MAKE) -f $(MAKEFILE) qemu-gdb

dump: $(MAKEFILE)
	$(MAKE) -f $(MAKEFILE) dump

raspbootin-server: raspbootin-server.c crc32.c
	$(CC) -g -std=c99 -o $@ $^
