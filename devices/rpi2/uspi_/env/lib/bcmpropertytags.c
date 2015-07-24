//
// bcmpropertytags.c
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
#include <uspienv/bcmpropertytags.h>
#include <uspienv/util.h>
#include <uspienv/synchronize.h>
#include <uspienv/bcm2835.h>
#include <uspienv/assert.h>

typedef struct TPropertyBuffer
{
	u32	nBufferSize;			// bytes
	u32	nCode;
	#define CODE_REQUEST		0x00000000
	#define CODE_RESPONSE_SUCCESS	0x80000000
	#define CODE_RESPONSE_FAILURE	0x80000001
	u8	Tags[0];
	// end tag follows
}
TPropertyBuffer;

void BcmPropertyTags (TBcmPropertyTags *pThis)
{
	assert (pThis != 0);

	BcmMailBox (&pThis->m_MailBox, BCM_MAILBOX_PROP_OUT);
}

void _BcmPropertyTags (TBcmPropertyTags *pThis)
{
	assert (pThis != 0);

	_BcmMailBox (&pThis->m_MailBox);
}

boolean BcmPropertyTagsGetTag (TBcmPropertyTags *pThis, u32 nTagId,
			       void *pTag, unsigned nTagSize, unsigned  nRequestParmSize)
{
	assert (pThis != 0);

	assert (pTag != 0);
	assert (nTagSize >= sizeof (TPropertyTagSimple));
	unsigned nBufferSize = sizeof (TPropertyBuffer) + nTagSize + sizeof (u32);
	assert ((nBufferSize & 3) == 0);

	// cannot use malloc() here because this is used before mem_init() is called
	u8 Buffer[nBufferSize + 15];
	TPropertyBuffer *pBuffer = (TPropertyBuffer *) (((u32) Buffer + 15) & ~15);
	
	pBuffer->nBufferSize = nBufferSize;
	pBuffer->nCode = CODE_REQUEST;
	memcpy (pBuffer->Tags, pTag, nTagSize);
	
	TPropertyTag *pHeader = (TPropertyTag *) pBuffer->Tags;
	pHeader->nTagId = nTagId;
	pHeader->nValueBufSize = nTagSize - sizeof (TPropertyTag);
	pHeader->nValueLength = nRequestParmSize & ~VALUE_LENGTH_RESPONSE;

	u32 *pEndTag = (u32 *) (pBuffer->Tags + nTagSize);
	*pEndTag = PROPTAG_END;

	CleanDataCache ();
	DataSyncBarrier ();

	u32 nBufferAddress = GPU_MEM_BASE + (u32) pBuffer;
	if (BcmMailBoxWriteRead (&pThis->m_MailBox, nBufferAddress) != nBufferAddress)
	{
		return FALSE;
	}
	
	InvalidateDataCache ();
	DataSyncBarrier ();

	if (pBuffer->nCode != CODE_RESPONSE_SUCCESS)
	{
		return FALSE;
	}
	
	if (!(pHeader->nValueLength & VALUE_LENGTH_RESPONSE))
	{
		return FALSE;
	}
	
	pHeader->nValueLength &= ~VALUE_LENGTH_RESPONSE;
	if (pHeader->nValueLength == 0)
	{
		return FALSE;
	}

	memcpy (pTag, pBuffer->Tags, nTagSize);

	return TRUE;
}
