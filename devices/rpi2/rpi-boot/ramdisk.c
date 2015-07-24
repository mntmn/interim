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

/* Support for a ramdisk based in memory */

#include <stdint.h>
#include <stdlib.h>
#include "block.h"
#include "vfs.h"
#include "memchunk.h"
#include "string.h"
#include "errno.h"
#include "util.h"

#define RAMDISK_BLOCK_SIZE		512
static char driver_name[] = "ramdisk";

int register_fs(struct block_device *dev, int part_id);
static int ramdisk_write(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no);
static int ramdisk_read(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no);

struct ramdisk_dev
{
	struct block_device bd;
	uintptr_t address;
	size_t size;
};

int ramdisk_init(uintptr_t address, size_t size, int fs_type, char *name)
{
	// Allocate memory if requested
	if(address == 0)
		address = (uintptr_t)chunk_get_any_chunk((uint32_t)size);
	if(address == 0)
	{
		errno = ENOMEM;
		return -1;
	}

	// First create a ramdisk block device
	struct ramdisk_dev *dev = (struct ramdisk_dev *)malloc(sizeof(struct ramdisk_dev));
	if(dev == NULL)
		return -1;

	memset(dev, 0, sizeof(struct ramdisk_dev));
	dev->bd.block_size = 512;
	dev->bd.device_name = (char *)malloc(strlen(name) + 1);
	if(dev->bd.device_name == NULL)
	{
		free(dev);
		return -1;
	}
	strcpy(dev->bd.device_name, name);
	dev->bd.driver_name = driver_name;
	dev->bd.supports_multiple_block_read = 1;
	dev->bd.supports_multiple_block_write = 1;
	dev->bd.read = ramdisk_read;
	dev->bd.write = ramdisk_write;

	// Now initialise the filesystem
	int ret = register_fs((struct block_device *)dev, fs_type);
	if(ret != 0)
		free(dev);
	return ret;
}

int ramdisk_write(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no)
{
	struct ramdisk_dev *rdev = (struct ramdisk_dev *)dev;

	uintptr_t start_address = rdev->address + (uintptr_t)(rdev->bd.block_size * block_no);
	uintptr_t ramdisk_end = rdev->address + rdev->size;

	// Ensure we are not accessing beyond the end of the ramdisk
	if(buf_size > (ramdisk_end - start_address))
		buf_size = ramdisk_end - start_address;

	// See if we can do quick copies
	if(((start_address & 0xf) == 0) && ((buf_size & 0xf) == 0))
		quick_memcpy((void *)start_address, buf, buf_size);
	else
		memcpy((void *)start_address, buf, buf_size);

	return buf_size;
}

int ramdisk_read(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no)
{
	struct ramdisk_dev *rdev = (struct ramdisk_dev *)dev;

	uintptr_t start_address = rdev->address + (uintptr_t)(rdev->bd.block_size * block_no);
	uintptr_t ramdisk_end = rdev->address + rdev->size;

	// Ensure we are not accessing beyond the end of the ramdisk
	if(buf_size > (ramdisk_end - start_address))
		buf_size = ramdisk_end - start_address;

	// See if we can do quick copies
	if(((start_address & 0xf) == 0) && ((buf_size & 0xf) == 0))
		quick_memcpy(buf, (void *)start_address, buf_size);
	else
		memcpy(buf, (void *)start_address, buf_size);

	return buf_size;
}
