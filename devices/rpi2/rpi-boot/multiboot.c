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

// Parses the .cfg file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "atag.h"
#include "elf.h"
#include "memchunk.h"
#include "vfs.h"
#include "multiboot.h"
#include "console.h"
#include "fb.h"
#include "timer.h"
#include "output.h"
#include "log.h"

#ifdef DEBUG2
#define MULTIBOOT_DEBUG
#endif

static int method_multiboot(char *args);
static int method_boot(char *args);
static int method_module(char *args);
static int method_kernel(char *args);
static int method_entry_addr(char *args);
static int method_binary_load_addr(char *args);
static int method_console_log(char *args);

static void atag_cb(struct atag *);
static void atag_cb2(struct atag *);

extern uint32_t _atags;
extern uint32_t _arm_m_type;
extern char *rpi_boot_name;
uint32_t *mmap_ptr;

struct multiboot_info *mbinfo = (void *)0;
uint32_t entry_addr = 0;
uint32_t binary_load_addr = 0;

struct multiboot_method
{
	char *name;
	int (*method)(char *args);
};

static struct multiboot_method methods[] =
{
	{
		.name = "multiboot",
		.method = method_multiboot
	},
	{
		.name = "boot",
		.method = method_boot
	},
	{
		.name = "module",
		.method = method_module
	},
	{
		.name = "kernel",
		.method = method_kernel
	},
	{
		.name = "entry_addr",
		.method = method_entry_addr
	},
	{
		.name = "binary_load_addr",
		.method = method_binary_load_addr
	},
	{
		.name = "console_log",
		.method = method_console_log
	},
};

static int mb_arm_version()
{
	return MB_ARM_VERSION;
}

/* Define a dummy clear() function if the framebuffer is not enabled, to prevent
 * buggy clients from calling a null pointer
 */
#ifndef ENABLE_FRAMEBUFFER
void clear()
{ }
#endif

int ramdisk_init(uintptr_t address, size_t size, int fs_type, char *name);
#ifndef ENABLE_RAMDISK
int ramdisk_init(uintptr_t address, size_t size, int fs_type, char *name)
{
	(void)address; (void)size; (void)fs_type; (void)name;
	return -1;
}
#endif

#ifndef ENABLE_CONSOLE_LOGFILE
int register_log_file(FILE *fp, size_t buffer_size)
{
	(void)fp; (void)buffer_size;
	return -1;
}
FILE *get_log_file()
{
	return NULL;
}
#endif

struct multiboot_arm_functions funcs =
{
	.printf = printf,
	.clear = clear,
	.fopen = fopen,
	.fread = fread,
	.fclose = fclose,
	.fseek = fseek,
	.ftell = ftell,
	.fsize = fsize,
	.ferror = ferror,
	.fwrite = fwrite,
	.feof = feof,
	.fflush = fflush,
	.opendir = opendir,
	.readdir = readdir,
	.closedir = closedir,
	.usleep = usleep,
	.output_get_state = output_get_state,
	.output_restore_state = output_restore_state,
	.output_enable_fb = output_enable_fb,
	.output_disable_fb = output_disable_fb,
	.output_enable_uart = output_enable_uart,
	.output_disable_uart = output_disable_uart,
	.output_enable_log = output_enable_log,
	.output_disable_log = output_disable_log,
	.output_enable_custom = output_enable_custom,
	.output_disable_custom = output_disable_custom,
	.register_custom_output_function = register_custom_output_function,
	.register_log_file = register_log_file,
	.get_log_file = get_log_file,
	.ramdisk_init = ramdisk_init,
	.mb_arm_version = mb_arm_version
};

static char *read_line(char **buf)
{
	char *start = *buf;
	char *ptr = *buf;

	if(!*start)
		return (void*)0;
	while((*ptr != 0) && (*ptr != '\n'))
		ptr++;

	if(*ptr == 0)
	{
		// End of the string
		*buf = ptr;
		return start;
	}
	else
	{
		// End of a line - null terminate
		*ptr = 0;
		ptr++;
		*buf = ptr;
		return start;
	}
}

/* Remove leading and trailing whitespace and trailing comments from a configuration line */
static char *strip_comments(char *buf)
{
	// Remove leading whitespace
	while(isspace(*buf))
		buf++;

	// Remove comments
	char *s = buf;
	while(*s)
	{
		if(*s == '#')
			*s = '\0';
		else
			s++;
	}

	// Remove trailing whitespace
	s--;
	while((s > buf) && isspace(*s))
	{
		*s = '\0';
		s--;
	}

	return buf;
}

char empty_string[] = "";

static void split_string(char *str, char **method, char **args)
{
	int state = 0;
	char *p = str;

	// state = 	0 - reading spaces before method
	// 		1 - reading method
	// 		2 - reading spaces before argument

	*method = empty_string;
	*args = empty_string;

	while(*p)
	{
		if(*p == ' ')
		{
			if(state == 1)
			{
				*p = 0;	// null terminate method
				state = 2;
			}
		}
		else
		{
			if(state == 0)
			{
				*method = p;
				state = 1;
			}
			else if(state == 2)
			{
				*args = p;
				return;
			}
		}
		p++;
	}
}

int cfg_parse(char *buf)
{
	char *line;
	char *b = buf;
	while((line = read_line(&b)))
	{
		line = strip_comments(line);
#ifdef MULTIBOOT_DEBUG
		printf("read_line: %s\n", line);
#endif
		char *method, *args;
		split_string(line, &method, &args);
#ifdef MULTIBOOT_DEBUG
		printf("method: %s, args: %s\n", method, args);
#endif

		if(!strcmp(method, empty_string))
			continue;

		// Find and run the method
		int method_count = sizeof(methods) / sizeof(struct multiboot_method);
		int found = 0;
		for(int i = 0; i < method_count; i++)
		{
			char *lwr = strlwr(method);

			if(!strcmp(lwr, methods[i].name))
			{
				found = 1;
				int retno = methods[i].method(args);
				if(retno != 0)
				{
					printf("cfg_parse: %s failed with "
							"%i\n", line,
							retno);
					return retno;
				}
				free(lwr);
				break;
			}
			free(lwr);
		}

		if(!found)
			printf("cfg_parse: unknown method %s\n", method);
	}

	return 0;
}

int method_multiboot(char *args)
{
#ifdef MULTIBOOT_DEBUG
	printf("Interpreting multiboot command\n");
#endif
	char *file, *cmd_line;
	split_string(args, &file, &cmd_line);

	// First load up the first 8192 bytes to look for the multiboot header
	FILE *fp = fopen(file, "r");
	if(!fp)
	{
		printf("MULTIBOOT: cannot load %s; errno=$d\n", file, errno);
		return -1;
	}
#ifdef MULTIBOOT_DEBUG
	printf("MULTIBOOT: loading first 8kiB of %s\n", file);
#endif
	uint32_t *first_8k = (uint32_t *)malloc(8192);
	int buf_size = fread(first_8k, 1, 8192, fp);

#ifdef MULTIBOOT_DEBUG
	printf("MULTIBOOT: loaded first %i bytes\n", buf_size);
#endif

	struct multiboot_header *mboot = (void*)0;
	uint32_t header_offset;

	for(int i = 0; i < (int)(buf_size / sizeof(uint32_t)); i++)
	{
		if(first_8k[i] == MULTIBOOT_HEADER_MAGIC)
		{
			// Found a multiboot header
			// Check the checksum
			struct multiboot_header *mb = (struct multiboot_header *)&first_8k[i];
			if((mb->magic + mb->flags + mb->checksum) == 0)
			{
				// Its valid, save it somewhere
				mboot = (struct multiboot_header *)
					malloc(sizeof(struct multiboot_header));
				memcpy(mboot, mb, sizeof(struct multiboot_header));
				header_offset = i * 4;
				break;
			}
		}
	}

	free(first_8k);

	if(!mboot)
	{
		printf("MULTIBOOT: no valid multiboot header found in %s\n", file);
		return -1;
	}

#ifdef MULTIBOOT_DEBUG
	printf("MULTIBOOT: valid multiboot header, flags: %08x\n", mboot->flags);
#endif

	// Create a multiboot info header
	mbinfo = (struct multiboot_info *)malloc(sizeof(struct multiboot_info));
	memset(mbinfo, 0, sizeof(struct multiboot_info));

	// Setup the fields
	if(mboot->flags & (1 << 1))
	{
		// Pass memory information

		// First count the number of memory sections and set mem_upper
		parse_atags(_atags, atag_cb);

		// Allocate the mmap buffer
		mbinfo->mmap_addr = (uint32_t)malloc(mbinfo->mmap_length);
		mmap_ptr = (uint32_t *)mbinfo->mmap_addr;

		// Skip the pointer to the first item (4 bytes in - structure
		// starts at offset -4)
		mbinfo->mmap_addr += 4;

		// Now fill in the buffer
		parse_atags(_atags, atag_cb2);

		// Set flags to say we've provided memory info
		mbinfo->flags |= (1 << 0);
		mbinfo->flags |= (1 << 6);
	}


	// Load the file
	if(mboot->flags & (1 << 16))
	{
		// This is an a.out file - load it
		//
		// Check we're loading above 1MiB

		if(mboot->load_addr < 0x100000)
		{
			printf("MULTIBOOT: a.out load below 1 MiB - not supported\n");
			return -1;
		}

		uint32_t file_offset = header_offset - mboot->header_addr + mboot->load_addr;
		uint32_t len;
		if(mboot->load_end_addr)
			len = mboot->load_end_addr - mboot->load_addr;
		else
			len = (uint32_t)fp->len - file_offset;
		uint32_t bss_len = 0;
		if(mboot->bss_end_addr > mboot->load_end_addr)
			bss_len = mboot->bss_end_addr - mboot->load_end_addr;

		entry_addr = mboot->entry_addr;

		// Try and allocate a chunk for the file
		if(!chunk_get_chunk(mboot->load_addr, len + bss_len))
		{
			printf("MULTIBOOT: a.out load - unable to allocate a chunk "
					"between 0x%08x and 0x%08x\n",
					mboot->load_addr, mboot->load_addr + len + bss_len);
			return -1;
		}

		// Load the file
		fseek(fp, (long)file_offset, SEEK_SET);
		size_t b_read = fread((void *)mboot->load_addr, 1, (size_t)len, fp);
		if(b_read != (size_t)len)
		{
			printf("MULTIBOOT: a.out load error - tried to load %i bytes "
					"but could only load %i\n", len, b_read);
			return -1;
		}

		// Zero bss
		if(bss_len)
			memset((void *)(mboot->load_end_addr), 0, bss_len);

		// We don't process a.out symbol tables, therefore don't set bit 4
	}
	else
	{
		// This is an ELF file
		//
		// Load the ELF header

		Elf32_Ehdr *ehdr;
		int retno = elf32_read_ehdr(fp, &ehdr);
		if(retno != ELF_OK)
			return retno;

		// Load up the section headers
		if(!ehdr->e_shoff || !ehdr->e_shnum)
		{
			printf("MULTIBOOT: %s does not contain a section table\n",
					file);
			free(ehdr);
			return -1;
		}

		uint8_t *sh_buf;
		retno = elf32_read_shdrs(fp, ehdr, &sh_buf);
		if(retno != ELF_OK)
		{
			free(ehdr);
			return retno;
		}

		// Now interpret and load them
		//
		// We do two passes - first loading the sections marked ALLOC to their
		// appropriate addresses, then loading the others (Multiboot requires
		// we load all sections).  This ensures we don't load the sections not
		// marked ALLOC to an address that a later section requires.

		for(unsigned int i = 0; i < ehdr->e_shnum; i++)
		{
			Elf32_Shdr *shdr = (Elf32_Shdr *)&sh_buf[i * ehdr->e_shentsize];

			if(shdr->sh_flags & SHF_ALLOC)
			{
#ifdef MULTIBOOT_DEBUG
				printf("MULTIBOOT: section %i is loadable\n", i);
#endif

				// Try and allocate space for it
				if(!shdr->sh_addr)
				{
					printf("MULTIBOOT: section %i has no defined "
							"load address\n", i);
					free(ehdr);
					free(sh_buf);
					return -1;
				}
				if(!shdr->sh_size)
				{
					printf("MULTIBOOT: section %i has no defined "
							"size\n", i);
					free(ehdr);
					free(sh_buf);
					return -1;
				}

				if(!chunk_get_chunk(shdr->sh_addr, shdr->sh_size))
				{
					printf("MULTIBOOT: unable to allocate a chunk "
						"between 0x%08x and 0x%08x for section %i\n",
						shdr->sh_addr, shdr->sh_addr + shdr->sh_size,
						i);
					free(ehdr);
					free(sh_buf);
					return -1;
				}

				// Now load or zero it
				retno = elf32_load_section(fp, shdr);
				if(retno != ELF_OK)
				{
					free(ehdr);
					free(sh_buf);
					return retno;
				}
			}
		}

		for(unsigned int i = 0; i < ehdr->e_shnum; i++)
		{
			Elf32_Shdr *shdr = (Elf32_Shdr *)&sh_buf[i * ehdr->e_shentsize];

			if(!(shdr->sh_flags & SHF_ALLOC))
			{
#ifdef MULTIBOOT_DEBUG
				printf("MULTIBOOT: section %i is not loadable\n", i);
#endif

				if(shdr->sh_size)
				{
					uint32_t load_addr = chunk_get_any_chunk(shdr->sh_size);

					if(!load_addr)
					{
						printf("MULTIBOOT: unable to allocate chunk of "
								"size %i for section %i\n",
								shdr->sh_size, i);
						return -1;
					}

					shdr->sh_addr = load_addr;
					retno = elf32_load_section(fp, shdr);
					if(retno != ELF_OK)
					{
						free(ehdr);
						free(sh_buf);
						return retno;
					}
				}
			}
		}

		// Set the ELF flags
		mbinfo->u.elf_sec.num = ehdr->e_shnum;
		mbinfo->u.elf_sec.size = ehdr->e_shentsize;
		mbinfo->u.elf_sec.addr = (uint32_t)sh_buf;
		mbinfo->u.elf_sec.shndx = ehdr->e_shstrndx;
		mbinfo->flags |= (1 << 6);

		entry_addr = ehdr->e_entry;

		free(ehdr);
	}

	// Set the cmd line
	mbinfo->cmdline = args;
	mbinfo->flags |= (1 << 2);

	// Set the boot device
	mbinfo->boot_device = fp->fs->parent->device_name;
	mbinfo->flags |= (1 << 1);

	// Set the boot loader name
	mbinfo->boot_loader_name = rpi_boot_name;
	mbinfo->flags |= (1 << 9);

	// Set the fb info
#ifdef ENABLE_FRAMEBUFFER
	mbinfo->fb_addr = (uint32_t)fb_get_framebuffer();
	mbinfo->fb_size = (fb_get_width() << 16) | (fb_get_height() & 0xffff);
	mbinfo->fb_pitch = fb_get_pitch();
	mbinfo->fb_depth = (fb_get_bpp() << 16) | (0x1);	// TODO: check pixel_order
	mbinfo->flags |= (1 << 11);
#endif

	// Set the device containing the kernel to be the default
	vfs_set_default(fp->fs->parent->device_name);

	printf("MULTIBOOT: loaded kernel %s\n", file);

	fclose(fp);

	return 0;
}

struct _module {
	uint32_t start;
	uint32_t end;
	char *name;
	struct _module *next;
};

struct _module *first_mod = (void*)0;
int mod_count = 0;

static void module_add(uint32_t start, uint32_t end, char *name)
{
	struct _module *m = (struct _module *)malloc(sizeof(struct module));
	m->start = start;
	m->end = end;
	m->name = name;
	m->next = first_mod;
	first_mod = m;
	mod_count++;
}

static void add_multiboot_modules()
{
	mbinfo->mods_count = mod_count;

	mbinfo->mods_addr = (uint32_t)malloc(16 * mod_count);
	struct _module *cur_mod = first_mod;
	for(int i = 0; i < mod_count; i++)
	{
		struct module *mmod = (struct module *)(mbinfo->mods_addr + i * 16);
		mmod->mod_start = cur_mod->start;
		mmod->mod_end = cur_mod->end;
		mmod->string = (uint32_t)cur_mod->name;
		mmod->reserved = 0;

		cur_mod = cur_mod->next;
	}
}

int method_module(char *args)
{
	char *file, *name;
	split_string(args, &file, &name);

	if(!strcmp(name, empty_string))
		name = file;

	// Load a module
	FILE *fp = fopen(name, "r");
	if(!fp)
	{
		printf("MODULE: cannot load file %s\n", name);
		return -1;
	}

	// Allocate a chunk for it
	uint32_t address = chunk_get_any_chunk((uint32_t)fp->len);
	if(!address)
	{
		printf("MODULE: unable to allocate a chunk of size %i for %s\n",
				fp->len, name);
		return -1;
	}

	// Load it
	size_t bytes_to_read = (size_t)fp->len;
	size_t bytes_read = fread((void*)address, 1, bytes_to_read, fp);
	fclose(fp);

	if(bytes_to_read != bytes_read)
	{
		printf("MODULE: error loading %s only %i out of %i bytes read\n",
				name, bytes_read, bytes_to_read);
		return -1;
	}

	module_add(address, address + (uint32_t)bytes_read, name);

	printf("MODULE: %s loaded\n", name);
	return 0;
}

int method_boot(char *args)
{
#ifdef MULTIBOOT_DEBUG
	printf("Interpreting boot command\n");
#endif
	(void)args;

	if(entry_addr == 0)
	{
		printf("BOOT: no valid kernel loaded\n");
		return -1;
	}

	if(mbinfo)
	{
		add_multiboot_modules();

		// Do a multiboot load
		printf("BOOT: multiboot load\n");

		void (*e_point)(uint32_t, uint32_t, uint32_t, uint32_t) =
			(void(*)(uint32_t, uint32_t, uint32_t, uint32_t))entry_addr;
		e_point(MULTIBOOT_BOOTLOADER_MAGIC, (uint32_t)mbinfo,
				_arm_m_type, (uint32_t)&funcs);
	}
	else
	{
		// Do a simple jump
		printf("BOOT: non-multiboot load\n");

		void (*e_point)(uint32_t, uint32_t, uint32_t, uint32_t) =
			(void(*)(uint32_t, uint32_t, uint32_t, uint32_t))entry_addr;
		e_point(0x0, _arm_m_type, _atags, (uint32_t)&funcs);
	}
	return 0;
}

int method_kernel(char *args)
{
	char *file, *name;
	split_string(args, &file, &name);

	FILE *fp = fopen(file, "r");
	if(!fp)
	{
		printf("KERNEL: unable to load %s; errno=%d\n", file, errno);
		return -1;
	}

	// Load up the first 0x30 bytes to determine the kernel type
	uint8_t *first_bytes = (uint8_t *)malloc(0x30);
	size_t bytes_to_read = 0x30;
	size_t bytes_read = fread(first_bytes, 1, bytes_to_read, fp);
	if(bytes_read <= 0)
	{
		free(first_bytes);
		printf("KERNEL: error reading from %s\n", file);
		return -1;
	}

	int kernel_type = 0;	// 0 = flat binary, 1 = ELF, 2 = linux

	// If the first 4 bytes are the ELF magic number, assume its ELF
	if((bytes_read >= 4) && (first_bytes[0] == 0x7f) &&
			(first_bytes[1] == 'E') && (first_bytes[2] == 'L')
			&& (first_bytes[3] == 'F'))
		kernel_type = 1;
	else if((bytes_read >= 0x30) &&
			(*(uint32_t *)&first_bytes[0x24] == 0x016F2818))
		kernel_type = 2;

	free(first_bytes);

	// Now load up the appropriate kernel type
	if(kernel_type == 0)
	{
		// Allocate memory for it
		uint32_t length = (uint32_t)fp->len;
		if(binary_load_addr)
		{
			if(!chunk_get_chunk(binary_load_addr, length))
			{
				printf("KERNEL: unable to allocate %i bytes "
						"at 0x%x for kernel %s.\n",
						length, binary_load_addr,
						file);
				return -1;
			}
		}
		else
		{
			binary_load_addr = chunk_get_any_chunk(length);
			if(!binary_load_addr)
			{
				printf("KERNEL: unable to allocate %i bytes "
						" for kernel %s.\n",
						length, file);
				return -1;
			}
		}

		// Load it
		fseek(fp, 0, SEEK_SET);
		bytes_read = fread((void *)binary_load_addr, 1,
				(size_t)length, fp);
		if(bytes_read != (size_t)length)
		{
			printf("KERNEL: unable to load kernel %s - only %i "
					"bytes loaded\n", file, length);
			return -1;
		}
		fclose(fp);

        // Set the entry point to the beginning of the file (if not already set)
        if(!entry_addr)
            entry_addr = binary_load_addr;

		return 0;
	}
	else if(kernel_type == 1)
	{
		// Perform an ELF load
		Elf32_Ehdr *ehdr;
		int retno = elf32_read_ehdr(fp, &ehdr);
		if(retno != ELF_OK)
		{
			fclose(fp);
			return retno;
		}

		uint8_t *ph_buf;
		retno = elf32_read_phdrs(fp, ehdr, &ph_buf);
		if(retno != ELF_OK)
		{
			free(ehdr);
			fclose(fp);
			return retno;
		}

		for(int i = 0; i < ehdr->e_phnum; i++)
		{
			Elf32_Phdr *phdr =
				(Elf32_Phdr *)&ph_buf[i * ehdr->e_phentsize];

			if(phdr->p_type != PT_LOAD)
				continue;

			uint32_t start = (uint32_t)phdr->p_vaddr;
			uint32_t length = (uint32_t)phdr->p_memsz;

			// Check we can load to this address
			if(!chunk_get_chunk(start, length))
			{
				free(ehdr);
				free(ph_buf);
				fclose(fp);
				return retno;
			}

			// Load the segment
			retno = elf32_load_segment(fp, phdr);
			if(retno != ELF_OK)
			{
				free(ehdr);
				free(ph_buf);
				fclose(fp);
				return retno;
			}
		}

		entry_addr = ehdr->e_entry;
	}
	else if (kernel_type == 2)
	{
		printf("KERNEL: Linux kernels not currently supported\n");
		return -1;
	}

	return 0;
}

int method_entry_addr(char *args)
{
	// strtol requires checking errno for success as the returned value
	// could be any integer even if successful
	errno = 0;
	char *endptr;
	long val = strtol(args, &endptr, 0);
	if((errno == 0) && (*args != '\0') && (*endptr == '\0'))
	{
		// valid string
		entry_addr = (uint32_t)val;
		return 0;
	}
	else
	{
		printf("ENTRY_ADDR: %s is not a valid address\n", args);
		return -1;
	}
}

int method_console_log(char *args)
{
	// Arguments: <log file name>[+] [buffer_size]
	// 	log file name, buffer_size
	//
	//	buffer_size is a byte count, it determines how many
	//  bytes are buffered before being flushed to disk.
	//  Flushing can be performed manually with fflush()ing
	//  the file pointer returned by get_log_file()
	//
	//  0 for no buffer
	//
	//  If log file name is appended by a '+' then the file is
	//  opened in append mode.

	char *logName, *buffer_size;
	long bufferSize;
	int is_append = 0;

	char *endptr;
	FILE *target;

	split_string(args, &logName, &buffer_size);

	// Determine if we are appending
	if(logName[strlen(logName) - 1] == '+')
	{
		is_append = 1;
		logName[strlen(logName) - 1] = '\0';
	}

	if(buffer_size == empty_string)
		bufferSize = LOG_DEFAULT_BUFFER_SIZE;
	else
	{
		bufferSize = strtol(buffer_size, &endptr, 0);
		if (endptr == buffer_size || '\0' != *endptr)
		{
			printf("CONSOLE_LOG: buffer end is missing, or junk on line; "
				   " found '%s'.", args);
			return -1;
		}
	}

#ifdef MULTIBOOT_DEBUG
	printf("CONSOLE_LOG: got arguments file='%s', "
	       "buffer size = %ld(0x%lx)\n",
	       logName, bufferSize, bufferSize);
#endif

	if (NULL == (target = fopen(logName, (is_append) ? "a+" : "w+"))) 
	{
		printf("CONSOLE_LOG: cannot open log '%s': errno=%d\n",
		       logName, errno);
		return -1;
	}

	register_log_file(target, bufferSize);

	return 0;
}

int method_binary_load_addr(char *args)
{
	// strtol requires checking errno for success as the returned value
	// could be any integer even if successful
	errno = 0;
	char *endptr;
	long val = strtol(args, &endptr, 0);
	if((errno == 0) && (*args != '\0') && (*endptr == '\0'))
	{
		// valid string
		binary_load_addr = (uint32_t)val;
		return 0;
	}
	else
	{
		printf("BINARY_LOAD_ADDR: %s is not a valid address\n", args);
		return -1;
	}
}

void atag_cb(struct atag *tag)
{
	if(tag->hdr.tag == ATAG_MEM)
	{
		uint32_t start = tag->u.mem.start;
		uint32_t size = tag->u.mem.size;
		uint32_t end = start + size;

		// mem_upper is number of kiB beyond 1 MiB
		if((start < 0x100000) && (end > 0x100000))
			mbinfo->mem_upper = end / 1024;

		mbinfo->mmap_length += 24;
	}
}

void atag_cb2(struct atag *tag)
{
	if(tag->hdr.tag == ATAG_MEM)
	{
		uint32_t start = tag->u.mem.start;
		uint32_t size = tag->u.mem.size;

		mmap_ptr[0] = 24;	// size of the tag
		mmap_ptr[1] = start;	// base addr
		mmap_ptr[2] = 0;	// upper 32 bits of base addr
		mmap_ptr[3] = size;	// length
		mmap_ptr[4] = 0;	// upper 32 bits of length
		mmap_ptr[5] = 1;	// available

		mmap_ptr += 6;		// skip to next
	}
}

