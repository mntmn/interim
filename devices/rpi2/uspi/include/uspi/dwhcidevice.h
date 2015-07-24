//
// dwhcidevice.h
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
#ifndef _dwhcidevice_h
#define _dwhcidevice_h

#include <uspi/usb.h>
#include <uspi/usbendpoint.h>
#include <uspi/usbrequest.h>
#include <uspi/dwhcirootport.h>
#include <uspi/dwhcixferstagedata.h>
#include <uspi/dwhciregister.h>
#include <uspi/dwhci.h>
#include <uspi/usb.h>
#include <uspi/types.h>
#include <uspios.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDWHCIDevice
{
	unsigned m_nChannels;
	volatile unsigned m_nChannelAllocated;		// one bit per channel, set if allocated

	TDWHCITransferStageData *m_pStageData[DWHCI_MAX_CHANNELS];

	volatile boolean m_bWaiting;

	TDWHCIRootPort m_RootPort;
}
TDWHCIDevice;

void DWHCIDevice (TDWHCIDevice *pThis);
void _DWHCIDevice (TDWHCIDevice *pThis);

boolean DWHCIDeviceInitialize (TDWHCIDevice *pThis);

// returns resulting length or < 0 on failure
int DWHCIDeviceGetDescriptor (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint,
			      unsigned char ucType, unsigned char ucIndex,
			      void *pBuffer, unsigned nBufSize,
			      unsigned char ucRequestType /* = REQUEST_IN */);

boolean DWHCIDeviceSetAddress (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint, u8 ucDeviceAddress);

boolean DWHCIDeviceSetConfiguration (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint, u8 ucConfigurationValue);

// returns resulting length or < 0 on failure
int DWHCIDeviceControlMessage (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint,
			u8 ucRequestType, u8 ucRequest, u16 usValue, u16 usIndex,
			void *pData, u16 usDataSize);

// returns resulting length or < 0 on failure
int DWHCIDeviceTransfer (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint, void *pBuffer, unsigned nBufSize);

boolean DWHCIDeviceSubmitBlockingRequest (TDWHCIDevice *pThis, TUSBRequest *pURB);
boolean DWHCIDeviceSubmitAsyncRequest (TDWHCIDevice *pThis, TUSBRequest *pURB);

TUSBSpeed DWHCIDeviceGetPortSpeed (TDWHCIDevice *pThis);
boolean DWHCIDeviceOvercurrentDetected (TDWHCIDevice *pThis);
void DWHCIDeviceDisableRootPort (TDWHCIDevice *pThis);

#ifdef __cplusplus
}
#endif

#endif
