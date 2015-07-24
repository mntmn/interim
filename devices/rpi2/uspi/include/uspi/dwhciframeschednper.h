//
// dwhciframeschednper.h
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
#ifndef _uspi_dwhciframeschednper_h
#define _uspi_dwhciframeschednper_h

#include <uspi/dwhciframescheduler.h>
#include <uspi/types.h>

typedef struct TDWHCIFrameSchedulerNonPeriodic
{
	TDWHCIFrameScheduler m_DWHCIFrameScheduler;
	 
	unsigned m_nState;
	unsigned m_nTries;
}
TDWHCIFrameSchedulerNonPeriodic;

void DWHCIFrameSchedulerNonPeriodic (TDWHCIFrameSchedulerNonPeriodic *pThis);
void _DWHCIFrameSchedulerNonPeriodic (TDWHCIFrameScheduler *pBase);

void DWHCIFrameSchedulerNonPeriodicStartSplit (TDWHCIFrameScheduler *pBase);
boolean DWHCIFrameSchedulerNonPeriodicCompleteSplit (TDWHCIFrameScheduler *pBase);
void DWHCIFrameSchedulerNonPeriodicTransactionComplete (TDWHCIFrameScheduler *pBase, u32 nStatus);

void DWHCIFrameSchedulerNonPeriodicWaitForFrame (TDWHCIFrameScheduler *pBase);

boolean DWHCIFrameSchedulerNonPeriodicIsOddFrame (TDWHCIFrameScheduler *pBase);

#endif
