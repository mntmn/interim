#
# Makefile
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

USPIHOME = ../..

AFLAGS	+= -I ../include
CFLAGS	+= -I ../include

OBJS	= alloc.o assert.o bcmmailbox.o bcmframebuffer.o bcmpropertytags.o chargenerator.o debug.o \
	  delayloop.o exceptionstub.o interrupt.o libhelper.o libstub.o logger.o memio.o screen.o \
	  string.o synchronize.o sysinit.o timer.o uspibind.o uspienv.o util.o util_fast.o \
	  memory.o pagetable.o exceptionhandler.o

all: startup.o libuspienv.a

startup.o: startup.S

libuspienv.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(USPIHOME)/Rules.mk
