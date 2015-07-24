//
// usbdevice.c
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
#include <uspi/usbdevice.h>
#include <uspi/dwhcidevice.h>
#include <uspi/usbendpoint.h>
#include <uspios.h>
#include <uspi/util.h>
#include <uspi/assert.h>

#define MAX_CONFIG_DESC_SIZE		512		// best guess

typedef struct TConfigurationHeader
{
	TUSBConfigurationDescriptor Configuration;
	TUSBInterfaceDescriptor	    Interface;
}
TConfigurationHeader;

void USBDeviceSetAddress (TUSBDevice *pThis, u8 ucAddress);

static const char FromDevice[] = "usbdev";

static u8 s_ucNextAddress = USB_FIRST_DEDICATED_ADDRESS;

void USBDevice (TUSBDevice *pThis, struct TDWHCIDevice *pHost, TUSBSpeed Speed, u8 ucHubAddress, u8 ucHubPortNumber)
{
	assert (pThis != 0);

	pThis->Configure = USBDeviceConfigure;

	pThis->m_pHost = pHost;
	pThis->m_ucAddress = USB_DEFAULT_ADDRESS;
	pThis->m_Speed = Speed;
	pThis->m_pEndpoint0 = 0;
	pThis->m_ucHubAddress = ucHubAddress;
	pThis->m_ucHubPortNumber = ucHubPortNumber;
	pThis->m_pDeviceDesc = 0;
	pThis->m_pConfigDesc = 0;
	pThis->m_pConfigParser = 0;

	assert (pThis->m_pHost != 0);

	assert (pThis->m_pEndpoint0 == 0);
	pThis->m_pEndpoint0 = (TUSBEndpoint *) malloc (sizeof (TUSBEndpoint));
	assert (pThis->m_pEndpoint0 != 0);
	USBEndpoint (pThis->m_pEndpoint0, pThis);
	
	assert (ucHubPortNumber >= 1);

	USBString (&pThis->m_ManufacturerString, pThis);
	USBString (&pThis->m_ProductString, pThis);
}

void USBDeviceCopy (TUSBDevice *pThis, TUSBDevice *pDevice)
{
	assert (pThis != 0);

	assert (pDevice != 0);

	pThis->Configure = pDevice->Configure;

	pThis->m_pEndpoint0 = 0;
	pThis->m_pDeviceDesc = 0;
	pThis->m_pConfigDesc = 0;
	pThis->m_pConfigParser = 0;

	pThis->m_pHost		 = pDevice->m_pHost;
	pThis->m_ucAddress	 = pDevice->m_ucAddress;
	pThis->m_Speed		 = pDevice->m_Speed;
	pThis->m_ucHubAddress	 = pDevice->m_ucHubAddress;
	pThis->m_ucHubPortNumber = pDevice->m_ucHubPortNumber;

	USBStringCopy (&pThis->m_ManufacturerString, &pDevice->m_ManufacturerString);
	USBStringCopy (&pThis->m_ProductString, &pDevice->m_ProductString);
	
	pThis->m_pEndpoint0 = (TUSBEndpoint *) malloc (sizeof (TUSBEndpoint));
	assert (pThis->m_pEndpoint0 != 0);
	USBEndpointCopy (pThis->m_pEndpoint0, pDevice->m_pEndpoint0, pThis);
	
	if (pDevice->m_pDeviceDesc != 0)
	{
		pThis->m_pDeviceDesc = (TUSBDeviceDescriptor *) malloc (sizeof (TUSBDeviceDescriptor));
		assert (pThis->m_pDeviceDesc != 0);

		memcpy (pThis->m_pDeviceDesc, pDevice->m_pDeviceDesc, sizeof (TUSBDeviceDescriptor));
	}

	if (pDevice->m_pConfigDesc != 0)
	{
		unsigned nTotalLength = pDevice->m_pConfigDesc->wTotalLength;
		assert (nTotalLength <= MAX_CONFIG_DESC_SIZE);

		pThis->m_pConfigDesc = (TUSBConfigurationDescriptor *) malloc (nTotalLength);
		assert (pThis->m_pConfigDesc != 0);

		memcpy (pThis->m_pConfigDesc, pDevice->m_pConfigDesc, nTotalLength);

		if (pDevice->m_pConfigParser != 0)
		{
			pThis->m_pConfigParser = (TUSBConfigurationParser *) malloc (sizeof (TUSBConfigurationParser));
			assert (pThis->m_pConfigParser != 0);
			USBConfigurationParser (pThis->m_pConfigParser, pThis->m_pConfigDesc, nTotalLength);
		}
	}
}

void _USBDevice (TUSBDevice *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pConfigParser != 0)
	{
		_USBConfigurationParser (pThis->m_pConfigParser);
		free (pThis->m_pConfigParser);
		pThis->m_pConfigParser = 0;
	}

	if (pThis->m_pConfigDesc != 0)
	{
		free (pThis->m_pConfigDesc);
		pThis->m_pConfigDesc = 0;
	}

	if (pThis->m_pDeviceDesc != 0)
	{
		free (pThis->m_pDeviceDesc);
		pThis->m_pDeviceDesc = 0;
	}

	if (pThis->m_pEndpoint0 != 0)
	{
		_USBEndpoint (pThis->m_pEndpoint0);
		free (pThis->m_pEndpoint0);
		pThis->m_pEndpoint0 = 0;
	}

	pThis->Configure = 0;
	
	pThis->m_pHost = 0;

	_USBString (&pThis->m_ProductString);
	_USBString (&pThis->m_ManufacturerString);
}

boolean USBDeviceInitialize (TUSBDevice *pThis)
{
	assert (pThis != 0);

	assert (pThis->m_pDeviceDesc == 0);
	pThis->m_pDeviceDesc = (TUSBDeviceDescriptor *) malloc (sizeof (TUSBDeviceDescriptor));
	assert (pThis->m_pDeviceDesc != 0);

	assert (pThis->m_pHost != 0);
	assert (pThis->m_pEndpoint0 != 0);
	
	assert (sizeof *pThis->m_pDeviceDesc >= USB_DEFAULT_MAX_PACKET_SIZE);
	if (DWHCIDeviceGetDescriptor (pThis->m_pHost, pThis->m_pEndpoint0,
				    DESCRIPTOR_DEVICE, DESCRIPTOR_INDEX_DEFAULT,
				    pThis->m_pDeviceDesc, USB_DEFAULT_MAX_PACKET_SIZE, REQUEST_IN)
	    != USB_DEFAULT_MAX_PACKET_SIZE)
	{
		LogWrite (FromDevice, LOG_ERROR, "Cannot get device descriptor (short)");

		free (pThis->m_pDeviceDesc);
		pThis->m_pDeviceDesc = 0;

		return FALSE;
	}

	if (   pThis->m_pDeviceDesc->bLength	     != sizeof *pThis->m_pDeviceDesc
	    || pThis->m_pDeviceDesc->bDescriptorType != DESCRIPTOR_DEVICE)
	{
		LogWrite (FromDevice, LOG_ERROR, "Invalid device descriptor");

		free (pThis->m_pDeviceDesc);
		pThis->m_pDeviceDesc = 0;

		return FALSE;
	}

	USBEndpointSetMaxPacketSize (pThis->m_pEndpoint0, pThis->m_pDeviceDesc->bMaxPacketSize0);

	if (DWHCIDeviceGetDescriptor (pThis->m_pHost, pThis->m_pEndpoint0,
				    DESCRIPTOR_DEVICE, DESCRIPTOR_INDEX_DEFAULT,
				    pThis->m_pDeviceDesc, sizeof *pThis->m_pDeviceDesc, REQUEST_IN)
	    != (int) sizeof *pThis->m_pDeviceDesc)
	{
		LogWrite (FromDevice, LOG_ERROR, "Cannot get device descriptor");

		free (pThis->m_pDeviceDesc);
		pThis->m_pDeviceDesc = 0;

		return FALSE;
	}

#ifndef NDEBUG
	//DebugHexdump (pThis->m_pDeviceDesc, sizeof *pThis->m_pDeviceDesc, FromDevice);
#endif
	
	u8 ucAddress = s_ucNextAddress++;
	if (ucAddress > USB_MAX_ADDRESS)
	{
		LogWrite (FromDevice, LOG_ERROR, "Too many devices");

		return FALSE;
	}

	if (!DWHCIDeviceSetAddress (pThis->m_pHost, pThis->m_pEndpoint0, ucAddress))
	{
		LogWrite (FromDevice, LOG_ERROR,
			     "Cannot set address %u", (unsigned) ucAddress);

		return FALSE;
	}
	
	USBDeviceSetAddress (pThis, ucAddress);

	if (   pThis->m_pDeviceDesc->iManufacturer != 0
	    || pThis->m_pDeviceDesc->iProduct != 0)
	{
		u16 usLanguageID = USBStringGetLanguageID (&pThis->m_ManufacturerString);

		if (pThis->m_pDeviceDesc->iManufacturer != 0)
		{
			USBStringGetFromDescriptor (&pThis->m_ManufacturerString,
						    pThis->m_pDeviceDesc->iManufacturer, usLanguageID);
		}

		if (pThis->m_pDeviceDesc->iProduct != 0)
		{
			USBStringGetFromDescriptor (&pThis->m_ProductString,
						    pThis->m_pDeviceDesc->iProduct, usLanguageID);
		}
	}

	assert (pThis->m_pConfigDesc == 0);
	pThis->m_pConfigDesc = (TUSBConfigurationDescriptor *) malloc (sizeof (TUSBConfigurationDescriptor));
	assert (pThis->m_pConfigDesc != 0);

	if (DWHCIDeviceGetDescriptor (pThis->m_pHost, pThis->m_pEndpoint0,
				    DESCRIPTOR_CONFIGURATION, DESCRIPTOR_INDEX_DEFAULT,
				    pThis->m_pConfigDesc, sizeof *pThis->m_pConfigDesc, REQUEST_IN)
	    != (int) sizeof *pThis->m_pConfigDesc)
	{
		LogWrite (FromDevice, LOG_ERROR, "Cannot get configuration descriptor (short)");

		free (pThis->m_pConfigDesc);
		pThis->m_pConfigDesc = 0;

		return FALSE;
	}

	if (   pThis->m_pConfigDesc->bLength         != sizeof *pThis->m_pConfigDesc
	    || pThis->m_pConfigDesc->bDescriptorType != DESCRIPTOR_CONFIGURATION
	    || pThis->m_pConfigDesc->wTotalLength    >  MAX_CONFIG_DESC_SIZE)
	{
		LogWrite (FromDevice, LOG_ERROR, "Invalid configuration descriptor");
		
		free (pThis->m_pConfigDesc);
		pThis->m_pConfigDesc = 0;

		return FALSE;
	}

	unsigned nTotalLength = pThis->m_pConfigDesc->wTotalLength;

	free (pThis->m_pConfigDesc);

	pThis->m_pConfigDesc = (TUSBConfigurationDescriptor *) malloc (nTotalLength);
	assert (pThis->m_pConfigDesc != 0);

	if (DWHCIDeviceGetDescriptor (pThis->m_pHost, pThis->m_pEndpoint0,
				    DESCRIPTOR_CONFIGURATION, DESCRIPTOR_INDEX_DEFAULT,
				    pThis->m_pConfigDesc, nTotalLength, REQUEST_IN)
	    != (int) nTotalLength)
	{
		LogWrite (FromDevice, LOG_ERROR, "Cannot get configuration descriptor");

		free (pThis->m_pConfigDesc);
		pThis->m_pConfigDesc = 0;

		return FALSE;
	}

#ifndef NDEBUG
	//DebugHexdump (pThis->m_pConfigDesc, nTotalLength, FromDevice);
#endif

	assert (pThis->m_pConfigParser == 0);
	pThis->m_pConfigParser = malloc (sizeof (TUSBConfigurationParser));
	assert (pThis->m_pConfigParser != 0);
	USBConfigurationParser (pThis->m_pConfigParser, pThis->m_pConfigDesc, nTotalLength);
	
	if (!USBConfigurationParserIsValid (pThis->m_pConfigParser))
	{
		USBDeviceConfigurationError (pThis, FromDevice);

		return FALSE;
	}

	return TRUE;
}

boolean USBDeviceConfigure (TUSBDevice *pThis)
{
	assert (pThis != 0);

	assert (pThis->m_pHost != 0);
	assert (pThis->m_pEndpoint0 != 0);

	if (pThis->m_pConfigDesc == 0)		// not initialized
	{
		return FALSE;
	}

	if (!DWHCIDeviceSetConfiguration (pThis->m_pHost, pThis->m_pEndpoint0, pThis->m_pConfigDesc->bConfigurationValue))
	{
		LogWrite (FromDevice, LOG_ERROR, "Cannot set configuration (%u)",
			     (unsigned) pThis->m_pConfigDesc->bConfigurationValue);

		return FALSE;
	}

	return TRUE;
}

TString *USBDeviceGetName (TUSBDevice *pThis, TDeviceNameSelector Selector)
{
	assert (pThis != 0);

	TString *pString = malloc (sizeof (TString));
	assert (pString != 0);
	String (pString);
	
	switch (Selector)
	{
	case DeviceNameVendor:
		assert (pThis->m_pDeviceDesc != 0);
		StringFormat (pString, "ven%x-%x",
				 (unsigned) pThis->m_pDeviceDesc->idVendor,
				 (unsigned) pThis->m_pDeviceDesc->idProduct);
		break;
		
	case DeviceNameDevice:
		assert (pThis->m_pDeviceDesc != 0);
		if (   pThis->m_pDeviceDesc->bDeviceClass == 0
		    || pThis->m_pDeviceDesc->bDeviceClass == 0xFF)
		{
			goto unknown;
		}
		StringFormat (pString, "dev%x-%x-%x",
				 (unsigned) pThis->m_pDeviceDesc->bDeviceClass,
				 (unsigned) pThis->m_pDeviceDesc->bDeviceSubClass,
				 (unsigned) pThis->m_pDeviceDesc->bDeviceProtocol);
		break;
		
	case DeviceNameInterface: {
		TConfigurationHeader *pConfig = (TConfigurationHeader *) pThis->m_pConfigDesc;
		assert (pConfig != 0);
		if (   pConfig->Configuration.wTotalLength < sizeof *pConfig
		    || pConfig->Interface.bInterfaceClass == 0
		    || pConfig->Interface.bInterfaceClass == 0xFF)
		{
			goto unknown;
		}
		StringFormat (pString, "int%x-%x-%x",
				 (unsigned) pConfig->Interface.bInterfaceClass,
				 (unsigned) pConfig->Interface.bInterfaceSubClass,
				 (unsigned) pConfig->Interface.bInterfaceProtocol);
		} break;

	default:
		assert (0);
	unknown:
		StringSet (pString, "unknown");
		break;
	}
	
	return pString;
}

u8 USBDeviceGetAddress (TUSBDevice *pThis)
{
	assert (pThis != 0);
	return pThis->m_ucAddress;
}

TUSBSpeed USBDeviceGetSpeed (TUSBDevice *pThis)
{
	assert (pThis != 0);
	return pThis->m_Speed;
}

u8 USBDeviceGetHubAddress (TUSBDevice *pThis)
{
	assert (pThis != 0);
	return pThis->m_ucHubAddress;
}

u8 USBDeviceGetHubPortNumber (TUSBDevice *pThis)
{
	assert (pThis != 0);
	return pThis->m_ucHubPortNumber;
}

struct TUSBEndpoint *USBDeviceGetEndpoint0 (TUSBDevice *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pEndpoint0 != 0);
	return pThis->m_pEndpoint0;
}

struct TDWHCIDevice *USBDeviceGetHost (TUSBDevice *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pHost != 0);
	return pThis->m_pHost;
}

const TUSBDeviceDescriptor *USBDeviceGetDeviceDescriptor (TUSBDevice *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pDeviceDesc != 0);
	return pThis->m_pDeviceDesc;
}

const TUSBConfigurationDescriptor *USBDeviceGetConfigurationDescriptor (TUSBDevice *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pConfigDesc != 0);
	return pThis->m_pConfigDesc;
}

const TUSBDescriptor *USBDeviceGetDescriptor (TUSBDevice *pThis, u8 ucType)
{
	assert (pThis != 0);
	assert (pThis->m_pConfigParser != 0);
	return USBConfigurationParserGetDescriptor (pThis->m_pConfigParser, ucType);
}

void USBDeviceConfigurationError (TUSBDevice *pThis, const char *pSource)
{
	assert (pThis != 0);
	assert (pThis->m_pConfigParser != 0);
	USBConfigurationParserError (pThis->m_pConfigParser, pSource);
}

void USBDeviceSetAddress (TUSBDevice *pThis, u8 ucAddress)
{
	assert (pThis != 0);

	assert (ucAddress <= USB_MAX_ADDRESS);
	pThis->m_ucAddress = ucAddress;

	//LogWrite (FromDevice, LOG_DEBUG, "Device address set to %u", (unsigned) pThis->m_ucAddress);
}
