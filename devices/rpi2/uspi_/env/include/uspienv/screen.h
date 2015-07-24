//
// screen.h
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
#ifndef _uspienv_screen_h
#define _uspienv_screen_h

#include <uspienv/bcmframebuffer.h>
#include <uspienv/chargenerator.h>
#include <uspienv/macros.h>
#include <uspienv/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEPTH	8		// can be: 8, 16 or 32

// really ((green) & 0x3F) << 5, but to have a 0-31 range for all colors
#define COLOR16(red, green, blue)	  (((red) & 0x1F) << 11 \
					| ((green) & 0x1F) << 6 \
					| ((blue) & 0x1F))

#define COLOR32(red, green, blue, alpha)  (((red) & 0xFF)        \
					| ((green) & 0xFF) << 8  \
					| ((blue) & 0xFF) << 16  \
					| ((alpha) & 0xFF) << 24)

#define BLACK_COLOR	0

#if DEPTH == 8
	typedef u8 TScreenColor;

	#define NORMAL_COLOR16			COLOR16 (31, 31, 31)
	#define HIGH_COLOR16			COLOR16 (31, 0, 0)
	#define HALF_COLOR16			COLOR16 (0, 0, 31)

	#define NORMAL_COLOR			1
	#define HIGH_COLOR			2
	#define HALF_COLOR			3
#elif DEPTH == 16
	typedef u16 TScreenColor;

	#define NORMAL_COLOR			COLOR16 (31, 31, 31)
	#define HIGH_COLOR			COLOR16 (31, 0, 0)
	#define HALF_COLOR			COLOR16 (0, 0, 31)
#elif DEPTH == 32
	typedef u32 TScreenColor;

	#define NORMAL_COLOR			COLOR32 (255, 255, 255, 255)
	#define HIGH_COLOR			COLOR32 (255, 0, 0, 255)
	#define HALF_COLOR			COLOR32 (0, 0, 255, 255)
#else
	#error DEPTH must be 8, 16 or 32
#endif

typedef struct TScreenDevice
{
	unsigned	 m_nInitWidth;
	unsigned	 m_nInitHeight;
	TBcmFrameBuffer	*m_pFrameBuffer;
	TCharGenerator	 m_CharGen;
	TScreenColor  	*m_pBuffer;
	unsigned	 m_nSize;
	unsigned	 m_nWidth;
	unsigned	 m_nHeight;
	unsigned	 m_nUsedHeight;
	unsigned	 m_nState;
	unsigned	 m_nCursorX;
	unsigned	 m_nCursorY;
	boolean		 m_bCursorOn;
	TScreenColor	 m_Color;
	boolean		 m_bInsertOn;
	unsigned	 m_nParam1;
	unsigned	 m_nParam2;
}
TScreenDevice;

void ScreenDevice (TScreenDevice *pThis, unsigned nWidth, unsigned nHeight);
void _ScreenDevice (TScreenDevice *pThis);

boolean ScreenDeviceInitialize (TScreenDevice *pThis);

unsigned ScreenDeviceGetWidth (TScreenDevice *pThis);
unsigned ScreenDeviceGetHeight (TScreenDevice *pThis);

int ScreenDeviceWrite (TScreenDevice *pThis, const void *pBuffer, unsigned nCount);

void ScreenDeviceSetPixel (TScreenDevice *pThis, unsigned nPosX, unsigned nPosY, TScreenColor Color);
TScreenColor ScreenDeviceGetPixel (TScreenDevice *pThis, unsigned nPosX, unsigned nPosY);

void ScreenDeviceRotor (TScreenDevice *pThis,
			unsigned nIndex,		// 0..3
			unsigned nCount);		// 0..3

#ifdef __cplusplus
}
#endif

#endif
