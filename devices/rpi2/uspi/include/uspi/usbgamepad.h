//
// usbgamepad.h
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
// Copyright (C) 2014  M. Maccaferri <macca@maccasoft.com>
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
#ifndef _usbgamepad_h
#define _usbgamepad_h

#include <uspi/usbdevice.h>
#include <uspi/usbendpoint.h>
#include <uspi/usbrequest.h>
#include <uspi/usbhid.h>
#include <uspi/types.h>
#include <uspi.h>

typedef struct TUSBGamePadDevice
{
	TUSBDevice m_USBDevice;
	unsigned m_nDeviceIndex;

	u8 m_ucInterfaceNumber;
	u8 m_ucAlternateSetting;

	TUSBEndpoint *m_pEndpointIn;
    TUSBEndpoint *m_pEndpointOut;

	USPiGamePadState   m_State;
	TGamePadStatusHandler *m_pStatusHandler;

	u16 m_usReportDescriptorLength;
    u8 *m_pHIDReportDescriptor;

	TUSBRequest *m_pURB;
	u8 *m_pReportBuffer;
	u16 m_nReportSize;
}
TUSBGamePadDevice;

void USBGamePadDevice (TUSBGamePadDevice *pThis, TUSBDevice *pDevice);
void _CUSBGamePadDevice (TUSBGamePadDevice *pThis);

boolean USBGamePadDeviceConfigure (TUSBDevice *pUSBDevice);

void USBGamePadDeviceGetReport (TUSBGamePadDevice *pThis);
void USBGamePadDeviceRegisterStatusHandler (TUSBGamePadDevice *pThis, TGamePadStatusHandler *pStatusHandler);

#endif
