/* multiboot.h - Multiboot header file. */
/* Copyright (C) 1999,2003,2007,2008,2009  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Modified from http://www.gnu.org/software/grub/manual/multiboot/multiboot.html

#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MULTIBOOT_HEADER_MAGIC		0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

#include <stdint.h>

#ifdef __ARMEL__
#include <stddef.h>
#include "timer.h"
#include "output.h"
#ifndef FILE
#ifdef VFS_H
#define FILE struct vfs_file
#else
#define FILE uint32_t *
#endif
#endif
#ifndef DIR
#define DIR uint32_t *
//typedef uint32_t * DIR;
#endif

#define SEEK_SET        0x1000
#define SEEK_CUR        0x1001
#define SEEK_END        0x1002
#define SEEK_START      SEEK_SET

struct dirent {
	struct dirent *next;
        char *name;
	uint32_t byte_size;
	uint8_t is_dir;
	void *opaque;
	struct fs *fs;
};

#define MB_ARM_VERSION		2

struct multiboot_arm_functions
{
    // Console output functions
    int (*printf)(const char *format, ...);
    void (*clear)();

    // File/directory read functions
    FILE *(*fopen)(const char *path, const char *mode);
    size_t (*fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
    int (*fclose)(FILE *fp);
    int (*fseek)(FILE *stream, long offset, int whence);
    DIR *(*opendir)(const char *name);
    struct dirent *(*readdir)(DIR *dirp);
    int (*closedir)(DIR *dirp);

    // Timer functions
    int (*usleep)(useconds_t usec);

    // Functions for controlling output to framebuffer or serial
    rpi_boot_output_state (*output_get_state)();
    void (*output_restore_state)(rpi_boot_output_state state);
    void (*output_enable_fb)();
    void (*output_disable_fb)();
    void (*output_enable_uart)();
    void (*output_disable_uart)();

	// Get the version of the multiboot arm header
	int (*mb_arm_version)();

	// More file functions
	int (*feof)(FILE *stream);
	int (*ferror)(FILE *stream);
	size_t (*fwrite)(void *ptr, size_t size, size_t nmemb, FILE *stream);
	long (*fsize)(FILE *stream);
	long (*ftell)(FILE *stream);
	int (*fflush)(FILE *stream);

	// Ramdisk functions
	int (*ramdisk_init)(uintptr_t address, size_t size, int fs_type, char *name);

	// Log functions
	int (*register_custom_output_function)(int (*putc_function)(int c));
	void (*output_enable_custom)();
	void (*output_disable_custom)();
	void (*output_enable_log)();
	void (*output_disable_log)();
	int (*register_log_file)(FILE *fp, size_t buffer_size);
	FILE *(*get_log_file)();
};

#endif // __ARMEL__

typedef struct multiboot_header
{
  uint32_t magic;
  uint32_t flags;
  uint32_t checksum;
  uint32_t header_addr;
  uint32_t load_addr;
  uint32_t load_end_addr;
  uint32_t bss_end_addr;
  uint32_t entry_addr;
} multiboot_header_t;

typedef struct aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
} aout_symbol_table_t;

typedef struct elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
} elf_section_header_table_t;

typedef struct multiboot_info
{
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
#ifdef __ARMEL__
  char *boot_device;
  char *cmdline;
#else
  uint32_t boot_device;
  uint32_t cmdline;
#endif
  uint32_t mods_count;
  uint32_t mods_addr;
  union
  {
    aout_symbol_table_t aout_sym;
    elf_section_header_table_t elf_sec;
  } u;
  uint32_t mmap_length;
  uint32_t mmap_addr;

  uint32_t drives_length;
#ifdef __ARMEL__
  char **drives_addr;
#else
  uint32_t drives_addr;
#endif

  /* ROM configuration table */
  uint32_t config_table;

  /* Boot Loader Name */
#ifdef __ARMEL__
  char *boot_loader_name;
#else
  uint32_t boot_loader_name;
#endif

  /* APM table */
  uint32_t apm_table;

  /* Video */
#ifdef __ARMEL__
  uint32_t fb_addr;
  uint32_t fb_size;
  uint32_t fb_pitch;
  uint32_t fb_depth;
#else
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
#endif
} multiboot_info_t;

typedef struct module
{
  uint32_t mod_start;
  uint32_t mod_end;
  uint32_t string;
  uint32_t reserved;
} module_t;

typedef struct memory_map
{
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
} memory_map_t;

#endif

