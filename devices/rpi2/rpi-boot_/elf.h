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

/* elf header definitions for 32 and 64 bits */
#include <stdint.h>

typedef uint32_t		Elf32_Addr;
typedef uint16_t		Elf32_Half;
typedef uint32_t		Elf32_Off;
typedef int32_t			Elf32_Sword;
typedef uint32_t		Elf32_Word;

typedef uint64_t		Elf64_Addr;
typedef uint64_t		Elf64_Off;
typedef uint16_t		Elf64_Half;
typedef uint32_t		Elf64_Word;
typedef int32_t			Elf64_Sword;
typedef uint64_t		Elf64_Xword;
typedef int64_t			Elf64_Sxword;

typedef struct {
	Elf64_Word			sh_name;
	Elf64_Word			sh_type;
	Elf64_Xword			sh_flags;
	Elf64_Addr			sh_addr;
	Elf64_Off			sh_offset;
	Elf64_Xword			sh_size;
	Elf64_Word			sh_link;
	Elf64_Word			sh_info;
	Elf64_Xword			sh_addralign;
	Elf64_Xword			sh_entsize;
} Elf64_Shdr;

typedef struct {
	Elf32_Word			sh_name;
	Elf32_Word			sh_type;
	Elf32_Word			sh_flags;
	Elf32_Addr			sh_addr;
	Elf32_Off			sh_offset;
	Elf32_Word			sh_size;
	Elf32_Word			sh_link;
	Elf32_Word			sh_info;
	Elf32_Word			sh_addralign;
	Elf32_Word			sh_entsize;
} Elf32_Shdr;

#define SHT_NULL		0
#define SHT_PROGBITS		1
#define SHT_SYMTAB		2
#define SHT_STRTAB		3
#define SHT_RELA		4
#define SHT_HASH		5
#define SHT_DYNAMIC		6
#define SHT_NOTE		7
#define SHT_NOBITS		8
#define SHT_REL			9
#define SHT_SHLIB		10
#define SHT_DYNSYM		11

typedef struct
{
	unsigned char e_ident[16]; /* ELF identification */
	Elf64_Half e_type; /* Object file type */
	Elf64_Half e_machine; /* Machine type */
	Elf64_Word e_version; /* Object file version */
	Elf64_Addr e_entry; /* Entry point address */
	Elf64_Off e_phoff; /* Program header offset */
	Elf64_Off e_shoff; /* Section header offset */
	Elf64_Word e_flags; /* Processor-specific flags */
	Elf64_Half e_ehsize; /* ELF header size */
	Elf64_Half e_phentsize; /* Size of program header entry */
	Elf64_Half e_phnum; /* Number of program header entries */
	Elf64_Half e_shentsize; /* Size of section header entry */
	Elf64_Half e_shnum; /* Number of section header entries */
	Elf64_Half e_shstrndx; /* Section name string table index */
} Elf64_Ehdr;

typedef struct
{
	unsigned char		e_ident[16];
	Elf32_Half		e_type;
	Elf32_Half		e_machine;
	Elf32_Word		e_version;
	Elf32_Addr		e_entry;
	Elf32_Off		e_phoff;
	Elf32_Off		e_shoff;
	Elf32_Word		e_flags;
	Elf32_Half		e_ehsize;
	Elf32_Half		e_phentsize;
	Elf32_Half		e_phnum;
	Elf32_Half		e_shentsize;
	Elf32_Half		e_shnum;
	Elf32_Half		e_shstrndx;
} Elf32_Ehdr;

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABIVERSION 8
#define EI_PAD 9
#define EI_NIDENT 16

#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4

typedef struct
{
	Elf64_Word p_type; /* Type of segment */
	Elf64_Word p_flags; /* Segment attributes */
	Elf64_Off p_offset; /* Offset in file */
	Elf64_Addr p_vaddr; /* Virtual address in memory */
	Elf64_Addr p_paddr; /* Reserved */
	Elf64_Xword p_filesz; /* Size of segment in file */
	Elf64_Xword p_memsz; /* Size of segment in memory */
	Elf64_Xword p_align; /* Alignment of segment */
} Elf64_Phdr;

typedef struct
{
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

#define EM_NONE		0
#define EM_M32		1
#define EM_SPARC	2
#define EM_386		3
#define EM_68K		4
#define EM_88K		5
#define EM_860		7
#define EM_MIPS		8
#define EM_MIPS_RS4_BE	10
#define EM_ARM		40
#define EM_X86_64 	62

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6

#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

typedef struct
{
	Elf64_Addr r_offset; /* Address of reference */
	Elf64_Xword r_info; /* Symbol index and type of relocation */
} Elf64_Rel;
typedef struct
{
	Elf64_Addr r_offset; /* Address of reference */
	Elf64_Xword r_info; /* Symbol index and type of relocation */
	Elf64_Sxword r_addend; /* Constant part of expression */
} Elf64_Rela;
typedef struct
{
	Elf64_Word	st_name;		/* Symbol name */
	unsigned char	st_info;	/* Type and binding attributes */
	unsigned char	st_other;	/* Reserved */
	Elf64_Half	st_shndx;		/* Section table index */
	Elf64_Addr	st_value;		/* Symbol value */
	Elf64_Xword	st_size;		/* Size of object */
} Elf64_Sym;

#define ELF64_R_SYM(i)		((i) >> 32)
#define ELF64_R_TYPE(i)		((i) & 0xffffffffL)
#define ELF64_R_INFO(s, t)	(((s) << 32) + ((t) & 0xffffffffL))

#define R_X86_64_RELATIVE	8
#define R_X86_64_64			1

#define SHN_UNDEF			0
#define SHN_ABS				0xfff1
#define SHN_COMMON			0xfff2

typedef struct
{
	Elf64_Sxword		d_tag;
	union
	{
		Elf64_Xword		d_val;
		Elf64_Addr		d_ptr;
	} d_un;
} Elf64_Dyn;

#define	DT_NULL				0
#define DT_STRTAB			5
#define DT_SYMTAB			6

int elf32_read_ehdr(FILE *fp, Elf32_Ehdr **ehdr);
int elf32_read_shdrs(FILE *fp, Elf32_Ehdr *ehdr, uint8_t **shdrs);
int elf32_load_section(FILE *fp, Elf32_Shdr *shdr);
int elf32_read_phdrs(FILE *fp, Elf32_Ehdr *ehdr, uint8_t **phdrs);
int elf32_load_segment(FILE *fp, Elf32_Phdr *phdr);

// Error return from the above functions
#define ELF_OK				0
#define ELF_NOT_ELF			-1
#define ELF_NOT_32_BIT			-2
#define ELF_NOT_LITTLE_ENDIAN		-3
#define ELF_NOT_EXEC			-4
#define ELF_NOT_ARM			-5
#define ELF_FILE_LOAD_ERROR		-6
#define ELF_NO_OFFSET			-7



