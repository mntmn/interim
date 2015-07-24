//
// dwhciregister.c
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
#include <uspi/dwhciregister.h>
#include <uspios.h>
#include <uspi/assert.h>

void DWHCIRegister (TDWHCIRegister *pThis, u32 nAddress)
{
	assert (pThis != 0);
	pThis->m_bValid = FALSE;
	pThis->m_nAddress = nAddress;
}

void DWHCIRegister2 (TDWHCIRegister *pThis, u32 nAddress, u32 nValue)
{
	assert (pThis != 0);
	pThis->m_bValid = TRUE;
	pThis->m_nAddress = nAddress;
	pThis->m_nBuffer = nValue;
}

void _DWHCIRegister (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	pThis->m_bValid = FALSE;
}

u32 DWHCIRegisterRead (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	pThis->m_nBuffer = *(u32 *) pThis->m_nAddress;
	pThis->m_bValid = TRUE;
	
	return pThis->m_nBuffer;
}

void DWHCIRegisterWrite (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	*(u32 *) pThis->m_nAddress = pThis->m_nBuffer;
}

u32 DWHCIRegisterGet (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	return pThis->m_nBuffer;
}

void DWHCIRegisterSet (TDWHCIRegister *pThis, u32 nValue)
{
	assert (pThis != 0);
	pThis->m_nBuffer = nValue;
	pThis->m_bValid = TRUE;
}

boolean DWHCIRegisterIsSet (TDWHCIRegister *pThis, u32 nMask)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	return pThis->m_nBuffer & nMask ? TRUE : FALSE;
}

void DWHCIRegisterAnd (TDWHCIRegister *pThis, u32 nMask)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	pThis->m_nBuffer &= nMask;
}

void DWHCIRegisterOr (TDWHCIRegister *pThis, u32 nMask)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	pThis->m_nBuffer |= nMask;
}

void DWHCIRegisterClearBit (TDWHCIRegister *pThis, unsigned nBit)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	assert (nBit < sizeof pThis->m_nBuffer * 8);
	pThis->m_nBuffer &= ~(1 << nBit);
}

void DWHCIRegisterSetBit (TDWHCIRegister *pThis, unsigned nBit)
{
	assert (pThis != 0);
	assert (pThis->m_bValid);
	assert (nBit < sizeof pThis->m_nBuffer * 8);
	pThis->m_nBuffer |= 1 << nBit;
}

void DWHCIRegisterClearAll (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	pThis->m_nBuffer = 0;
	pThis->m_bValid = TRUE;
}

void DWHCIRegisterSetAll (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	pThis->m_nBuffer = (u32) -1;
	pThis->m_bValid = TRUE;
}

#ifndef NDEBUG

void DWHCIRegisterDump (TDWHCIRegister *pThis)
{
	assert (pThis != 0);
	if (pThis->m_bValid)
	{
		LogWrite ("dwhci", LOG_DEBUG,
			     "Register at 0x%X is 0x%X",
			     pThis->m_nAddress & 0xFFF, pThis->m_nBuffer);
	}
	else
	{
		LogWrite ("dwhci", LOG_DEBUG,
			     "Register at 0x%X was not set",
			     pThis->m_nAddress & 0xFFF);
	}
}

#endif
