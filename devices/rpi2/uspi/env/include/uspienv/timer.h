//
// timer.h
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
#ifndef _uspienv_timer_h
#define _uspienv_timer_h

#include <uspienv/interrupt.h>
#include <uspienv/string.h>
#include <uspienv/sysconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HZ		100			// ticks per second

#define MSEC2HZ(msec)	((msec) * HZ / 1000)

typedef void TKernelTimerHandler (unsigned hTimer, void *pParam, void *pContext);

typedef struct TKernelTimer
{
	TKernelTimerHandler *m_pHandler;
	unsigned	     m_nElapsesAt;
	void 		    *m_pParam;
	void 		    *m_pContext;
}
TKernelTimer;

typedef struct TTimer
{
	TInterruptSystem	*m_pInterruptSystem;
	volatile unsigned	 m_nTicks;
	volatile unsigned	 m_nTime;
	volatile TKernelTimer	 m_KernelTimer[KERNEL_TIMERS];	// TODO: should be linked list
	unsigned		 m_nMsDelay;
	unsigned		 m_nusDelay;
}
TTimer;

void Timer (TTimer *pThis, TInterruptSystem *pInterruptSystem);
void _Timer (TTimer *pThis);

boolean TimerInitialize (TTimer *pThis);

unsigned TimerGetClockTicks (TTimer *pThis);		// 1 MHz counter
#define CLOCKHZ	1000000

unsigned TimerGetTicks (TTimer *pThis);			// 1/HZ seconds since system boot
unsigned TimerGetTime (TTimer *pThis);			// Seconds since system boot

// "HH:MM:SS.ss", 0 if Initialize() was not yet called
TString *TimerGetTimeString (TTimer *pThis);		// CString object must be deleted by caller

// returns timer handle (0 on failure)
unsigned TimerStartKernelTimer (TTimer *pThis,
				unsigned nDelay,		// in HZ units
				TKernelTimerHandler *pHandler,
				void *pParam,
				void *pContext);
void TimerCancelKernelTimer (TTimer *pThis, unsigned hTimer);

// when a CTimer object is available better use these methods
void TimerMsDelay (TTimer *pThis, unsigned nMilliSeconds);
void TimerusDelay (TTimer *pThis, unsigned nMicroSeconds);

TTimer *TimerGet (void);

// can be used before Timer is constructed
void TimerSimpleMsDelay (unsigned nMilliSeconds);
void TimerSimpleusDelay (unsigned nMicroSeconds);

#ifdef __cplusplus
}
#endif

#endif
