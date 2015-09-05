sudo mount /dev/sdb1 /1/
sudo cp ./build/kernel7.img /1/
sudo cp ./sledge/unifont.bin /1/
sudo cp ./sledge/tests/shell.l /1/
sudo cp ./sledge/tests/editlite.l /1/
sudo cp ./sledge/tests/gfx.l /1/
sudo sync
sudo umount /dev/sdb1
sudo sync
