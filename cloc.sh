#!/bin/sh
# calculate the number of code lines of the OS

cloc sledge/alloc* sledge/compiler_arm_new* sledge/jit_arm* sledge/minilisp* sledge/reader* sledge/sledge.c sledge/stream* sledge/strmap* sledge/writer* os/libc* os/mini-ip* devices/rpi2/arm_start.S devices/rpi2/dev_eth* devices/rpi2/fatfs* devices/rpi2/main* devices/rpi2/mmu* devices/rpi2/raspi* devices/rpi2/uart* devices/rpi2/uspi_glue*    devices/rpi2/rpi-boot/fat* devices/rpi2/rpi-boot/emmc* devices/rpi2/rpi-boot/block* devices/rpi2/rpi-boot/libfs* devices/rpi2/rpi-boot/vfs* devices/rpi2/rpi-boot/timer* devices/rpi2/rpi-boot/mbox* devices/rpi2/rpi-boot/mbr*   devices/rpi2/uspi/env/lib/timer.c devices/rpi2/uspi/env/lib/interrupt.c devices/rpi2/uspi/env/lib/memio.c devices/rpi2/uspi/env/lib/assert.c devices/rpi2/uspi/env/lib/bcmpropertytags.c devices/rpi2/uspi/env/lib/bcmmailbox.c devices/rpi2/uspi/env/lib/exceptionstub.S devices/rpi2/uspi/env/lib/exceptionhandler.c devices/rpi2/uspi/lib/*.c

cloc --force-lang="Lisp",l sledge/tests/shell.l sledge/tests/editlite.l
