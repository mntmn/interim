//
// usbdevicefactory.c
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
#include <uspi/usbdevicefactory.h>
#include <uspios.h>
#include <uspi/assert.h>

// for factory
#include <uspi/usbstandardhub.h>
#include <uspi/usbmassdevice.h>
#include <uspi/usbkeyboard.h>
#include <uspi/usbmouse.h>
#include <uspi/usbgamepad.h>
#include <uspi/smsc951x.h>

TUSBDevice *GetDevice (TUSBDevice *pParent, TString *pName);

TUSBDevice *USBDeviceFactoryGetDevice (TUSBDevice *pParent)
{
	assert (pParent != 0);

	TUSBDevice *pResult;
	if (   (pResult = GetDevice (pParent, USBDeviceGetName (pParent, DeviceNameVendor))) == 0
	    && (pResult = GetDevice (pParent, USBDeviceGetName (pParent, DeviceNameDevice))) == 0
	    && (pResult = GetDevice (pParent, USBDeviceGetName (pParent, DeviceNameInterface))) == 0)
	{
		return 0;
	}

	assert (pResult != 0);

	return pResult;
}

TUSBDevice *GetDevice (TUSBDevice *pParent, TString *pName)
{
	assert (pParent != 0);
	assert (pName != 0);
	
	TUSBDevice *pResult = 0;

	if (StringCompare (pName, "dev9-0-2") == 0)
	{
		TUSBStandardHub *pDevice = (TUSBStandardHub *) malloc (sizeof (TUSBStandardHub));
		assert (pDevice != 0);
		USBStandardHub (pDevice, pParent);
		pResult = (TUSBDevice *) pDevice;
	}
	else if (StringCompare (pName, "int8-6-50") == 0)
	{
		TUSBBulkOnlyMassStorageDevice *pDevice = (TUSBBulkOnlyMassStorageDevice *) malloc (sizeof (TUSBBulkOnlyMassStorageDevice));
		assert (pDevice != 0);
		USBBulkOnlyMassStorageDevice (pDevice, pParent);
		pResult = (TUSBDevice *) pDevice;
	}
	else if (StringCompare (pName, "int3-1-1") == 0)
	{
		TUSBKeyboardDevice *pDevice = (TUSBKeyboardDevice *) malloc (sizeof (TUSBKeyboardDevice));
		assert (pDevice != 0);
		USBKeyboardDevice (pDevice, pParent);
		pResult = (TUSBDevice *) pDevice;
	}
	else if (StringCompare (pName, "int3-1-2") == 0)
	{
		TUSBMouseDevice *pDevice = (TUSBMouseDevice *) malloc (sizeof (TUSBMouseDevice));
		assert (pDevice != 0);
		USBMouseDevice (pDevice, pParent);
		pResult = (TUSBDevice *) pDevice;
	}
	else if (StringCompare (pName, "ven424-ec00") == 0)
	{
		TSMSC951xDevice *pDevice = (TSMSC951xDevice *) malloc (sizeof (TSMSC951xDevice));
		assert (pDevice != 0);
		SMSC951xDevice (pDevice, pParent);
		pResult = (TUSBDevice *) pDevice;
	}
    else if (StringCompare (pName, "int3-0-0") == 0)
    {
        TUSBGamePadDevice *pDevice = (TUSBGamePadDevice *) malloc (sizeof (TUSBGamePadDevice));
        assert (pDevice != 0);
        USBGamePadDevice (pDevice, pParent);
        pResult = (TUSBDevice *) pDevice;
    }
	// new devices follow

	if (pResult != 0)
	{
		LogWrite ("usbdev", LOG_NOTICE, "Using device %s", StringGet (pName));
	}
	
	_String (pName);
	free (pName);
	
	return pResult;
}
