//
// dwhciframeschednsplit.h
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
#ifndef _uspi_dwhciframeschednsplit_h
#define _uspi_dwhciframeschednsplit_h

#include <uspi/dwhciframescheduler.h>
#include <uspi/types.h>

typedef struct TDWHCIFrameSchedulerNoSplit
{
	TDWHCIFrameScheduler m_DWHCIFrameScheduler;
	
	boolean m_bIsPeriodic;
	unsigned m_nNextFrame;
}
TDWHCIFrameSchedulerNoSplit;

void DWHCIFrameSchedulerNoSplit (TDWHCIFrameSchedulerNoSplit *pThis, boolean bIsPeriodic);
void _DWHCIFrameSchedulerNoSplit (TDWHCIFrameScheduler *pBase);

void DWHCIFrameSchedulerNoSplitStartSplit (TDWHCIFrameScheduler *pBase);
boolean DWHCIFrameSchedulerNoSplitCompleteSplit (TDWHCIFrameScheduler *pBase);
void DWHCIFrameSchedulerNoSplitTransactionComplete (TDWHCIFrameScheduler *pBase, u32 nStatus);

void DWHCIFrameSchedulerNoSplitWaitForFrame (TDWHCIFrameScheduler *pBase);

boolean DWHCIFrameSchedulerNoSplitIsOddFrame (TDWHCIFrameScheduler *pBase);

#endif
