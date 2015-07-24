//
// dwhciframeschednper.c
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
#include <uspi/dwhciframeschednper.h>
#include <uspi/dwhci.h>
#include <uspi/assert.h>
#include <uspios.h>

#define uFRAME			125		// micro seconds

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

void DWHCIFrameSchedulerNonPeriodic (TDWHCIFrameSchedulerNonPeriodic *pThis)
{
	assert (pThis != 0);

	TDWHCIFrameScheduler *pBase = (TDWHCIFrameScheduler *) pThis;

	pBase->_DWHCIFrameScheduler = _DWHCIFrameSchedulerNonPeriodic;
	pBase->StartSplit = DWHCIFrameSchedulerNonPeriodicStartSplit;
	pBase->CompleteSplit = DWHCIFrameSchedulerNonPeriodicCompleteSplit;
	pBase->TransactionComplete = DWHCIFrameSchedulerNonPeriodicTransactionComplete;
	pBase->WaitForFrame = DWHCIFrameSchedulerNonPeriodicWaitForFrame;
	pBase->IsOddFrame = DWHCIFrameSchedulerNonPeriodicIsOddFrame;

	pThis->m_nState = StateUnknown;
}

void _DWHCIFrameSchedulerNonPeriodic (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerNonPeriodic *pThis = (TDWHCIFrameSchedulerNonPeriodic *) pBase;
	assert (pThis != 0);

	pThis->m_nState = StateUnknown;
}

void DWHCIFrameSchedulerNonPeriodicStartSplit (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerNonPeriodic *pThis = (TDWHCIFrameSchedulerNonPeriodic *) pBase;
	assert (pThis != 0);

	pThis->m_nState = StateStartSplit;
}

boolean DWHCIFrameSchedulerNonPeriodicCompleteSplit (TDWHCIFrameScheduler *pBase)
{
	TDWHCIFrameSchedulerNonPeriodic *pThis = (TDWHCIFrameSchedulerNonPeriodic *) pBase;
	assert (pThis != 0);

	boolean bResult = FALSE;

	switch (pThis->m_nState)
	{
	case StateStartSplitComplete:
		pThis->m_nState = StateCompleteSplit;
		pThis->m_nTries = 3;
		bResult = TRUE;
		break;

	case StateCompleteSplit:
	case StateCompleteRetry:
		usDelay (5 * uFRAME);
		bResult = TRUE;
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

void DWHCIFrameSchedulerNonPeriodicTransactionComplete (TDWHCIFrameScheduler *pBase, u32 nStatus)
{
	TDWHCIFrameSchedulerNonPeriodic *pThis = (TDWHCIFrameSchedulerNonPeriodic *) pBase;
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
			}
			else
			{
				pThis->m_nState = StateCompleteRetry;
			}
		}
		else if (nStatus & DWHCI_HOST_CHAN_INT_NAK)
		{
			if (pThis->m_nTries-- == 0)
			{
				usDelay (5 * uFRAME);
				pThis->m_nState = StateCompleteSplitFailed;
			}
			else
			{
				pThis->m_nState = StateCompleteRetry;
			}
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

void DWHCIFrameSchedulerNonPeriodicWaitForFrame (TDWHCIFrameScheduler *pBase)
{
}

boolean DWHCIFrameSchedulerNonPeriodicIsOddFrame (TDWHCIFrameScheduler *pBase)
{
	return FALSE;
}
