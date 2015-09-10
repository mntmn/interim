//
// interrupt.c
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
#include <uspienv/interrupt.h>
#include <uspienv/synchronize.h>
#include <uspienv/bcm2835.h>
#include <uspienv/memio.h>
#include <uspienv/types.h>
#include <uspienv/assert.h>

#define ARM_IC_IRQ_PENDING(irq)	(  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_IRQ_PENDING_1		\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_IRQ_PENDING_2	\
				   : ARM_IC_IRQ_BASIC_PENDING))
#define ARM_IC_IRQS_ENABLE(irq)	(  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_ENABLE_IRQS_1		\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_ENABLE_IRQS_2	\
				   : ARM_IC_ENABLE_BASIC_IRQS))
#define ARM_IC_IRQS_DISABLE(irq) (  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_DISABLE_IRQS_1	\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_DISABLE_IRQS_2	\
				   : ARM_IC_DISABLE_BASIC_IRQS))
#define ARM_IRQ_MASK(irq)	(1 << ((irq) & (ARM_IRQS_PER_REG-1)))
				   
static TInterruptSystem *s_pThis = 0;

int InterruptSystemCallIRQHandler (TInterruptSystem *pThis, unsigned nIRQ);

void InterruptSystem (TInterruptSystem *pThis)
{
	assert (pThis != 0);

	for (unsigned nIRQ = 0; nIRQ < IRQ_LINES; nIRQ++)
	{
		pThis->m_apIRQHandler[nIRQ] = 0;
		pThis->m_pParam[nIRQ] = 0;
	}

	s_pThis = pThis;
}

void _InterruptSystem (TInterruptSystem *pThis)
{
	s_pThis = 0;
}

int InterruptSystemInitialize (TInterruptSystem *pThis)
{
	assert (pThis != 0);

	TExceptionTable *pTable = (TExceptionTable *) ARM_EXCEPTION_TABLE_BASE;
	pTable->IRQ = ARM_OPCODE_BRANCH (ARM_DISTANCE (pTable->IRQ, IRQStub));

	CleanDataCache();
	DataSyncBarrier();

	InvalidateInstructionCache();
	FlushBranchTargetCache();
	DataSyncBarrier();

	InstructionSyncBarrier();

	DataMemBarrier();

	write32(ARM_IC_FIQ_CONTROL, 0);

	write32(ARM_IC_DISABLE_IRQS_1, (u32) -1);
	write32(ARM_IC_DISABLE_IRQS_2, (u32) -1);
	write32(ARM_IC_DISABLE_BASIC_IRQS, (u32) -1);

	// Ack pending IRQs
	write32(ARM_IC_IRQ_BASIC_PENDING, read32 (ARM_IC_IRQ_BASIC_PENDING));
	write32(ARM_IC_IRQ_PENDING_1, 	   read32 (ARM_IC_IRQ_PENDING_1));
	write32(ARM_IC_IRQ_PENDING_2,     read32 (ARM_IC_IRQ_PENDING_2));

	DataMemBarrier();

	EnableInterrupts();

	return TRUE;
}

void InterruptSystemConnectIRQ (TInterruptSystem *pThis, unsigned nIRQ, TIRQHandler *pHandler, void *pParam)
{
	assert (pThis != 0);

	assert (nIRQ < IRQ_LINES);
	assert (pThis->m_apIRQHandler[nIRQ] == 0);

	pThis->m_apIRQHandler[nIRQ] = pHandler;
	pThis->m_pParam[nIRQ] = pParam;

	InterruptSystemEnableIRQ (nIRQ);
}

void InterruptSystemDisconnectIRQ (TInterruptSystem *pThis, unsigned nIRQ)
{
	assert (pThis != 0);

	assert (nIRQ < IRQ_LINES);
	assert (pThis->m_apIRQHandler[nIRQ] != 0);

	InterruptSystemDisableIRQ (nIRQ);

	pThis->m_apIRQHandler[nIRQ] = 0;
	pThis->m_pParam[nIRQ] = 0;
}

void InterruptSystemEnableIRQ (unsigned nIRQ)
{
	DataMemBarrier ();

	assert (nIRQ < IRQ_LINES);

	write32 (ARM_IC_IRQS_ENABLE (nIRQ), ARM_IRQ_MASK (nIRQ));

	DataMemBarrier ();
}

void InterruptSystemDisableIRQ (unsigned nIRQ)
{
	DataMemBarrier ();

	assert (nIRQ < IRQ_LINES);

	write32 (ARM_IC_IRQS_DISABLE (nIRQ), ARM_IRQ_MASK (nIRQ));

	DataMemBarrier ();
}

TInterruptSystem *InterruptSystemGet (void)
{
	assert (s_pThis != 0);
	return s_pThis;
}

int InterruptSystemCallIRQHandler (TInterruptSystem *pThis, unsigned nIRQ)
{
	assert (pThis != 0);

	assert (nIRQ < IRQ_LINES);
	TIRQHandler *pHandler = pThis->m_apIRQHandler[nIRQ];

	if (pHandler != 0)
	{
		(*pHandler) (pThis->m_pParam[nIRQ]);
		
		return 1;
	}
	else
	{
		InterruptSystemDisableIRQ (nIRQ);
	}
	
	return 0;
}

#include <stdio.h>

void InterruptHandler (void)
{
	assert (s_pThis != 0);

	DataMemBarrier ();
	
	for (unsigned nIRQ = 0; nIRQ < IRQ_LINES; nIRQ++)
	{
		u32 nPendReg = ARM_IC_IRQ_PENDING (nIRQ);
		u32 nIRQMask = ARM_IRQ_MASK (nIRQ);
		
		if (read32 (nPendReg) & nIRQMask)
		{
      /*if (nIRQ!=3) { // 3 is timer?
        printf("[irq%d]\r\n",nIRQ);
        }*/
			if (InterruptSystemCallIRQHandler (s_pThis, nIRQ))
			{
				write32 (nPendReg, nIRQMask);
			
				DataMemBarrier ();

				return;
			}

			write32 (nPendReg, nIRQMask);
		}
	}

	DataMemBarrier ();
}
