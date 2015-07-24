//
// dwhciregister.h
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
#ifndef _uspi_dwhciregister_h
#define _uspi_dwhciregister_h

#include <uspi/dwhci.h>
#include <uspi/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDWHCIRegister
{
	boolean	m_bValid;
	u32	m_nAddress;
	u32	m_nBuffer;
}
TDWHCIRegister;

void DWHCIRegister (TDWHCIRegister *pThis, u32 nAddress);
void DWHCIRegister2 (TDWHCIRegister *pThis, u32 nAddress, u32 nValue);
void _DWHCIRegister (TDWHCIRegister *pThis);

u32 DWHCIRegisterRead (TDWHCIRegister *pThis);
void DWHCIRegisterWrite (TDWHCIRegister *pThis);

u32 DWHCIRegisterGet (TDWHCIRegister *pThis);
void DWHCIRegisterSet (TDWHCIRegister *pThis, u32 nValue);

boolean DWHCIRegisterIsSet (TDWHCIRegister *pThis, u32 nMask);

void DWHCIRegisterAnd (TDWHCIRegister *pThis, u32 nMask);
void DWHCIRegisterOr (TDWHCIRegister *pThis, u32 nMask);

void DWHCIRegisterClearBit (TDWHCIRegister *pThis, unsigned nBit);
void DWHCIRegisterSetBit (TDWHCIRegister *pThis, unsigned nBit);
void DWHCIRegisterClearAll (TDWHCIRegister *pThis);
void DWHCIRegisterSetAll (TDWHCIRegister *pThis);

#ifndef NDEBUG

void DWHCIRegisterDump (TDWHCIRegister *pThis);

#endif

#ifdef __cplusplus
}
#endif

#endif
