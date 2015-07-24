//
// alloc.c
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
#include <uspienv/alloc.h>
#include <uspienv/synchronize.h>
#include <uspienv/sysconfig.h>
#include <uspienv/util.h>
#include <uspienv/macros.h>
#include <uspienv/assert.h>

#ifdef MEM_DEBUG
	#include <uspienv/logger.h>
#endif

#define BLOCK_ALIGN	16
#define ALIGN_MASK	(BLOCK_ALIGN-1)

#define PAGE_MASK	(PAGE_SIZE-1)

typedef struct TBlockHeader
{
	unsigned int	nMagic		PACKED;
#define BLOCK_MAGIC	0x424C4D43
	unsigned int	nSize		PACKED;
	struct TBlockHeader *pNext	PACKED;
	unsigned int	nPadding	PACKED;
	unsigned char	Data[0];
}
TBlockHeader;

typedef struct TBlockBucket
{
	unsigned int	nSize;
#ifdef MEM_DEBUG
	unsigned int	nCount;
	unsigned int	nMaxCount;
#endif
	TBlockHeader	*pFreeList;
}
TBlockBucket;

#ifdef MEM_PAGE_ALLOC

typedef struct TFreePage
{
	unsigned int	nMagic;
#define FREEPAGE_MAGIC	0x50474D43
	struct TFreePage *pNext;
}
TFreePage;

typedef struct TPageBucket
{
#ifdef MEM_DEBUG
	unsigned int	nCount;
	unsigned int	nMaxCount;
#endif
	TFreePage	*pFreeList;
}
TPageBucket;

#endif

static unsigned char *s_pNextBlock;
static unsigned char *s_pBlockLimit;

#ifdef MEM_PAGE_ALLOC
static unsigned char *s_pNextPage;
static unsigned char *s_pPageLimit;
#endif

static TBlockBucket s_BlockBucket[] = {{0x40}, {0x400}, {0x1000}, {0x4000}, {0x40000}, {0x80000}, {0}};

#ifdef MEM_PAGE_ALLOC
static TPageBucket s_PageBucket;
#endif

void mem_init (unsigned long ulBase, unsigned long ulSize)
{
	unsigned long ulLimit = ulBase + ulSize;

	if (ulBase < MEM_HEAP_START)
	{
		ulBase = MEM_HEAP_START;
	}
	
	ulSize = ulLimit - ulBase;
#ifdef MEM_PAGE_ALLOC
	unsigned long ulQuarterSize = ulSize / 4;

	s_pNextBlock = (unsigned char *) ulBase;
	s_pBlockLimit = (unsigned char *) (ulBase + ulQuarterSize);

	s_pNextPage = (unsigned char *) ((ulBase + ulQuarterSize + PAGE_SIZE) & ~PAGE_MASK);
	s_pPageLimit = (unsigned char *) ulLimit;
#else
	s_pNextBlock = (unsigned char *) ulBase;
	s_pBlockLimit = (unsigned char *) (ulBase + ulSize);
#endif
}

unsigned long mem_get_size (void)
{
#ifdef MEM_PAGE_ALLOC
	return (unsigned long) (s_pBlockLimit - s_pNextBlock) + (s_pPageLimit - s_pNextPage);
#else
	return (unsigned long) (s_pBlockLimit - s_pNextBlock);
#endif
}

void *malloc (unsigned long ulSize)
{
	assert (s_pNextBlock != 0);
	
	EnterCritical ();

	TBlockBucket *pBucket;
	for (pBucket = s_BlockBucket; pBucket->nSize > 0; pBucket++)
	{
		if (ulSize <= pBucket->nSize)
		{
			ulSize = pBucket->nSize;

#ifdef MEM_DEBUG
			if (++pBucket->nCount > pBucket->nMaxCount)
			{
				pBucket->nMaxCount = pBucket->nCount;
			}
#endif

			break;
		}
	}

	TBlockHeader *pBlockHeader;
	if (   pBucket->nSize > 0
	    && (pBlockHeader = pBucket->pFreeList) != 0)
	{
		assert (pBlockHeader->nMagic == BLOCK_MAGIC);
		pBucket->pFreeList = pBlockHeader->pNext;
	}
	else
	{
		pBlockHeader = (TBlockHeader *) s_pNextBlock;

		s_pNextBlock += (sizeof (TBlockHeader) + ulSize + BLOCK_ALIGN-1) & ~ALIGN_MASK;
		if (s_pNextBlock > s_pBlockLimit)
		{
			LeaveCritical ();

			return 0;		// TODO: system should panic here
		}
	
		pBlockHeader->nMagic = BLOCK_MAGIC;
		pBlockHeader->nSize = (unsigned) ulSize;
	}
	
	LeaveCritical ();

	pBlockHeader->pNext = 0;

	void *pResult = pBlockHeader->Data;
	assert (((unsigned long) pResult & ALIGN_MASK) == 0);

	return pResult;
}

void free (void *pBlock)
{
	assert (pBlock != 0);
	TBlockHeader *pBlockHeader = (TBlockHeader *) ((unsigned long) pBlock - sizeof (TBlockHeader));
	assert (pBlockHeader->nMagic == BLOCK_MAGIC);

	for (TBlockBucket *pBucket = s_BlockBucket; pBucket->nSize > 0; pBucket++)
	{
		if (pBlockHeader->nSize == pBucket->nSize)
		{
			EnterCritical ();

			pBlockHeader->pNext = pBucket->pFreeList;
			pBucket->pFreeList = pBlockHeader;

#ifdef MEM_DEBUG
			pBucket->nCount--;
#endif

			LeaveCritical ();

			break;
		}
	}
}

#ifdef MEM_PAGE_ALLOC

void *palloc (void)
{
	assert (s_pNextPage != 0);

	EnterCritical ();

#ifdef MEM_DEBUG
	if (++s_PageBucket.nCount > s_PageBucket.nMaxCount)
	{
		s_PageBucket.nMaxCount = s_PageBucket.nCount;
	}
#endif

	TFreePage *pFreePage;
	if ((pFreePage = s_PageBucket.pFreeList) != 0)
	{
		assert (pFreePage->nMagic == FREEPAGE_MAGIC);
		s_PageBucket.pFreeList = pFreePage->pNext;
		pFreePage->nMagic = 0;
	}
	else
	{
		pFreePage = (TFreePage *) s_pNextPage;
		
		s_pNextPage += PAGE_SIZE;

		if (s_pNextPage > s_pPageLimit)
		{
			LeaveCritical ();

			return 0;		// TODO: system should panic here
		}
	}

	LeaveCritical ();
	
	return pFreePage;
}

void pfree (void *pPage)
{
	assert (pPage != 0);
	TFreePage *pFreePage = (TFreePage *) pPage;
	
	EnterCritical ();

	pFreePage->nMagic = FREEPAGE_MAGIC;

	pFreePage->pNext = s_PageBucket.pFreeList;
	s_PageBucket.pFreeList = pFreePage;

#ifdef MEM_DEBUG
	s_PageBucket.nCount--;
#endif

	LeaveCritical ();
}

#endif

#ifdef MEM_DEBUG

void mem_info (void)
{
	for (TBlockBucket *pBucket = s_BlockBucket; pBucket->nSize > 0; pBucket++)
	{
		LoggerWrite (LoggerGet (), "alloc", LogDebug, "malloc(%lu): %u blocks (max %u)",
			     pBucket->nSize, pBucket->nCount, pBucket->nMaxCount);
	}

#ifdef MEM_PAGE_ALLOC
	LoggerWrite (LoggerGet (), "alloc", LogDebug, "palloc: %u pages (max %u)",
		     s_PageBucket.nCount, s_PageBucket.nMaxCount);
#endif
}

#endif
