//
// chargenerator.c
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
#include <uspienv/chargenerator.h>
#include <uspienv/assert.h>
#include "font.h"

#undef GIMP_HEADER			// if font saved with GIMP with .h extension

#define FIRSTCHAR	'!'
#define LASTCHAR	'~'
#define CHARCOUNT	(LASTCHAR - FIRSTCHAR + 1)

void CharGenerator (TCharGenerator *pThis)
{
	assert (pThis != 0);

#ifdef GIMP_HEADER
	pThis->m_nCharWidth = width / CHARCOUNT;
#else
	pThis->m_nCharWidth = width;
#endif
}

void _CharGenerator (TCharGenerator *pThis)
{
}

unsigned CharGeneratorGetCharWidth (TCharGenerator *pThis)
{
	assert (pThis != 0);
	return pThis->m_nCharWidth;
}

unsigned CharGeneratorGetCharHeight (TCharGenerator *pThis)
{
	assert (pThis != 0);

#ifdef GIMP_HEADER
	return height;
#else
	return height + extraheight;
#endif
}

unsigned CharGeneratorGetUnderline (TCharGenerator *pThis)
{
	assert (pThis != 0);

#ifdef GIMP_HEADER
	return height-3;
#else
	return height;
#endif
}

boolean CharGeneratorGetPixel (TCharGenerator *pThis, char chAscii, unsigned nPosX, unsigned nPosY)
{
	assert (pThis != 0);

	if (   chAscii < FIRSTCHAR
	    || chAscii > LASTCHAR)
	{
		return FALSE;
	}

	unsigned nIndex = chAscii - FIRSTCHAR;
	assert (nIndex < CHARCOUNT);

	assert (nPosX < pThis->m_nCharWidth);

#ifdef GIMP_HEADER
	assert (nPosY < height);
	unsigned nOffset = nPosY * width + nIndex * pThis->m_nCharWidth + nPosX;

	assert (nOffset < sizeof header_data / sizeof header_data[0]);
	return header_data[nOffset] ? TRUE : FALSE;
#else
	if (nPosY >= height)
	{
		return FALSE;
	}

	return font_data[nIndex][nPosY] & (0x80 >> nPosX) ? TRUE : FALSE;
#endif
}
