
gcc -g -o sledge --std=gnu99 -I. sledge.c reader.c writer.c alloc.c strmap.c stream.c ../devices/linux/dev_linuxfb.c -lm -DCPU_ARM -DDEV_LINUXFB

