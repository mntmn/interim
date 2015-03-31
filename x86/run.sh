#!/bin/sh

cd bin
qemu-system-x86_64 --enable-kvm -monitor stdio -vga std -smp 2 -m 256 -device ahci,id=ahci -drive id=disk,file=bmfs.image,if=none -device ide-drive,drive=disk,bus=ahci.0 -name "Bomber Jacket OS" -netdev bridge,id=br0  -device e1000-82544gc,netdev=br0,mac=52:54:de:ad:be:ef,id=nd0

# -net nic,model=rtl8139 

# -netdev tap,helper=/usr/local/libexec/qemu-bridge-helper,id=net0
# ,macaddr=52:54:00:12:be:ef 
# sysctl net.ipv4.ip_forward=1
# e1000-82544gc,e1000-82544gc,
