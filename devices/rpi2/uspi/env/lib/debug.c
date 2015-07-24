//
// debug.cpp
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
#include <uspienv/debug.h>
#include <uspienv/logger.h>
#include <uspienv/string.h>
#include <uspienv/sysconfig.h>

#ifndef NDEBUG

static const char FromDebug[] = "debug";

void debug_hexdump (const void *pStart, unsigned nBytes, const char *pSource)
{
	u8 *pOffset = (u8 *) pStart;

	if (pSource == 0)
	{
		pSource = FromDebug;
	}

	LoggerWrite (LoggerGet (), pSource, LogDebug, "Dumping 0x%X bytes starting at 0x%X", nBytes, (unsigned) pOffset);
	
	while (nBytes > 0)
	{
		LoggerWrite (LoggerGet (), pSource, LogDebug,
				"%04X: %02X %02X %02X %02X %02X %02X %02X %02X-%02X %02X %02X %02X %02X %02X %02X %02X",
				(unsigned) pOffset & 0xFFFF,
				(unsigned) pOffset[0],  (unsigned) pOffset[1],  (unsigned) pOffset[2],  (unsigned) pOffset[3],
				(unsigned) pOffset[4],  (unsigned) pOffset[5],  (unsigned) pOffset[6],  (unsigned) pOffset[7],
				(unsigned) pOffset[8],  (unsigned) pOffset[9],  (unsigned) pOffset[10], (unsigned) pOffset[11],
				(unsigned) pOffset[12], (unsigned) pOffset[13], (unsigned) pOffset[14], (unsigned) pOffset[15]);

		pOffset += 16;

		if (nBytes >= 16)
		{
			nBytes -= 16;
		}
		else
		{
			nBytes = 0;
		}
	}
}

void debug_stacktrace (const u32 *pStackPtr, const char *pSource)
{
	if (pSource == 0)
	{
		pSource = FromDebug;
	}
	
	for (unsigned i = 0; i < 64; i++, pStackPtr++)
	{
		extern unsigned char _etext;

		if (   *pStackPtr >= MEM_KERNEL_START
		    && *pStackPtr < (u32) &_etext)
		{
			LoggerWrite (LoggerGet (), pSource, LogDebug, "stack[%u] is 0x%X", i, (unsigned) *pStackPtr);
		}
	}
}

#endif
