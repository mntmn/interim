sudo mount /dev/sdb1 /1/
sudo cp ./build/kernel7.img /1/
sudo cp ./bjos/rootfs/*.l /1/
sudo sync
sudo umount /dev/sdb1
sudo sync
