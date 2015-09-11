#!/bin/sh
mkdir -p build/386iso/isodir
mkdir -p build/386iso/boot
cp build/interim.bin build/386iso/boot/interim.386
mkdir -p build/386iso/boot/grub
cp devices/grub.cfg build/386iso/boot/grub/grub.cfg
grub-mkrescue -o interim386.iso build/386iso

