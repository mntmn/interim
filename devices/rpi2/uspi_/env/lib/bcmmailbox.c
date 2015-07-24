//
// bcmmailbox.c
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
#include <uspienv/bcmmailbox.h>
#include <uspienv/memio.h>
#include <uspienv/synchronize.h>
#include <uspienv/timer.h>
#include <uspienv/assert.h>

void BcmMailBoxFlush (TBcmMailBox *pThis);
unsigned BcmMailBoxRead (TBcmMailBox *pThis);
void BcmMailBoxWrite (TBcmMailBox *pThis, unsigned nData);

void BcmMailBox (TBcmMailBox *pThis, unsigned nChannel)
{
	assert (pThis != 0);

	pThis->m_nChannel = nChannel;
}

void _BcmMailBox (TBcmMailBox *pThis)
{
	assert (pThis != 0);

}

unsigned BcmMailBoxWriteRead (TBcmMailBox *pThis, unsigned nData)
{
	assert (pThis != 0);

	DataMemBarrier ();

	BcmMailBoxFlush (pThis);

	BcmMailBoxWrite (pThis, nData);

	unsigned nResult = BcmMailBoxRead (pThis);

	DataMemBarrier ();

	return nResult;
}

void BcmMailBoxFlush (TBcmMailBox *pThis)
{
	assert (pThis != 0);

	while (!(read32 (MAILBOX_STATUS) & MAILBOX_STATUS_EMPTY))
	{
		read32 (MAILBOX_READ);

		TimerSimpleMsDelay (20);
	}
}

unsigned BcmMailBoxRead (TBcmMailBox *pThis)
{
	assert (pThis != 0);

	unsigned nResult;
	
	do
	{
		while (read32 (MAILBOX_STATUS) & MAILBOX_STATUS_EMPTY)
		{
			// do nothing
		}
		
		nResult = read32 (MAILBOX_READ);
	}
	while ((nResult & 0xF) != pThis->m_nChannel);		// channel number is in the lower 4 bits

	return nResult & ~0xF;
}

void BcmMailBoxWrite (TBcmMailBox *pThis, unsigned nData)
{
	assert (pThis != 0);

	while (read32 (MAILBOX_STATUS) & MAILBOX_STATUS_FULL)
	{
		// do nothing
	}

	assert ((nData & 0xF) == 0);
	write32 (MAILBOX_WRITE, pThis->m_nChannel | nData);	// channel number is in the lower 4 bits
}
