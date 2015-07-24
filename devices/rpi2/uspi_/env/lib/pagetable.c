//
// pagetable.c
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
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
#include <uspienv/pagetable.h>
#include <uspienv/alloc.h>
#include <uspienv/sysconfig.h>
#include <uspienv/synchronize.h>
#include <uspienv/util.h>
#include <uspienv/assert.h>

#if RASPPI == 1
#define SDRAM_SIZE_MBYTE	512

#define TTBR_MODE	(  ARM_TTBR_INNER_CACHEABLE		\
			 | ARM_TTBR_OUTER_NON_CACHEABLE)
#else
#define SDRAM_SIZE_MBYTE	1024

#define TTBR_MODE	(  ARM_TTBR_INNER_WRITE_BACK		\
			 | ARM_TTBR_OUTER_WRITE_BACK)
#endif

void PageTable (TPageTable *pThis)
{
	assert (pThis != 0);

	pThis->m_bTableAllocated = FALSE;
	pThis->m_pTable = (TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR *) MEM_PAGE_TABLE1;

	assert (((u32) pThis->m_pTable & 0x3FFF) == 0);

	for (unsigned nEntry = 0; nEntry < 4096; nEntry++)
	{
		u32 nBaseAddress = MEGABYTE * nEntry;
		
		TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR *pEntry = &pThis->m_pTable[nEntry];

		// shared device
		pEntry->Value10	= 2;
		pEntry->BBit    = 1;
		pEntry->CBit    = 0;
		pEntry->XNBit   = 0;
		pEntry->Domain	= 0;
		pEntry->IMPBit	= 0;
		pEntry->AP	= AP_SYSTEM_ACCESS;
		pEntry->TEX     = 0;
		pEntry->APXBit	= APX_RW_ACCESS;
		pEntry->SBit    = 1;
		pEntry->NGBit	= 0;
		pEntry->Value0	= 0;
		pEntry->SBZ	= 0;
		pEntry->Base	= ARMV6MMUL1SECTIONBASE (nBaseAddress);

		if (nEntry >= SDRAM_SIZE_MBYTE)
		{
			pEntry->XNBit = 1;
		}
	}

	CleanDataCache ();
	DataSyncBarrier ();
}

void PageTable2 (TPageTable *pThis, u32 nMemSize)
{
	assert (pThis != 0);

	pThis->m_bTableAllocated = TRUE;
	pThis->m_pTable = (TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR *) palloc ();

	assert (pThis->m_pTable != 0);
	assert (((u32) pThis->m_pTable & 0xFFF) == 0);

	for (unsigned nEntry = 0; nEntry < SDRAM_SIZE_MBYTE; nEntry++)
	{
		u32 nBaseAddress = MEGABYTE * nEntry;

		TARMV6MMU_LEVEL1_SECTION_DESCRIPTOR *pEntry = &pThis->m_pTable[nEntry];

		// outer and inner write back, no write allocate
		pEntry->Value10	= 2;
		pEntry->BBit	= 1;
		pEntry->CBit	= 1;
		pEntry->XNBit	= 0;
		pEntry->Domain	= 0;
		pEntry->IMPBit	= 0;
		pEntry->AP	= AP_SYSTEM_ACCESS;
		pEntry->TEX	= 0;
		pEntry->APXBit	= APX_RW_ACCESS;
		pEntry->SBit	= 0;
		pEntry->NGBit	= 0;
		pEntry->Value0	= 0;
		pEntry->SBZ	= 0;
		pEntry->Base	= ARMV6MMUL1SECTIONBASE (nBaseAddress);

		extern u8 _etext;
		if (nBaseAddress >= (u32) &_etext)
		{
			pEntry->XNBit = 1;

			if (nBaseAddress >= nMemSize)
			{
				// shared device
				pEntry->BBit  = 1;
				pEntry->CBit  = 0;
				pEntry->TEX   = 0;
				pEntry->SBit  = 1;
			}
		}
	}

	CleanDataCache ();
	DataSyncBarrier ();
}

void _PageTable (TPageTable *pThis)
{
	assert (pThis != 0);

	if (pThis->m_bTableAllocated)
	{
		pfree (pThis->m_pTable);
		pThis->m_pTable = 0;
	}
}

u32 PageTableGetBaseAddress (TPageTable *pThis)
{
	assert (pThis != 0);

	return (u32) pThis->m_pTable | TTBR_MODE;
}
