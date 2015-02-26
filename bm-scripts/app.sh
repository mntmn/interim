#!/bin/sh
cd src/BareMetal-OS/programs/
nasm $1.asm -o ../../../bin/$1.app
if [ $? -eq 0 ]; then
cd ../../../bin
./bmfs bmfs.image create $1.app 2
./bmfs bmfs.image write $1.app
cd ..
else
echo "Error"
fi
