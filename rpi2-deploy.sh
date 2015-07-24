sudo mount /dev/sdb1 /1/
sudo cp ./build/kernel7.img /1/
sudo cp ./sledge/tests/gfxtest4.l /1/
sudo sync
sudo umount /dev/sdb1
sudo sync
