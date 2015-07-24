//
// alloc.h
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
#ifndef _uspienv_alloc_h
#define _uspienv_alloc_h

#define MEM_PAGE_ALLOC
//#define MEM_DEBUG

#ifdef __cplusplus
extern "C" {
#endif

void mem_init (unsigned long ulBase, unsigned long ulSize);

unsigned long mem_get_size (void);

void *malloc (unsigned long ulSize);	// resulting block is always 16 bytes aligned
void free (void *pBlock);

#ifdef MEM_PAGE_ALLOC

void *palloc (void);		// returns 4K page (aligned)
void pfree (void *pPage);

#endif

#ifdef MEM_DEBUG

void mem_info (void);

#endif

#ifdef __cplusplus
}
#endif

#endif
