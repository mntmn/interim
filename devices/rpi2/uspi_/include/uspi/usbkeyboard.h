//
// usbkeyboard.h
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
#ifndef _usbkeyboard_h
#define _usbkeyboard_h

#include <uspi/usbdevice.h>
#include <uspi/usbendpoint.h>
#include <uspi/usbrequest.h>
#include <uspi/keymap.h>
#include <uspi/types.h>

#define BOOT_REPORT_SIZE	8

typedef void TKeyPressedHandler (const char *pString);
typedef void TSelectConsoleHandler (unsigned nConsole);
typedef void TShutdownHandler (void);

// The raw handler is called when the keyboard sends a status report (on status change and/or continously).
typedef void TKeyStatusHandlerRaw (unsigned char	ucModifiers,	// see usbhid.h
				   const unsigned char  RawKeys[6]);	// key code or 0 in each byte

typedef struct TUSBKeyboardDevice
{
	TUSBDevice m_USBDevice;

	u8 m_ucInterfaceNumber;
	u8 m_ucAlternateSetting;

	TUSBEndpoint *m_pReportEndpoint;

	TKeyPressedHandler	*m_pKeyPressedHandler;
	TSelectConsoleHandler	*m_pSelectConsoleHandler;
	TShutdownHandler	*m_pShutdownHandler;
	TKeyStatusHandlerRaw	*m_pKeyStatusHandlerRaw;

	TUSBRequest *m_pURB;
	u8 *m_pReportBuffer;

	u8 m_ucLastPhyCode;
	unsigned m_hTimer;

	TKeyMap m_KeyMap;
}
TUSBKeyboardDevice;

void USBKeyboardDevice (TUSBKeyboardDevice *pThis, TUSBDevice *pDevice);
void _CUSBKeyboardDevice (TUSBKeyboardDevice *pThis);

boolean USBKeyboardDeviceConfigure (TUSBDevice *pUSBDevice);

// cooked mode
void USBKeyboardDeviceRegisterKeyPressedHandler (TUSBKeyboardDevice *pThis, TKeyPressedHandler *pKeyPressedHandler);
void USBKeyboardDeviceRegisterSelectConsoleHandler (TUSBKeyboardDevice *pThis, TSelectConsoleHandler *pSelectConsoleHandler);
void USBKeyboardDeviceRegisterShutdownHandler (TUSBKeyboardDevice *pThis, TShutdownHandler *pShutdownHandler);

// raw mode (if this handler is registered the others are ignored)
void USBKeyboardDeviceRegisterKeyStatusHandlerRaw (TUSBKeyboardDevice *pThis, TKeyStatusHandlerRaw *pKeyStatusHandlerRaw);

#endif
