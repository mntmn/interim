#!/bin/sh
cd src/BareMetal-OS/programs/
gcc -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o $1.o $1.c
gcc -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o libBareMetal.o libBareMetal.c
ld -T app.ld -o $1.app $1.o libBareMetal.o
mv $1.app ../../../bin/
if [ $? -eq 0 ]; then
cd ../../../bin
./bmfs bmfs.image create $1.app 2
./bmfs bmfs.image write $1.app
cd ..
else
echo "Error"
fi
