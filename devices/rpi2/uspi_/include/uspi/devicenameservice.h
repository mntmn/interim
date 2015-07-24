//
// devicenameservice.h
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
#ifndef _uspi_devicenameservice_h
#define _uspi_devicenameservice_h

#include <uspi/types.h>

typedef struct TDeviceInfo
{
	struct TDeviceInfo	*pNext;
	char			*pName;
	void			*pDevice;
	boolean			 bBlockDevice;
}
TDeviceInfo;

typedef struct TDeviceNameService
{
	TDeviceInfo *m_pList;
}
TDeviceNameService;

void DeviceNameService (TDeviceNameService *pThis);
void _DeviceNameService (TDeviceNameService *pThis);

void DeviceNameServiceAddDevice (TDeviceNameService *pThis, const char *pName, void *pDevice, boolean bBlockDevice);

void *DeviceNameServiceGetDevice (TDeviceNameService *pThis, const char *pName, boolean bBlockDevice);

TDeviceNameService *DeviceNameServiceGet (void);

#endif
