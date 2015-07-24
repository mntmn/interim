//
// util.c
//
// Circle - A C++ bare metal environment for Raspberry Pi
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
#include <uspi/util.h>

#ifdef USPI_PROVIDE_MEM_FUNCTIONS

void *uspi_memset (void *pBuffer, int nValue, size_t nLength)
{
	char *p = (char *) pBuffer;

	while (nLength--)
	{
		*p++ = (char) nValue;
	}

	return pBuffer;
}

void *uspi_memcpy (void *pDest, const void *pSrc, size_t nLength)
{
	char *pd = (char *) pDest;
	char *ps = (char *) pSrc;

	while (nLength--)
	{
		*pd++ = *ps++;
	}

	return pDest;
}

int uspi_memcmp (const void *pBuffer1, const void *pBuffer2, size_t nLength)
{
	const unsigned char *p1 = (const unsigned char *) pBuffer1;
	const unsigned char *p2 = (const unsigned char *) pBuffer2;
	
	while (nLength-- > 0)
	{
		if (*p1 > *p2)
		{
			return 1;
		}
		else if (*p1 < *p2)
		{
			return -1;
		}

		p1++;
		p2++;
	}

	return 0;
}

#endif

#ifdef USPI_PROVIDE_STR_FUNCTIONS

size_t uspi_strlen (const char *pString)
{
	size_t nResult = 0;

	while (*pString++)
	{
		nResult++;
	}

	return nResult;
}

int uspi_strcmp (const char *pString1, const char *pString2)
{
	while (   *pString1 != '\0'
	       && *pString2 != '\0')
	{
		if (*pString1 > *pString2)
		{
			return 1;
		}
		else if (*pString1 < *pString2)
		{
			return -1;
		}

		pString1++;
		pString2++;
	}

	if (*pString1 > *pString2)
	{
		return 1;
	}
	else if (*pString1 < *pString2)
	{
		return -1;
	}

	return 0;
}

char *uspi_strcpy (char *pDest, const char *pSrc)
{
	char *p = pDest;

	while (*pSrc)
	{
		*p++ = *pSrc++;
	}

	*p = '\0';

	return pDest;
}

char *uspi_strncpy (char *pDest, const char *pSrc, size_t nMaxLen)
{
	char *pResult = pDest;

	while (nMaxLen > 0)
	{
		if (*pSrc == '\0')
		{
			break;
		}

		*pDest++ = *pSrc++;
		nMaxLen--;
	}

	if (nMaxLen > 0)
	{
		*pDest = '\0';
	}

	return pResult;
}

char *uspi_strcat (char *pDest, const char *pSrc)
{
	char *p = pDest;

	while (*p)
	{
		p++;
	}

	while (*pSrc)
	{
		*p++ = *pSrc++;
	}

	*p = '\0';

	return pDest;
}

#endif

int uspi_char2int (char chValue)
{
	int nResult = chValue;

	if (nResult > 0x7F)
	{
		nResult |= -0x100;
	}

	return nResult;
}

u16 uspi_le2be16 (u16 usValue)
{
	return    ((usValue & 0x00FF) << 8)
		| ((usValue & 0xFF00) >> 8);
}

u32 uspi_le2be32 (u32 ulValue)
{
	return    ((ulValue & 0x000000FF) << 24)
		| ((ulValue & 0x0000FF00) << 8)
		| ((ulValue & 0x00FF0000) >> 8)
		| ((ulValue & 0xFF000000) >> 24);
}
