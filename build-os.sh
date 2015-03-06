#!/bin/sh

GCC_OPTS="-m64 -g -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone"
OPTIMIZE="-O3"

COMPILE="gcc -std=c99 $GCC_OPTS -I newlib/newlib-2.2.0/newlib/libc/include/ -I./lightning/lightning -I./lightning/"

if [ $1 ]
then
   mkdir -p obj
   $COMPILE -c $OPTIMIZE -o obj/lightning.o  lightning/lightning.c 
   $COMPILE -c $OPTIMIZE -o obj/jit_names.o  lightning/jit_names.c
   $COMPILE -c $OPTIMIZE -o obj/jit_note.o   lightning/jit_note.c
   $COMPILE -c $OPTIMIZE -o obj/jit_size.o   lightning/jit_size.c
   $COMPILE -c $OPTIMIZE -o obj/jit_memory.o lightning/jit_memory.c

   $COMPILE -c $OPTIMIZE -o obj/glue.o   lightning/glue.c
   $COMPILE -c $OPTIMIZE -o obj/reader.o sledge/reader.c
   $COMPILE -c $OPTIMIZE -o obj/alloc.o  sledge/alloc.c
   $COMPILE -c $OPTIMIZE -o obj/blit.o   sledge/blit.c
   $COMPILE -c $OPTIMIZE -o obj/writer.o sledge/writer.c
fi

#gcc -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o libBareMetal.o libBareMetal.c

$COMPILE $OPTIMIZE -o lightning.app -g -T newlib/app.ld newlib/crt0.o sledge/sledge_x86_bare.c obj/lightning.o obj/jit_names.o obj/jit_note.o obj/jit_size.o obj/jit_memory.o obj/glue.o obj/reader.o obj/alloc.o obj/blit.o obj/writer.o newlib/libc.a newlib/libBareMetal.o

#ld -T newlib/app.ld -o lightning.app newlib/crt0.o newlib/libc.a lightning.o jit_names.o jit_note.o jit_size.o glue.o newlib/libBareMetal.o

cp sledge/editor.l bin/editor.l
cp sledge/fs/keymap-bm bin/keymap
cp sledge/fs/unifont bin/
cp sledge/fs/buffer bin/
cp sledge/goa.l bin/goa
mv lightning.app bin/l
if [ $? -eq 0 ]; then
cd bin
./bmfs bmfs.image create l 2
./bmfs bmfs.image write l
./bmfs bmfs.image create editor.l 1
./bmfs bmfs.image write editor.l
./bmfs bmfs.image create keymap 1
./bmfs bmfs.image write keymap
./bmfs bmfs.image create unifont 5
./bmfs bmfs.image write unifont
./bmfs bmfs.image create buffer 1
./bmfs bmfs.image write buffer
./bmfs bmfs.image create goa 1
./bmfs bmfs.image write goa
cd ..
else
echo "Error"
fi
