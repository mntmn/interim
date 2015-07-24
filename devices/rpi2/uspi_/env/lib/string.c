//
// string.c
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
#include <uspienv/string.h>
#include <uspienv/alloc.h>
#include <uspienv/util.h>
#include <uspienv/assert.h>

#define FORMAT_RESERVE		64	// additional bytes to allocate
#define MAX_NUMBER_LEN		11	// 32 bit octal number

void StringPutChar (TString *pThis, char chChar, size_t nCount /* = 1 */);
void StringPutString (TString *pThis, const char *pString);
void StringReserveSpace (TString *pThis, size_t nSpace);
char *ntoa (char *pDest, unsigned long ulNumber, unsigned nBase, boolean bUpcase);

void String (TString *pThis)
{
	assert (pThis != 0);
	pThis->m_pBuffer = 0;
	pThis->m_nSize = 0;
}

void String2 (TString *pThis, const char *pString)
{
	assert (pThis != 0);

	pThis->m_nSize = strlen (pString)+1;

	pThis->m_pBuffer = (char *) malloc (pThis->m_nSize);

	strcpy (pThis->m_pBuffer, pString);
}

void _String (TString *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pBuffer != 0)
	{
		free (pThis->m_pBuffer);
		pThis->m_pBuffer = 0;
	}
}

const char *StringGet (TString *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pBuffer != 0)
	{
		return pThis->m_pBuffer;
	}

	return "";
}

const char *StringSet (TString *pThis, const char *pString)
{
	assert (pThis != 0);

	if (pThis->m_pBuffer != 0)
	{
		free (pThis->m_pBuffer);
	}
	
	pThis->m_nSize = strlen (pString)+1;

	pThis->m_pBuffer = (char *) malloc (pThis->m_nSize);

	strcpy (pThis->m_pBuffer, pString);

	return pThis->m_pBuffer;
}

size_t StringGetLength (TString *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pBuffer == 0)
	{
		return 0;
	}
	
	return strlen (pThis->m_pBuffer);
}

void StringAppend (TString *pThis, const char *pString)
{
	assert (pThis != 0);

	pThis->m_nSize = 1;		// for terminating '\0'
	if (pThis->m_pBuffer != 0)
	{
		pThis->m_nSize += strlen (pThis->m_pBuffer);
	}
	pThis->m_nSize += strlen (pString);

	char *pBuffer = (char *) malloc (pThis->m_nSize);

	if (pThis->m_pBuffer != 0)
	{
		strcpy (pBuffer, pThis->m_pBuffer);
		free (pThis->m_pBuffer);
	}
	else
	{
		*pBuffer = '\0';
	}

	strcat (pBuffer, pString);

	pThis->m_pBuffer = pBuffer;
}

int StringCompare (TString *pThis, const char *pString)
{
	assert (pThis != 0);

	return strcmp (pThis->m_pBuffer, pString);
}

int StringFind (TString *pThis, char chChar)
{
	assert (pThis != 0);

	int nPos = 0;

	char *p;
	for (p = pThis->m_pBuffer; *p; p++)
	{
		if (*p == chChar)
		{
			return nPos;
		}

		nPos++;
	}

	return -1;
}

void StringFormat (TString *pThis, const char *pFormat, ...)
{
	assert (pThis != 0);

	va_list var;
	va_start (var, pFormat);

	StringFormatV (pThis, pFormat, var);

	va_end (var);
}

void StringFormatV (TString *pThis, const char *pFormat, va_list Args)
{
	assert (pThis != 0);

	if (pThis->m_pBuffer != 0)
	{
		free (pThis->m_pBuffer);
	}
	
	pThis->m_nSize = FORMAT_RESERVE;
	pThis->m_pBuffer = (char *) malloc (pThis->m_nSize);
	pThis->m_pInPtr = pThis->m_pBuffer;

	while (*pFormat != '\0')
	{
		if (*pFormat == '%')
		{
			if (*++pFormat == '%')
			{
				StringPutChar (pThis, '%', 1);
				
				pFormat++;

				continue;
			}

			boolean bLeft = FALSE;
			if (*pFormat == '-')
			{
				bLeft = TRUE;

				pFormat++;
			}

			boolean bNull = FALSE;
			if (*pFormat == '0')
			{
				bNull = TRUE;

				pFormat++;
			}

			size_t nWidth = 0;
			while ('0' <= *pFormat && *pFormat <= '9')
			{
				nWidth = nWidth * 10 + (*pFormat - '0');

				pFormat++;
			}

			boolean bLong = FALSE;
			if (*pFormat == 'l')
			{
				bLong = TRUE;

				pFormat++;
			}

			char chArg;
			const char *pArg;
			unsigned long ulArg;
			size_t nLen;
			unsigned nBase;
			char NumBuf[MAX_NUMBER_LEN+1];
			boolean bMinus = FALSE;
			long lArg;

			switch (*pFormat)
			{
			case 'c':
				chArg = (char) va_arg (Args, int);
				if (bLeft)
				{
					StringPutChar (pThis, chArg, 1);
					if (nWidth > 1)
					{
						StringPutChar (pThis, ' ', nWidth-1);
					}
				}
				else
				{
					if (nWidth > 1)
					{
						StringPutChar (pThis, ' ', nWidth-1);
					}
					StringPutChar (pThis, chArg, 1);
				}
				break;

			case 'd':
				if (bLong)
				{
					lArg = va_arg (Args, long);
				}
				else
				{
					lArg = va_arg (Args, int);
				}
				if (lArg < 0)
				{
					bMinus = TRUE;
					lArg = -lArg;
				}
				ntoa (NumBuf, (unsigned long) lArg, 10, FALSE);
				nLen = strlen (NumBuf) + (bMinus ? 1 : 0);
				if (bLeft)
				{
					if (bMinus)
					{
						StringPutChar (pThis, '-', 1);
					}
					StringPutString (pThis, NumBuf);
					if (nWidth > nLen)
					{
						StringPutChar (pThis, ' ', nWidth-nLen);
					}
				}
				else
				{
					if (nWidth > nLen)
					{
						StringPutChar (pThis, ' ', nWidth-nLen);
					}
					if (bMinus)
					{
						StringPutChar (pThis, '-', 1);
					}
					StringPutString (pThis, NumBuf);
				}
				break;

			case 'o':
				nBase = 8;
				goto FormatNumber;

			case 's':
				pArg = va_arg (Args, const char *);
				nLen = strlen (pArg);
				if (bLeft)
				{
					StringPutString (pThis, pArg);
					if (nWidth > nLen)
					{
						StringPutChar (pThis, ' ', nWidth-nLen);
					}
				}
				else
				{
					if (nWidth > nLen)
					{
						StringPutChar (pThis, ' ', nWidth-nLen);
					}
					StringPutString (pThis, pArg);
				}
				break;

			case 'u':
				nBase = 10;
				goto FormatNumber;

			case 'x':
			case 'X':
				nBase = 16;
				goto FormatNumber;

			FormatNumber:
				if (bLong)
				{
					ulArg = va_arg (Args, unsigned long);
				}
				else
				{
					ulArg = va_arg (Args, unsigned);
				}
				ntoa (NumBuf, ulArg, nBase, *pFormat == 'X');
				nLen = strlen (NumBuf);
				if (bLeft)
				{
					StringPutString (pThis, NumBuf);
					if (nWidth > nLen)
					{
						StringPutChar (pThis, ' ', nWidth-nLen);
					}
				}
				else
				{
					if (nWidth > nLen)
					{
						StringPutChar (pThis, bNull ? '0' : ' ', nWidth-nLen);
					}
					StringPutString (pThis, NumBuf);
				}
				break;

			default:
				StringPutChar (pThis, '%', 1);
				StringPutChar (pThis, *pFormat, 1);
				break;
			}
		}
		else
		{
			StringPutChar (pThis, *pFormat, 1);
		}

		pFormat++;
	}

	*pThis->m_pInPtr = '\0';
}

void StringPutChar (TString *pThis, char chChar, size_t nCount)
{
	assert (pThis != 0);

	StringReserveSpace (pThis, nCount);

	while (nCount--)
	{
		*pThis->m_pInPtr++ = chChar;
	}
}

void StringPutString (TString *pThis, const char *pString)
{
	assert (pThis != 0);

	size_t nLen = strlen (pString);
	
	StringReserveSpace (pThis, nLen);
	
	strcpy (pThis->m_pInPtr, pString);
	
	pThis->m_pInPtr += nLen;
}

void StringReserveSpace (TString *pThis, size_t nSpace)
{
	assert (pThis != 0);

	if (nSpace == 0)
	{
		return;
	}
	
	size_t nOffset = pThis->m_pInPtr - pThis->m_pBuffer;
	size_t nNewSize = nOffset + nSpace + 1;
	if (pThis->m_nSize >= nNewSize)
	{
		return;
	}
	
	nNewSize += FORMAT_RESERVE;
	char *pNewBuffer = (char *) malloc (nNewSize);
		
	*pThis->m_pInPtr = '\0';
	strcpy (pNewBuffer, pThis->m_pBuffer);
	
	free (pThis->m_pBuffer);
	
	pThis->m_pBuffer = pNewBuffer;
	pThis->m_nSize = nNewSize;

	pThis->m_pInPtr = pThis->m_pBuffer + nOffset;
}

char *ntoa (char *pDest, unsigned long ulNumber, unsigned nBase, boolean bUpcase)
{
	unsigned long ulDigit;

	unsigned long ulDivisor = 1UL;
	while (1)
	{
		ulDigit = ulNumber / ulDivisor;
		if (ulDigit < nBase)
		{
			break;
		}

		ulDivisor *= nBase;
	}

	char *p = pDest;
	while (1)
	{
		ulNumber %= ulDivisor;

		*p++ = ulDigit < 10 ? '0' + ulDigit : '0' + ulDigit + 7 + (bUpcase ? 0 : 0x20);

		ulDivisor /= nBase;
		if (ulDivisor == 0)
		{
			break;
		}

		ulDigit = ulNumber / ulDivisor;
	}

	*p = '\0';

	return pDest;
}
