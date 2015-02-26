#!/bin/sh

cd bin
echo Writing Pure64+Software
cat pure64.sys kernel64.sys > software.sys
dd if=software.sys of=bmfs.image bs=512 seek=16 conv=notrunc
