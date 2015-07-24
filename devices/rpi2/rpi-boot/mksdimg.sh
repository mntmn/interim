dd if=/dev/zero bs=1k count=65536 of=sd.img
echo ';;b;;' | sfdisk sd.img
sudo losetup -o512 /dev/loop0 sd.img
sudo mkfs.vfat /dev/loop0 64259

