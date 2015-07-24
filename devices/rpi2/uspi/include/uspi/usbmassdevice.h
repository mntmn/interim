//
// usbmassdevice.h
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
#ifndef _uspi_usbmassdevice_h
#define _uspi_usbmassdevice_h

#include <uspi/usbdevice.h>
#include <uspi/usbendpoint.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UMSD_BLOCK_SIZE		512
#define UMSD_BLOCK_MASK		(UMSD_BLOCK_SIZE-1)
#define UMSD_BLOCK_SHIFT	9

#define UMSD_MAX_OFFSET		0x1FFFFFFFFFFULL		// 2TB

typedef struct TUSBBulkOnlyMassStorageDevice
{
	TUSBDevice m_USBDevice;

	TUSBEndpoint *m_pEndpointIn;
	TUSBEndpoint *m_pEndpointOut;

	unsigned m_nCWBTag;
	unsigned m_nBlockCount;
	unsigned long long m_ullOffset;
}
TUSBBulkOnlyMassStorageDevice;

void USBBulkOnlyMassStorageDevice (TUSBBulkOnlyMassStorageDevice *pThis, TUSBDevice *pDevice);
void _USBBulkOnlyMassStorageDevice (TUSBBulkOnlyMassStorageDevice *pThis);

boolean USBBulkOnlyMassStorageDeviceConfigure (TUSBDevice *pUSBDevice);

int USBBulkOnlyMassStorageDeviceRead (TUSBBulkOnlyMassStorageDevice *pThis, void *pBuffer, unsigned nCount);
int USBBulkOnlyMassStorageDeviceWrite (TUSBBulkOnlyMassStorageDevice *pThis, const void *pBuffer, unsigned nCount);

unsigned long long USBBulkOnlyMassStorageDeviceSeek (TUSBBulkOnlyMassStorageDevice *pThis, unsigned long long ullOffset);

unsigned USBBulkOnlyMassStorageDeviceGetCapacity (TUSBBulkOnlyMassStorageDevice *pThis);

#ifdef __cplusplus
}
#endif

#endif
