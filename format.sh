#!/bin/sh
cd bin
echo Formatting Disk Image
./bmfs bmfs.image format /force
echo Writing Master Boot Record
dd if=bmfs_mbr.sys of=bmfs.image bs=512 conv=notrunc
