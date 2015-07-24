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

#include <stdint.h>
#include <stdio.h>
#include "block.h"

#define MAX_TRIES		1

size_t block_read(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block)
{
	// Read the required number of blocks to satisfy the request
	int buf_offset = 0;
	uint32_t block_offset = 0;

	if(!dev->read)
		return 0;

	// Perform a multi-block read if the device supports it
	if(dev->supports_multiple_block_read && ((buf_size / dev->block_size) > 1))
	{
#ifdef BLOCK_DEBUG
		printf("block_read: performing multi block read (%i blocks) from "
			"block %i on %s\n", buf_size / dev->block_size, starting_block,
			dev->device_name);
#endif
		return dev->read(dev, buf, buf_size, starting_block);
	}

	do
	{
		size_t to_read = buf_size;
		if(to_read > dev->block_size)
			to_read = dev->block_size;

#ifdef BLOCK_DEBUG
		printf("block_read: reading %i bytes from block %i on %s\n", to_read,
				starting_block + block_offset, dev->device_name);
#endif

		int tries = 0;
		while(1)
		{
			int ret = dev->read(dev, &buf[buf_offset], to_read,
					starting_block + block_offset);
			if(ret < 0)
			{
				tries++;
				if(tries >= MAX_TRIES)
					return ret;
			}
			else
				break;
		}

		buf_offset += (int)to_read;
		block_offset++;

		if(buf_size < dev->block_size)
			buf_size = 0;
		else
			buf_size -= dev->block_size;
	} while(buf_size > 0);

	return (size_t)buf_offset;
}

size_t block_write(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block)
{
	// Write the required number of blocks to satisfy the request
	int buf_offset = 0;
	uint32_t block_offset = 0;

	if(!dev->write)
		return 0;

	do
	{
		size_t to_write = buf_size;
		if(to_write > dev->block_size)
			to_write = dev->block_size;

#ifdef BLOCK_DEBUG
		printf("block_write: writing %i bytes to block %i on %s\n", to_write,
				starting_block + block_offset, dev->device_name);
#endif

		int tries = 0;
		while(1)
		{
			int ret = dev->write(dev, &buf[buf_offset], to_write,
					starting_block + block_offset);
			if(ret < 0)
			{
				tries++;
				if(tries >= MAX_TRIES)
					return ret;
			}
			else
				break;
		}

		buf_offset += (int)to_write;
		block_offset++;

		if(buf_size < dev->block_size)
			buf_size = 0;
		else
			buf_size -= dev->block_size;
	} while(buf_size > 0);

	return (size_t)buf_offset;
}
