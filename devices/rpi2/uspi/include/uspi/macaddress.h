//
// macaddress.h
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
#ifndef _uspi_macaddress_h
#define _uspi_macaddress_h

#include <uspi/string.h>
#include <uspi/types.h>

#define MAC_ADDRESS_SIZE	6

typedef struct TMACAddress
{
	boolean m_bValid;

	u8 m_Address[MAC_ADDRESS_SIZE];
}
TMACAddress;

void MACAddress (TMACAddress *pThis);
void MACAddress2 (TMACAddress *pThis, const u8 *pAddress);
void _MACAddress (TMACAddress *pThis);

boolean MACAddressIsEqual (TMACAddress *pThis, TMACAddress *pAddress2);

void MACAddressSet (TMACAddress *pThis, const u8 *pAddress);
void MACAddressSetBroadcast (TMACAddress *pThis);
const u8 *MACAddressGet (TMACAddress *pThis);
void MACAddressCopyTo (TMACAddress *pThis, u8 *pBuffer);

boolean MACAddressIsBroadcast (TMACAddress *pThis);
unsigned MACAddressGetSize (TMACAddress *pThis);

void MACAddressFormat (TMACAddress *pThis, TString *pString);

#endif
