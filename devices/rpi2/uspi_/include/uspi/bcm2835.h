//
// bcm2835.h
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _uspi_bcm2835_h
#define _uspi_bcm2835_h

#include <uspios.h>

#if RASPPI == 1
#define ARM_IO_BASE		0x20000000
#else
#define ARM_IO_BASE		0x3F000000
#endif

#define GPU_IO_BASE		0x7E000000

#define GPU_CACHED_BASE		0x40000000
#define GPU_UNCACHED_BASE	0xC0000000

#if RASPPI == 1
	#ifdef GPU_L2_CACHE_ENABLED
		#define GPU_MEM_BASE	GPU_CACHED_BASE
	#else
		#define GPU_MEM_BASE	GPU_UNCACHED_BASE
	#endif
#else
	#define GPU_MEM_BASE	GPU_UNCACHED_BASE
#endif

//
// USB Host Controller
//
#define ARM_USB_BASE		(ARM_IO_BASE + 0x980000)

#define ARM_USB_CORE_BASE	ARM_USB_BASE
#define ARM_USB_HOST_BASE	(ARM_USB_BASE + 0x400)
#define ARM_USB_POWER		(ARM_USB_BASE + 0xE00)

#endif
