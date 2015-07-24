//
// dwhciframeschedper.c
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
#include <uspi/dwhciframeschedper.h>
#include <uspi/dwhciregister.h>
#include <uspi/dwhci.h>
#include <uspi/assert.h>
#include <uspios.h>

#define uFRAME			125		// micro seconds

#define FRAME_UNSET		8

typedef enum
{
	StateStartSplit,
	StateStartSplitComplete,
	StateCompleteSplit,
	StateCompleteRetry,
	StateCompleteSplitComplete,
	StateCompleteSplitFailed,
	StateUnknown
}
TFrameSchedulerState;

void DWHCIFrameSchedulerPeriodic (TDWHCIFrameSchedulerPeriodic *pThis)
{
	assert (pThis != 0);

	TDWHCIFrameScheduler *pBase = (TDWHCIFrameScheduler *) pThis;

	pBase->_DWHCIFrameScheduler = _DWHCIFrameSchedulerPeriodic;
	pBase->StartSplit = DWHCIFrameSchedulerPeriodicStartSplit;
	pBase->CompleteSplit = DWHCIFrameSchedulerPeriodicCompleteSplit;
	pBase->TransactionComplete = DWHCIFrameSchedulerPeriodicTransactionComplete;
	pBase->WaitForFrame = DWHCIFrameSchedulerPeriodicWaitForFrame;
	pBase->IsOddFrame = DWHCIFrameSchedulerPeriodicIsOddFrame;

	pThis->m_nState = StateUnknown;
	pThis->m_nNextFrame = FRAME_UNSET;
}

void _DWHCIFrameSchedulerPeriodic (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerPeriodic *pThis = (TDWHCIFrameSchedulerPeriodic *) pBase;
	assert (pThis != 0);

	pThis->m_nState = StateUnknown;
}

void DWHCIFrameSchedulerPeriodicStartSplit (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerPeriodic *pThis = (TDWHCIFrameSchedulerPeriodic *) pBase;
	assert (pThis != 0);

	pThis->m_nState = StateStartSplit;
	pThis->m_nNextFrame = FRAME_UNSET;
}

boolean DWHCIFrameSchedulerPeriodicCompleteSplit (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerPeriodic *pThis = (TDWHCIFrameSchedulerPeriodic *) pBase;
	assert (pThis != 0);

	boolean bResult = FALSE;

	switch (pThis->m_nState)
	{
	case StateStartSplitComplete:
		pThis->m_nState = StateCompleteSplit;
		pThis->m_nTries = pThis->m_nNextFrame != 5 ? 3 : 2;
		pThis->m_nNextFrame = (pThis->m_nNextFrame  + 2) & 7;
		bResult = TRUE;
		break;

	case StateCompleteRetry:
		bResult = TRUE;
		pThis->m_nNextFrame = (pThis->m_nNextFrame + 1) & 7;
		break;

	case StateCompleteSplitComplete:
	case StateCompleteSplitFailed:
		break;
		
	default:
		assert (0);
		break;
	}
	
	return bResult;
}

void DWHCIFrameSchedulerPeriodicTransactionComplete (TDWHCIFrameScheduler *pBase, u32 nStatus)
{
	TDWHCIFrameSchedulerPeriodic *pThis = (TDWHCIFrameSchedulerPeriodic *) pBase;
	assert (pThis != 0);

	switch (pThis->m_nState)
	{
	case StateStartSplit:
		assert (nStatus & DWHCI_HOST_CHAN_INT_ACK);
		pThis->m_nState = StateStartSplitComplete;
		break;

	case StateCompleteSplit:
	case StateCompleteRetry:
		if (nStatus & DWHCI_HOST_CHAN_INT_XFER_COMPLETE)
		{
			pThis->m_nState = StateCompleteSplitComplete;
		}
		else if (nStatus & (DWHCI_HOST_CHAN_INT_NYET | DWHCI_HOST_CHAN_INT_ACK))
		{
			if (pThis->m_nTries-- == 0)
			{
				pThis->m_nState = StateCompleteSplitFailed;

				usDelay (8 * uFRAME);
			}
			else
			{
				pThis->m_nState = StateCompleteRetry;
			}
		}
		else if (nStatus & DWHCI_HOST_CHAN_INT_NAK)
		{
			usDelay (5 * uFRAME);
			pThis->m_nState = StateCompleteSplitFailed;
		}
		else
		{
			LogWrite ("dwsched", LOG_ERROR, "Invalid status 0x%X", nStatus);
			assert (0);
		}
		break;
		
	default:
		assert (0);
		break;
	}
}

void DWHCIFrameSchedulerPeriodicWaitForFrame (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerPeriodic *pThis = (TDWHCIFrameSchedulerPeriodic *) pBase;
	assert (pThis != 0);

	TDWHCIRegister FrameNumber;
	DWHCIRegister (&FrameNumber, DWHCI_HOST_FRM_NUM);

	if (pThis->m_nNextFrame == FRAME_UNSET)
	{
		pThis->m_nNextFrame = (DWHCI_HOST_FRM_NUM_NUMBER (DWHCIRegisterRead (&FrameNumber)) + 1) & 7;
		if (pThis->m_nNextFrame == 6)
		{
			pThis->m_nNextFrame++;
		}
	}

	while ((DWHCI_HOST_FRM_NUM_NUMBER (DWHCIRegisterRead (&FrameNumber)) & 7) != pThis->m_nNextFrame)
	{
		// do nothing
	}

	_DWHCIRegister (&FrameNumber);
}

boolean DWHCIFrameSchedulerPeriodicIsOddFrame (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerPeriodic *pThis = (TDWHCIFrameSchedulerPeriodic *) pBase;
	assert (pThis != 0);

	return pThis->m_nNextFrame & 1 ? TRUE : FALSE;
}
