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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include "vfs.h"
#include "fs.h"
#include "errno.h"

#ifdef DEBUG2
#define EXT2_DEBUG
#endif

struct ext2_bgd
{
	uint32_t block_bitmap_block_address;
	uint32_t inode_bitmap_block_address;
	uint32_t inode_table_start_block;
	uint16_t unallocated_block_count;
	uint16_t unallocated_inode_count;
	uint16_t directory_count;
	uint8_t reserved[14];
} __attribute__ ((packed));

struct ext2_fs {
	struct fs b;

	uint32_t total_inodes;
	uint32_t total_blocks;
	uint32_t inode_size;
	uint32_t inodes_per_group;
	uint32_t blocks_per_group;
	uint32_t total_groups;
	int major_version;
	int minor_version;
	uint32_t pointers_per_indirect_block;
	uint32_t pointers_per_indirect_block_2;
	uint32_t pointers_per_indirect_block_3;

	int type_flags_used;

	// cache the block group descriptor table
	struct ext2_bgd *bgdt;
};

struct ext2_inode {
	uint16_t type_permissions;
	uint16_t user_id;
	uint32_t size;
	uint32_t last_access_time;
	uint32_t creation_time;
	uint32_t last_modification_time;
	uint32_t deletion_time;
	uint16_t group_id;
	uint16_t hard_link_count;
	uint32_t sector_count;
	uint32_t flags;
	uint32_t os_specific_1;
	uint32_t db0;
	uint32_t db1;
	uint32_t db2;
	uint32_t db3;
	uint32_t db4;
	uint32_t db5;
	uint32_t db6;
	uint32_t db7;
	uint32_t db8;
	uint32_t db9;
	uint32_t db10;
	uint32_t db11;
	uint32_t sibp;
	uint32_t dibp;
	uint32_t tibp;
	uint32_t generation_number;
	uint32_t extended_attribute_block;
	uint32_t size_upper_bits;
	uint32_t fragment_block_address;
	uint32_t os_opecific[3];
} __attribute__ ((packed));

static struct dirent *ext2_read_directory(struct fs *fs, char **name);
static struct dirent *ext2_read_dir(struct ext2_fs *fs, struct dirent *d);
static FILE *ext2_fopen(struct fs *fs, struct dirent *path, const char *mode);
static size_t ext2_fread(struct fs *fs, void *ptr, size_t byte_size, FILE *stream);
static struct ext2_inode *ext2_read_inode(struct ext2_fs *fs,
		uint32_t inode_idx);

static char ext2_name[] = "ext2";

static uint32_t get_sector_num(struct ext2_fs *fs, uint32_t block_no)
{
	return block_no * fs->b.block_size / fs->b.parent->block_size;
}

static uint8_t *read_block(struct ext2_fs *fs, uint32_t block_no)
{
	uint8_t *ret = (uint8_t *)malloc(fs->b.block_size);
	int br_ret = block_read(fs->b.parent, ret, fs->b.block_size,
			get_sector_num(fs, block_no));
	if(br_ret < 0)
	{
		printf("EXT2: block_read returned %i\n");
		return (void*)0;
	}
	return ret;
}

static uint32_t get_block_no_from_inode(struct ext2_fs *fs, struct ext2_inode *i, uint32_t index, int add_blocks)
{
	// If the block index is < 12 use the direct block pointers
	if(index < 12)
		return ((uint32_t *)&i->db0)[index];
	else
	{
		// If the block index is < (12 + pointers_per_indirect_block),
		// use the singly-indirect block pointer
		index -= 12;

		if(index < fs->pointers_per_indirect_block)
		{
			// Load the singly indirect block
			uint8_t *sib = read_block(fs, i->sibp);
			if(!sib)
				return 0;

			uint32_t ret = ((uint32_t *)sib)[index];
			free(sib);
			return ret;
		}
		else
		{
			index -= fs->pointers_per_indirect_block;

			// If the index is < pointers_per_indirect_block ^ 2,
			// use the doubly-indirect block pointer

			if(index < (fs->pointers_per_indirect_block_2))
			{
				uint32_t dib_index = index / fs->pointers_per_indirect_block;
				uint32_t sib_index = index % fs->pointers_per_indirect_block;

				// Load the doubly indirect block
				uint8_t *dib = read_block(fs, i->dibp);
				if(!dib)
					return 0;

				uint32_t sib_block = ((uint32_t *)dib)[dib_index];
				free(dib);

				// Load the appropriate singly indirect block
				uint8_t *sib = read_block(fs, sib_block);
				if(!sib)
					return 0;

				uint32_t ret = ((uint32_t *)sib)[sib_index];
				free(sib);
				return ret;
			}
			else
			{
				index -= fs->pointers_per_indirect_block_2;

				// Else use a triply indirect block or fail
				if(index < fs->pointers_per_indirect_block_3)
				{
					uint32_t tib_index = index /
						fs->pointers_per_indirect_block_2;
					uint32_t tib_rem = index %
						fs->pointers_per_indirect_block_2;
					uint32_t dib_index = tib_rem /
						fs->pointers_per_indirect_block;
					uint32_t sib_index = tib_rem %
						fs->pointers_per_indirect_block;

					// Load the triply indirect block
					uint8_t *tib = read_block(fs, i->tibp);
					if(!tib)
						return 0;

					uint32_t dib_block = ((uint32_t *)tib)[tib_index];
					free(tib);

					// Load the appropriate doubly indirect block
					uint8_t *dib = read_block(fs, dib_block);
					if(!dib)
						return 0;

					uint32_t sib_block = ((uint32_t *)dib)[dib_index];
					free(dib);

					// Load the appropriate singly indirect block
					uint8_t *sib = read_block(fs, sib_block);
					if(!sib)
						return 0;

					uint32_t ret = ((uint32_t *)sib)[sib_index];
					free(sib);
					return ret;
				}
				else
				{
					if(add_blocks)
					{
						printf("EXT2: request to extend file not currently supported\n");
						return 0;
					}
					printf("EXT2: invalid block number\n");
					return 0;
				}
			}
		}
	}
}

static FILE *ext2_fopen(struct fs *fs, struct dirent *path, const char *mode)
{
	if(fs != path->fs)
	{
		errno = EFAULT;
		return (FILE *)0;
	}

	if(strcmp(mode, "r"))
	{
		errno = EROFS;
		return (FILE *)0;
	}

	struct ext2_fs *ext2 = (struct ext2_fs *)fs;

	struct vfs_file *ret = (struct vfs_file *)malloc(sizeof(struct vfs_file));
	memset(ret, 0, sizeof(struct vfs_file));
	ret->fs = fs;
	ret->pos = 0;
	ret->opaque = path->opaque;

	// We have to load the inode to get the length
	struct ext2_inode *inode = ext2_read_inode(ext2,
			(uint32_t)path->opaque);
	ret->len = (long)inode->size;	// no support for large files
	free(inode);

	return ret;
}

static uint32_t ext2_get_next_bdev_block_num(uint32_t f_block_idx, FILE *s, void *opaque, int add_blocks)
{
	return get_block_no_from_inode((struct ext2_fs *)s->fs, (struct ext2_inode *)opaque,
		f_block_idx, add_blocks);
}

static size_t ext2_fread(struct fs *fs, void *ptr, size_t byte_size, FILE *stream)
{
	if(stream->fs != fs)
		return -1;
	if(stream->opaque == (void *)0)
		return -1;

	struct ext2_inode *inode = ext2_read_inode((struct ext2_fs *)fs,
		(uint32_t)stream->opaque);

	return fs_fread(ext2_get_next_bdev_block_num, fs, ptr, byte_size, stream, (void *)inode);
}

static int ext2_fclose(struct fs *fs, FILE *fp)
{
	(void)fs;
	(void)fp;
	return 0;
}

static struct ext2_inode *ext2_read_inode(struct ext2_fs *fs,
		uint32_t inode_idx)
{
	// Inode addresses start at 1
	inode_idx--;

	// First find which block group the inode is in
	uint32_t block_idx = inode_idx / fs->inodes_per_group;
	uint32_t block_offset = inode_idx % fs->inodes_per_group;
	struct ext2_bgd *b = &fs->bgdt[block_idx];

	// Now find which block in its inode table its in
	uint32_t inodes_per_block = fs->b.block_size / fs->inode_size;
	block_idx = block_offset / inodes_per_block;
	block_offset = block_offset % inodes_per_block;

	// Now read the appropriate block and extract the inode
	uint8_t *itable = read_block(fs, b->inode_table_start_block + block_idx);
	if(itable == (void*)0)
		return (void*)0;

	struct ext2_inode *inode =
		(struct ext2_inode *)malloc(sizeof(struct ext2_inode));
	memcpy(inode, &itable[block_offset * fs->inode_size],
			sizeof(struct ext2_inode));
	free(itable);
	return inode;
}

int ext2_init(struct block_device *parent, struct fs **fs)
{
	// Interpret an EXT2 file system
#ifdef EXT2_DEBUG
	printf("EXT2: looking for a filesytem on %s\n", parent->device_name);
#endif

	// Read superblock
	uint8_t *sb = (uint8_t *)malloc(1024);
	int r = block_read(parent, sb, 1024, 1024 / parent->block_size);
	if(r < 0)
	{
		printf("EXT2: error %i reading block 0\n", r);
		return r;
	}
	if(r != 1024)
	{
		printf("EXT2: error reading superblock (only %i bytes read)\n", r);
		return -1;
	}

	// Confirm its ext2
	if(*(uint16_t *)&sb[56] != 0xef53)
	{
		printf("EXT2: not a valid ext2 filesystem on %s\n", parent->device_name);
		return -1;
	}

	struct ext2_fs *ret = (struct ext2_fs *)malloc(sizeof(struct ext2_fs));
	memset(ret, 0, sizeof(struct ext2_fs));
	ret->b.fopen = ext2_fopen;
	ret->b.fread = ext2_fread;
	ret->b.fclose = ext2_fclose;
	ret->b.read_directory = ext2_read_directory;
	ret->b.parent = parent;
	ret->b.fs_name = ext2_name;

	ret->total_inodes = *(uint32_t *)&sb[0];
	ret->total_blocks = *(uint32_t *)&sb[4];
	ret->inodes_per_group = *(uint32_t *)&sb[40];
	ret->blocks_per_group = *(uint32_t *)&sb[32];
	ret->b.block_size = 1024 << *(uint32_t *)&sb[24];
	ret->minor_version = *(int16_t *)&sb[62];
	ret->major_version = *(int32_t *)&sb[76];

	if(ret->major_version >= 1)
	{
		// Read extended superblock
		ret->inode_size = *(uint16_t *)&sb[88];
		uint32_t required_flags = *(uint32_t *)&sb[96];
		if(required_flags & 0x2)
			ret->type_flags_used = 1;
	}
	else
		ret->inode_size = 128;

	// Calculate the number of block groups by two different methods and ensure they tally
	uint32_t i_calc_val = ret->total_inodes / ret->inodes_per_group;
	uint32_t i_calc_rem = ret->total_inodes % ret->inodes_per_group;
	if(i_calc_rem)
		i_calc_val++;

	uint32_t b_calc_val = ret->total_blocks / ret->blocks_per_group;
	uint32_t b_calc_rem = ret->total_blocks % ret->blocks_per_group;
	if(b_calc_rem)
		b_calc_val++;

	if(i_calc_val != b_calc_val)
	{
		printf("EXT2: total group calculation by block method (%i) and inode "
				"method (%i) differs\n", b_calc_val, i_calc_val);
		return -1;
	}

	ret->total_groups = i_calc_val;
	ret->pointers_per_indirect_block = ret->b.block_size / 4;
	ret->pointers_per_indirect_block_2 = ret->pointers_per_indirect_block *
		ret->pointers_per_indirect_block;
	ret->pointers_per_indirect_block_3 = ret->pointers_per_indirect_block_2 *
		ret->pointers_per_indirect_block;

	// Read the block group descriptor table
	ret->bgdt = (struct ext2_bgd *)malloc(ret->total_groups * sizeof(struct ext2_bgd));
	int bgdt_block = 1;
	if(ret->b.block_size == 1024)
		bgdt_block = 2;

    uint32_t bgdt_size = ret->total_groups * sizeof(struct ext2_bgd);
    // round up to a multiple of block_size
    if(bgdt_size % ret->b.block_size)
        bgdt_size = (bgdt_size / ret->b.block_size + 1) * ret->b.block_size;

	block_read(parent, (uint8_t *)ret->bgdt, bgdt_size,
			get_sector_num(ret, bgdt_block));

	*fs = (struct fs *)ret;
	free(sb);

	printf("EXT2: found an ext2 filesystem on %s\n", ret->b.parent->device_name);

	return 0;
}

struct dirent *ext2_read_directory(struct fs *fs, char **name)
{
	struct dirent *cur_dir = ext2_read_dir((struct ext2_fs *)fs, (void*)0);
	while(*name)
	{
		// Search the directory entries for one of the requested name
		int found = 0;
		while(cur_dir)
		{
			if(!strcmp(*name, cur_dir->name))
			{
				if(!cur_dir->is_dir)
				{
					errno = ENOTDIR;
					return (void *)0;
				}

				found = 1;
				cur_dir = ext2_read_dir((struct ext2_fs *)fs, cur_dir);
				name++;
				break;
			}
			cur_dir = cur_dir->next;
		}
		if(!found)
		{
#ifdef EXT2_DEBUG
			printf("EXT2: path part %s not found\n", *name);
#endif
			errno = ENOENT;
			return (void*)0;
		}
	}
	return cur_dir;
}

struct dirent *ext2_read_dir(struct ext2_fs *fs, struct dirent *d)
{
	struct ext2_fs *ext2 = (struct ext2_fs *)fs;

	uint32_t inode_idx = 2;	// root
	if(d != (void*)0)
		inode_idx = (uint32_t)d->opaque;

	struct dirent *ret = (void *)0;
	struct dirent *prev = (void *)0;

	// Load the inode of the directory
	struct ext2_inode *inode = ext2_read_inode(ext2, inode_idx);

	// Iterate through loading the blocks
	uint32_t total_blocks = inode->size / ext2->b.block_size;
	uint32_t block_rem = inode->size % ext2->b.block_size;
	if(block_rem)
		total_blocks++;

	uint32_t cur_block_idx = 0;

	while(cur_block_idx < total_blocks)
	{
		uint32_t block_no = get_block_no_from_inode(ext2, inode,
				cur_block_idx, 0);
		uint8_t *block = read_block(ext2, block_no);

		uint32_t total_block_size = ext2->b.block_size;
		// If inode->size is not a complete multiple of
		// ext2->block_size then only read part of the last block
		if((cur_block_idx == (total_blocks - 1)) && block_rem)
			total_block_size = block_rem;

		uint32_t ptr = 0;

		while(ptr < total_block_size)
		{
			uint32_t de_inode_idx = *(uint32_t *)&block[ptr];
			uint16_t de_entry_size = *(uint16_t *)&block[ptr + 4];
			uint16_t de_name_length = *(uint16_t *)&block[ptr + 6];
			uint8_t de_type_flags = *(uint8_t *)&block[ptr + 7];

			// Does the entry exist?
			if(!de_inode_idx)
			{
				ptr += de_entry_size;
				continue;
			}

			// Read it
			struct dirent *de = (struct dirent *)malloc(sizeof(struct dirent));
			memset(de, 0, sizeof(struct dirent));
			if(ret == (void *)0)
				ret = de;
			if(prev != (void *)0)
				prev->next = de;
			prev = de;

			// Determine the length of the name part
			uint32_t name_length = de_name_length;
			if(ext2->type_flags_used)
				name_length &= 0xff;

			// Store the name
			de->name = (char *)malloc(name_length + 1);
			memset(de->name, 0, name_length + 1);
			memcpy(de->name, &block[ptr + 8], name_length);

			// Don't return special files
			if(!strcmp(de->name, ".") || !strcmp(de->name, "..") ||
					!strcmp(de->name, "lost+found"))
			{
				free(de);
				ptr += de_entry_size;
				continue;
			}

			// Determine if its a directory
			if(ext2->type_flags_used)
			{
				if(de_type_flags == 2)
					de->is_dir = 1;
			}
			else
			{
				// If directory type flags are not supported
				// we have to load the inode and do it that
				// way
				struct ext2_inode *de_inode =
					ext2_read_inode(ext2, de_inode_idx);
				if(de_inode->type_permissions & 0x2000)
					de->is_dir = 1;
				free(de_inode);
			}

			de->fs = &fs->b;
			de->next = (void *)0;

			de->opaque = (void*)de_inode_idx;

			ptr += de_entry_size;
		}
		free(block);

		cur_block_idx++;
	}

	free(inode);

	return ret;
}

