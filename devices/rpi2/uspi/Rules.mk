#
# Rules.mk
#
# USPi - An USB driver for Raspberry Pi written in C
# Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

ifeq ($(strip $(USPIHOME)),)
USPIHOME = ..
endif

-include $(USPIHOME)/Config.mk

RASPPI	?= 2
PREFIX	?= arm-none-eabi-

CC	= $(PREFIX)gcc
AS	= $(CC)
LD	= $(PREFIX)ld
AR	= $(PREFIX)ar

ifeq ($(strip $(RASPPI)),1)
ARCH	?= -march=armv6j -mtune=arm1176jzf-s -mfloat-abi=hard 
else
ARCH	?= -march=armv7-a -mtune=cortex-a7 -mfloat-abi=hard
endif

AFLAGS	+= $(ARCH) -DRASPPI=$(RASPPI)
CFLAGS	+= $(ARCH) -Wall -Wno-psabi -fsigned-char -fno-builtin -nostdinc -nostdlib \
	   -std=gnu99 -undef -DRASPPI=$(RASPPI) -I $(USPIHOME)/include -O #-DNDEBUG

%.o: %.S
	$(AS) $(AFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.a *.elf *.lst *.img *.cir *.map *~ $(EXTRACLEAN)
