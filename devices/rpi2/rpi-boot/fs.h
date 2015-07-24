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

#ifndef FS_H
#define FS_H

#include "dirent.h"
#include <stdio.h>
#include <stdint.h>
#include "block.h"

#define FS_FLAG_SUPPORTS_EMPTY_FNAME		1

typedef struct fs_file fs_file;

struct fs {
	struct block_device *parent;
	const char *fs_name;
	uint32_t flags;
	size_t block_size;

	fs_file *(*fopen)(struct fs *, struct dirent *, const char *mode);
	size_t (*fread)(struct fs *, void *ptr, size_t byte_size, fs_file *stream);
	size_t (*fwrite)(struct fs *, void *ptr, size_t byte_size, fs_file *stream);
	int (*fclose)(struct fs *, fs_file *fp);
	long (*fsize)(fs_file *fp);
    int (*fseek)(fs_file *stream, long offset, int whence);
	long (*ftell)(fs_file *fp);
	int (*fflush)(fs_file *fp);

	struct dirent *(*read_directory)(struct fs *, char **name);
};


struct fs_file
{
  struct fs *fs;
  long pos;
	int mode;
  void *opaque;
  long len;
	int flags;
	int (*fflush_cb)(fs_file *f);
};


int register_fs(struct block_device *dev, int part_id);
int fs_interpret_mode(const char *mode);
size_t fs_fread(uint32_t (*get_next_bdev_block_num)(uint32_t f_block_idx, fs_file *s, void *opaque, int add_blocks),
	struct fs *fs, void *ptr, size_t byte_size,
	fs_file *stream, void *opaque);
size_t fs_fwrite(uint32_t (*get_next_bdev_block_num)(uint32_t f_block_idx, fs_file *s, void *opaque, int add_blocks),
	struct fs *fs, void *ptr, size_t byte_size,
	fs_file *stream, void *opaque);

void vfs_register(struct fs *fs);

#endif

