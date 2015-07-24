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
#ifndef _uspi_util_h
#define _uspi_util_h

#include <uspios.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USPI_PROVIDE_MEM_FUNCTIONS
	#define memset		uspi_memset
	#define memcpy		uspi_memcpy
	#define memcmp		uspi_memcmp
#endif

#ifdef USPI_PROVIDE_STR_FUNCTIONS
	#define strlen		uspi_strlen
	#define strcmp		uspi_strcmp
	#define strcpy		uspi_strcpy
	#define strncpy		uspi_strncpy
	#define strcat		uspi_strcat
#endif

void *memset (void *pBuffer, int nValue, size_t nLength);

void *memcpy (void *pDest, const void *pSrc, size_t nLength);

int memcmp (const void *pBuffer1, const void *pBuffer2, size_t nLength);

size_t strlen (const char *pString);

int strcmp (const char *pString1, const char *pString2);

char *strcpy (char *pDest, const char *pSrc);

char *strncpy (char *pDest, const char *pSrc, size_t nMaxLen);

char *strcat (char *pDest, const char *pSrc);

int uspi_char2int (char chValue);		// with sign extension

u16 uspi_le2be16 (u16 usValue);

u32 uspi_le2be32 (u32 ulValue);

#ifdef __cplusplus
}
#endif

#endif
