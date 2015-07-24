//
// interrupt.h
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
#ifndef _uspienv_interrupt_h
#define _uspienv_interrupt_h

#include <uspienv/bcm2835int.h>
#include <uspienv/exceptionstub.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void TIRQHandler (void *pParam);

typedef struct TInterruptSystem
{
	TIRQHandler	*m_apIRQHandler[IRQ_LINES];
	void		*m_pParam[IRQ_LINES];
}
TInterruptSystem;

void InterruptSystem (TInterruptSystem *pThis);
void _InterruptSystem (TInterruptSystem *pThis);

int InterruptSystemInitialize (TInterruptSystem *pThis);

void InterruptSystemConnectIRQ (TInterruptSystem *pThis, unsigned nIRQ, TIRQHandler *pHandler, void *pParam);
void InterruptSystemDisconnectIRQ (TInterruptSystem *pThis, unsigned nIRQ);

void InterruptSystemEnableIRQ (unsigned nIRQ);
void InterruptSystemDisableIRQ (unsigned nIRQ);

TInterruptSystem *InterruptSystemGet (void);

void InterruptHandler (void);

#ifdef __cplusplus
}
#endif

#endif
