//
// dwhcixferstagedata.c
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
#include <uspi/dwhcixferstagedata.h>
#include <uspi/dwhciframeschedper.h>
#include <uspi/dwhciframeschednper.h>
#include <uspi/dwhciframeschednsplit.h>
#include <uspi/dwhci.h>
#include <uspios.h>
#include <uspi/assert.h>

void DWHCITransferStageData (TDWHCITransferStageData *pThis, unsigned nChannel, TUSBRequest *pURB, boolean bIn, boolean bStatusStage)
{
	assert (pThis != 0);

	pThis->m_nChannel = nChannel;
	pThis->m_pURB = pURB;
	pThis->m_bIn = bIn;
	pThis->m_bStatusStage = bStatusStage;
	pThis->m_bSplitComplete = FALSE;
	pThis->m_nTotalBytesTransfered = 0;
	pThis->m_nState = 0;
	pThis->m_nSubState = 0;
	pThis->m_nTransactionStatus = 0;
	pThis->m_pTempBuffer = 0;
	pThis->m_pFrameScheduler = 0;

	assert (pThis->m_pURB != 0);

	pThis->m_pEndpoint = USBRequestGetEndpoint (pURB);
	assert (pThis->m_pEndpoint != 0);
	pThis->m_pDevice = USBEndpointGetDevice (pThis->m_pEndpoint);
	assert (pThis->m_pDevice != 0);

	pThis->m_Speed = USBDeviceGetSpeed (pThis->m_pDevice);
	pThis->m_nMaxPacketSize = USBEndpointGetMaxPacketSize (pThis->m_pEndpoint);
	
	pThis->m_bSplitTransaction =    USBDeviceGetHubAddress (pThis->m_pDevice) != 0
				     && pThis->m_Speed != USBSpeedHigh;

	if (!bStatusStage)
	{
		if (USBEndpointGetNextPID (pThis->m_pEndpoint, bStatusStage) == USBPIDSetup)
		{
			pThis->m_pBufferPointer = USBRequestGetSetupData (pURB);
			pThis->m_nTransferSize = sizeof (TSetupData);
		}
		else
		{
			pThis->m_pBufferPointer = USBRequestGetBuffer (pURB);
			pThis->m_nTransferSize = USBRequestGetBufLen (pURB);
		}

		pThis->m_nPackets = (pThis->m_nTransferSize + pThis->m_nMaxPacketSize - 1) / pThis->m_nMaxPacketSize;
		
		if (pThis->m_bSplitTransaction)
		{
			if (pThis->m_nTransferSize > pThis->m_nMaxPacketSize)
			{
				pThis->m_nBytesPerTransaction = pThis->m_nMaxPacketSize;
			}
			else
			{
				pThis->m_nBytesPerTransaction = pThis->m_nTransferSize;
			}
			
			pThis->m_nPacketsPerTransaction = 1;
		}
		else
		{
			pThis->m_nBytesPerTransaction = pThis->m_nTransferSize;
			pThis->m_nPacketsPerTransaction = pThis->m_nPackets;
		}
	}
	else
	{
		assert (pThis->m_pTempBuffer == 0);
		pThis->m_pTempBuffer = (u32 *) malloc (sizeof (u32));
		assert (pThis->m_pTempBuffer != 0);
		pThis->m_pBufferPointer = pThis->m_pTempBuffer;

		pThis->m_nTransferSize = 0;
		pThis->m_nBytesPerTransaction = 0;
		pThis->m_nPackets = 1;
		pThis->m_nPacketsPerTransaction = 1;
	}

	assert (pThis->m_pBufferPointer != 0);
	assert (((u32) pThis->m_pBufferPointer & 3) == 0);

	if (pThis->m_bSplitTransaction)
	{
		if (DWHCITransferStageDataIsPeriodic (pThis))
		{
			pThis->m_pFrameScheduler = (TDWHCIFrameScheduler *) malloc (sizeof (TDWHCIFrameSchedulerPeriodic));
			DWHCIFrameSchedulerPeriodic ((TDWHCIFrameSchedulerPeriodic *) pThis->m_pFrameScheduler);
		}
		else
		{
			pThis->m_pFrameScheduler = (TDWHCIFrameScheduler *) malloc (sizeof (TDWHCIFrameSchedulerNonPeriodic));
			DWHCIFrameSchedulerNonPeriodic ((TDWHCIFrameSchedulerNonPeriodic *) pThis->m_pFrameScheduler);
		}

		assert (pThis->m_pFrameScheduler != 0);
	}
	else
	{
		if (   USBDeviceGetHubAddress (pThis->m_pDevice) == 0
		    && pThis->m_Speed != USBSpeedHigh)
		{
			pThis->m_pFrameScheduler = (TDWHCIFrameScheduler *) malloc (sizeof (TDWHCIFrameSchedulerNoSplit));
			DWHCIFrameSchedulerNoSplit ((TDWHCIFrameSchedulerNoSplit *) pThis->m_pFrameScheduler, DWHCITransferStageDataIsPeriodic (pThis));
			assert (pThis->m_pFrameScheduler != 0);
		}
	}
}

void _DWHCITransferStageData (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pFrameScheduler != 0)
	{
		pThis->m_pFrameScheduler->_DWHCIFrameScheduler (pThis->m_pFrameScheduler);
		free (pThis->m_pFrameScheduler);
		pThis->m_pFrameScheduler = 0;
	}

	pThis->m_pBufferPointer = 0;

	if (pThis->m_pTempBuffer != 0)
	{
		free (pThis->m_pTempBuffer);
		pThis->m_pTempBuffer = 0;
	}

	pThis->m_pEndpoint = 0;
	pThis->m_pDevice = 0;
	pThis->m_pURB = 0;
}

void DWHCITransferStageDataTransactionComplete (TDWHCITransferStageData *pThis, u32 nStatus, u32 nPacketsLeft, u32 nBytesLeft)
{
	assert (pThis != 0);

#if 0
	if (pThis->m_bSplitTransaction)
	{
		Logger->Write ("udata", LOG_DEBUG,
			       "Transaction complete (status 0x%X, packets 0x%X, bytes 0x%X)",
			       nStatus, nPacketsLeft, nBytesLeft);
	}
#endif

	pThis->m_nTransactionStatus = nStatus;

	if (  nStatus
	    & (  DWHCI_HOST_CHAN_INT_ERROR_MASK
	       | DWHCI_HOST_CHAN_INT_NAK
	       | DWHCI_HOST_CHAN_INT_NYET))
	{
		return;
	}

	u32 nPacketsTransfered = pThis->m_nPacketsPerTransaction - nPacketsLeft;
	u32 nBytesTransfered = pThis->m_nBytesPerTransaction - nBytesLeft;

	if (   pThis->m_bSplitTransaction
	    && pThis->m_bSplitComplete
	    && nBytesTransfered == 0
	    && pThis->m_nBytesPerTransaction > 0)
	{
		nBytesTransfered = pThis->m_nMaxPacketSize * nPacketsTransfered;
	}
	
	pThis->m_nTotalBytesTransfered += nBytesTransfered;
	pThis->m_pBufferPointer = (u8 *) pThis->m_pBufferPointer + nBytesTransfered;
	
	if (   !pThis->m_bSplitTransaction
	    || pThis->m_bSplitComplete)
	{
		USBEndpointSkipPID (pThis->m_pEndpoint, nPacketsTransfered, pThis->m_bStatusStage);
	}

	assert (nPacketsTransfered <= pThis->m_nPackets);
	pThis->m_nPackets -= nPacketsTransfered;

	// if (pThis->m_nTotalBytesTransfered > pThis->m_nTransferSize) this will be false:
	if (pThis->m_nTransferSize - pThis->m_nTotalBytesTransfered < pThis->m_nBytesPerTransaction)
	{
		assert (pThis->m_nTotalBytesTransfered <= pThis->m_nTransferSize);
		pThis->m_nBytesPerTransaction = pThis->m_nTransferSize - pThis->m_nTotalBytesTransfered;
	}
}

void DWHCITransferStageDataSetSplitComplete (TDWHCITransferStageData *pThis, boolean bComplete)
{
	assert (pThis != 0);
	assert (pThis->m_bSplitTransaction);
	
	pThis->m_bSplitComplete = bComplete;
}

void DWHCITransferStageDataSetState (TDWHCITransferStageData *pThis, unsigned nState)
{
	assert (pThis != 0);
	pThis->m_nState = nState;
}

unsigned DWHCITransferStageDataGetState (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nState;
}

void DWHCITransferStageDataSetSubState (TDWHCITransferStageData *pThis, unsigned nSubState)
{
	assert (pThis != 0);
	pThis->m_nSubState = nSubState;
}

unsigned DWHCITransferStageDataGetSubState (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nSubState;
}

boolean DWHCITransferStageDataBeginSplitCycle (TDWHCITransferStageData *pThis)
{
	return TRUE;
}

unsigned DWHCITransferStageDataGetChannelNumber (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nChannel;
}

boolean DWHCITransferStageDataIsPeriodic (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pEndpoint != 0);
	TEndpointType Type = USBEndpointGetType (pThis->m_pEndpoint);
	
	return    Type == EndpointTypeInterrupt
	       || Type == EndpointTypeIsochronous;
}

u8 DWHCITransferStageDataGetDeviceAddress (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pDevice != 0);
	return USBDeviceGetAddress (pThis->m_pDevice);
}

u8 DWHCITransferStageDataGetEndpointType (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pEndpoint != 0);
	
	unsigned nEndpointType = 0;

	switch (USBEndpointGetType (pThis->m_pEndpoint))
	{
	case EndpointTypeControl:
		nEndpointType = DWHCI_HOST_CHAN_CHARACTER_EP_TYPE_CONTROL;
		break;

	case EndpointTypeBulk:
		nEndpointType = DWHCI_HOST_CHAN_CHARACTER_EP_TYPE_BULK;
		break;

	case EndpointTypeInterrupt:
		nEndpointType = DWHCI_HOST_CHAN_CHARACTER_EP_TYPE_INTERRUPT;
		break;

	default:
		assert (0);
		break;
	}
	
	return nEndpointType;
}

u8 DWHCITransferStageDataGetEndpointNumber (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pEndpoint != 0);
	return USBEndpointGetNumber (pThis->m_pEndpoint);
}

u32 DWHCITransferStageDataGetMaxPacketSize (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nMaxPacketSize;
}

TUSBSpeed DWHCITransferStageDataGetSpeed (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_Speed;
}

u8 DWHCITransferStageDataGetPID (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pEndpoint != 0);
	
	u8 ucPID = 0;
	
	switch (USBEndpointGetNextPID (pThis->m_pEndpoint, pThis->m_bStatusStage))
	{
	case USBPIDSetup:
		ucPID = DWHCI_HOST_CHAN_XFER_SIZ_PID_SETUP;
		break;

	case USBPIDData0:
		ucPID = DWHCI_HOST_CHAN_XFER_SIZ_PID_DATA0;
		break;
		
	case USBPIDData1:
		ucPID = DWHCI_HOST_CHAN_XFER_SIZ_PID_DATA1;
		break;

	default:
		assert (0);
		break;
	}
	
	return ucPID;
}

boolean DWHCITransferStageDataIsDirectionIn (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_bIn;
}

boolean DWHCITransferStageDataIsStatusStage (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_bStatusStage;
}

u32 DWHCITransferStageDataGetDMAAddress (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pBufferPointer != 0);

	return (u32) pThis->m_pBufferPointer;
}

u32 DWHCITransferStageDataGetBytesToTransfer (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nBytesPerTransaction;
}

u32 DWHCITransferStageDataGetPacketsToTransfer (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nPacketsPerTransaction;
}

boolean DWHCITransferStageDataIsSplit (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_bSplitTransaction;
}

boolean DWHCITransferStageDataIsSplitComplete (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_bSplitTransaction);
	
	return pThis->m_bSplitComplete;
}

u8 DWHCITransferStageDataGetHubAddress (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_bSplitTransaction);

	assert (pThis->m_pDevice != 0);
	return USBDeviceGetHubAddress (pThis->m_pDevice);
}

u8 DWHCITransferStageDataGetHubPortAddress (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_bSplitTransaction);

	assert (pThis->m_pDevice != 0);
	return USBDeviceGetHubPortNumber (pThis->m_pDevice);
}

u8 DWHCITransferStageDataGetSplitPosition (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_nTransferSize <= 188);		// TODO
	return DWHCI_HOST_CHAN_SPLIT_CTRL_ALL;
}

u32 DWHCITransferStageDataGetStatusMask (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	u32 nMask =   DWHCI_HOST_CHAN_INT_XFER_COMPLETE
		    | DWHCI_HOST_CHAN_INT_HALTED
		    | DWHCI_HOST_CHAN_INT_ERROR_MASK;
		    
	if (   pThis->m_bSplitTransaction
	    || DWHCITransferStageDataIsPeriodic (pThis))
	{
		nMask |=   DWHCI_HOST_CHAN_INT_ACK
			 | DWHCI_HOST_CHAN_INT_NAK
			 | DWHCI_HOST_CHAN_INT_NYET;
	}
	
	return	nMask;
}

u32 DWHCITransferStageDataGetTransactionStatus (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_nTransactionStatus != 0);
	return pThis->m_nTransactionStatus;
}

boolean DWHCITransferStageDataIsStageComplete (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_nPackets == 0;
}

u32 DWHCITransferStageDataGetResultLen (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	if (pThis->m_nTotalBytesTransfered > pThis->m_nTransferSize)
	{
		return pThis->m_nTransferSize;
	}
	
	return pThis->m_nTotalBytesTransfered;
}

TUSBRequest *DWHCITransferStageDataGetURB (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_pURB != 0);
	return pThis->m_pURB;
}

TDWHCIFrameScheduler *DWHCITransferStageDataGetFrameScheduler (TDWHCITransferStageData *pThis)
{
	assert (pThis != 0);
	return pThis->m_pFrameScheduler;
}
