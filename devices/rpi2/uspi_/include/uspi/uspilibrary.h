//
// uspilibrary.h
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
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
#ifndef _uspi_uspilibrary_h
#define _uspi_uspilibrary_h

#include <uspi/devicenameservice.h>
#include <uspi/dwhcidevice.h>
#include <uspi/usbkeyboard.h>
#include <uspi/usbmouse.h>
#include <uspi/usbgamepad.h>
#include <uspi/usbmassdevice.h>
#include <uspi/smsc951x.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DEVICES	4

typedef struct TUSPiLibrary
{
	TDeviceNameService		 NameService;
	TDWHCIDevice			 DWHCI;
	TUSBKeyboardDevice		*pUKBD1;
	TUSBMouseDevice			*pUMouse1;
	TUSBBulkOnlyMassStorageDevice	*pUMSD[MAX_DEVICES];
	TSMSC951xDevice			*pEth0;
	TUSBGamePadDevice       	*pUPAD[MAX_DEVICES];
}
TUSPiLibrary;

#ifdef __cplusplus
}
#endif

#endif
