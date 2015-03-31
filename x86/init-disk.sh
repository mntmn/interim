#!/bin/sh
cd bin

echo Formatting Disk Image
dd if=/dev/zero of=./bmfs.image bs=1M count=128
./bmfs bmfs.image format /force
echo Writing Master Boot Record
dd if=bmfs_mbr.sys of=bmfs.image bs=512 conv=notrunc

echo Writing Pure64
cat pure64.sys kernel64.sys > software.sys
dd if=software.sys of=bmfs.image bs=512 seek=16 conv=notrunc
