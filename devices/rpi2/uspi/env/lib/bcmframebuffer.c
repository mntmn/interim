//
// bcmframebuffer.c
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
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
#include <uspienv/bcmframebuffer.h>
#include <uspienv/bcmpropertytags.h>
#include <uspienv/synchronize.h>
#include <uspienv/alloc.h>
#include <uspienv/bcm2835.h>
#include <uspienv/util.h>
#include <uspienv/assert.h>

void BcmFrameBuffer (TBcmFrameBuffer *pThis, unsigned nWidth, unsigned nHeight, unsigned nDepth)
{
	assert (pThis != 0);

	BcmMailBox (&pThis->m_MailBox, MAILBOX_CHANNEL_FB);
	
	if (   nWidth  == 0
	    || nHeight == 0)
	{
		// detect optimal display size (if not configured)
		TBcmPropertyTags Tags;
		BcmPropertyTags (&Tags);
		TPropertyTagDisplayDimensions Dimensions;
		if (BcmPropertyTagsGetTag (&Tags, PROPTAG_GET_DISPLAY_DIMENSIONS, &Dimensions, sizeof Dimensions, 0))
		{
			nWidth  = Dimensions.nWidth;
			nHeight = Dimensions.nHeight;
		}
		else
		{
			nWidth  = 640;
			nHeight = 480;
		}

		_BcmPropertyTags (&Tags);
	}

	if (nDepth == 8)
	{
		pThis->m_pInfo = (Bcm2835FrameBufferInfo *) malloc (  sizeof (Bcm2835FrameBufferInfo)
								    + PALETTE_ENTRIES * sizeof (u16));

		memset ((void *) pThis->m_pInfo->Palette, 0, PALETTE_ENTRIES * sizeof (u16));	// clear palette
	}
	else
	{
		pThis->m_pInfo = malloc (sizeof (Bcm2835FrameBufferInfo));
	}

	// must be 16-byte aligned
	pThis->m_pInfo->Width      = nWidth;
	pThis->m_pInfo->Height     = nHeight;
	pThis->m_pInfo->VirtWidth  = nWidth;
	pThis->m_pInfo->VirtHeight = nHeight;
	pThis->m_pInfo->Pitch      = 0;
	pThis->m_pInfo->Depth      = nDepth;
	pThis->m_pInfo->OffsetX    = 0;
	pThis->m_pInfo->OffsetY    = 0;
	pThis->m_pInfo->BufferPtr  = 0;
	pThis->m_pInfo->BufferSize = 0;
}

void _BcmFrameBuffer (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);

	free ((void *) pThis->m_pInfo);
	pThis->m_pInfo = 0;

	_BcmMailBox (&pThis->m_MailBox);
}

void BcmFrameBufferSetPalette (TBcmFrameBuffer *pThis, u8 nIndex, u16 nColor)
{
	assert (pThis != 0);

	if (pThis->m_pInfo->Depth == 8)
	{
		pThis->m_pInfo->Palette[nIndex] = nColor;
	}
}

boolean BcmFrameBufferInitialize (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);

	CleanDataCache ();
	DataSyncBarrier ();
	u32 nResult = BcmMailBoxWriteRead (&pThis->m_MailBox, GPU_MEM_BASE + (u32) pThis->m_pInfo);
	InvalidateDataCache ();
	
	if (nResult != 0)
	{
		return FALSE;
	}
	
	if (pThis->m_pInfo->BufferPtr == 0)
	{
		return FALSE;
	}
	
	return TRUE;
}

u32 BcmFrameBufferGetWidth (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);
	return pThis->m_pInfo->Width;
}

u32 BcmFrameBufferGetHeight (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);
	return pThis->m_pInfo->Height;
}

u32 BcmFrameBufferGetPitch (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);
	return pThis->m_pInfo->Pitch;
}

u32 BcmFrameBufferGetDepth (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);
	return pThis->m_pInfo->Depth;
}

u32 BcmFrameBufferGetBuffer (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);

#if RASPPI == 1
	return pThis->m_pInfo->BufferPtr;
#else
	return pThis->m_pInfo->BufferPtr & 0x3FFFFFFF;
#endif
}

u32 BcmFrameBufferGetSize (TBcmFrameBuffer *pThis)
{
	assert (pThis != 0);
	return pThis->m_pInfo->BufferSize;
}
