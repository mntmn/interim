export CROSSPATH=/media/storage2/amiga-code/m68k-amigaos
export GCCLIBPATH=$CROSSPATH/lib/gcc-lib/m68k-amigaos/2.95.3
export PATH=$CROSSPATH/bin:$GCCLIBPATH:$PATH
export GCC_EXEC_PREFIX=m68k-amigaos
export LIBS=$CROSSPATH/lib

m68k-amigaos-gcc -o build/interim.amiga --std=c9x -noixemul -I$CROSSPATH/m68k-amigaos/sys-include -I$CROSSPATH/os-include  -Isledge -L$LIBS/libnix -L$LIBS -L$LIBS/gcc-lib/m68k-amigaos/2.95.3/ -DCPU_M86K sledge/strmap.c sledge/reader.c sledge/alloc.c sledge/writer.c sledge/sledge.c sledge/stream.c devices/debug_util.c devices/amiga.c devices/posixfs.c $LIBS/libnix/swapstack.o

cp build/interim.amiga ~/amiga/interim/

#m68k-amigaos-gcc -o build/test --std=c9x -noixemul -I$CROSSPATH/m68k-amigaos/sys-include -Isledge -L$LIBS/libnix -L$LIBS -L$LIBS/gcc-lib/m68k-amigaos/2.95.3/ -DCPU_M86K test.c

