/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* A driver to cache block device accesses
 *
 * Implemented as write through for now
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "block.h"
#include "util.h"

// The name to append to the device name to distinguish this as a cached device
//#define CACHE_DEV_NAME_APPEND "_cache"
#define CACHE_DEV_NAME_APPEND ""

static char dev_name[] = CACHE_DEV_NAME_APPEND;
static char drv_name[] = "block_cache";

struct cache_dev
{
	struct block_device bd;
	struct block_device *parent;

	uintptr_t cache_start;
	int cache_entries;
	uint32_t cache_mask;

	uint32_t *cached_blocks;
};

int cache_read(struct block_device *, uint8_t *buf, size_t buf_size, uint32_t starting_block);
int cache_write(struct block_device *, uint8_t *buf, size_t buf_size, uint32_t starting_block);
inline static int cache_idx(struct cache_dev *dev, uint32_t block_no);

int cache_init(struct block_device *parent, struct block_device **dev, uintptr_t cache_start, size_t cache_length)
{
	if(parent == NULL)
		return -1;
	if(dev == NULL)
		return -1;

	// Build a block device structure
	struct cache_dev *cd = (struct cache_dev *)malloc(sizeof(struct cache_dev));
	if(cd == NULL)
		return -1;
	memset(cd, 0, sizeof(struct cache_dev));

	cd->bd.block_size = parent->block_size;
	cd->bd.device_name = (char *)malloc(strlen(parent->device_name) + strlen(dev_name) + 1);
	if(cd->bd.device_name == NULL)
	{
		free(cd);
		return -1;
	}
	strcpy(cd->bd.device_name, parent->device_name);
	strcat(cd->bd.device_name, dev_name);
	cd->bd.driver_name = drv_name;
	cd->bd.num_blocks = parent->num_blocks;
	if(parent->read)
		cd->bd.read = cache_read;
	if(parent->write)
		cd->bd.write = cache_write;
	cd->bd.supports_multiple_block_read = parent->supports_multiple_block_read;
	cd->bd.supports_multiple_block_write = parent->supports_multiple_block_write;
	
	// Calculate the number of cache entries
	int cache_entries = cache_length / cd->bd.block_size;
	if(cache_entries == 0)
	{
		free(cd->bd.device_name);
		free(cd);
		return -1;
	}

	// Calculate log2 of cache_entries
	int lg2 = 0;
	while(cache_entries >>= 1)
		lg2++;

	// Now restore to cache_entries (this has the effect of rounding down to a power
	//  of two, so we can generate a simple mask function)
	cache_entries = (1 << lg2);

	cd->cache_entries = cache_entries;
	cd->cache_mask = (uint32_t)(cache_entries - 1);
	cd->cache_start = cache_start;
	cd->parent = parent;

	// Allocate an array to store the actual cached blocks in
	cd->cached_blocks = (uint32_t *)malloc(cd->cache_entries * sizeof(uint32_t));
	if(cd->cached_blocks == NULL)
	{
		free(cd->bd.device_name);
		free(cd);
		return -1;
	}
	// We use -1 to indicate an empty cache slot (as block 0 is a valid block)
	memset(cd->cached_blocks, 0xff, cd->cache_entries * sizeof(uint32_t));

#ifdef DEBUG_CACHE
	printf("CACHE: initialised block cache for device %s, cache_entries %i, cache_mask %08x, cache_start %08x\n",
		parent->device_name, cd->cache_entries, cd->cache_mask, cd->cache_start);
#endif

	*dev = (struct block_device *)cd;
	return 0;
}

inline static int cache_idx(struct cache_dev *dev, uint32_t block_no)
{
	return (int)(block_no & dev->cache_mask);
}

int cache_read(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block)
{
	struct cache_dev *cd = (struct cache_dev *)dev;

#ifdef DEBUG_CACHE
	printf("CACHE: request for %i bytes starting at block %i\n",
		buf_size, starting_block);
#endif
	
	// Is this a multiblock request?  Pass through to the parent
	if(buf_size > cd->bd.block_size)
	{
#ifdef DEBUG_CACHE
		printf("CACHE: request for buf_size %i - passing through to parent\n",
			buf_size);
#endif
		return cd->parent->read(cd->parent, buf, buf_size, starting_block);
	}

	// Else do we cache this block?
	int idx = cache_idx(cd, starting_block);
	void *cache_buf = (void *)(cd->cache_start + cd->bd.block_size * idx);
	
	// If not, load it up to the cache
	if(cd->cached_blocks[idx] != starting_block)
	{
		// If write-back, we need to evict a current cache entry and flush it
		//  back to disk
		// TODO

		// Load up the new block
#ifdef DEBUG_CACHE
		printf("CACHE: not in cache - requesting from parent\n");
#endif
		int bytes_read = cd->parent->read(cd->parent, (uint8_t *)cache_buf,
			buf_size, starting_block);

		// Only cache if we loaded the entire block
		if(bytes_read == (int)cd->bd.block_size)
		{
			// Store this cache entry
			cd->cached_blocks[idx] = starting_block;
#ifdef DEBUG_CACHE
			printf("CACHE: storing to slot %i\n", idx);
#endif
		}
		else
		{
			// Invalidate the cache line
			cd->cached_blocks[idx] = 0xffffffff;
		}

		buf_size = bytes_read;
	}
	else
	{
#ifdef DEBUG_CACHE
		printf("CACHE: fetching from cache slot %i\n", idx);
#endif
	}

	// Copy the cached data to the calling process
	qmemcpy(buf, cache_buf, buf_size);
	return buf_size;
}

int cache_write(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block)
{
	struct cache_dev *cd = (struct cache_dev *)dev;

	// Only support whole block writes
	if(buf_size % cd->bd.block_size)
	{
		errno = EFAULT;
		return -1;
	}

	// Write through to the underlying device
	buf_size = cd->parent->write(cd->parent, buf, buf_size, starting_block);

	// How many blocks?
	int block_count = buf_size / cd->bd.block_size;

	// Update the cache if required
	for(int i = 0; i < block_count; i++)
	{
		int idx = cache_idx(cd, starting_block + i);
		if(cd->cached_blocks[idx] == starting_block + i)
			qmemcpy((void *)(cd->cache_start + idx * cd->bd.block_size), &buf[i * cd->bd.block_size], cd->bd.block_size);
	}

	return buf_size;
}
