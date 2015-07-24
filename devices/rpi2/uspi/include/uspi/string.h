//
// string.h
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
#ifndef _uspi_string_h
#define _uspi_string_h

#include <uspi/stdarg.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct TString
{
	char 	 *m_pBuffer;
	unsigned  m_nSize;
	char	 *m_pInPtr;
}
TString;

void String (TString *pThis);
void String2 (TString *pThis, const char *pString);
void _String (TString *pThis);

const char *StringGet (TString *pThis);
const char *StringSet (TString *pThis, const char *pString);

size_t StringGetLength (TString *pThis);

void StringAppend (TString *pThis, const char *pString);
int StringCompare (TString *pThis, const char *pString);
int StringFind (TString *pThis, char chChar);			// returns index or -1 if not found

void StringFormat (TString *pThis, const char *pFormat, ...);	// supports only a small subset of printf(3)
void StringFormatV (TString *pThis, const char *pFormat, va_list Args);

#ifdef __cplusplus
}
#endif

#endif
