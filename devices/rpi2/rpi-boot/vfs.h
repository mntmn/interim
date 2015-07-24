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

#ifndef VFS_H
#define VFS_H

#define ENABLE_SD 1
#define ENABLE_MBR 1
#define ENABLE_FAT 1

struct vfs_file;

#include "dirent.h"
//#include "multiboot.h"

#ifndef EOF
#define EOF 0xff
#endif

#ifdef FILE
#undef FILE
#endif
#define FILE struct vfs_file

#include "fs.h"

struct vfs_entry
{
    char *device_name;
    struct fs *fs;
    struct vfs_entry *next;
};

#define VFS_MODE_R		1
#define VFS_MODE_W		2
#define VFS_MODE_RW		3
#define VFS_MODE_APPEND	4
#define VFS_MODE_CREATE	8

#define VFS_FLAGS_EOF	1
#define VFS_FLAGS_ERROR	2
/*
struct vfs_file
{
    struct fs *fs;
    long pos;
	int mode;
    void *opaque;
    long len;
	int flags;
	int (*fflush_cb)(FILE *f);
};
*/
/*int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
long fsize(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
int fflush(FILE *stream);
void rewind(FILE *stream);
*/
/*int vfs_register(struct fs *fs);
void vfs_list_devices();
char **vfs_get_device_list();
int vfs_set_default(char *dev_name);
char *vfs_get_default();*/
/*
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
*/
#endif

