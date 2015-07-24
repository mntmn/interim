//
// usbrequest.c
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
#include <uspi/usbrequest.h>
#include <uspi/assert.h>

void USBRequest (TUSBRequest *pThis, TUSBEndpoint *pEndpoint, void *pBuffer, u32 nBufLen, TSetupData *pSetupData)
{
	assert (pThis != 0);

	pThis->m_pEndpoint = pEndpoint;
	pThis->m_pSetupData = pSetupData;
	pThis->m_pBuffer = pBuffer;
	pThis->m_nBufLen = nBufLen;
	pThis->m_bStatus = 0;
	pThis->m_nResultLen = 0;
	pThis->m_pCompletionRoutine = 0;
	pThis->m_pCompletionParam = 0;
	pThis->m_pCompletionContext = 0;

	assert (pThis->m_pEndpoint != 0);
	assert (pThis->m_pBuffer != 0 || pThis->m_nBufLen == 0);
}

void _USBRequest (TUSBRequest *pThis)
{
	assert (pThis != 0);
	pThis->m_pEndpoint = 0;
	pThis->m_pSetupData = 0;
	pThis->m_pBuffer = 0;
	pThis->m_pCompletionRoutine = 0;
}

TUSBEndpoint *USBRequestGetEndpoint (TUSBRequest *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pEndpoint != 0);
	return pThis->m_pEndpoint;
}

void USBRequestSetStatus (TUSBRequest *pThis, int bStatus)
{
	assert (pThis != 0);
	pThis->m_bStatus = bStatus;
}

void USBRequestSetResultLen (TUSBRequest *pThis, u32 nLength)
{
	assert (pThis != 0);
	pThis->m_nResultLen = nLength;
}

int USBRequestGetStatus (TUSBRequest *pThis)
{
	assert (pThis != 0);
	return pThis->m_bStatus;
}

u32 USBRequestGetResultLength (TUSBRequest *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_bStatus);

	return pThis->m_nResultLen;
}

TSetupData *USBRequestGetSetupData (TUSBRequest *pThis)
{
	assert (pThis != 0);
	assert (USBEndpointGetType (pThis->m_pEndpoint) == EndpointTypeControl);
	assert (pThis->m_pSetupData != 0);

	return pThis->m_pSetupData;
}

void *USBRequestGetBuffer (TUSBRequest *pThis)
{
	assert (pThis != 0);
	assert (   pThis->m_pBuffer != 0
		|| pThis->m_nBufLen == 0);

	return pThis->m_pBuffer;
}

u32 USBRequestGetBufLen (TUSBRequest *pThis)
{
	assert (pThis != 0);
	return pThis->m_nBufLen;
}

void USBRequestSetCompletionRoutine (TUSBRequest *pThis, TURBCompletionRoutine *pRoutine, void *pParam, void *pContext)
{
	assert (pThis != 0);
	pThis->m_pCompletionRoutine = pRoutine;
	pThis->m_pCompletionParam   = pParam;
	pThis->m_pCompletionContext = pContext;

	assert (pThis->m_pCompletionRoutine != 0);
}

void USBRequestCallCompletionRoutine (TUSBRequest *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pCompletionRoutine != 0);
	
	(*pThis->m_pCompletionRoutine) (pThis, pThis->m_pCompletionParam, pThis->m_pCompletionContext);
}
