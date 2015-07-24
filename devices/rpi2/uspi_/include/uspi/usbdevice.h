//
// usbdevice.h
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
#ifndef _uspi_usbdevice_h
#define _uspi_usbdevice_h

#include <uspi/usb.h>
#include <uspi/usbconfigparser.h>
#include <uspi/usbstring.h>
#include <uspi/string.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum		// do not change this order
{
	DeviceNameVendor,
	DeviceNameDevice,
	DeviceNameInterface,
	DeviceNameUnknown
}
TDeviceNameSelector;

struct TDWHCIDevice;
struct TUSBEndpoint;

typedef struct TUSBDevice
{
	boolean (*Configure) (struct TUSBDevice *pThis);

	struct TDWHCIDevice *m_pHost;

	u8		    m_ucAddress;
	TUSBSpeed	    m_Speed;
	struct TUSBEndpoint *m_pEndpoint0;

	u8		    m_ucHubAddress;
	u8		    m_ucHubPortNumber;
	
	TUSBDeviceDescriptor	    *m_pDeviceDesc;
	TUSBConfigurationDescriptor *m_pConfigDesc;

	TUSBConfigurationParser *m_pConfigParser;

	TUSBString m_ManufacturerString;
	TUSBString m_ProductString;
}
TUSBDevice;

void USBDevice (TUSBDevice *pThis, struct TDWHCIDevice *pHost, TUSBSpeed Speed, u8 ucHubAddress, u8 ucHubPortNumber);
void USBDeviceCopy (TUSBDevice *pThis, TUSBDevice *pDevice);
void _USBDevice (TUSBDevice *pThis);

boolean USBDeviceInitialize (TUSBDevice *pThis);		// onto address state (phase 1)
boolean USBDeviceConfigure (TUSBDevice *pThis);			// onto configured state (phase 2)

TString *USBDeviceGetName (TUSBDevice *pThis, TDeviceNameSelector Selector);	// string deleted by caller

u8 USBDeviceGetAddress (TUSBDevice *pThis);
TUSBSpeed USBDeviceGetSpeed (TUSBDevice *pThis);

u8 USBDeviceGetHubAddress (TUSBDevice *pThis);
u8 USBDeviceGetHubPortNumber (TUSBDevice *pThis);

struct TUSBEndpoint *USBDeviceGetEndpoint0 (TUSBDevice *pThis);
struct TDWHCIDevice *USBDeviceGetHost (TUSBDevice *pThis);

const TUSBDeviceDescriptor *USBDeviceGetDeviceDescriptor (TUSBDevice *pThis);
const TUSBConfigurationDescriptor *USBDeviceGetConfigurationDescriptor (TUSBDevice *pThis); // default config

// get next sub descriptor of ucType from configuration descriptor
const TUSBDescriptor *USBDeviceGetDescriptor (TUSBDevice *pThis, u8 ucType);	// returns 0 if not found
void USBDeviceConfigurationError (TUSBDevice *pThis, const char *pSource);

#ifdef __cplusplus
}
#endif

#endif
