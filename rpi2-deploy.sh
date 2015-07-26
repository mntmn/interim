sudo mount /dev/sdb1 /1/
sudo cp ./build/kernel7.img /1/
sudo cp ./sledge/tests/font.l /1/
sudo cp ./os/rootfs/unifont.565 /1/
sudo sync
sudo umount /dev/sdb1
sudo sync
