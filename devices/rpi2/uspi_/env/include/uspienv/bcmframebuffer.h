//
// bcmframebuffer.h
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
#ifndef _uspienv_bcmframebuffer_h
#define _uspienv_bcmframebuffer_h

#include <uspienv/bcmmailbox.h>
#include <uspienv/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// This struct is shared between GPU and ARM side
// Must be 16 byte aligned in memory
typedef struct Bcm2835FrameBufferInfo
{
	u32 Width;		// Physical width of display in pixel
	u32 Height;		// Physical height of display in pixel
	u32 VirtWidth;		// always as physical width so far
	u32 VirtHeight;		// always as physical height so far
	u32 Pitch;		// Should be init with 0
	u32 Depth;		// Number of bits per pixel
	u32 OffsetX;		// Normally zero
	u32 OffsetY;		// Normally zero
	u32 BufferPtr;		// Address of frame buffer (init with 0, set by GPU)
	u32 BufferSize;		// Size of frame buffer (init with 0, set by GPU)

	u16 Palette[0];		// with Depth == 8 only (256 entries)
#define PALETTE_ENTRIES		256
}
Bcm2835FrameBufferInfo;

typedef struct TBcmFrameBuffer
{
	volatile Bcm2835FrameBufferInfo *m_pInfo;

	TBcmMailBox m_MailBox;
}
TBcmFrameBuffer;

void BcmFrameBuffer (TBcmFrameBuffer *pThis, unsigned nWidth, unsigned nHeight, unsigned nDepth);
void _BcmFrameBuffer (TBcmFrameBuffer *pThis);

void BcmFrameBufferSetPalette (TBcmFrameBuffer *pThis, u8 nIndex, u16 nColor);	// with Depth == 8 only

boolean BcmFrameBufferInitialize (TBcmFrameBuffer *pThis);

u32 BcmFrameBufferGetWidth (TBcmFrameBuffer *pThis);
u32 BcmFrameBufferGetHeight (TBcmFrameBuffer *pThis);
u32 BcmFrameBufferGetPitch (TBcmFrameBuffer *pThis);
u32 BcmFrameBufferGetDepth (TBcmFrameBuffer *pThis);
u32 BcmFrameBufferGetBuffer (TBcmFrameBuffer *pThis);
u32 BcmFrameBufferGetSize (TBcmFrameBuffer *pThis);

#ifdef __cplusplus
}
#endif

#endif
