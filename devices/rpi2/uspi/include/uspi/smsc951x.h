//
// smsc951x.h
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
#ifndef _uspi_smsc951x_h
#define _uspi_smsc951x_h

#include <uspi/usbdevice.h>
#include <uspi/usbendpoint.h>
#include <uspi/usbrequest.h>
#include <uspi/macaddress.h>
#include <uspi/types.h>

#define FRAME_BUFFER_SIZE	1600

typedef struct TSMSC951xDevice
{
	TUSBDevice m_USBDevice;

	TUSBEndpoint *m_pEndpointBulkIn;
	TUSBEndpoint *m_pEndpointBulkOut;

	TMACAddress m_MACAddress;

	u8 *m_pTxBuffer;
}
TSMSC951xDevice;

void SMSC951xDevice (TSMSC951xDevice *pThis, TUSBDevice *pDevice);
void _SMSC951xDevice (TSMSC951xDevice *pThis);

boolean SMSC951xDeviceConfigure (TUSBDevice *pUSBDevice);

TMACAddress *SMSC951xDeviceGetMACAddress (TSMSC951xDevice *pThis);

boolean SMSC951xDeviceSendFrame (TSMSC951xDevice *pThis, const void *pBuffer, unsigned nLength);

// pBuffer must have size FRAME_BUFFER_SIZE
boolean SMSC951xDeviceReceiveFrame (TSMSC951xDevice *pThis, void *pBuffer, unsigned *pResultLength);

// private:
boolean SMSC951xDeviceWriteReg (TSMSC951xDevice *pThis, u32 nIndex, u32 nValue);
boolean SMSC951xDeviceReadReg (TSMSC951xDevice *pThis, u32 nIndex, u32 *pValue);

#ifndef NDEBUG
void SMSC951xDeviceDumpReg (TSMSC951xDevice *pThis, const char *pName, u32 nIndex);
void SMSC951xDeviceDumpRegs (TSMSC951xDevice *pThis);
#endif

#endif
