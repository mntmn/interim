//
// memory.h
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
#ifndef _uspienv_memory_h
#define _uspienv_memory_h

#include <uspienv/pagetable.h>
#include <uspienv/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TMemorySystem
{
	boolean m_bEnableMMU;
	u32 m_nMemSize;

	TPageTable *m_pPageTable0Default;
	TPageTable *m_pPageTable1;
}
TMemorySystem;

void MemorySystem (TMemorySystem *pThis, boolean bEnableMMU);
void _MemorySystem (TMemorySystem *pThis);

u32 MemorySystemGetMemSize (TMemorySystem *pThis);

#ifdef __cplusplus
}
#endif

#endif
