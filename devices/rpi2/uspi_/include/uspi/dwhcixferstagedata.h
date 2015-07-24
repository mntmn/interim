//
// dwhcixferstagedata.h
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
#ifndef _uspi__dwhcixferstagedata_h
#define _uspi_dwhcixferstagedata_h

#include <uspi/usb.h>
#include <uspi/usbrequest.h>
#include <uspi/usbdevice.h>
#include <uspi/usbendpoint.h>
#include <uspi/dwhciframescheduler.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDWHCITransferStageData
{
	unsigned	 m_nChannel;			// parameters
	TUSBRequest	*m_pURB;
	boolean		 m_bIn;
	boolean		 m_bStatusStage;

	boolean		 m_bSplitTransaction;
	boolean		 m_bSplitComplete;

	TUSBDevice	*m_pDevice;			// cached from *pURB
	TUSBEndpoint	*m_pEndpoint;
	TUSBSpeed	 m_Speed;
	u32		 m_nMaxPacketSize;
	
	u32		 m_nTransferSize;
	unsigned	 m_nPackets;
	u32		 m_nBytesPerTransaction;
	unsigned	 m_nPacketsPerTransaction;
	u32		 m_nTotalBytesTransfered;

	unsigned	 m_nState;
	unsigned	 m_nSubState;
	u32		 m_nTransactionStatus;

	u32		*m_pTempBuffer;
	void		*m_pBufferPointer;

	TDWHCIFrameScheduler *m_pFrameScheduler;
}
TDWHCITransferStageData;

void DWHCITransferStageData (TDWHCITransferStageData *pThis, unsigned nChannel, TUSBRequest *pURB, boolean bIn, boolean bStatusStage);
void _DWHCITransferStageData (TDWHCITransferStageData *pThis);

// change status
void DWHCITransferStageDataTransactionComplete (TDWHCITransferStageData *pThis, u32 nStatus, u32 nPacketsLeft, u32 nBytesLeft);
void DWHCITransferStageDataSetSplitComplete (TDWHCITransferStageData *pThis, boolean bComplete);

void DWHCITransferStageDataSetState (TDWHCITransferStageData *pThis, unsigned nState);
unsigned DWHCITransferStageDataGetState (TDWHCITransferStageData *pThis);
void DWHCITransferStageDataSetSubState (TDWHCITransferStageData *pThis, unsigned nSubState);
unsigned DWHCITransferStageDataGetSubState (TDWHCITransferStageData *pThis);

boolean DWHCITransferStageDataBeginSplitCycle (TDWHCITransferStageData *pThis);

// get transaction parameters
unsigned DWHCITransferStageDataGetChannelNumber (TDWHCITransferStageData *pThis);
u8 DWHCITransferStageDataGetDeviceAddress (TDWHCITransferStageData *pThis);
boolean DWHCITransferStageDataIsPeriodic (TDWHCITransferStageData *pThis);
u8 DWHCITransferStageDataGetEndpointType (TDWHCITransferStageData *pThis);
u8 DWHCITransferStageDataGetEndpointNumber (TDWHCITransferStageData *pThis);
u32 DWHCITransferStageDataGetMaxPacketSize (TDWHCITransferStageData *pThis);
TUSBSpeed DWHCITransferStageDataGetSpeed (TDWHCITransferStageData *pThis);

u8 DWHCITransferStageDataGetPID (TDWHCITransferStageData *pThis);
boolean DWHCITransferStageDataIsDirectionIn (TDWHCITransferStageData *pThis);
boolean DWHCITransferStageDataIsStatusStage (TDWHCITransferStageData *pThis);

u32 DWHCITransferStageDataGetDMAAddress (TDWHCITransferStageData *pThis);
u32 DWHCITransferStageDataGetBytesToTransfer (TDWHCITransferStageData *pThis);
u32 DWHCITransferStageDataGetPacketsToTransfer (TDWHCITransferStageData *pThis);

boolean DWHCITransferStageDataIsSplit (TDWHCITransferStageData *pThis);
boolean DWHCITransferStageDataIsSplitComplete (TDWHCITransferStageData *pThis);
u8 DWHCITransferStageDataGetHubAddress (TDWHCITransferStageData *pThis);
u8 DWHCITransferStageDataGetHubPortAddress (TDWHCITransferStageData *pThis);
u8 DWHCITransferStageDataGetSplitPosition (TDWHCITransferStageData *pThis);

u32 DWHCITransferStageDataGetStatusMask (TDWHCITransferStageData *pThis);

// check status after transaction
u32 DWHCITransferStageDataGetTransactionStatus (TDWHCITransferStageData *pThis);
boolean DWHCITransferStageDataIsStageComplete (TDWHCITransferStageData *pThis);
u32 DWHCITransferStageDataGetResultLen (TDWHCITransferStageData *pThis);

TUSBRequest *DWHCITransferStageDataGetURB (TDWHCITransferStageData *pThis);
TDWHCIFrameScheduler *DWHCITransferStageDataGetFrameScheduler (TDWHCITransferStageData *pThis);

#ifdef __cplusplus
}
#endif

#endif
