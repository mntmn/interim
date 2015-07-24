//
// usbrequest.h
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
#ifndef _uspi_usbrequest_h
#define _uspi_usbrequest_h

#include <uspi/usb.h>
#include <uspi/usbendpoint.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TUSBRequest;

typedef void TURBCompletionRoutine (struct TUSBRequest *pURB, void *pParam, void *pContext);

typedef struct TUSBRequest		// URB
{
	TUSBEndpoint *m_pEndpoint;
	
	TSetupData *m_pSetupData;
	void	   *m_pBuffer;
	u32	    m_nBufLen;
	
	int	    m_bStatus;
	u32	    m_nResultLen;
	
	TURBCompletionRoutine *m_pCompletionRoutine;
	void *m_pCompletionParam;
	void *m_pCompletionContext;
}
TUSBRequest;

void USBRequest (TUSBRequest *pThis, TUSBEndpoint *pEndpoint, void *pBuffer, u32 nBufLen, TSetupData *pSetupData /* = 0 */);
void _USBRequest (TUSBRequest *pThis);

TUSBEndpoint *USBRequestGetEndpoint (TUSBRequest *pThis);

void USBRequestSetStatus (TUSBRequest *pThis, int bStatus);
void USBRequestSetResultLen (TUSBRequest *pThis, u32 nLength);

int USBRequestGetStatus (TUSBRequest *pThis);
u32 USBRequestGetResultLength (TUSBRequest *pThis);

TSetupData *USBRequestGetSetupData (TUSBRequest *pThis);
void *USBRequestGetBuffer (TUSBRequest *pThis);
u32 USBRequestGetBufLen (TUSBRequest *pThis);

void USBRequestSetCompletionRoutine (TUSBRequest *pThis, TURBCompletionRoutine *pRoutine, void *pParam, void *pContext);
void USBRequestCallCompletionRoutine (TUSBRequest *pThis);

#ifdef __cplusplus
}
#endif

#endif
