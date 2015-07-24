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

#include "vfs.h"
#include "errno.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "output.h"

static struct vfs_entry *first = (void*)0;
static struct vfs_entry *def = (void*)0;

#define MAX_DEV_NAMES	256
static char *device_names[MAX_DEV_NAMES] = { 0 };
static int next_dev_name = 0;

static void vfs_add(struct vfs_entry *ve)
{
	ve->next = first;
	first = ve;
	
	if(next_dev_name < (MAX_DEV_NAMES - 1))
	{
		device_names[next_dev_name] = ve->device_name;
		device_names[next_dev_name + 1] = 0;
		next_dev_name++;
	}
}

char **vfs_get_device_list()
{
	return device_names;
}

static struct vfs_entry *find_ve(const char *path)
{
	struct vfs_entry *cur = first;
	while(cur)
	{
		if(!strcmp(cur->device_name, path))
			return cur;
		cur = cur->next;
	}
	return (void *)0;
}

int vfs_set_default(char *dev_name)
{
	struct vfs_entry *dev = find_ve(dev_name);
	if(dev)
	{
		def = dev;
		return 0;
	}
	return -1;
}

static void free_split_dir(char **sp)
{
	char **s = sp;
	while(*s)
	{
		free(*s);
		s++;
	}
	free(sp);
}

static void free_dirent_list(struct dirent *d)
{
	while(d)
	{
		struct dirent *tmp = d;
		d = d->next;
		free(tmp);
	}
}

static char **split_dir(const char *path, struct vfs_entry **ve)
{
	int dir_start = 0;
	int dir_count = 0;

	// First iterate through counting the number of directories
	int slen = strlen(path);
	int reading_dev = 0;

	*ve = def;

	for(int i = 0; i < slen; i++)
	{
		if(path[i] == '(')
		{
			if(i == 0)
				reading_dev = 1;
			else
			{
				printf("VFS: dir parse error, invalid '(' in %s position %i\n",
						path, i);
				return (void*)0;
			}
		}
		else if(path[i] == ')')
		{
			if(!reading_dev)
			{
				printf("VFS: dir parse error, invalid ')' in %s position %i\n",
						path, i);
				return (void*)0;
			}
			// The device name runs from position 1 to 'i'
			char *dev_name = (char *)malloc(i);
			strncpy(dev_name, &path[1], i - 1);
			dev_name[i - 1] = 0;
			*ve = find_ve(dev_name);
			reading_dev = 0;
			dir_start = i + 1;
		}
		else if(path[i] == '/')
		{
			if(reading_dev)
			{
				printf("VFS: dir parse error, invalid '/' in device name in %s "
						"at position %i\n", path, i);
				return (void*)0;
			}

			if(i != (dir_start))
				dir_count++;
			else
				dir_start++;
		}
		else if(i == (slen - 1))
			dir_count++;
	}

	if(*ve == (void*)0)
	{
		printf("VFS: unable to determine device name when parsing %s\n", path);
		return (void*)0;
	}

	// Now iterate through again assigning to the path array
	int cur_dir = 0;
	char **ret = (char **)malloc((dir_count + 1) * sizeof(char *));
	ret[dir_count] = 0;	// null terminate
	int cur_idx = dir_start;
	int cur_dir_start = dir_start;
	while(cur_dir < dir_count)
	{
		while((path[cur_idx] != '/') && (path[cur_idx] != 0))
			cur_idx++;
		// Found a '/'
		int path_bit_length = cur_idx - cur_dir_start;
		char *pb = (char *)malloc((path_bit_length + 1) * sizeof(char));
		for(int i = 0; i < path_bit_length; i++)
			pb[i] = path[cur_dir_start + i];
		pb[path_bit_length] = 0;

		cur_idx++;
		cur_dir_start = cur_idx;
		ret[cur_dir++] = pb;
	}

	return ret;
}

int vfs_register(struct fs *fs)
{
	if(fs == (void *)0)
		return -1;
	if(fs->parent == (void *)0)
		return -1;

	struct vfs_entry *ve = (struct vfs_entry *)malloc(sizeof(struct vfs_entry));
	memset(ve, 0, sizeof(struct vfs_entry));

	ve->device_name = fs->parent->device_name;
	ve->fs = fs;
	vfs_add(ve);

	if(def == (void*)0)
		def = ve;

	return 0;
}

void vfs_list_devices()
{
	struct vfs_entry *cur = first;
	while(cur)
	{
		printf("%s(%s) ", cur->device_name, cur->fs->fs_name);
		cur = cur->next;
	}
}

static struct dirent *read_directory(const char *path)
{
	char **p;
	struct vfs_entry *ve;
	p = split_dir(path, &ve);
	if(p == (void *)0)
		return (void *)0;

	struct dirent *ret = ve->fs->read_directory(ve->fs, p);
	free_split_dir(p);
	return ret;
}

/*
DIR *opendir(const char *name)
{
	struct dirent *ret = read_directory(name);
	if(ret == (void*)0)
		return (void*)0;
	struct dir_info *di = (struct dir_info *)malloc(sizeof(struct dir_info));
	di->first = ret;
	di->next = ret;
	return di;
}

struct dirent *readdir(DIR *dirp)
{
	if(dirp == (void*)0)
		return (void*)0;
	struct dirent *ret = dirp->next;
	if(dirp->next)
		dirp->next = dirp->next->next;
	return ret;
}

int closedir(DIR *dirp)
{
	if(dirp)
	{
		if(dirp->first)
			free_dirent_list(dirp->first);
		free(dirp);
		return 0;
	}
	else
		return -1;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if(stream == (void *)0)
		return 0;

	size_t bytes_to_read = size * nmemb;
	if(bytes_to_read > (size_t)(stream->len - stream->pos))
		bytes_to_read = (size_t)(stream->len - stream->pos);
	size_t nmemb_to_read = bytes_to_read / size;
	bytes_to_read = nmemb_to_read * size;

	bytes_to_read = stream->fs->fread(stream->fs, ptr, bytes_to_read, stream);
	return bytes_to_read / size;
}

size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if(stream == NULL)
	{
		errno = EINVAL;
		return 0;
	}

	size_t bytes_to_write = size * nmemb;
	if((stream == stdout) || (stream == stderr))
	{
		uint8_t *c_buf = (uint8_t *)ptr;
		for(size_t i = 0; i < bytes_to_write; i++)
			split_putc((char)c_buf[i]);
	}
	else
	{
		size_t nmemb_to_write = bytes_to_write / size;
		bytes_to_write = nmemb_to_write * size;
		if(stream->fs->fwrite == NULL)
		{
			errno = EROFS;
			return 0;
		}
		bytes_to_write = stream->fs->fwrite(stream->fs, ptr, bytes_to_write, stream);
	}
	return bytes_to_write / size;
}

int fflush(FILE *fp)
{
	if(fp == NULL)
	{
		errno = EINVAL;
		return -1;
	}
	if(fp->fflush_cb)
		fp->fflush_cb(fp);
	if(fp->fs->fflush)
		fp->fs->fflush(fp);
	return 0;
}

int fclose(FILE *fp)
{
	if(fp == NULL)
	{
		errno = EINVAL;
		return -1;
	}
	fflush(fp);
	if(fp->fs->fclose)
		fp->fs->fclose(fp->fs, fp);

	free(fp);
	return 0;
}

int feof(FILE *stream)
{
	if(!stream)
	{
		errno = EINVAL;
		return -1;
	}
	if(stream->flags & VFS_FLAGS_EOF)
		return 1;
	else
		return 0;
}

int ferror(FILE *stream)
{
	if(!stream)
	{
		errno = EINVAL;
		return -1;
	}
	if(stream->flags & VFS_FLAGS_ERROR)
		return 1;
	else
		return 0;
}

long fsize(FILE *stream)
{
	if(!stream)
	{
		errno = EINVAL;
		return -1;
	}
	if(stream->fs->fsize)
		return stream->fs->fsize(stream);
	else
		return stream->len;
}

long ftell(FILE *stream)
{
	if(!stream)
	{
		errno = EINVAL;
		return -1;
	}
	if(stream->fs->ftell)
		return stream->fs->ftell(stream);
	else
		return stream->pos;
}

int fseek(FILE *stream, long offset, int whence)
{
	if(!stream)
	{
		errno = EINVAL;
		return -1;
	}

	if(stream->fs->fseek)
		return stream->fs->fseek(stream, offset, whence);
	
	switch(whence)
	{
		case SEEK_SET:
			stream->pos = offset;
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

	if(stream->pos < 0)
		stream->pos = 0;
	if(stream->pos > stream->len)
		stream->pos = stream->len;
	return 0;
}

FILE *fopen(const char *path, const char *mode)
{
	char **p;
	struct vfs_entry *ve;

	if(path == (void *)0)
	{
		errno = EFAULT;
		return (void*)0;
	}

	p = split_dir(path, &ve);
	if(p == (void *)0)
	{
		errno = EFAULT;
		return (void *)0;
	}

	if((NULL == p[0]) || (!strcmp(p[0], ":")))
	{
		free_split_dir(p);

		// These represent attempts to open the whole device as a single file
		// We can only do this if the filesystem allows it
		if(ve->fs->flags & FS_FLAG_SUPPORTS_EMPTY_FNAME)
			return ve->fs->fopen(ve->fs, NULL, mode);
		else
		{
			errno = EFAULT;
			return NULL;
		}
	}

	// Trim off the last entry
	char **p_iter = p;
	while(*p_iter)
		p_iter++;
	char *fname = p[(p_iter - p) - 1];
	p[(p_iter - p) - 1] = 0;

	// Read the containing directory
	struct dirent *dir = ve->fs->read_directory(ve->fs, p);
	struct dirent *dir_start = dir;
	if(dir == (void*)0)
	{
		free_split_dir(p);
		free(fname);
		return (void*)0;
	}

	struct dirent *file = (void *)0;
	while(dir)
	{
		if(!strcmp(dir->name, fname))
		{
			file = dir;
			break;
		}
		dir = dir->next;
	}

	free_split_dir(p);
	free(fname);

	if(!file)
	{
		free_dirent_list(dir_start);
		return (void*)0;
	}

	// Read the file
	FILE *ret = ve->fs->fopen(ve->fs, file, mode);
	free_dirent_list(dir_start);
	return ret;
}

*/
