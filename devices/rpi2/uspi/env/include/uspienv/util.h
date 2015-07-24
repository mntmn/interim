//
// util.h
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
#ifndef _uspienv_util_h
#define _uspienv_util_h

#include <uspienv/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void *memset (void *pBuffer, int nValue, size_t nLength);

void *memcpy (void *pDest, const void *pSrc, size_t nLength);

int memcmp (const void *pBuffer1, const void *pBuffer2, size_t nLength);

size_t strlen (const char *pString);

int strcmp (const char *pString1, const char *pString2);

char *strcpy (char *pDest, const char *pSrc);

char *strncpy (char *pDest, const char *pSrc, size_t nMaxLen);

char *strcat (char *pDest, const char *pSrc);

int char2int (char chValue);			// with sign extension

u16 le2be16 (u16 usValue);

u32 le2be32 (u32 ulValue);

// util_fast
void *memcpyblk (void *pDest, const void *pSrc, size_t nLength);	// nLength must be multiple of 16

#ifdef __cplusplus
}
#endif

#endif
