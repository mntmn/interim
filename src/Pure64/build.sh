#!/bin/bash

nasm src/bootsectors/bmfs_mbr.asm -o bmfs_mbr.sys
nasm src/bootsectors/pxestart.asm -o pxestart.sys
cd src
nasm pure64.asm -o ../pure64.sys
cd ..
