#!/bin/sh

if [ ! -d "$src" ]; then
  mkdir src
fi
cd src
git clone https://github.com/ReturnInfinity/BMFS.git
git clone https://github.com/ReturnInfinity/Pure64.git
git clone https://github.com/ReturnInfinity/BareMetal-OS.git
cd ..

if [ ! -d "$bin" ]; then
  mkdir bin
fi
platform=`uname`
case "${platform}" in
  Darwin)
    dd if=/dev/zero of=bin/bmfs.image bs=1m count=128
    ;;
  *)
    dd if=/dev/zero of=bin/bmfs.image bs=1M count=128
    ;;
esac

./build.sh
./format.sh
./install.sh
