# BMFS

Utility for accessing a disk or disk image formatted with BareMetal File System (BMFS).


## Creating a new, formatted disk image

    bmfs disk.image initialize 128M


## Creating a new disk image that boots BareMetal OS

    bmfs disk.image initialize 128M path/to/bmfs_mbr.sys path/to/pure64.sys path/to/kernel64.sys

or if the Pure64 boot loader and BareMetal-OS kernel are combined into one file:

    bmfs disk.image initialize 128M path/to/bmfs_mbr.sys path/to/software.sys


## Formatting a disk image

	bmfs disk.image format

In Linux/Unix/Mac OS X you can also format a physical drive by passing the correct path.

	sudo bmfs /dev/sdc format


## Display BMFS disk contents

	bmfs disk.image list

Sample output:

	C:\baremetal>utils\bmfs BMFS-256-flat.vmdk list
	BMFS-256-flat.vmdk
	Disk Size: 256 MiB
	Name                            |            Size (B)|      Reserved (MiB)
	==========================================================================
	test.app                                           31                    2
	Another file.app                                    1                    2
	helloc.app                                        800                    2


## Create a new file and reserve space for it

	bmfs disk.image create FileName.Ext

You will be prompted for the size to reserve.

Alternately, you can specify the reserved size after the file name. The reserved size is given in Megabytes and will automatically round up to an even number.

	bmfs disk.image create FileName.Ext 4


## Read from BMFS to a local file

	bmfs disk.image read FileName.Ext


## Write a local file to BMFS

	bmfs disk.image write FileName.Ext


## Delete a file on BMFS

	bmfs disk.image delete FileName.Ext

