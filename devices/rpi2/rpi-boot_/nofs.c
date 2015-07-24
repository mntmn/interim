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

/* Support for the 'nofs' filesystem
 *
 * This is a file system driver without the notion of files or directories.
 * It is implemented as a single file starting at byte 0 and potentially
 * spanning the entire partition.
 * It maintains an 'end-of-file' marker and ensures the last bytes of the file are
 * either EOF followed by ~~~~~~~~ (8 ~s) or just EOF regardless of fseeking
 * backward/forwards etc
 * The first three bytes of the filesystem are the UTF-8 byte order mark
 * (0xef, 0xbb, 0xbf) which should be ignored by any program which reads the
 * filesystem.  If we read this on fopen, we search for an EOF marker to set the
 * current position (if mode & APPEND)
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "vfs.h"


// Define to be the EOF marker to use
//#define EOF_MARK		{ EOF }
#define EOF_MARK		{ EOF, '~', '~', '~', '~', '~', '~', '~', '~' }

// UTF BOM
#define UTF_BOM_1		0xef
#define UTF_BOM_2		0xbb
#define UTF_BOM_3		0xbf

static uint8_t eof_mark[] = EOF_MARK;
static char nofs_name[] = "nofs";

#ifdef NOFS_DEBUG
static char nofs_debug_startup[] = "NOFS started up in debug mode\n";
#endif

struct nofs_fs
{
	struct fs b;
};

static struct dirent *nofs_read_directory(struct fs *fs, char **name);
static size_t nofs_fread(struct fs *fs, void *ptr, size_t byte_size, FILE *stream);
static size_t nofs_fwrite(struct fs *fs, void *ptr, size_t byte_size, FILE *stream);
static int nofs_fclose(struct fs *fs, FILE *fp);
static FILE *nofs_fopen(struct fs *fs, struct dirent *path, const char *mode);
static long nofs_fsize(FILE *fp);
static int nofs_fseek(FILE *stream, long offset, int whence);
static long nofs_ftell(FILE *fp);

int nofs_init(struct block_device *parent, struct fs **fs)
{
	// There is no way of identifying a nofs filesystem (aside from partition code)
	// Therefore just create the data structures and hope for the best

	struct nofs_fs *nofs = (struct nofs_fs *)malloc(sizeof(struct nofs_fs));
	if(fs == NULL)
		return -1;

	memset(nofs, 0, sizeof(struct nofs_fs));
	nofs->b.parent = parent;
	nofs->b.fs_name = nofs_name;
	nofs->b.flags = FS_FLAG_SUPPORTS_EMPTY_FNAME;
	nofs->b.fopen = nofs_fopen;
	nofs->b.fread = nofs_fread;
	nofs->b.fwrite = nofs_fwrite;
	nofs->b.fclose = nofs_fclose;
	nofs->b.fseek = nofs_fseek;
	nofs->b.fsize = nofs_fsize;
	nofs->b.ftell = nofs_ftell;
	nofs->b.read_directory = nofs_read_directory;
	nofs->b.block_size = parent->block_size;

	*fs = (struct fs *)nofs;

	printf("NOFS: found a nofs partition on %s\n", parent->device_name);

#ifdef NOFS_DEBUG
	FILE *nfs_f = nofs_fopen(*fs, NULL, "a+");
	if(nfs_f == NULL)
		printf("NOFS: fopen failed %i\n", errno);
	nofs_fwrite(*fs, nofs_debug_startup, strlen(nofs_debug_startup), nfs_f);
#endif
	return 0;
}

FILE *nofs_fopen(struct fs *fs, struct dirent *path, const char *mode)
{
	struct vfs_file *f = (struct vfs_file *)malloc(sizeof(struct vfs_file));
	if(f == NULL)
		return NULL;
	memset(f, 0, sizeof(struct vfs_file));

	// Ignore path
	(void)path;

	// Interpret the mode argument
	int m = fs_interpret_mode(mode);

	// Read the first block
	uint8_t *block_0 = (uint8_t *)malloc(fs->block_size);
	if(block_0 == NULL)
	{
		free(f);
		return NULL;
	}
	long start_pos = 3;
	errno = 0;
	size_t bytes_read = block_read(fs->parent, block_0, fs->block_size, 0);
	if(bytes_read != fs->block_size)
	{
		free(f);
		free(block_0);
		if(errno == 0)
			errno = EFAULT;

#ifdef NOFS_DEBUG
		printf("NOFS: unable to read block_0\n");
#endif
		return NULL;
	}

	// Does the block contain a BOM?
	if((block_0[0] == UTF_BOM_1) && (block_0[1] == UTF_BOM_2) &&
		(block_0[2] == UTF_BOM_3))
	{
#ifdef NOFS_DEBUG
		printf("NOFS: found UTF-8 BOM at start of partition, mode = %i, num_blocks = %i\n",
			m, fs->parent->num_blocks);
#endif
		free(block_0);
		// If the mode argument includes an append, we have to search the
		//  file looking for an EOF marker.  We can only do this if the
		//  underlying block device has a valid length
		if((m & VFS_MODE_APPEND) && fs->parent->num_blocks)
		{
#ifdef NOFS_DEBUG
			printf("NOFS: searching for an EOF marker: ");
#endif
			int found = 0;
			size_t block_no = 0;
			uint8_t *buf = (uint8_t *)malloc(fs->parent->block_size);
			if(buf == NULL)
				return NULL;
		
			while((found == 0) && (block_no < fs->parent->num_blocks))
			{
				bytes_read = fs->parent->read(fs->parent, buf,
					fs->parent->block_size, block_no);

				if(bytes_read == 0)
					break;

				for(size_t i = 0; i < bytes_read; i++)
				{
					if(buf[i] == EOF)
					{
						start_pos = block_no * fs->parent->block_size +
							i;
						found = 1;
						break;
					}
				}
			}

#ifdef NOFS_DEBUG
			if(found)
				printf("found at position %i\n", start_pos);
			else
				printf("not found\n");
#endif

			free(buf);
		}
	}
	else
	{
#ifdef NOFS_DEBUG
		printf("NOFS: no BOM found: writing one\n");
#endif
		// Write a UTF BOM followed by EOF to signify a new file
		block_0[0] = UTF_BOM_1;
		block_0[1] = UTF_BOM_2;
		block_0[2] = UTF_BOM_3;

		for(size_t i = 0; i < sizeof(eof_mark); i++)
			block_0[i + 3] = eof_mark[i];

		errno = 0;
		size_t bytes_written = block_write(fs->parent, block_0, fs->block_size, 0);

		free(block_0);
		if(bytes_written != fs->block_size)
		{
			free(f);
			if(errno == 0)
				errno = EFAULT;

#ifdef NOFS_DEBUG
			printf("NOFS: unable to write block 0\n");
#endif
			return NULL;
		}
	}

	// Now fill in the FILE structure
	f->fs = fs;
	f->pos = start_pos;
	f->mode = m;
	f->len = start_pos;

#ifdef NOFS_DEBUG
	printf("NOFS: current file pointer %i\n", start_pos);
#endif

	return (FILE *)f;
}

static uint32_t nofs_get_next_bdev_block_num(uint32_t f_block_idx, FILE *s, void *opaque, int add_blocks)
{
	if((s->fs->parent->block_size) && (f_block_idx >= s->fs->parent->block_size))
	{
		errno = ENOSPC;
		return 0xffffffff;
	}
	if((f_block_idx > (s->len / s->fs->block_size)) && (add_blocks == 0))
		return 0xffffffff;

	(void)opaque;
	return f_block_idx;
}

size_t nofs_fread(struct fs *fs, void *ptr, size_t byte_size, FILE *stream)
{
	return fs_fread(nofs_get_next_bdev_block_num, fs, ptr, byte_size, stream, NULL);
}

size_t nofs_fwrite(struct fs *fs, void *ptr, size_t byte_size, FILE *stream)
{
	long old_len = stream->len;
	size_t ret = fs_fwrite(nofs_get_next_bdev_block_num, fs, ptr, byte_size, stream, NULL);

	if(stream->len > old_len)
	{
		// Terminate with an EOF if appropriate
		long new_pos = stream->len;
		fs_fwrite(nofs_get_next_bdev_block_num, fs, eof_mark, sizeof(eof_mark), stream, NULL);
		stream->pos = new_pos;
		stream->len = new_pos;
	}
	return ret;
}

static struct dirent *nofs_read_directory(struct fs *fs, char **name)
{
	(void)fs;
	(void)name;
	return NULL;
}

static int nofs_fclose(struct fs *fs, FILE *fp)
{
	(void)fs;
	(void)fp;
	return 0;
}

static long nofs_fsize(FILE *fp)
{
	return fp->len - 3;
}

static int nofs_fseek(FILE *stream, long offset, int whence)
{
	switch(whence)
	{
		case SEEK_SET:
			stream->pos = offset + 3;
			break;
		case SEEK_END:
			stream->pos = stream->len - offset;
			break;
		case SEEK_CUR:
			stream->pos += offset;
			break;
		default:
			return -1;
	}

	if(stream->pos < 3)
		stream->pos = 3;
	if(stream->pos > stream->len)
		stream->pos = stream->len;
	return 0;
}

static long nofs_ftell(FILE *fp)
{
	return fp->pos - 3;
}
