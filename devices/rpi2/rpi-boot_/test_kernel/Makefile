ARM-CC ?= arm-none-eabi-gcc
ARM-LD ?= arm-none-eabi-ld

all: test.elf

CFLAGS := -nostdlib -nostartfiles -ffreestanding -g -I.. -std=c99

OBJS = loader.o main.o

.PHONY: clean

test.elf: $(OBJS) linker.ld
	$(ARM-CC) -nostdlib $(OBJS) -Wl,-T,linker.ld -Wl,-z,max-page-size=0x1000 -o $@ -lgcc

clean:
	$(RM) -f $(OBJS) test.elf

%.o: %.c Makefile
	$(ARM-CC) $(CFLAGS) -c $< -o $@

%.o: %.s Makefile
	$(ARM-CC) $(ASFLAGS) -c $< -o $@

