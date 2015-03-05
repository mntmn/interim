/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `disassemble_init_for_target' function. */
/* #undef HAVE_DISASSEMBLE_INIT_FOR_TARGET */

/* Define to 1 if you have the `disassemble_init_powerpc' function. */
/* #undef HAVE_DISASSEMBLE_INIT_POWERPC */

#define DISASSEMBLER 1

#include <unistd.h>
#include <stdlib.h>

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 0

/* Define to 1 if you have the `ffsl' function. */
#define HAVE_FFSL 0

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 0

/* Define to 1 if you have the `getopt_long_only' function. */
#define HAVE_GETOPT_LONG_ONLY 0

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 0

/* Define to 1 if you have the `isinf' function. */
#define HAVE_ISINF 0

/* Define to 1 if you have the `isnan' function. */
#define HAVE_ISNAN 0

/* Define to 1 if you have the `bfd' library (-lbfd). */
#define HAVE_LIBBFD 0

/* Define to 1 if you have the `iberty' library (-liberty). */
#define HAVE_LIBIBERTY 0

/* Define to 1 if you have the `m' library (-lm). */
/* #undef HAVE_LIBM */

/* Define to 1 if you have the `opcodes' library (-lopcodes). */
#define HAVE_LIBOPCODES 0

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 0

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 0

/* Define to 1 if you have the `mremap' function. */
#define HAVE_MREMAP 0

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 0

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 0

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "lightning"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "pcpa@gnu.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GNU lightning"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GNU lightning 2.0.5"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "lightning"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.gnu.org/software/lightning/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.0.5"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "2.0.5"

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

#define PROT_EXEC 0
#define PROT_READ 0
#define PROT_WRITE 0
#define MAP_PRIVATE 0
#define MAP_ANON 0
#define MAP_FAILED 0
