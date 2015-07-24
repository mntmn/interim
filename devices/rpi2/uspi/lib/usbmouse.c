//
// usbmouse.c
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
#include <uspi/usbmouse.h>
#include <uspi/usbhid.h>
#include <uspi/usbhostcontroller.h>
#include <uspi/devicenameservice.h>
#include <uspi/util.h>
#include <uspi/assert.h>
#include <uspios.h>

static unsigned s_nDeviceNumber = 1;

static const char FromUSBKbd[] = "umouse";

static boolean USBMouseDeviceStartRequest (TUSBMouseDevice *pThis);
static void USBMouseDeviceCompletionRoutine (TUSBRequest *pURB, void *pParam, void *pContext);

void USBMouseDevice (TUSBMouseDevice *pThis, TUSBDevice *pDevice)
{
	assert (pThis != 0);

	USBDeviceCopy (&pThis->m_USBDevice, pDevice);
	pThis->m_USBDevice.Configure = USBMouseDeviceConfigure;

	pThis->m_pReportEndpoint = 0;
	pThis->m_pStatusHandler = 0;
	pThis->m_pURB = 0;
	pThis->m_pReportBuffer = 0;

	pThis->m_pReportBuffer = malloc (MOUSE_BOOT_REPORT_SIZE);
	assert (pThis->m_pReportBuffer != 0);
}

void _CUSBMouseDevice (TUSBMouseDevice *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pReportBuffer != 0)
	{
		free (pThis->m_pReportBuffer);
		pThis->m_pReportBuffer = 0;
	}

	if (pThis->m_pReportEndpoint != 0)
	{
		_USBEndpoint (pThis->m_pReportEndpoint);
		free (pThis->m_pReportEndpoint);
		pThis->m_pReportEndpoint = 0;
	}

	_USBDevice (&pThis->m_USBDevice);
}

boolean USBMouseDeviceConfigure (TUSBDevice *pUSBDevice)
{
	TUSBMouseDevice *pThis = (TUSBMouseDevice *) pUSBDevice;
	assert (pThis != 0);

	TUSBConfigurationDescriptor *pConfDesc =
		(TUSBConfigurationDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_CONFIGURATION);
	if (   pConfDesc == 0
	    || pConfDesc->bNumInterfaces <  1)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromUSBKbd);

		return FALSE;
	}

	TUSBInterfaceDescriptor *pInterfaceDesc;
	while ((pInterfaceDesc = (TUSBInterfaceDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_INTERFACE)) != 0)
	{
		if (   pInterfaceDesc->bNumEndpoints	  <  1
		    || pInterfaceDesc->bInterfaceClass	  != 0x03	// HID Class
		    || pInterfaceDesc->bInterfaceSubClass != 0x01	// Boot Interface Subclass
		    || pInterfaceDesc->bInterfaceProtocol != 0x02)	// Mouse
		{
			continue;
		}

		pThis->m_ucInterfaceNumber  = pInterfaceDesc->bInterfaceNumber;
		pThis->m_ucAlternateSetting = pInterfaceDesc->bAlternateSetting;

		TUSBEndpointDescriptor *pEndpointDesc =
			(TUSBEndpointDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_ENDPOINT);
		if (   pEndpointDesc == 0
		    || (pEndpointDesc->bEndpointAddress & 0x80) != 0x80		// Input EP
		    || (pEndpointDesc->bmAttributes     & 0x3F)	!= 0x03)	// Interrupt EP
		{
			continue;
		}

		assert (pThis->m_pReportEndpoint == 0);
		pThis->m_pReportEndpoint = malloc (sizeof (TUSBEndpoint));
		assert (pThis->m_pReportEndpoint != 0);
		USBEndpoint2 (pThis->m_pReportEndpoint, &pThis->m_USBDevice, pEndpointDesc);

		break;
	}

	if (pThis->m_pReportEndpoint == 0)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromUSBKbd);

		return FALSE;
	}
	
	if (!USBDeviceConfigure (&pThis->m_USBDevice))
	{
		LogWrite (FromUSBKbd, LOG_ERROR, "Cannot set configuration");

		return FALSE;
	}

	if (pThis->m_ucAlternateSetting != 0)
	{
		if (DWHCIDeviceControlMessage (USBDeviceGetHost (&pThis->m_USBDevice),
					USBDeviceGetEndpoint0 (&pThis->m_USBDevice),
					REQUEST_OUT | REQUEST_TO_INTERFACE, SET_INTERFACE,
					pThis->m_ucAlternateSetting,
					pThis->m_ucInterfaceNumber, 0, 0) < 0)
		{
			LogWrite (FromUSBKbd, LOG_ERROR, "Cannot set interface");

			return FALSE;
		}
	}

	if (DWHCIDeviceControlMessage (USBDeviceGetHost (&pThis->m_USBDevice),
				       USBDeviceGetEndpoint0 (&pThis->m_USBDevice),
				       REQUEST_OUT | REQUEST_CLASS | REQUEST_TO_INTERFACE,
				       SET_PROTOCOL, BOOT_PROTOCOL,
				       pThis->m_ucInterfaceNumber, 0, 0) < 0)
	{
		LogWrite (FromUSBKbd, LOG_ERROR, "Cannot set boot protocol");

		return FALSE;
	}

	TString DeviceName;
	String (&DeviceName);
	StringFormat (&DeviceName, "umouse%u", s_nDeviceNumber++);
	DeviceNameServiceAddDevice (DeviceNameServiceGet (), StringGet (&DeviceName), pThis, FALSE);

	_String (&DeviceName);

	return USBMouseDeviceStartRequest (pThis);
}

void USBMouseDeviceRegisterStatusHandler (TUSBMouseDevice *pThis, TMouseStatusHandler *pStatusHandler)
{
	assert (pThis != 0);
	assert (pStatusHandler != 0);
	pThis->m_pStatusHandler = pStatusHandler;
}

boolean USBMouseDeviceStartRequest (TUSBMouseDevice *pThis)
{
	assert (pThis != 0);

	assert (pThis->m_pReportEndpoint != 0);
	assert (pThis->m_pReportBuffer != 0);
	
	assert (pThis->m_pURB == 0);
	pThis->m_pURB = malloc (sizeof (TUSBRequest));
	assert (pThis->m_pURB != 0);
	USBRequest (pThis->m_pURB, pThis->m_pReportEndpoint, pThis->m_pReportBuffer, MOUSE_BOOT_REPORT_SIZE, 0);
	USBRequestSetCompletionRoutine (pThis->m_pURB, USBMouseDeviceCompletionRoutine, 0, pThis);
	
	return DWHCIDeviceSubmitAsyncRequest (USBDeviceGetHost (&pThis->m_USBDevice), pThis->m_pURB);
}

void USBMouseDeviceCompletionRoutine (TUSBRequest *pURB, void *pParam, void *pContext)
{
	TUSBMouseDevice *pThis = (TUSBMouseDevice *) pContext;
	assert (pThis != 0);
	
	assert (pURB != 0);
	assert (pThis->m_pURB == pURB);

	if (   USBRequestGetStatus (pURB) != 0
	    && USBRequestGetResultLength (pURB) == MOUSE_BOOT_REPORT_SIZE
	    && pThis->m_pStatusHandler != 0)
	{
		assert (pThis->m_pReportBuffer != 0);
		(*pThis->m_pStatusHandler) (pThis->m_pReportBuffer[0],
					    uspi_char2int ((char) pThis->m_pReportBuffer[1]),
					    uspi_char2int ((char) pThis->m_pReportBuffer[2]));
	}

	_USBRequest (pThis->m_pURB);
	free (pThis->m_pURB);
	pThis->m_pURB = 0;
	
	USBMouseDeviceStartRequest (pThis);
}
