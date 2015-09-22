cp ./build/kernel7.img ./release-rpi2/
cp ./sledge/unifont.bin ./release-rpi2/
cp ./sledge/os/lib.l ./release-rpi2/
cp ./sledge/os/gfx.l ./release-rpi2/
cp ./sledge/os/shell.l ./release-rpi2/
cp ./sledge/os/editor.l ./release-rpi2/
cp ./sledge/os/paint.l ./release-rpi2/
cp ./sledge/tests/gtn.l ./release-rpi2/
rm docs/interim-0.1.0-rpi2.tgz
tar cfz docs/interim-0.1.0-rpi2.tgz ./release-rpi2
