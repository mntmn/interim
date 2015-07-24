//
// chargenerator.h
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
#ifndef _uspienv_chargenerator_h
#define _uspienv_chargenerator_h

#include <uspienv/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TCharGenerator
{
	unsigned m_nCharWidth;
}
TCharGenerator;

void CharGenerator (TCharGenerator *pThis);
void _CharGenerator (TCharGenerator *pThis);

unsigned CharGeneratorGetCharWidth (TCharGenerator *pThis);
unsigned CharGeneratorGetCharHeight (TCharGenerator *pThis);
unsigned CharGeneratorGetUnderline (TCharGenerator *pThis);

boolean CharGeneratorGetPixel (TCharGenerator *pThis, char chAscii, unsigned nPosX, unsigned nPosY);

#ifdef __cplusplus
}
#endif

#endif
