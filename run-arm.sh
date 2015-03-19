#!/bin/bash

/opt/qemu-rpi/bin/qemu-system-arm -M raspi -cpu arm1176 -m 512 -no-reboot -serial stdio -kernel ./bomberjacket-arm.elf

