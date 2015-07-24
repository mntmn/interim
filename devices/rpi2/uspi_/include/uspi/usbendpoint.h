//
// usbendpoint.h
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
#ifndef _uspi_usbendpoint_h
#define _uspi_usbendpoint_h

#include <uspi/usb.h>
#include <uspi/usbdevice.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	EndpointTypeControl,
	EndpointTypeBulk,
	EndpointTypeInterrupt,
	EndpointTypeIsochronous
}
TEndpointType;

typedef struct TUSBEndpoint
{
	TUSBDevice	*m_pDevice;
	u8		 m_ucNumber;
	TEndpointType	 m_Type;
	boolean		 m_bDirectionIn;
	u32		 m_nMaxPacketSize;
	unsigned	 m_nInterval;			// Milliseconds
	TUSBPID		 m_NextPID;
}
TUSBEndpoint;

void USBEndpoint (TUSBEndpoint *pThis, TUSBDevice *pDevice);		// control endpoint 0
void USBEndpoint2 (TUSBEndpoint *pThis, TUSBDevice *pDevice, const TUSBEndpointDescriptor *pDesc);
void USBEndpointCopy (TUSBEndpoint *pThis, TUSBEndpoint *pEndpoint, TUSBDevice *pDevice);
void _USBEndpoint (TUSBEndpoint *pThis);

TUSBDevice *USBEndpointGetDevice (TUSBEndpoint *pThis);

u8 USBEndpointGetNumber (TUSBEndpoint *pThis);
TEndpointType USBEndpointGetType (TUSBEndpoint *pThis);
boolean USBEndpointIsDirectionIn (TUSBEndpoint *pThis);

void USBEndpointSetMaxPacketSize (TUSBEndpoint *pThis, u32 nMaxPacketSize);
u32 USBEndpointGetMaxPacketSize (TUSBEndpoint *pThis);

unsigned USBEndpointGetInterval (TUSBEndpoint *pThis);		// Milliseconds

TUSBPID USBEndpointGetNextPID (TUSBEndpoint *pThis, boolean bStatusStage);
void USBEndpointSkipPID (TUSBEndpoint *pThis, unsigned nPackets, boolean bStatusStage);
void USBEndpointResetPID (TUSBEndpoint *pThis);

#ifdef __cplusplus
}
#endif

#endif
