//
// memory.c
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
#include <uspienv/memory.h>
#include <uspienv/armv6mmu.h>
#include <uspienv/bcmpropertytags.h>
#include <uspienv/alloc.h>
#include <uspienv/synchronize.h>
#include <uspienv/sysconfig.h>
#include <uspienv/assert.h>

#if RASPPI == 1
#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION	\
			 | ARM_CONTROL_EXTENDED_PAGE_TABLE)

#define TTBCR_SPLIT	3
#else
#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION)

#define TTBCR_SPLIT	2
#endif

void MemorySystemEnableMMU (TMemorySystem *pThis);

void MemorySystem (TMemorySystem *pThis, boolean bEnableMMU)
{
	assert (pThis != 0);

	pThis->m_bEnableMMU = bEnableMMU;
	pThis->m_nMemSize = 0;
	pThis->m_pPageTable0Default = 0;
	pThis->m_pPageTable1 = 0;

	TBcmPropertyTags Tags;
	BcmPropertyTags (&Tags);
	TPropertyTagMemory TagMemory;
	if (!BcmPropertyTagsGetTag (&Tags, PROPTAG_GET_ARM_MEMORY, &TagMemory, sizeof TagMemory, 0))
	{
		TagMemory.nBaseAddress = 0;
		TagMemory.nSize = ARM_MEM_SIZE;
	}

	assert (TagMemory.nBaseAddress == 0);
	pThis->m_nMemSize = TagMemory.nSize;

	mem_init (TagMemory.nBaseAddress, pThis->m_nMemSize);

	if (pThis->m_bEnableMMU)
	{
		pThis->m_pPageTable0Default = (TPageTable *) malloc (sizeof (TPageTable));
		assert (pThis->m_pPageTable0Default != 0);
		PageTable2 (pThis->m_pPageTable0Default, pThis->m_nMemSize);

		pThis->m_pPageTable1 = (TPageTable *) malloc (sizeof (TPageTable));
		assert (pThis->m_pPageTable1 != 0);
		PageTable (pThis->m_pPageTable1);

		MemorySystemEnableMMU (pThis);
	}

	_BcmPropertyTags (&Tags);
}

void _MemorySystem (TMemorySystem *pThis)
{
	assert (pThis != 0);

	if (pThis->m_bEnableMMU)
	{
		// disable MMU
		u32 nControl;
		asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (nControl));
		nControl &=  ~MMU_MODE;
		asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (nControl) : "memory");

		// invalidate unified TLB (if MMU is re-enabled later)
		asm volatile ("mcr p15, 0, %0, c8, c7,  0" : : "r" (0) : "memory");
	}
	
	_PageTable (pThis->m_pPageTable1);
	free (pThis->m_pPageTable1);
	pThis->m_pPageTable1 = 0;
	
	_PageTable (pThis->m_pPageTable0Default);
	free (pThis->m_pPageTable0Default);
	pThis->m_pPageTable0Default = 0;
}

u32 MemorySystemGetMemSize (TMemorySystem *pThis)
{
	assert (pThis != 0);

	return pThis->m_nMemSize;
}

void MemorySystemEnableMMU (TMemorySystem *pThis)
{
	assert (pThis != 0);

	assert (pThis->m_bEnableMMU);

	u32 nAuxControl;
	asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (nAuxControl));
#if RASPPI == 1
	nAuxControl |= ARM_AUX_CONTROL_CACHE_SIZE;	// restrict cache size (no page coloring)
#else
	nAuxControl |= ARM_AUX_CONTROL_SMP;
#endif
	asm volatile ("mcr p15, 0, %0, c1, c0,  1" : : "r" (nAuxControl));

	u32 nTLBType;
	asm volatile ("mrc p15, 0, %0, c0, c0,  3" : "=r" (nTLBType));
	assert (!(nTLBType & ARM_TLB_TYPE_SEPARATE_TLBS));

	// set TTB control
	asm volatile ("mcr p15, 0, %0, c2, c0,  2" : : "r" (TTBCR_SPLIT));

	// set TTBR0
	assert (pThis->m_pPageTable0Default != 0);
	asm volatile ("mcr p15, 0, %0, c2, c0,  0" : : "r" (PageTableGetBaseAddress (pThis->m_pPageTable0Default)));

	// set TTBR1
	assert (pThis->m_pPageTable1 != 0);
	asm volatile ("mcr p15, 0, %0, c2, c0,  1" : : "r" (PageTableGetBaseAddress (pThis->m_pPageTable1)));
	
	// set Domain Access Control register (Domain 0 and 1 to client)
	asm volatile ("mcr p15, 0, %0, c3, c0,  0" : : "r" (  DOMAIN_CLIENT << 0
							    | DOMAIN_CLIENT << 2));

	InvalidateDataCache ();
	FlushPrefetchBuffer ();

	// enable MMU
	u32 nControl;
	asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (nControl));
#if RASPPI == 1
#ifdef ARM_STRICT_ALIGNMENT
	nControl &= ~ARM_CONTROL_UNALIGNED_PERMITTED;
	nControl |= ARM_CONTROL_STRICT_ALIGNMENT;
#else
	nControl &= ~ARM_CONTROL_STRICT_ALIGNMENT;
	nControl |= ARM_CONTROL_UNALIGNED_PERMITTED;
#endif
#endif
	nControl |= MMU_MODE;
	asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (nControl) : "memory");
}
