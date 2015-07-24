//
// usbstring.h
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
#ifndef _uspi_usbstring_h
#define _uspi_usbstring_h

#include <uspi/usb.h>
#include <uspi/string.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TUSBDevice;

typedef struct TUSBString
{
	struct TUSBDevice *m_pDevice;

	TUSBStringDescriptor *m_pUSBString;

	TString *m_pString;
}
TUSBString;

void USBString (TUSBString *pThis, struct TUSBDevice *pDevice);
void USBStringCopy (TUSBString *pThis, TUSBString *pParent);
void _USBString (TUSBString *pThis);

boolean USBStringGetFromDescriptor (TUSBString *pThis, u8 ucID, u16 usLanguageID);

const char *USBStringGet (TUSBString *pThis);

u16 USBStringGetLanguageID (TUSBString *pThis);

#ifdef __cplusplus
}
#endif

#endif
