
# Makefile.in is generated from Makefile.tpl by 'autogen Makefile.def'.
#
# Makefile for directory with subdirs to build.
#   Copyright (C) 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
#   1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011
#   Free Software Foundation
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.
#

# First, test for a proper version of make, but only where one is required.


# -------------------------------
# Standard Autoconf-set variables
# -------------------------------
VPATH=../newlib-2.2.0

build_alias=x86_64-unknown-linux-gnu
build_vendor=unknown
build_os=linux-gnu
build=x86_64-unknown-linux-gnu
host_alias=x86_64-unknown-linux-gnu
host_vendor=unknown
host_os=linux-gnu
host=x86_64-unknown-linux-gnu
target_alias=x86_64-pc-baremetal
target_vendor=pc
target_os=baremetal
target=x86_64-pc-baremetal

program_transform_name = s&^&x86_64-pc-baremetal-&

prefix = /usr/local
exec_prefix = ${prefix}

srcdir = ../newlib-2.2.0

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${datarootdir}
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
oldincludedir = /usr/include
infodir = ${datarootdir}/info
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE}
pdfdir = ${docdir}
htmldir = ${docdir}
mandir = ${datarootdir}/man
man1dir = $(mandir)/man1
man2dir = $(mandir)/man2
man3dir = $(mandir)/man3
man4dir = $(mandir)/man4
man5dir = $(mandir)/man5
man6dir = $(mandir)/man6
man7dir = $(mandir)/man7
man8dir = $(mandir)/man8
man9dir = $(mandir)/man9

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
LN = ln
LN_S = ln -s
MAINT = #
MAINTAINER_MODE_FALSE = 
MAINTAINER_MODE_TRUE = #

# -------------------------------------------------
# Miscellaneous non-standard autoconf-set variables
# -------------------------------------------------

# The gcc driver likes to know the arguments it was configured with.
TOPLEVEL_CONFIGURE_ARGUMENTS=../newlib-2.2.0/configure --target=x86_64-pc-baremetal --disable-multilib

tooldir = ${exec_prefix}/x86_64-pc-baremetal
build_tooldir = ${exec_prefix}/x86_64-pc-baremetal

# This is the name of the environment variable used for the path to
# the libraries.
RPATH_ENVVAR = LD_LIBRARY_PATH

# On targets where RPATH_ENVVAR is PATH, a subdirectory of the GCC build path
# is used instead of the directory itself to avoid including built
# executables in PATH.
GCC_SHLIB_SUBDIR = 

# Build programs are put under this directory.
BUILD_SUBDIR = build-x86_64-unknown-linux-gnu
# This is set by the configure script to the arguments to use when configuring
# directories built for the build system.
BUILD_CONFIGARGS =  --cache-file=../config.cache '--disable-multilib' --program-transform-name='s&^&x86_64-pc-baremetal-&' --disable-option-checking --with-build-subdir="$(BUILD_SUBDIR)"

# Linker flags to use on the host, for stage1 or when not
# bootstrapping.
STAGE1_LDFLAGS = -static-libstdc++ -static-libgcc

# Libraries to use on the host, for stage1 or when not bootstrapping.
STAGE1_LIBS = 

# Linker flags to use for stage2 and later.
POSTSTAGE1_LDFLAGS = -static-libstdc++ -static-libgcc

# Libraries to use for stage2 and later.
POSTSTAGE1_LIBS = 

# This is the list of variables to export in the environment when
# configuring any subdirectory.  It must also be exported whenever
# recursing into a build directory in case that directory's Makefile
# re-runs configure.
BASE_EXPORTS = \
	FLEX="$(FLEX)"; export FLEX; \
	LEX="$(LEX)"; export LEX; \
	BISON="$(BISON)"; export BISON; \
	YACC="$(YACC)"; export YACC; \
	M4="$(M4)"; export M4; \
	SED="$(SED)"; export SED; \
	AWK="$(AWK)"; export AWK; \
	MAKEINFO="$(MAKEINFO)"; export MAKEINFO;

# This is the list of variables to export in the environment when
# configuring subdirectories for the build system.
BUILD_EXPORTS = \
	$(BASE_EXPORTS) \
	AR="$(AR_FOR_BUILD)"; export AR; \
	AS="$(AS_FOR_BUILD)"; export AS; \
	CC="$(CC_FOR_BUILD)"; export CC; \
	CFLAGS="$(CFLAGS_FOR_BUILD)"; export CFLAGS; \
	CONFIG_SHELL="$(SHELL)"; export CONFIG_SHELL; \
	CXX="$(CXX_FOR_BUILD)"; export CXX; \
	CXXFLAGS="$(CXXFLAGS_FOR_BUILD)"; export CXXFLAGS; \
	GCJ="$(GCJ_FOR_BUILD)"; export GCJ; \
	GFORTRAN="$(GFORTRAN_FOR_BUILD)"; export GFORTRAN; \
	GOC="$(GOC_FOR_BUILD)"; export GOC; \
	GOCFLAGS="$(GOCFLAGS_FOR_BUILD)"; export GOCFLAGS; \
	DLLTOOL="$(DLLTOOL_FOR_BUILD)"; export DLLTOOL; \
	LD="$(LD_FOR_BUILD)"; export LD; \
	LDFLAGS="$(LDFLAGS_FOR_BUILD)"; export LDFLAGS; \
	NM="$(NM_FOR_BUILD)"; export NM; \
	RANLIB="$(RANLIB_FOR_BUILD)"; export RANLIB; \
	WINDRES="$(WINDRES_FOR_BUILD)"; export WINDRES; \
	WINDMC="$(WINDMC_FOR_BUILD)"; export WINDMC;

# These variables must be set on the make command line for directories
# built for the build system to override those in BASE_FLAGS_TO_PASSS.
EXTRA_BUILD_FLAGS = \
	CFLAGS="$(CFLAGS_FOR_BUILD)" \
	LDFLAGS="$(LDFLAGS_FOR_BUILD)"

# This is the list of directories to built for the host system.
SUBDIRS =  etc
TARGET_CONFIGDIRS =  libgloss newlib
# This is set by the configure script to the arguments to use when configuring
# directories built for the host system.
HOST_CONFIGARGS =  --cache-file=./config.cache  --with-system-zlib --with-newlib '--disable-multilib' --program-transform-name='s&^&x86_64-pc-baremetal-&' --disable-option-checking
# Host programs are put under this directory, which is . except if building
# with srcdir=..
HOST_SUBDIR = .
# This is the list of variables to export in the environment when
# configuring subdirectories for the host system.  We need to pass
# some to the GCC configure because of its hybrid host/target nature.
HOST_EXPORTS = \
	$(BASE_EXPORTS) \
	CC="$(CC)"; export CC; \
	ADA_CFLAGS="$(ADA_CFLAGS)"; export ADA_CFLAGS; \
	CFLAGS="$(CFLAGS)"; export CFLAGS; \
	CONFIG_SHELL="$(SHELL)"; export CONFIG_SHELL; \
	CXX="$(CXX)"; export CXX; \
	CXXFLAGS="$(CXXFLAGS)"; export CXXFLAGS; \
	GCJ="$(GCJ)"; export GCJ; \
	GFORTRAN="$(GFORTRAN)"; export GFORTRAN; \
	GOC="$(GOC)"; export GOC; \
	AR="$(AR)"; export AR; \
	AS="$(AS)"; export AS; \
	CC_FOR_BUILD="$(CC_FOR_BUILD)"; export CC_FOR_BUILD; \
	DLLTOOL="$(DLLTOOL)"; export DLLTOOL; \
	LD="$(LD)"; export LD; \
	LDFLAGS="$(STAGE1_LDFLAGS) $(LDFLAGS)"; export LDFLAGS; \
	NM="$(NM)"; export NM; \
	RANLIB="$(RANLIB)"; export RANLIB; \
	WINDRES="$(WINDRES)"; export WINDRES; \
	WINDMC="$(WINDMC)"; export WINDMC; \
	OBJCOPY="$(OBJCOPY)"; export OBJCOPY; \
	OBJDUMP="$(OBJDUMP)"; export OBJDUMP; \
	READELF="$(READELF)"; export READELF; \
	AR_FOR_TARGET="$(AR_FOR_TARGET)"; export AR_FOR_TARGET; \
	AS_FOR_TARGET="$(AS_FOR_TARGET)"; export AS_FOR_TARGET; \
	GCC_FOR_TARGET="$(GCC_FOR_TARGET)"; export GCC_FOR_TARGET; \
	LD_FOR_TARGET="$(LD_FOR_TARGET)"; export LD_FOR_TARGET; \
	NM_FOR_TARGET="$(NM_FOR_TARGET)"; export NM_FOR_TARGET; \
	OBJDUMP_FOR_TARGET="$(OBJDUMP_FOR_TARGET)"; export OBJDUMP_FOR_TARGET; \
	RANLIB_FOR_TARGET="$(RANLIB_FOR_TARGET)"; export RANLIB_FOR_TARGET; \
	READELF_FOR_TARGET="$(READELF_FOR_TARGET)"; export READELF_FOR_TARGET; \
	TOPLEVEL_CONFIGURE_ARGUMENTS="$(TOPLEVEL_CONFIGURE_ARGUMENTS)"; export TOPLEVEL_CONFIGURE_ARGUMENTS; \
	HOST_LIBS="$(STAGE1_LIBS)"; export HOST_LIBS; \
	GMPLIBS="$(HOST_GMPLIBS)"; export GMPLIBS; \
	GMPINC="$(HOST_GMPINC)"; export GMPINC; \
	ISLINC="$(HOST_ISLINC)"; export ISLINC; \
	CLOOGLIBS="$(HOST_CLOOGLIBS)"; export CLOOGLIBS; \
	CLOOGINC="$(HOST_CLOOGINC)"; export CLOOGINC; \
	LIBELFLIBS="$(HOST_LIBELFLIBS)" ; export LIBELFLIBS; \
	LIBELFINC="$(HOST_LIBELFINC)" ; export LIBELFINC; \
	$(RPATH_ENVVAR)=`echo "$(HOST_LIB_PATH)$$$(RPATH_ENVVAR)" | sed 's,::*,:,g;s,^:*,,;s,:*$$,,'`; export $(RPATH_ENVVAR);

POSTSTAGE1_CXX_EXPORT = \
	CXX='$(CXX)'; export CXX; \
	CXX_FOR_BUILD='$(CXX_FOR_BUILD)'; export CXX_FOR_BUILD;

# Similar, for later GCC stages.
POSTSTAGE1_HOST_EXPORTS = \
	$(HOST_EXPORTS) \
	CC="$(STAGE_CC_WRAPPER) $$r/$(HOST_SUBDIR)/prev-gcc/xgcc$(exeext) \
	  -B$$r/$(HOST_SUBDIR)/prev-gcc/ -B$(build_tooldir)/bin/ \
	  $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export CC; \
	CC_FOR_BUILD="$$CC"; export CC_FOR_BUILD; \
	$(POSTSTAGE1_CXX_EXPORT) \
	GNATBIND="$$r/$(HOST_SUBDIR)/prev-gcc/gnatbind"; export GNATBIND; \
	LDFLAGS="$(POSTSTAGE1_LDFLAGS) $(BOOT_LDFLAGS)"; export LDFLAGS; \
	HOST_LIBS="$(POSTSTAGE1_LIBS)"; export HOST_LIBS;

# Target libraries are put under this directory:
TARGET_SUBDIR = x86_64-pc-baremetal
# This is set by the configure script to the arguments to use when configuring
# directories built for the target.
TARGET_CONFIGARGS = --cache-file=./config.cache --with-newlib --with-cross-host=x86_64-unknown-linux-gnu   '--disable-multilib' --program-transform-name='s&^&x86_64-pc-baremetal-&' --disable-option-checking --with-target-subdir="$(TARGET_SUBDIR)"
# This is the list of variables to export in the environment when
# configuring subdirectories for the target system.
BASE_TARGET_EXPORTS = \
	$(BASE_EXPORTS) \
	AR="$(AR_FOR_TARGET)"; export AR; \
	AS="$(COMPILER_AS_FOR_TARGET)"; export AS; \
	CC="$(CC_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export CC; \
	CFLAGS="$(CFLAGS_FOR_TARGET)"; export CFLAGS; \
	CONFIG_SHELL="$(SHELL)"; export CONFIG_SHELL; \
	CPPFLAGS="$(CPPFLAGS_FOR_TARGET)"; export CPPFLAGS; \
	CXXFLAGS="$(CXXFLAGS_FOR_TARGET)"; export CXXFLAGS; \
	GCJ="$(GCJ_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export GCJ; \
	GFORTRAN="$(GFORTRAN_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export GFORTRAN; \
	GOC="$(GOC_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export GOC; \
	DLLTOOL="$(DLLTOOL_FOR_TARGET)"; export DLLTOOL; \
	LD="$(COMPILER_LD_FOR_TARGET)"; export LD; \
	LDFLAGS="$(LDFLAGS_FOR_TARGET)"; export LDFLAGS; \
	LIPO="$(LIPO_FOR_TARGET)"; export LIPO; \
	NM="$(COMPILER_NM_FOR_TARGET)"; export NM; \
	OBJDUMP="$(OBJDUMP_FOR_TARGET)"; export OBJDUMP; \
	RANLIB="$(RANLIB_FOR_TARGET)"; export RANLIB; \
	READELF="$(READELF_FOR_TARGET)"; export READELF; \
	STRIP="$(STRIP_FOR_TARGET)"; export STRIP; \
	WINDRES="$(WINDRES_FOR_TARGET)"; export WINDRES; \
	WINDMC="$(WINDMC_FOR_TARGET)"; export WINDMC; \
	$(RPATH_ENVVAR)=`echo "$(HOST_LIB_PATH)$$$(RPATH_ENVVAR)" | sed 's,::*,:,g;s,^:*,,;s,:*$$,,'`; export $(RPATH_ENVVAR); \
	TARGET_CONFIGDIRS="$(TARGET_CONFIGDIRS)"; export TARGET_CONFIGDIRS;

RAW_CXX_TARGET_EXPORTS = \
	$(BASE_TARGET_EXPORTS) \
	CXX_FOR_TARGET="$(RAW_CXX_FOR_TARGET)"; export CXX_FOR_TARGET; \
	CXX="$(RAW_CXX_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export CXX;

NORMAL_TARGET_EXPORTS = \
	$(BASE_TARGET_EXPORTS) \
	CXX="$(CXX_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export CXX;

# Where to find GMP
HOST_GMPLIBS = -lmpc -lmpfr -lgmp
HOST_GMPINC = 

# Where to find ISL
HOST_ISLINC = 

# Where to find CLOOG
HOST_CLOOGLIBS = 
HOST_CLOOGINC = 

# Where to find libelf
HOST_LIBELFLIBS = @libelflibs@
HOST_LIBELFINC = @libelfinc@

EXTRA_CONFIGARGS_LIBJAVA = --disable-static

# ----------------------------------------------
# Programs producing files for the BUILD machine
# ----------------------------------------------

SHELL = /bin/bash

# pwd command to use.  Allow user to override default by setting PWDCMD in
# the environment to account for automounters.  The make variable must not
# be called PWDCMD, otherwise the value set here is passed to make
# subprocesses and overrides the setting from the user's environment.
# Don't use PWD since it is a common shell environment variable and we
# don't want to corrupt it.
PWD_COMMAND = $${PWDCMD-pwd}

# compilers to use to create programs which must be run in the build
# environment.
AR_FOR_BUILD = $(AR)
AS_FOR_BUILD = $(AS)
CC_FOR_BUILD = $(CC)
CFLAGS_FOR_BUILD = -g -O2
CXXFLAGS_FOR_BUILD = -g -O2
CXX_FOR_BUILD = $(CXX)
DLLTOOL_FOR_BUILD = $(DLLTOOL)
GCJ_FOR_BUILD = $(GCJ)
GFORTRAN_FOR_BUILD = $(GFORTRAN)
GOC_FOR_BUILD = $(GOC)
LDFLAGS_FOR_BUILD = 
LD_FOR_BUILD = $(LD)
NM_FOR_BUILD = $(NM)
RANLIB_FOR_BUILD = $(RANLIB)
WINDMC_FOR_BUILD = $(WINDMC)
WINDRES_FOR_BUILD = $(WINDRES)

# Special variables passed down in EXTRA_GCC_FLAGS.  They are defined
# here so that they can be overridden by Makefile fragments.
BUILD_PREFIX = @BUILD_PREFIX@
BUILD_PREFIX_1 = @BUILD_PREFIX_1@

# Flags to pass to stage2 and later makes.  They are defined
# here so that they can be overridden by Makefile fragments.
BOOT_CFLAGS= -g -O2
BOOT_LDFLAGS=
BOOT_ADAFLAGS= -gnatpg

AWK = gawk
SED = /bin/sed
BISON = /home/mntmn/code/BareMetal/newlib/newlib-2.2.0/missing bison
YACC = /home/mntmn/code/BareMetal/newlib/newlib-2.2.0/missing bison -y
FLEX = /home/mntmn/code/BareMetal/newlib/newlib-2.2.0/missing flex
LEX = /home/mntmn/code/BareMetal/newlib/newlib-2.2.0/missing flex
M4 = m4
MAKEINFO = makeinfo
EXPECT = expect
RUNTEST = runtest

# This just becomes part of the MAKEINFO definition passed down to
# sub-makes.  It lets flags be given on the command line while still
# using the makeinfo from the object tree.
# (Default to avoid splitting info files by setting the threshold high.)
MAKEINFOFLAGS = --split-size=5000000

# ---------------------------------------------
# Programs producing files for the HOST machine
# ---------------------------------------------

AS = as
AR = ar
AR_FLAGS = rc
CC = gcc
CXX = g++
DLLTOOL = dlltool
LD = ld
LIPO = lipo
NM = nm
OBJDUMP = objdump
RANLIB = ranlib
READELF = readelf
STRIP = strip
WINDRES = windres
WINDMC = windmc

GNATBIND = no
GNATMAKE = no

CFLAGS = -g -O2
LDFLAGS = 
LIBCFLAGS = $(CFLAGS)
CXXFLAGS = -g -O2
LIBCXXFLAGS = $(CXXFLAGS) -fno-implicit-templates
GOCFLAGS = $(CFLAGS)

TFLAGS =

# Defaults for all stages; some are overridden below.

STAGE_CFLAGS = $(BOOT_CFLAGS)
STAGE_TFLAGS = $(TFLAGS)
STAGE_CONFIGURE_FLAGS=


# Defaults for stage 1; some are overridden below.
STAGE1_CFLAGS = $(STAGE_CFLAGS)
STAGE1_CXXFLAGS = $(CXXFLAGS)
STAGE1_TFLAGS = $(STAGE_TFLAGS)
STAGE1_CONFIGURE_FLAGS = $(STAGE_CONFIGURE_FLAGS)

# Defaults for stage 2; some are overridden below.
STAGE2_CFLAGS = $(STAGE_CFLAGS)
STAGE2_CXXFLAGS = $(CXXFLAGS)
STAGE2_TFLAGS = $(STAGE_TFLAGS)
STAGE2_CONFIGURE_FLAGS = $(STAGE_CONFIGURE_FLAGS)

# Defaults for stage 3; some are overridden below.
STAGE3_CFLAGS = $(STAGE_CFLAGS)
STAGE3_CXXFLAGS = $(CXXFLAGS)
STAGE3_TFLAGS = $(STAGE_TFLAGS)
STAGE3_CONFIGURE_FLAGS = $(STAGE_CONFIGURE_FLAGS)

# Defaults for stage 4; some are overridden below.
STAGE4_CFLAGS = $(STAGE_CFLAGS)
STAGE4_CXXFLAGS = $(CXXFLAGS)
STAGE4_TFLAGS = $(STAGE_TFLAGS)
STAGE4_CONFIGURE_FLAGS = $(STAGE_CONFIGURE_FLAGS)

# Defaults for stage profile; some are overridden below.
STAGEprofile_CFLAGS = $(STAGE_CFLAGS)
STAGEprofile_CXXFLAGS = $(CXXFLAGS)
STAGEprofile_TFLAGS = $(STAGE_TFLAGS)
STAGEprofile_CONFIGURE_FLAGS = $(STAGE_CONFIGURE_FLAGS)

# Defaults for stage feedback; some are overridden below.
STAGEfeedback_CFLAGS = $(STAGE_CFLAGS)
STAGEfeedback_CXXFLAGS = $(CXXFLAGS)
STAGEfeedback_TFLAGS = $(STAGE_TFLAGS)
STAGEfeedback_CONFIGURE_FLAGS = $(STAGE_CONFIGURE_FLAGS)


# Only build the C compiler for stage1, because that is the only one that
# we can guarantee will build with the native compiler, and also it is the
# only thing useful for building stage2. STAGE1_CFLAGS (via CFLAGS),
# MAKEINFO and MAKEINFOFLAGS are explicitly passed here to make them
# overrideable (for a bootstrap build stage1 also builds gcc.info).

STAGE1_CFLAGS = -g
STAGE1_CHECKING = --enable-checking=yes,types
STAGE1_LANGUAGES = ,c,
# * We force-disable intermodule optimizations, even if
#   --enable-intermodule was passed, since the installed compiler
#   probably can't handle them.  Luckily, autoconf always respects
#   the last argument when conflicting --enable arguments are passed.
# * Likewise, we force-disable coverage flags, since the installed
#   compiler probably has never heard of them.
STAGE1_CONFIGURE_FLAGS = --disable-intermodule $(STAGE1_CHECKING) \
	  --disable-coverage --enable-languages="$(STAGE1_LANGUAGES)"

STAGEprofile_CFLAGS = $(STAGE2_CFLAGS) -fprofile-generate
STAGEprofile_TFLAGS = $(STAGE2_TFLAGS)

STAGEfeedback_CFLAGS = $(STAGE3_CFLAGS) -fprofile-use
STAGEfeedback_TFLAGS = $(STAGE3_TFLAGS)

do-compare = cmp --ignore-initial=16 $$f1 $$f2
do-compare3 = $(do-compare)

# -----------------------------------------------
# Programs producing files for the TARGET machine
# -----------------------------------------------

AR_FOR_TARGET=ar
AS_FOR_TARGET=as
CC_FOR_TARGET=$(STAGE_CC_WRAPPER) cc

# If GCC_FOR_TARGET is not overriden on the command line, then this
# variable is passed down to the gcc Makefile, where it is used to
# build libgcc2.a.  We define it here so that it can itself be
# overridden on the command line.
GCC_FOR_TARGET=$(STAGE_CC_WRAPPER) gcc
CXX_FOR_TARGET=$(STAGE_CC_WRAPPER) c++
RAW_CXX_FOR_TARGET=$(STAGE_CC_WRAPPER) c++
GCJ_FOR_TARGET=$(STAGE_CC_WRAPPER) gcj
GFORTRAN_FOR_TARGET=$(STAGE_CC_WRAPPER) gfortran
GOC_FOR_TARGET=$(STAGE_CC_WRAPPER) gccgo
DLLTOOL_FOR_TARGET=dlltool
LD_FOR_TARGET=ld

LIPO_FOR_TARGET=lipo
NM_FOR_TARGET=nm
OBJDUMP_FOR_TARGET=objdump
RANLIB_FOR_TARGET=ranlib
READELF_FOR_TARGET=readelf
STRIP_FOR_TARGET=strip
WINDRES_FOR_TARGET=windres
WINDMC_FOR_TARGET=windmc

COMPILER_AS_FOR_TARGET=$(AS_FOR_TARGET)
COMPILER_LD_FOR_TARGET=$(LD_FOR_TARGET)
COMPILER_NM_FOR_TARGET=$(NM_FOR_TARGET)

CFLAGS_FOR_TARGET = -g -O2
CXXFLAGS_FOR_TARGET = -g -O2

LIBCFLAGS_FOR_TARGET = $(CFLAGS_FOR_TARGET)
LIBCXXFLAGS_FOR_TARGET = $(CXXFLAGS_FOR_TARGET) -fno-implicit-templates
LDFLAGS_FOR_TARGET = 
GOCFLAGS_FOR_TARGET = -O2 -g

FLAGS_FOR_TARGET =  -B$$r/$(TARGET_SUBDIR)/newlib/ -isystem $$r/$(TARGET_SUBDIR)/newlib/targ-include -isystem $$s/newlib/libc/include -B$$r/$(TARGET_SUBDIR)/libgloss/x86_64 -L$$r/$(TARGET_SUBDIR)/libgloss/libnosys -L$$s/libgloss/x86_64
SYSROOT_CFLAGS_FOR_TARGET = 
DEBUG_PREFIX_CFLAGS_FOR_TARGET = 

XGCC_FLAGS_FOR_TARGET = $(FLAGS_FOR_TARGET) $(SYSROOT_CFLAGS_FOR_TARGET) $(DEBUG_PREFIX_CFLAGS_FOR_TARGET)

# ------------------------------------
# Miscellaneous targets and flag lists
# ------------------------------------

# The first rule in the file had better be this one.  Don't put any above it.
# This lives here to allow makefile fragments to contain dependencies.
all:

#### host and target specific makefile fragments come in here.
###

# This is the list of directories that may be needed in RPATH_ENVVAR
# so that programs built for the target machine work.
TARGET_LIB_PATH = $(TARGET_LIB_PATH_libstdc++-v3)$(TARGET_LIB_PATH_libmudflap)$(TARGET_LIB_PATH_libsanitizer)$(TARGET_LIB_PATH_libvtv)$(TARGET_LIB_PATH_libssp)$(TARGET_LIB_PATH_libgomp)$(TARGET_LIB_PATH_libitm)$(TARGET_LIB_PATH_libatomic)$(HOST_LIB_PATH_gcc)











# This is the list of directories that may be needed in RPATH_ENVVAR
# so that programs built for the host machine work.
HOST_LIB_PATH = $(HOST_LIB_PATH_gmp)$(HOST_LIB_PATH_mpfr)$(HOST_LIB_PATH_mpc)$(HOST_LIB_PATH_isl)$(HOST_LIB_PATH_cloog)$(HOST_LIB_PATH_libelf)

# Define HOST_LIB_PATH_gcc here, for the sake of TARGET_LIB_PATH, ouch









CXX_FOR_TARGET_FLAG_TO_PASS = \
	"CXX_FOR_TARGET=$(CXX_FOR_TARGET)"

# Flags to pass down to all sub-makes.
BASE_FLAGS_TO_PASS = \
	"DESTDIR=$(DESTDIR)" \
	"RPATH_ENVVAR=$(RPATH_ENVVAR)" \
	"TARGET_SUBDIR=$(TARGET_SUBDIR)" \
	"bindir=$(bindir)" \
	"datadir=$(datadir)" \
	"exec_prefix=$(exec_prefix)" \
	"includedir=$(includedir)" \
	"datarootdir=$(datarootdir)" \
	"docdir=$(docdir)" \
	"infodir=$(infodir)" \
	"pdfdir=$(pdfdir)" \
	"htmldir=$(htmldir)" \
	"libdir=$(libdir)" \
	"libexecdir=$(libexecdir)" \
	"lispdir=$(lispdir)" \
	"localstatedir=$(localstatedir)" \
	"mandir=$(mandir)" \
	"oldincludedir=$(oldincludedir)" \
	"prefix=$(prefix)" \
	"sbindir=$(sbindir)" \
	"sharedstatedir=$(sharedstatedir)" \
	"sysconfdir=$(sysconfdir)" \
	"tooldir=$(tooldir)" \
	"build_tooldir=$(build_tooldir)" \
	"target_alias=$(target_alias)" \
	"AWK=$(AWK)" \
	"BISON=$(BISON)" \
	"CC_FOR_BUILD=$(CC_FOR_BUILD)" \
	"CFLAGS_FOR_BUILD=$(CFLAGS_FOR_BUILD)" \
	"CXX_FOR_BUILD=$(CXX_FOR_BUILD)" \
	"EXPECT=$(EXPECT)" \
	"FLEX=$(FLEX)" \
	"INSTALL=$(INSTALL)" \
	"INSTALL_DATA=$(INSTALL_DATA)" \
	"INSTALL_PROGRAM=$(INSTALL_PROGRAM)" \
	"INSTALL_SCRIPT=$(INSTALL_SCRIPT)" \
	"LDFLAGS_FOR_BUILD=$(LDFLAGS_FOR_BUILD)" \
	"LEX=$(LEX)" \
	"M4=$(M4)" \
	"MAKE=$(MAKE)" \
	"RUNTEST=$(RUNTEST)" \
	"RUNTESTFLAGS=$(RUNTESTFLAGS)" \
	"SED=$(SED)" \
	"SHELL=$(SHELL)" \
	"YACC=$(YACC)" \
	"`echo 'ADAFLAGS=$(ADAFLAGS)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"ADA_CFLAGS=$(ADA_CFLAGS)" \
	"AR_FLAGS=$(AR_FLAGS)" \
	"`echo 'BOOT_ADAFLAGS=$(BOOT_ADAFLAGS)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"BOOT_CFLAGS=$(BOOT_CFLAGS)" \
	"BOOT_LDFLAGS=$(BOOT_LDFLAGS)" \
	"CFLAGS=$(CFLAGS)" \
	"CXXFLAGS=$(CXXFLAGS)" \
	"LDFLAGS=$(LDFLAGS)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"LIBCXXFLAGS=$(LIBCXXFLAGS)" \
	"STAGE1_CHECKING=$(STAGE1_CHECKING)" \
	"STAGE1_LANGUAGES=$(STAGE1_LANGUAGES)" \
	"GNATBIND=$(GNATBIND)" \
	"GNATMAKE=$(GNATMAKE)" \
	"AR_FOR_TARGET=$(AR_FOR_TARGET)" \
	"AS_FOR_TARGET=$(AS_FOR_TARGET)" \
	"CC_FOR_TARGET=$(CC_FOR_TARGET)" \
	"CFLAGS_FOR_TARGET=$(CFLAGS_FOR_TARGET)" \
	"CPPFLAGS_FOR_TARGET=$(CPPFLAGS_FOR_TARGET)" \
	"CXXFLAGS_FOR_TARGET=$(CXXFLAGS_FOR_TARGET)" \
	"DLLTOOL_FOR_TARGET=$(DLLTOOL_FOR_TARGET)" \
	"FLAGS_FOR_TARGET=$(FLAGS_FOR_TARGET)" \
	"GCJ_FOR_TARGET=$(GCJ_FOR_TARGET)" \
	"GFORTRAN_FOR_TARGET=$(GFORTRAN_FOR_TARGET)" \
	"GOC_FOR_TARGET=$(GOC_FOR_TARGET)" \
	"GOCFLAGS_FOR_TARGET=$(GOCFLAGS_FOR_TARGET)" \
	"LD_FOR_TARGET=$(LD_FOR_TARGET)" \
	"LIPO_FOR_TARGET=$(LIPO_FOR_TARGET)" \
	"LDFLAGS_FOR_TARGET=$(LDFLAGS_FOR_TARGET)" \
	"LIBCFLAGS_FOR_TARGET=$(LIBCFLAGS_FOR_TARGET)" \
	"LIBCXXFLAGS_FOR_TARGET=$(LIBCXXFLAGS_FOR_TARGET)" \
	"NM_FOR_TARGET=$(NM_FOR_TARGET)" \
	"OBJDUMP_FOR_TARGET=$(OBJDUMP_FOR_TARGET)" \
	"RANLIB_FOR_TARGET=$(RANLIB_FOR_TARGET)" \
	"READELF_FOR_TARGET=$(READELF_FOR_TARGET)" \
	"STRIP_FOR_TARGET=$(STRIP_FOR_TARGET)" \
	"WINDRES_FOR_TARGET=$(WINDRES_FOR_TARGET)" \
	"WINDMC_FOR_TARGET=$(WINDMC_FOR_TARGET)" \
	"BUILD_CONFIG=$(BUILD_CONFIG)" \
	"`echo 'LANGUAGES=$(LANGUAGES)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"LEAN=$(LEAN)" \
	"STAGE1_CFLAGS=$(STAGE1_CFLAGS)" \
	"STAGE1_CXXFLAGS=$(STAGE1_CXXFLAGS)" \
	"STAGE1_TFLAGS=$(STAGE1_TFLAGS)" \
	"STAGE2_CFLAGS=$(STAGE2_CFLAGS)" \
	"STAGE2_CXXFLAGS=$(STAGE2_CXXFLAGS)" \
	"STAGE2_TFLAGS=$(STAGE2_TFLAGS)" \
	"STAGE3_CFLAGS=$(STAGE3_CFLAGS)" \
	"STAGE3_CXXFLAGS=$(STAGE3_CXXFLAGS)" \
	"STAGE3_TFLAGS=$(STAGE3_TFLAGS)" \
	"STAGE4_CFLAGS=$(STAGE4_CFLAGS)" \
	"STAGE4_CXXFLAGS=$(STAGE4_CXXFLAGS)" \
	"STAGE4_TFLAGS=$(STAGE4_TFLAGS)" \
	"STAGEprofile_CFLAGS=$(STAGEprofile_CFLAGS)" \
	"STAGEprofile_CXXFLAGS=$(STAGEprofile_CXXFLAGS)" \
	"STAGEprofile_TFLAGS=$(STAGEprofile_TFLAGS)" \
	"STAGEfeedback_CFLAGS=$(STAGEfeedback_CFLAGS)" \
	"STAGEfeedback_CXXFLAGS=$(STAGEfeedback_CXXFLAGS)" \
	"STAGEfeedback_TFLAGS=$(STAGEfeedback_TFLAGS)" \
	$(CXX_FOR_TARGET_FLAG_TO_PASS) \
	"TFLAGS=$(TFLAGS)" \
	"CONFIG_SHELL=$(SHELL)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)" 

# We leave this in just in case, but it is not needed anymore.
RECURSE_FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS)

# Flags to pass down to most sub-makes, in which we're building with
# the host environment.
EXTRA_HOST_FLAGS = \
	'AR=$(AR)' \
	'AS=$(AS)' \
	'CC=$(CC)' \
	'CXX=$(CXX)' \
	'DLLTOOL=$(DLLTOOL)' \
	'GCJ=$(GCJ)' \
	'GFORTRAN=$(GFORTRAN)' \
	'GOC=$(GOC)' \
	'LD=$(LD)' \
	'LIPO=$(LIPO)' \
	'NM=$(NM)' \
	'OBJDUMP=$(OBJDUMP)' \
	'RANLIB=$(RANLIB)' \
	'READELF=$(READELF)' \
	'STRIP=$(STRIP)' \
	'WINDRES=$(WINDRES)' \
	'WINDMC=$(WINDMC)'

FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS) $(EXTRA_HOST_FLAGS)

# Flags to pass to stage1 or when not bootstrapping.

STAGE1_FLAGS_TO_PASS = \
	LDFLAGS="$${LDFLAGS}" \
	HOST_LIBS="$${HOST_LIBS}"

# Flags to pass to stage2 and later makes.

POSTSTAGE1_FLAGS_TO_PASS = \
	CC="$${CC}" CC_FOR_BUILD="$${CC_FOR_BUILD}" \
	CXX="$${CXX}" CXX_FOR_BUILD="$${CXX_FOR_BUILD}" \
	GNATBIND="$${GNATBIND}" \
	LDFLAGS="$${LDFLAGS}" \
	HOST_LIBS="$${HOST_LIBS}" \
	"`echo 'ADAFLAGS=$(BOOT_ADAFLAGS)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`"

# Flags to pass down to makes which are built with the target environment.
# The double $ decreases the length of the command line; those variables
# are set in BASE_FLAGS_TO_PASS, and the sub-make will expand them.  The
# *_CFLAGS_FOR_TARGET variables are not passed down and most often empty,
# so we expand them here.
EXTRA_TARGET_FLAGS = \
	'AR=$$(AR_FOR_TARGET)' \
	'AS=$(COMPILER_AS_FOR_TARGET)' \
	'CC=$$(CC_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'CFLAGS=$$(CFLAGS_FOR_TARGET)' \
	'CXX=$$(CXX_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'CXXFLAGS=$$(CXXFLAGS_FOR_TARGET)' \
	'DLLTOOL=$$(DLLTOOL_FOR_TARGET)' \
	'GCJ=$$(GCJ_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'GFORTRAN=$$(GFORTRAN_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'GOC=$$(GOC_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'GOCFLAGS=$$(GOCFLAGS_FOR_TARGET)' \
	'LD=$(COMPILER_LD_FOR_TARGET)' \
	'LDFLAGS=$$(LDFLAGS_FOR_TARGET)' \
	'LIBCFLAGS=$$(LIBCFLAGS_FOR_TARGET)' \
	'LIBCXXFLAGS=$$(LIBCXXFLAGS_FOR_TARGET)' \
	'NM=$(COMPILER_NM_FOR_TARGET)' \
	'OBJDUMP=$$(OBJDUMP_FOR_TARGET)' \
	'RANLIB=$$(RANLIB_FOR_TARGET)' \
	'READELF=$$(READELF_FOR_TARGET)' \
	'WINDRES=$$(WINDRES_FOR_TARGET)' \
	'WINDMC=$$(WINDMC_FOR_TARGET)' \
	'XGCC_FLAGS_FOR_TARGET=$(XGCC_FLAGS_FOR_TARGET)' \
	"TFLAGS=$$TFLAGS"

TARGET_FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS) $(EXTRA_TARGET_FLAGS)

# Flags to pass down to gcc.  gcc builds a library, libgcc.a, so it
# unfortunately needs the native compiler and the target ar and
# ranlib.
# If any variables are added here, they must be added to do-*, below.
# The BUILD_* variables are a special case, which are used for the gcc
# cross-building scheme.
EXTRA_GCC_FLAGS = \
	"GCC_FOR_TARGET=$(GCC_FOR_TARGET)" \
	"`echo 'STMP_FIXPROTO=$(STMP_FIXPROTO)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'LIMITS_H_TEST=$(LIMITS_H_TEST)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`"

GCC_FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS) $(EXTRA_HOST_FLAGS) $(EXTRA_GCC_FLAGS)


.PHONY: configure-host
configure-host:  \
    maybe-configure-bfd \
    maybe-configure-opcodes \
    maybe-configure-binutils \
    maybe-configure-bison \
    maybe-configure-cgen \
    maybe-configure-dejagnu \
    maybe-configure-etc \
    maybe-configure-fastjar \
    maybe-configure-fixincludes \
    maybe-configure-flex \
    maybe-configure-gas \
    maybe-configure-gcc \
    maybe-configure-gmp \
    maybe-configure-mpfr \
    maybe-configure-mpc \
    maybe-configure-isl \
    maybe-configure-cloog \
    maybe-configure-libelf \
    maybe-configure-gold \
    maybe-configure-gprof \
    maybe-configure-intl \
    maybe-configure-tcl \
    maybe-configure-itcl \
    maybe-configure-ld \
    maybe-configure-libbacktrace \
    maybe-configure-libcpp \
    maybe-configure-libdecnumber \
    maybe-configure-libgui \
    maybe-configure-libiberty \
    maybe-configure-libiconv \
    maybe-configure-m4 \
    maybe-configure-readline \
    maybe-configure-sid \
    maybe-configure-sim \
    maybe-configure-texinfo \
    maybe-configure-zlib \
    maybe-configure-gdb \
    maybe-configure-expect \
    maybe-configure-guile \
    maybe-configure-tk \
    maybe-configure-libtermcap \
    maybe-configure-utils \
    maybe-configure-gnattools \
    maybe-configure-lto-plugin
.PHONY: configure-target
configure-target:  \
    maybe-configure-target-libstdc++-v3 \
    maybe-configure-target-libmudflap \
    maybe-configure-target-libsanitizer \
    maybe-configure-target-libvtv \
    maybe-configure-target-libssp \
    maybe-configure-target-newlib \
    maybe-configure-target-libgcc \
    maybe-configure-target-libbacktrace \
    maybe-configure-target-libquadmath \
    maybe-configure-target-libgfortran \
    maybe-configure-target-libobjc \
    maybe-configure-target-libgo \
    maybe-configure-target-libtermcap \
    maybe-configure-target-winsup \
    maybe-configure-target-libgloss \
    maybe-configure-target-libffi \
    maybe-configure-target-libjava \
    maybe-configure-target-zlib \
    maybe-configure-target-boehm-gc \
    maybe-configure-target-rda \
    maybe-configure-target-libada \
    maybe-configure-target-libgomp \
    maybe-configure-target-libitm \
    maybe-configure-target-libatomic

# The target built for a native non-bootstrap build.
.PHONY: all
all:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-host all-target \
	&& :

.PHONY: all-build

all-build: maybe-all-build-libiberty
all-build: maybe-all-build-bison
all-build: maybe-all-build-flex
all-build: maybe-all-build-m4
all-build: maybe-all-build-texinfo
all-build: maybe-all-build-fixincludes

.PHONY: all-host

all-host: maybe-all-bison
all-host: maybe-all-cgen
all-host: maybe-all-dejagnu
all-host: maybe-all-etc
all-host: maybe-all-fastjar
all-host: maybe-all-fixincludes
all-host: maybe-all-flex
all-host: maybe-all-gprof
all-host: maybe-all-tcl
all-host: maybe-all-itcl
all-host: maybe-all-libgui
all-host: maybe-all-libiconv
all-host: maybe-all-m4
all-host: maybe-all-readline
all-host: maybe-all-sid
all-host: maybe-all-sim
all-host: maybe-all-texinfo
all-host: maybe-all-gdb
all-host: maybe-all-expect
all-host: maybe-all-guile
all-host: maybe-all-tk
all-host: maybe-all-libtermcap
all-host: maybe-all-utils
all-host: maybe-all-gnattools

.PHONY: all-target

all-target: maybe-all-target-libmudflap
all-target: maybe-all-target-libssp
all-target: maybe-all-target-newlib
all-target: maybe-all-target-libbacktrace
all-target: maybe-all-target-libquadmath
all-target: maybe-all-target-libgfortran
all-target: maybe-all-target-libobjc
all-target: maybe-all-target-libgo
all-target: maybe-all-target-libtermcap
all-target: maybe-all-target-winsup
all-target: maybe-all-target-libgloss
all-target: maybe-all-target-libffi
all-target: maybe-all-target-libjava
all-target: maybe-all-target-zlib
all-target: maybe-all-target-boehm-gc
all-target: maybe-all-target-rda
all-target: maybe-all-target-libada
all-target: maybe-all-target-libitm
all-target: maybe-all-target-libatomic

# Do a target for all the subdirectories.  A ``make do-X'' will do a
# ``make X'' in all subdirectories (because, in general, there is a
# dependency (below) of X upon do-X, a ``make X'' will also do this,
# but it may do additional work as well).

.PHONY: do-info
do-info:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) info-host \
	  info-target


.PHONY: info-host

info-host: maybe-info-bfd
info-host: maybe-info-opcodes
info-host: maybe-info-binutils
info-host: maybe-info-bison
info-host: maybe-info-cgen
info-host: maybe-info-dejagnu
info-host: maybe-info-etc
info-host: maybe-info-fastjar
info-host: maybe-info-fixincludes
info-host: maybe-info-flex
info-host: maybe-info-gas
info-host: maybe-info-gcc
info-host: maybe-info-gmp
info-host: maybe-info-mpfr
info-host: maybe-info-mpc
info-host: maybe-info-isl
info-host: maybe-info-cloog
info-host: maybe-info-libelf
info-host: maybe-info-gold
info-host: maybe-info-gprof
info-host: maybe-info-intl
info-host: maybe-info-tcl
info-host: maybe-info-itcl
info-host: maybe-info-ld
info-host: maybe-info-libbacktrace
info-host: maybe-info-libcpp
info-host: maybe-info-libdecnumber
info-host: maybe-info-libgui
info-host: maybe-info-libiberty
info-host: maybe-info-libiconv
info-host: maybe-info-m4
info-host: maybe-info-readline
info-host: maybe-info-sid
info-host: maybe-info-sim
info-host: maybe-info-texinfo
info-host: maybe-info-zlib
info-host: maybe-info-gdb
info-host: maybe-info-expect
info-host: maybe-info-guile
info-host: maybe-info-tk
info-host: maybe-info-libtermcap
info-host: maybe-info-utils
info-host: maybe-info-gnattools
info-host: maybe-info-lto-plugin

.PHONY: info-target

info-target: maybe-info-target-libstdc++-v3
info-target: maybe-info-target-libmudflap
info-target: maybe-info-target-libsanitizer
info-target: maybe-info-target-libvtv
info-target: maybe-info-target-libssp
info-target: maybe-info-target-newlib
info-target: maybe-info-target-libgcc
info-target: maybe-info-target-libbacktrace
info-target: maybe-info-target-libquadmath
info-target: maybe-info-target-libgfortran
info-target: maybe-info-target-libobjc
info-target: maybe-info-target-libgo
info-target: maybe-info-target-libtermcap
info-target: maybe-info-target-winsup
info-target: maybe-info-target-libgloss
info-target: maybe-info-target-libffi
info-target: maybe-info-target-libjava
info-target: maybe-info-target-zlib
info-target: maybe-info-target-boehm-gc
info-target: maybe-info-target-rda
info-target: maybe-info-target-libada
info-target: maybe-info-target-libgomp
info-target: maybe-info-target-libitm
info-target: maybe-info-target-libatomic

.PHONY: do-dvi
do-dvi:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) dvi-host \
	  dvi-target


.PHONY: dvi-host

dvi-host: maybe-dvi-bfd
dvi-host: maybe-dvi-opcodes
dvi-host: maybe-dvi-binutils
dvi-host: maybe-dvi-bison
dvi-host: maybe-dvi-cgen
dvi-host: maybe-dvi-dejagnu
dvi-host: maybe-dvi-etc
dvi-host: maybe-dvi-fastjar
dvi-host: maybe-dvi-fixincludes
dvi-host: maybe-dvi-flex
dvi-host: maybe-dvi-gas
dvi-host: maybe-dvi-gcc
dvi-host: maybe-dvi-gmp
dvi-host: maybe-dvi-mpfr
dvi-host: maybe-dvi-mpc
dvi-host: maybe-dvi-isl
dvi-host: maybe-dvi-cloog
dvi-host: maybe-dvi-libelf
dvi-host: maybe-dvi-gold
dvi-host: maybe-dvi-gprof
dvi-host: maybe-dvi-intl
dvi-host: maybe-dvi-tcl
dvi-host: maybe-dvi-itcl
dvi-host: maybe-dvi-ld
dvi-host: maybe-dvi-libbacktrace
dvi-host: maybe-dvi-libcpp
dvi-host: maybe-dvi-libdecnumber
dvi-host: maybe-dvi-libgui
dvi-host: maybe-dvi-libiberty
dvi-host: maybe-dvi-libiconv
dvi-host: maybe-dvi-m4
dvi-host: maybe-dvi-readline
dvi-host: maybe-dvi-sid
dvi-host: maybe-dvi-sim
dvi-host: maybe-dvi-texinfo
dvi-host: maybe-dvi-zlib
dvi-host: maybe-dvi-gdb
dvi-host: maybe-dvi-expect
dvi-host: maybe-dvi-guile
dvi-host: maybe-dvi-tk
dvi-host: maybe-dvi-libtermcap
dvi-host: maybe-dvi-utils
dvi-host: maybe-dvi-gnattools
dvi-host: maybe-dvi-lto-plugin

.PHONY: dvi-target

dvi-target: maybe-dvi-target-libstdc++-v3
dvi-target: maybe-dvi-target-libmudflap
dvi-target: maybe-dvi-target-libsanitizer
dvi-target: maybe-dvi-target-libvtv
dvi-target: maybe-dvi-target-libssp
dvi-target: maybe-dvi-target-newlib
dvi-target: maybe-dvi-target-libgcc
dvi-target: maybe-dvi-target-libbacktrace
dvi-target: maybe-dvi-target-libquadmath
dvi-target: maybe-dvi-target-libgfortran
dvi-target: maybe-dvi-target-libobjc
dvi-target: maybe-dvi-target-libgo
dvi-target: maybe-dvi-target-libtermcap
dvi-target: maybe-dvi-target-winsup
dvi-target: maybe-dvi-target-libgloss
dvi-target: maybe-dvi-target-libffi
dvi-target: maybe-dvi-target-libjava
dvi-target: maybe-dvi-target-zlib
dvi-target: maybe-dvi-target-boehm-gc
dvi-target: maybe-dvi-target-rda
dvi-target: maybe-dvi-target-libada
dvi-target: maybe-dvi-target-libgomp
dvi-target: maybe-dvi-target-libitm
dvi-target: maybe-dvi-target-libatomic

.PHONY: do-pdf
do-pdf:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) pdf-host \
	  pdf-target


.PHONY: pdf-host

pdf-host: maybe-pdf-bfd
pdf-host: maybe-pdf-opcodes
pdf-host: maybe-pdf-binutils
pdf-host: maybe-pdf-bison
pdf-host: maybe-pdf-cgen
pdf-host: maybe-pdf-dejagnu
pdf-host: maybe-pdf-etc
pdf-host: maybe-pdf-fastjar
pdf-host: maybe-pdf-fixincludes
pdf-host: maybe-pdf-flex
pdf-host: maybe-pdf-gas
pdf-host: maybe-pdf-gcc
pdf-host: maybe-pdf-gmp
pdf-host: maybe-pdf-mpfr
pdf-host: maybe-pdf-mpc
pdf-host: maybe-pdf-isl
pdf-host: maybe-pdf-cloog
pdf-host: maybe-pdf-libelf
pdf-host: maybe-pdf-gold
pdf-host: maybe-pdf-gprof
pdf-host: maybe-pdf-intl
pdf-host: maybe-pdf-tcl
pdf-host: maybe-pdf-itcl
pdf-host: maybe-pdf-ld
pdf-host: maybe-pdf-libbacktrace
pdf-host: maybe-pdf-libcpp
pdf-host: maybe-pdf-libdecnumber
pdf-host: maybe-pdf-libgui
pdf-host: maybe-pdf-libiberty
pdf-host: maybe-pdf-libiconv
pdf-host: maybe-pdf-m4
pdf-host: maybe-pdf-readline
pdf-host: maybe-pdf-sid
pdf-host: maybe-pdf-sim
pdf-host: maybe-pdf-texinfo
pdf-host: maybe-pdf-zlib
pdf-host: maybe-pdf-gdb
pdf-host: maybe-pdf-expect
pdf-host: maybe-pdf-guile
pdf-host: maybe-pdf-tk
pdf-host: maybe-pdf-libtermcap
pdf-host: maybe-pdf-utils
pdf-host: maybe-pdf-gnattools
pdf-host: maybe-pdf-lto-plugin

.PHONY: pdf-target

pdf-target: maybe-pdf-target-libstdc++-v3
pdf-target: maybe-pdf-target-libmudflap
pdf-target: maybe-pdf-target-libsanitizer
pdf-target: maybe-pdf-target-libvtv
pdf-target: maybe-pdf-target-libssp
pdf-target: maybe-pdf-target-newlib
pdf-target: maybe-pdf-target-libgcc
pdf-target: maybe-pdf-target-libbacktrace
pdf-target: maybe-pdf-target-libquadmath
pdf-target: maybe-pdf-target-libgfortran
pdf-target: maybe-pdf-target-libobjc
pdf-target: maybe-pdf-target-libgo
pdf-target: maybe-pdf-target-libtermcap
pdf-target: maybe-pdf-target-winsup
pdf-target: maybe-pdf-target-libgloss
pdf-target: maybe-pdf-target-libffi
pdf-target: maybe-pdf-target-libjava
pdf-target: maybe-pdf-target-zlib
pdf-target: maybe-pdf-target-boehm-gc
pdf-target: maybe-pdf-target-rda
pdf-target: maybe-pdf-target-libada
pdf-target: maybe-pdf-target-libgomp
pdf-target: maybe-pdf-target-libitm
pdf-target: maybe-pdf-target-libatomic

.PHONY: do-html
do-html:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) html-host \
	  html-target


.PHONY: html-host

html-host: maybe-html-bfd
html-host: maybe-html-opcodes
html-host: maybe-html-binutils
html-host: maybe-html-bison
html-host: maybe-html-cgen
html-host: maybe-html-dejagnu
html-host: maybe-html-etc
html-host: maybe-html-fastjar
html-host: maybe-html-fixincludes
html-host: maybe-html-flex
html-host: maybe-html-gas
html-host: maybe-html-gcc
html-host: maybe-html-gmp
html-host: maybe-html-mpfr
html-host: maybe-html-mpc
html-host: maybe-html-isl
html-host: maybe-html-cloog
html-host: maybe-html-libelf
html-host: maybe-html-gold
html-host: maybe-html-gprof
html-host: maybe-html-intl
html-host: maybe-html-tcl
html-host: maybe-html-itcl
html-host: maybe-html-ld
html-host: maybe-html-libbacktrace
html-host: maybe-html-libcpp
html-host: maybe-html-libdecnumber
html-host: maybe-html-libgui
html-host: maybe-html-libiberty
html-host: maybe-html-libiconv
html-host: maybe-html-m4
html-host: maybe-html-readline
html-host: maybe-html-sid
html-host: maybe-html-sim
html-host: maybe-html-texinfo
html-host: maybe-html-zlib
html-host: maybe-html-gdb
html-host: maybe-html-expect
html-host: maybe-html-guile
html-host: maybe-html-tk
html-host: maybe-html-libtermcap
html-host: maybe-html-utils
html-host: maybe-html-gnattools
html-host: maybe-html-lto-plugin

.PHONY: html-target

html-target: maybe-html-target-libstdc++-v3
html-target: maybe-html-target-libmudflap
html-target: maybe-html-target-libsanitizer
html-target: maybe-html-target-libvtv
html-target: maybe-html-target-libssp
html-target: maybe-html-target-newlib
html-target: maybe-html-target-libgcc
html-target: maybe-html-target-libbacktrace
html-target: maybe-html-target-libquadmath
html-target: maybe-html-target-libgfortran
html-target: maybe-html-target-libobjc
html-target: maybe-html-target-libgo
html-target: maybe-html-target-libtermcap
html-target: maybe-html-target-winsup
html-target: maybe-html-target-libgloss
html-target: maybe-html-target-libffi
html-target: maybe-html-target-libjava
html-target: maybe-html-target-zlib
html-target: maybe-html-target-boehm-gc
html-target: maybe-html-target-rda
html-target: maybe-html-target-libada
html-target: maybe-html-target-libgomp
html-target: maybe-html-target-libitm
html-target: maybe-html-target-libatomic

.PHONY: do-TAGS
do-TAGS:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) TAGS-host \
	  TAGS-target


.PHONY: TAGS-host

TAGS-host: maybe-TAGS-bfd
TAGS-host: maybe-TAGS-opcodes
TAGS-host: maybe-TAGS-binutils
TAGS-host: maybe-TAGS-bison
TAGS-host: maybe-TAGS-cgen
TAGS-host: maybe-TAGS-dejagnu
TAGS-host: maybe-TAGS-etc
TAGS-host: maybe-TAGS-fastjar
TAGS-host: maybe-TAGS-fixincludes
TAGS-host: maybe-TAGS-flex
TAGS-host: maybe-TAGS-gas
TAGS-host: maybe-TAGS-gcc
TAGS-host: maybe-TAGS-gmp
TAGS-host: maybe-TAGS-mpfr
TAGS-host: maybe-TAGS-mpc
TAGS-host: maybe-TAGS-isl
TAGS-host: maybe-TAGS-cloog
TAGS-host: maybe-TAGS-libelf
TAGS-host: maybe-TAGS-gold
TAGS-host: maybe-TAGS-gprof
TAGS-host: maybe-TAGS-intl
TAGS-host: maybe-TAGS-tcl
TAGS-host: maybe-TAGS-itcl
TAGS-host: maybe-TAGS-ld
TAGS-host: maybe-TAGS-libbacktrace
TAGS-host: maybe-TAGS-libcpp
TAGS-host: maybe-TAGS-libdecnumber
TAGS-host: maybe-TAGS-libgui
TAGS-host: maybe-TAGS-libiberty
TAGS-host: maybe-TAGS-libiconv
TAGS-host: maybe-TAGS-m4
TAGS-host: maybe-TAGS-readline
TAGS-host: maybe-TAGS-sid
TAGS-host: maybe-TAGS-sim
TAGS-host: maybe-TAGS-texinfo
TAGS-host: maybe-TAGS-zlib
TAGS-host: maybe-TAGS-gdb
TAGS-host: maybe-TAGS-expect
TAGS-host: maybe-TAGS-guile
TAGS-host: maybe-TAGS-tk
TAGS-host: maybe-TAGS-libtermcap
TAGS-host: maybe-TAGS-utils
TAGS-host: maybe-TAGS-gnattools
TAGS-host: maybe-TAGS-lto-plugin

.PHONY: TAGS-target

TAGS-target: maybe-TAGS-target-libstdc++-v3
TAGS-target: maybe-TAGS-target-libmudflap
TAGS-target: maybe-TAGS-target-libsanitizer
TAGS-target: maybe-TAGS-target-libvtv
TAGS-target: maybe-TAGS-target-libssp
TAGS-target: maybe-TAGS-target-newlib
TAGS-target: maybe-TAGS-target-libgcc
TAGS-target: maybe-TAGS-target-libbacktrace
TAGS-target: maybe-TAGS-target-libquadmath
TAGS-target: maybe-TAGS-target-libgfortran
TAGS-target: maybe-TAGS-target-libobjc
TAGS-target: maybe-TAGS-target-libgo
TAGS-target: maybe-TAGS-target-libtermcap
TAGS-target: maybe-TAGS-target-winsup
TAGS-target: maybe-TAGS-target-libgloss
TAGS-target: maybe-TAGS-target-libffi
TAGS-target: maybe-TAGS-target-libjava
TAGS-target: maybe-TAGS-target-zlib
TAGS-target: maybe-TAGS-target-boehm-gc
TAGS-target: maybe-TAGS-target-rda
TAGS-target: maybe-TAGS-target-libada
TAGS-target: maybe-TAGS-target-libgomp
TAGS-target: maybe-TAGS-target-libitm
TAGS-target: maybe-TAGS-target-libatomic

.PHONY: do-install-info
do-install-info:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) install-info-host \
	  install-info-target


.PHONY: install-info-host

install-info-host: maybe-install-info-bfd
install-info-host: maybe-install-info-opcodes
install-info-host: maybe-install-info-binutils
install-info-host: maybe-install-info-bison
install-info-host: maybe-install-info-cgen
install-info-host: maybe-install-info-dejagnu
install-info-host: maybe-install-info-etc
install-info-host: maybe-install-info-fastjar
install-info-host: maybe-install-info-fixincludes
install-info-host: maybe-install-info-flex
install-info-host: maybe-install-info-gas
install-info-host: maybe-install-info-gcc
install-info-host: maybe-install-info-gmp
install-info-host: maybe-install-info-mpfr
install-info-host: maybe-install-info-mpc
install-info-host: maybe-install-info-isl
install-info-host: maybe-install-info-cloog
install-info-host: maybe-install-info-libelf
install-info-host: maybe-install-info-gold
install-info-host: maybe-install-info-gprof
install-info-host: maybe-install-info-intl
install-info-host: maybe-install-info-tcl
install-info-host: maybe-install-info-itcl
install-info-host: maybe-install-info-ld
install-info-host: maybe-install-info-libbacktrace
install-info-host: maybe-install-info-libcpp
install-info-host: maybe-install-info-libdecnumber
install-info-host: maybe-install-info-libgui
install-info-host: maybe-install-info-libiberty
install-info-host: maybe-install-info-libiconv
install-info-host: maybe-install-info-m4
install-info-host: maybe-install-info-readline
install-info-host: maybe-install-info-sid
install-info-host: maybe-install-info-sim
install-info-host: maybe-install-info-texinfo
install-info-host: maybe-install-info-zlib
install-info-host: maybe-install-info-gdb
install-info-host: maybe-install-info-expect
install-info-host: maybe-install-info-guile
install-info-host: maybe-install-info-tk
install-info-host: maybe-install-info-libtermcap
install-info-host: maybe-install-info-utils
install-info-host: maybe-install-info-gnattools
install-info-host: maybe-install-info-lto-plugin

.PHONY: install-info-target

install-info-target: maybe-install-info-target-libstdc++-v3
install-info-target: maybe-install-info-target-libmudflap
install-info-target: maybe-install-info-target-libsanitizer
install-info-target: maybe-install-info-target-libvtv
install-info-target: maybe-install-info-target-libssp
install-info-target: maybe-install-info-target-newlib
install-info-target: maybe-install-info-target-libgcc
install-info-target: maybe-install-info-target-libbacktrace
install-info-target: maybe-install-info-target-libquadmath
install-info-target: maybe-install-info-target-libgfortran
install-info-target: maybe-install-info-target-libobjc
install-info-target: maybe-install-info-target-libgo
install-info-target: maybe-install-info-target-libtermcap
install-info-target: maybe-install-info-target-winsup
install-info-target: maybe-install-info-target-libgloss
install-info-target: maybe-install-info-target-libffi
install-info-target: maybe-install-info-target-libjava
install-info-target: maybe-install-info-target-zlib
install-info-target: maybe-install-info-target-boehm-gc
install-info-target: maybe-install-info-target-rda
install-info-target: maybe-install-info-target-libada
install-info-target: maybe-install-info-target-libgomp
install-info-target: maybe-install-info-target-libitm
install-info-target: maybe-install-info-target-libatomic

.PHONY: do-install-pdf
do-install-pdf:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) install-pdf-host \
	  install-pdf-target


.PHONY: install-pdf-host

install-pdf-host: maybe-install-pdf-bfd
install-pdf-host: maybe-install-pdf-opcodes
install-pdf-host: maybe-install-pdf-binutils
install-pdf-host: maybe-install-pdf-bison
install-pdf-host: maybe-install-pdf-cgen
install-pdf-host: maybe-install-pdf-dejagnu
install-pdf-host: maybe-install-pdf-etc
install-pdf-host: maybe-install-pdf-fastjar
install-pdf-host: maybe-install-pdf-fixincludes
install-pdf-host: maybe-install-pdf-flex
install-pdf-host: maybe-install-pdf-gas
install-pdf-host: maybe-install-pdf-gcc
install-pdf-host: maybe-install-pdf-gmp
install-pdf-host: maybe-install-pdf-mpfr
install-pdf-host: maybe-install-pdf-mpc
install-pdf-host: maybe-install-pdf-isl
install-pdf-host: maybe-install-pdf-cloog
install-pdf-host: maybe-install-pdf-libelf
install-pdf-host: maybe-install-pdf-gold
install-pdf-host: maybe-install-pdf-gprof
install-pdf-host: maybe-install-pdf-intl
install-pdf-host: maybe-install-pdf-tcl
install-pdf-host: maybe-install-pdf-itcl
install-pdf-host: maybe-install-pdf-ld
install-pdf-host: maybe-install-pdf-libbacktrace
install-pdf-host: maybe-install-pdf-libcpp
install-pdf-host: maybe-install-pdf-libdecnumber
install-pdf-host: maybe-install-pdf-libgui
install-pdf-host: maybe-install-pdf-libiberty
install-pdf-host: maybe-install-pdf-libiconv
install-pdf-host: maybe-install-pdf-m4
install-pdf-host: maybe-install-pdf-readline
install-pdf-host: maybe-install-pdf-sid
install-pdf-host: maybe-install-pdf-sim
install-pdf-host: maybe-install-pdf-texinfo
install-pdf-host: maybe-install-pdf-zlib
install-pdf-host: maybe-install-pdf-gdb
install-pdf-host: maybe-install-pdf-expect
install-pdf-host: maybe-install-pdf-guile
install-pdf-host: maybe-install-pdf-tk
install-pdf-host: maybe-install-pdf-libtermcap
install-pdf-host: maybe-install-pdf-utils
install-pdf-host: maybe-install-pdf-gnattools
install-pdf-host: maybe-install-pdf-lto-plugin

.PHONY: install-pdf-target

install-pdf-target: maybe-install-pdf-target-libstdc++-v3
install-pdf-target: maybe-install-pdf-target-libmudflap
install-pdf-target: maybe-install-pdf-target-libsanitizer
install-pdf-target: maybe-install-pdf-target-libvtv
install-pdf-target: maybe-install-pdf-target-libssp
install-pdf-target: maybe-install-pdf-target-newlib
install-pdf-target: maybe-install-pdf-target-libgcc
install-pdf-target: maybe-install-pdf-target-libbacktrace
install-pdf-target: maybe-install-pdf-target-libquadmath
install-pdf-target: maybe-install-pdf-target-libgfortran
install-pdf-target: maybe-install-pdf-target-libobjc
install-pdf-target: maybe-install-pdf-target-libgo
install-pdf-target: maybe-install-pdf-target-libtermcap
install-pdf-target: maybe-install-pdf-target-winsup
install-pdf-target: maybe-install-pdf-target-libgloss
install-pdf-target: maybe-install-pdf-target-libffi
install-pdf-target: maybe-install-pdf-target-libjava
install-pdf-target: maybe-install-pdf-target-zlib
install-pdf-target: maybe-install-pdf-target-boehm-gc
install-pdf-target: maybe-install-pdf-target-rda
install-pdf-target: maybe-install-pdf-target-libada
install-pdf-target: maybe-install-pdf-target-libgomp
install-pdf-target: maybe-install-pdf-target-libitm
install-pdf-target: maybe-install-pdf-target-libatomic

.PHONY: do-install-html
do-install-html:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) install-html-host \
	  install-html-target


.PHONY: install-html-host

install-html-host: maybe-install-html-bfd
install-html-host: maybe-install-html-opcodes
install-html-host: maybe-install-html-binutils
install-html-host: maybe-install-html-bison
install-html-host: maybe-install-html-cgen
install-html-host: maybe-install-html-dejagnu
install-html-host: maybe-install-html-etc
install-html-host: maybe-install-html-fastjar
install-html-host: maybe-install-html-fixincludes
install-html-host: maybe-install-html-flex
install-html-host: maybe-install-html-gas
install-html-host: maybe-install-html-gcc
install-html-host: maybe-install-html-gmp
install-html-host: maybe-install-html-mpfr
install-html-host: maybe-install-html-mpc
install-html-host: maybe-install-html-isl
install-html-host: maybe-install-html-cloog
install-html-host: maybe-install-html-libelf
install-html-host: maybe-install-html-gold
install-html-host: maybe-install-html-gprof
install-html-host: maybe-install-html-intl
install-html-host: maybe-install-html-tcl
install-html-host: maybe-install-html-itcl
install-html-host: maybe-install-html-ld
install-html-host: maybe-install-html-libbacktrace
install-html-host: maybe-install-html-libcpp
install-html-host: maybe-install-html-libdecnumber
install-html-host: maybe-install-html-libgui
install-html-host: maybe-install-html-libiberty
install-html-host: maybe-install-html-libiconv
install-html-host: maybe-install-html-m4
install-html-host: maybe-install-html-readline
install-html-host: maybe-install-html-sid
install-html-host: maybe-install-html-sim
install-html-host: maybe-install-html-texinfo
install-html-host: maybe-install-html-zlib
install-html-host: maybe-install-html-gdb
install-html-host: maybe-install-html-expect
install-html-host: maybe-install-html-guile
install-html-host: maybe-install-html-tk
install-html-host: maybe-install-html-libtermcap
install-html-host: maybe-install-html-utils
install-html-host: maybe-install-html-gnattools
install-html-host: maybe-install-html-lto-plugin

.PHONY: install-html-target

install-html-target: maybe-install-html-target-libstdc++-v3
install-html-target: maybe-install-html-target-libmudflap
install-html-target: maybe-install-html-target-libsanitizer
install-html-target: maybe-install-html-target-libvtv
install-html-target: maybe-install-html-target-libssp
install-html-target: maybe-install-html-target-newlib
install-html-target: maybe-install-html-target-libgcc
install-html-target: maybe-install-html-target-libbacktrace
install-html-target: maybe-install-html-target-libquadmath
install-html-target: maybe-install-html-target-libgfortran
install-html-target: maybe-install-html-target-libobjc
install-html-target: maybe-install-html-target-libgo
install-html-target: maybe-install-html-target-libtermcap
install-html-target: maybe-install-html-target-winsup
install-html-target: maybe-install-html-target-libgloss
install-html-target: maybe-install-html-target-libffi
install-html-target: maybe-install-html-target-libjava
install-html-target: maybe-install-html-target-zlib
install-html-target: maybe-install-html-target-boehm-gc
install-html-target: maybe-install-html-target-rda
install-html-target: maybe-install-html-target-libada
install-html-target: maybe-install-html-target-libgomp
install-html-target: maybe-install-html-target-libitm
install-html-target: maybe-install-html-target-libatomic

.PHONY: do-installcheck
do-installcheck:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) installcheck-host \
	  installcheck-target


.PHONY: installcheck-host

installcheck-host: maybe-installcheck-bfd
installcheck-host: maybe-installcheck-opcodes
installcheck-host: maybe-installcheck-binutils
installcheck-host: maybe-installcheck-bison
installcheck-host: maybe-installcheck-cgen
installcheck-host: maybe-installcheck-dejagnu
installcheck-host: maybe-installcheck-etc
installcheck-host: maybe-installcheck-fastjar
installcheck-host: maybe-installcheck-fixincludes
installcheck-host: maybe-installcheck-flex
installcheck-host: maybe-installcheck-gas
installcheck-host: maybe-installcheck-gcc
installcheck-host: maybe-installcheck-gmp
installcheck-host: maybe-installcheck-mpfr
installcheck-host: maybe-installcheck-mpc
installcheck-host: maybe-installcheck-isl
installcheck-host: maybe-installcheck-cloog
installcheck-host: maybe-installcheck-libelf
installcheck-host: maybe-installcheck-gold
installcheck-host: maybe-installcheck-gprof
installcheck-host: maybe-installcheck-intl
installcheck-host: maybe-installcheck-tcl
installcheck-host: maybe-installcheck-itcl
installcheck-host: maybe-installcheck-ld
installcheck-host: maybe-installcheck-libbacktrace
installcheck-host: maybe-installcheck-libcpp
installcheck-host: maybe-installcheck-libdecnumber
installcheck-host: maybe-installcheck-libgui
installcheck-host: maybe-installcheck-libiberty
installcheck-host: maybe-installcheck-libiconv
installcheck-host: maybe-installcheck-m4
installcheck-host: maybe-installcheck-readline
installcheck-host: maybe-installcheck-sid
installcheck-host: maybe-installcheck-sim
installcheck-host: maybe-installcheck-texinfo
installcheck-host: maybe-installcheck-zlib
installcheck-host: maybe-installcheck-gdb
installcheck-host: maybe-installcheck-expect
installcheck-host: maybe-installcheck-guile
installcheck-host: maybe-installcheck-tk
installcheck-host: maybe-installcheck-libtermcap
installcheck-host: maybe-installcheck-utils
installcheck-host: maybe-installcheck-gnattools
installcheck-host: maybe-installcheck-lto-plugin

.PHONY: installcheck-target

installcheck-target: maybe-installcheck-target-libstdc++-v3
installcheck-target: maybe-installcheck-target-libmudflap
installcheck-target: maybe-installcheck-target-libsanitizer
installcheck-target: maybe-installcheck-target-libvtv
installcheck-target: maybe-installcheck-target-libssp
installcheck-target: maybe-installcheck-target-newlib
installcheck-target: maybe-installcheck-target-libgcc
installcheck-target: maybe-installcheck-target-libbacktrace
installcheck-target: maybe-installcheck-target-libquadmath
installcheck-target: maybe-installcheck-target-libgfortran
installcheck-target: maybe-installcheck-target-libobjc
installcheck-target: maybe-installcheck-target-libgo
installcheck-target: maybe-installcheck-target-libtermcap
installcheck-target: maybe-installcheck-target-winsup
installcheck-target: maybe-installcheck-target-libgloss
installcheck-target: maybe-installcheck-target-libffi
installcheck-target: maybe-installcheck-target-libjava
installcheck-target: maybe-installcheck-target-zlib
installcheck-target: maybe-installcheck-target-boehm-gc
installcheck-target: maybe-installcheck-target-rda
installcheck-target: maybe-installcheck-target-libada
installcheck-target: maybe-installcheck-target-libgomp
installcheck-target: maybe-installcheck-target-libitm
installcheck-target: maybe-installcheck-target-libatomic

.PHONY: do-mostlyclean
do-mostlyclean:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) mostlyclean-host \
	  mostlyclean-target


.PHONY: mostlyclean-host

mostlyclean-host: maybe-mostlyclean-bfd
mostlyclean-host: maybe-mostlyclean-opcodes
mostlyclean-host: maybe-mostlyclean-binutils
mostlyclean-host: maybe-mostlyclean-bison
mostlyclean-host: maybe-mostlyclean-cgen
mostlyclean-host: maybe-mostlyclean-dejagnu
mostlyclean-host: maybe-mostlyclean-etc
mostlyclean-host: maybe-mostlyclean-fastjar
mostlyclean-host: maybe-mostlyclean-fixincludes
mostlyclean-host: maybe-mostlyclean-flex
mostlyclean-host: maybe-mostlyclean-gas
mostlyclean-host: maybe-mostlyclean-gcc
mostlyclean-host: maybe-mostlyclean-gmp
mostlyclean-host: maybe-mostlyclean-mpfr
mostlyclean-host: maybe-mostlyclean-mpc
mostlyclean-host: maybe-mostlyclean-isl
mostlyclean-host: maybe-mostlyclean-cloog
mostlyclean-host: maybe-mostlyclean-libelf
mostlyclean-host: maybe-mostlyclean-gold
mostlyclean-host: maybe-mostlyclean-gprof
mostlyclean-host: maybe-mostlyclean-intl
mostlyclean-host: maybe-mostlyclean-tcl
mostlyclean-host: maybe-mostlyclean-itcl
mostlyclean-host: maybe-mostlyclean-ld
mostlyclean-host: maybe-mostlyclean-libbacktrace
mostlyclean-host: maybe-mostlyclean-libcpp
mostlyclean-host: maybe-mostlyclean-libdecnumber
mostlyclean-host: maybe-mostlyclean-libgui
mostlyclean-host: maybe-mostlyclean-libiberty
mostlyclean-host: maybe-mostlyclean-libiconv
mostlyclean-host: maybe-mostlyclean-m4
mostlyclean-host: maybe-mostlyclean-readline
mostlyclean-host: maybe-mostlyclean-sid
mostlyclean-host: maybe-mostlyclean-sim
mostlyclean-host: maybe-mostlyclean-texinfo
mostlyclean-host: maybe-mostlyclean-zlib
mostlyclean-host: maybe-mostlyclean-gdb
mostlyclean-host: maybe-mostlyclean-expect
mostlyclean-host: maybe-mostlyclean-guile
mostlyclean-host: maybe-mostlyclean-tk
mostlyclean-host: maybe-mostlyclean-libtermcap
mostlyclean-host: maybe-mostlyclean-utils
mostlyclean-host: maybe-mostlyclean-gnattools
mostlyclean-host: maybe-mostlyclean-lto-plugin

.PHONY: mostlyclean-target

mostlyclean-target: maybe-mostlyclean-target-libstdc++-v3
mostlyclean-target: maybe-mostlyclean-target-libmudflap
mostlyclean-target: maybe-mostlyclean-target-libsanitizer
mostlyclean-target: maybe-mostlyclean-target-libvtv
mostlyclean-target: maybe-mostlyclean-target-libssp
mostlyclean-target: maybe-mostlyclean-target-newlib
mostlyclean-target: maybe-mostlyclean-target-libgcc
mostlyclean-target: maybe-mostlyclean-target-libbacktrace
mostlyclean-target: maybe-mostlyclean-target-libquadmath
mostlyclean-target: maybe-mostlyclean-target-libgfortran
mostlyclean-target: maybe-mostlyclean-target-libobjc
mostlyclean-target: maybe-mostlyclean-target-libgo
mostlyclean-target: maybe-mostlyclean-target-libtermcap
mostlyclean-target: maybe-mostlyclean-target-winsup
mostlyclean-target: maybe-mostlyclean-target-libgloss
mostlyclean-target: maybe-mostlyclean-target-libffi
mostlyclean-target: maybe-mostlyclean-target-libjava
mostlyclean-target: maybe-mostlyclean-target-zlib
mostlyclean-target: maybe-mostlyclean-target-boehm-gc
mostlyclean-target: maybe-mostlyclean-target-rda
mostlyclean-target: maybe-mostlyclean-target-libada
mostlyclean-target: maybe-mostlyclean-target-libgomp
mostlyclean-target: maybe-mostlyclean-target-libitm
mostlyclean-target: maybe-mostlyclean-target-libatomic

.PHONY: do-clean
do-clean:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) clean-host \
	  clean-target


.PHONY: clean-host

clean-host: maybe-clean-bfd
clean-host: maybe-clean-opcodes
clean-host: maybe-clean-binutils
clean-host: maybe-clean-bison
clean-host: maybe-clean-cgen
clean-host: maybe-clean-dejagnu
clean-host: maybe-clean-etc
clean-host: maybe-clean-fastjar
clean-host: maybe-clean-fixincludes
clean-host: maybe-clean-flex
clean-host: maybe-clean-gas
clean-host: maybe-clean-gcc
clean-host: maybe-clean-gmp
clean-host: maybe-clean-mpfr
clean-host: maybe-clean-mpc
clean-host: maybe-clean-isl
clean-host: maybe-clean-cloog
clean-host: maybe-clean-libelf
clean-host: maybe-clean-gold
clean-host: maybe-clean-gprof
clean-host: maybe-clean-intl
clean-host: maybe-clean-tcl
clean-host: maybe-clean-itcl
clean-host: maybe-clean-ld
clean-host: maybe-clean-libbacktrace
clean-host: maybe-clean-libcpp
clean-host: maybe-clean-libdecnumber
clean-host: maybe-clean-libgui
clean-host: maybe-clean-libiberty
clean-host: maybe-clean-libiconv
clean-host: maybe-clean-m4
clean-host: maybe-clean-readline
clean-host: maybe-clean-sid
clean-host: maybe-clean-sim
clean-host: maybe-clean-texinfo
clean-host: maybe-clean-zlib
clean-host: maybe-clean-gdb
clean-host: maybe-clean-expect
clean-host: maybe-clean-guile
clean-host: maybe-clean-tk
clean-host: maybe-clean-libtermcap
clean-host: maybe-clean-utils
clean-host: maybe-clean-gnattools
clean-host: maybe-clean-lto-plugin

.PHONY: clean-target

clean-target: maybe-clean-target-libstdc++-v3
clean-target: maybe-clean-target-libmudflap
clean-target: maybe-clean-target-libsanitizer
clean-target: maybe-clean-target-libvtv
clean-target: maybe-clean-target-libssp
clean-target: maybe-clean-target-newlib
clean-target: maybe-clean-target-libgcc
clean-target: maybe-clean-target-libbacktrace
clean-target: maybe-clean-target-libquadmath
clean-target: maybe-clean-target-libgfortran
clean-target: maybe-clean-target-libobjc
clean-target: maybe-clean-target-libgo
clean-target: maybe-clean-target-libtermcap
clean-target: maybe-clean-target-winsup
clean-target: maybe-clean-target-libgloss
clean-target: maybe-clean-target-libffi
clean-target: maybe-clean-target-libjava
clean-target: maybe-clean-target-zlib
clean-target: maybe-clean-target-boehm-gc
clean-target: maybe-clean-target-rda
clean-target: maybe-clean-target-libada
clean-target: maybe-clean-target-libgomp
clean-target: maybe-clean-target-libitm
clean-target: maybe-clean-target-libatomic

.PHONY: do-distclean
do-distclean:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) distclean-host \
	  distclean-target


.PHONY: distclean-host

distclean-host: maybe-distclean-bfd
distclean-host: maybe-distclean-opcodes
distclean-host: maybe-distclean-binutils
distclean-host: maybe-distclean-bison
distclean-host: maybe-distclean-cgen
distclean-host: maybe-distclean-dejagnu
distclean-host: maybe-distclean-etc
distclean-host: maybe-distclean-fastjar
distclean-host: maybe-distclean-fixincludes
distclean-host: maybe-distclean-flex
distclean-host: maybe-distclean-gas
distclean-host: maybe-distclean-gcc
distclean-host: maybe-distclean-gmp
distclean-host: maybe-distclean-mpfr
distclean-host: maybe-distclean-mpc
distclean-host: maybe-distclean-isl
distclean-host: maybe-distclean-cloog
distclean-host: maybe-distclean-libelf
distclean-host: maybe-distclean-gold
distclean-host: maybe-distclean-gprof
distclean-host: maybe-distclean-intl
distclean-host: maybe-distclean-tcl
distclean-host: maybe-distclean-itcl
distclean-host: maybe-distclean-ld
distclean-host: maybe-distclean-libbacktrace
distclean-host: maybe-distclean-libcpp
distclean-host: maybe-distclean-libdecnumber
distclean-host: maybe-distclean-libgui
distclean-host: maybe-distclean-libiberty
distclean-host: maybe-distclean-libiconv
distclean-host: maybe-distclean-m4
distclean-host: maybe-distclean-readline
distclean-host: maybe-distclean-sid
distclean-host: maybe-distclean-sim
distclean-host: maybe-distclean-texinfo
distclean-host: maybe-distclean-zlib
distclean-host: maybe-distclean-gdb
distclean-host: maybe-distclean-expect
distclean-host: maybe-distclean-guile
distclean-host: maybe-distclean-tk
distclean-host: maybe-distclean-libtermcap
distclean-host: maybe-distclean-utils
distclean-host: maybe-distclean-gnattools
distclean-host: maybe-distclean-lto-plugin

.PHONY: distclean-target

distclean-target: maybe-distclean-target-libstdc++-v3
distclean-target: maybe-distclean-target-libmudflap
distclean-target: maybe-distclean-target-libsanitizer
distclean-target: maybe-distclean-target-libvtv
distclean-target: maybe-distclean-target-libssp
distclean-target: maybe-distclean-target-newlib
distclean-target: maybe-distclean-target-libgcc
distclean-target: maybe-distclean-target-libbacktrace
distclean-target: maybe-distclean-target-libquadmath
distclean-target: maybe-distclean-target-libgfortran
distclean-target: maybe-distclean-target-libobjc
distclean-target: maybe-distclean-target-libgo
distclean-target: maybe-distclean-target-libtermcap
distclean-target: maybe-distclean-target-winsup
distclean-target: maybe-distclean-target-libgloss
distclean-target: maybe-distclean-target-libffi
distclean-target: maybe-distclean-target-libjava
distclean-target: maybe-distclean-target-zlib
distclean-target: maybe-distclean-target-boehm-gc
distclean-target: maybe-distclean-target-rda
distclean-target: maybe-distclean-target-libada
distclean-target: maybe-distclean-target-libgomp
distclean-target: maybe-distclean-target-libitm
distclean-target: maybe-distclean-target-libatomic

.PHONY: do-maintainer-clean
do-maintainer-clean:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) maintainer-clean-host \
	  maintainer-clean-target


.PHONY: maintainer-clean-host

maintainer-clean-host: maybe-maintainer-clean-bfd
maintainer-clean-host: maybe-maintainer-clean-opcodes
maintainer-clean-host: maybe-maintainer-clean-binutils
maintainer-clean-host: maybe-maintainer-clean-bison
maintainer-clean-host: maybe-maintainer-clean-cgen
maintainer-clean-host: maybe-maintainer-clean-dejagnu
maintainer-clean-host: maybe-maintainer-clean-etc
maintainer-clean-host: maybe-maintainer-clean-fastjar
maintainer-clean-host: maybe-maintainer-clean-fixincludes
maintainer-clean-host: maybe-maintainer-clean-flex
maintainer-clean-host: maybe-maintainer-clean-gas
maintainer-clean-host: maybe-maintainer-clean-gcc
maintainer-clean-host: maybe-maintainer-clean-gmp
maintainer-clean-host: maybe-maintainer-clean-mpfr
maintainer-clean-host: maybe-maintainer-clean-mpc
maintainer-clean-host: maybe-maintainer-clean-isl
maintainer-clean-host: maybe-maintainer-clean-cloog
maintainer-clean-host: maybe-maintainer-clean-libelf
maintainer-clean-host: maybe-maintainer-clean-gold
maintainer-clean-host: maybe-maintainer-clean-gprof
maintainer-clean-host: maybe-maintainer-clean-intl
maintainer-clean-host: maybe-maintainer-clean-tcl
maintainer-clean-host: maybe-maintainer-clean-itcl
maintainer-clean-host: maybe-maintainer-clean-ld
maintainer-clean-host: maybe-maintainer-clean-libbacktrace
maintainer-clean-host: maybe-maintainer-clean-libcpp
maintainer-clean-host: maybe-maintainer-clean-libdecnumber
maintainer-clean-host: maybe-maintainer-clean-libgui
maintainer-clean-host: maybe-maintainer-clean-libiberty
maintainer-clean-host: maybe-maintainer-clean-libiconv
maintainer-clean-host: maybe-maintainer-clean-m4
maintainer-clean-host: maybe-maintainer-clean-readline
maintainer-clean-host: maybe-maintainer-clean-sid
maintainer-clean-host: maybe-maintainer-clean-sim
maintainer-clean-host: maybe-maintainer-clean-texinfo
maintainer-clean-host: maybe-maintainer-clean-zlib
maintainer-clean-host: maybe-maintainer-clean-gdb
maintainer-clean-host: maybe-maintainer-clean-expect
maintainer-clean-host: maybe-maintainer-clean-guile
maintainer-clean-host: maybe-maintainer-clean-tk
maintainer-clean-host: maybe-maintainer-clean-libtermcap
maintainer-clean-host: maybe-maintainer-clean-utils
maintainer-clean-host: maybe-maintainer-clean-gnattools
maintainer-clean-host: maybe-maintainer-clean-lto-plugin

.PHONY: maintainer-clean-target

maintainer-clean-target: maybe-maintainer-clean-target-libstdc++-v3
maintainer-clean-target: maybe-maintainer-clean-target-libmudflap
maintainer-clean-target: maybe-maintainer-clean-target-libsanitizer
maintainer-clean-target: maybe-maintainer-clean-target-libvtv
maintainer-clean-target: maybe-maintainer-clean-target-libssp
maintainer-clean-target: maybe-maintainer-clean-target-newlib
maintainer-clean-target: maybe-maintainer-clean-target-libgcc
maintainer-clean-target: maybe-maintainer-clean-target-libbacktrace
maintainer-clean-target: maybe-maintainer-clean-target-libquadmath
maintainer-clean-target: maybe-maintainer-clean-target-libgfortran
maintainer-clean-target: maybe-maintainer-clean-target-libobjc
maintainer-clean-target: maybe-maintainer-clean-target-libgo
maintainer-clean-target: maybe-maintainer-clean-target-libtermcap
maintainer-clean-target: maybe-maintainer-clean-target-winsup
maintainer-clean-target: maybe-maintainer-clean-target-libgloss
maintainer-clean-target: maybe-maintainer-clean-target-libffi
maintainer-clean-target: maybe-maintainer-clean-target-libjava
maintainer-clean-target: maybe-maintainer-clean-target-zlib
maintainer-clean-target: maybe-maintainer-clean-target-boehm-gc
maintainer-clean-target: maybe-maintainer-clean-target-rda
maintainer-clean-target: maybe-maintainer-clean-target-libada
maintainer-clean-target: maybe-maintainer-clean-target-libgomp
maintainer-clean-target: maybe-maintainer-clean-target-libitm
maintainer-clean-target: maybe-maintainer-clean-target-libatomic


# Here are the targets which correspond to the do-X targets.

.PHONY: info installcheck dvi pdf html
.PHONY: install-info install-pdf install-html
.PHONY: clean distclean mostlyclean maintainer-clean realclean
.PHONY: local-clean local-distclean local-maintainer-clean
info: do-info
installcheck: do-installcheck
dvi: do-dvi
pdf: do-pdf
html: do-html

# Make sure makeinfo is built before we do a `make info', if we're
# in fact building texinfo.
do-info: maybe-all-texinfo

install-info: do-install-info dir.info
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if [ -f dir.info ] ; then \
	  $(INSTALL_DATA) dir.info $(DESTDIR)$(infodir)/dir.info ; \
	else true ; fi

install-pdf: do-install-pdf

install-html: do-install-html

local-clean:
	-rm -f *.a TEMP errs core *.o *~ \#* TAGS *.E *.log

local-distclean:
	-rm -f Makefile config.status config.cache mh-frag mt-frag
	-rm -f maybedep.tmp serdep.tmp
	-if [ "$(TARGET_SUBDIR)" != "." ]; then \
	  rm -rf $(TARGET_SUBDIR); \
	else true; fi
	-rm -rf $(BUILD_SUBDIR)
	-if [ "$(HOST_SUBDIR)" != "." ]; then \
	  rm -rf $(HOST_SUBDIR); \
	else true; fi
	-rm -f texinfo/po/Makefile texinfo/po/Makefile.in texinfo/info/Makefile
	-rm -f texinfo/doc/Makefile texinfo/po/POTFILES
	-rmdir texinfo/doc texinfo/info texinfo/intl texinfo/lib 2>/dev/null
	-rmdir texinfo/makeinfo texinfo/po texinfo/util 2>/dev/null
	-rmdir fastjar gcc libiberty texinfo zlib 2>/dev/null
	-find . -name config.cache -exec rm -f {} \; \; 2>/dev/null

local-maintainer-clean:
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

clean: do-clean local-clean
mostlyclean: do-mostlyclean local-clean
distclean: do-distclean local-clean local-distclean
maintainer-clean: local-maintainer-clean do-maintainer-clean local-clean 
maintainer-clean: local-distclean
realclean: maintainer-clean

# Check target.

.PHONY: check do-check
check: do-check

# Only include modules actually being configured and built.
.PHONY: check-host
check-host:  \
    maybe-check-bfd \
    maybe-check-opcodes \
    maybe-check-binutils \
    maybe-check-bison \
    maybe-check-cgen \
    maybe-check-dejagnu \
    maybe-check-etc \
    maybe-check-fastjar \
    maybe-check-fixincludes \
    maybe-check-flex \
    maybe-check-gas \
    maybe-check-gcc \
    maybe-check-gmp \
    maybe-check-mpfr \
    maybe-check-mpc \
    maybe-check-isl \
    maybe-check-cloog \
    maybe-check-libelf \
    maybe-check-gold \
    maybe-check-gprof \
    maybe-check-intl \
    maybe-check-tcl \
    maybe-check-itcl \
    maybe-check-ld \
    maybe-check-libbacktrace \
    maybe-check-libcpp \
    maybe-check-libdecnumber \
    maybe-check-libgui \
    maybe-check-libiberty \
    maybe-check-libiconv \
    maybe-check-m4 \
    maybe-check-readline \
    maybe-check-sid \
    maybe-check-sim \
    maybe-check-texinfo \
    maybe-check-zlib \
    maybe-check-gdb \
    maybe-check-expect \
    maybe-check-guile \
    maybe-check-tk \
    maybe-check-libtermcap \
    maybe-check-utils \
    maybe-check-gnattools \
    maybe-check-lto-plugin

.PHONY: check-target
check-target:  \
    maybe-check-target-libstdc++-v3 \
    maybe-check-target-libmudflap \
    maybe-check-target-libsanitizer \
    maybe-check-target-libvtv \
    maybe-check-target-libssp \
    maybe-check-target-newlib \
    maybe-check-target-libgcc \
    maybe-check-target-libbacktrace \
    maybe-check-target-libquadmath \
    maybe-check-target-libgfortran \
    maybe-check-target-libobjc \
    maybe-check-target-libgo \
    maybe-check-target-libtermcap \
    maybe-check-target-winsup \
    maybe-check-target-libgloss \
    maybe-check-target-libffi \
    maybe-check-target-libjava \
    maybe-check-target-zlib \
    maybe-check-target-boehm-gc \
    maybe-check-target-rda \
    maybe-check-target-libada \
    maybe-check-target-libgomp \
    maybe-check-target-libitm \
    maybe-check-target-libatomic

do-check:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) check-host check-target

# Automated reporting of test results.

warning.log: build.log
	$(srcdir)/contrib/warn_summary build.log > $@

mail-report.log:
	if test x'$(BOOT_CFLAGS)' != x''; then \
	    BOOT_CFLAGS='$(BOOT_CFLAGS)'; export BOOT_CFLAGS; \
	fi; \
	$(srcdir)/contrib/test_summary -t >$@
	chmod +x $@
	echo If you really want to send e-mail, run ./$@ now

mail-report-with-warnings.log: warning.log
	if test x'$(BOOT_CFLAGS)' != x''; then \
	    BOOT_CFLAGS='$(BOOT_CFLAGS)'; export BOOT_CFLAGS; \
	fi; \
	$(srcdir)/contrib/test_summary -t -i warning.log >$@
	chmod +x $@
	echo If you really want to send e-mail, run ./$@ now

# Installation targets.

.PHONY: install uninstall
install:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) installdirs install-host install-target

.PHONY: install-host-nogcc
install-host-nogcc:  \
    maybe-install-bfd \
    maybe-install-opcodes \
    maybe-install-binutils \
    maybe-install-bison \
    maybe-install-cgen \
    maybe-install-dejagnu \
    maybe-install-etc \
    maybe-install-fastjar \
    maybe-install-fixincludes \
    maybe-install-flex \
    maybe-install-gas \
    maybe-install-gmp \
    maybe-install-mpfr \
    maybe-install-mpc \
    maybe-install-isl \
    maybe-install-cloog \
    maybe-install-libelf \
    maybe-install-gold \
    maybe-install-gprof \
    maybe-install-intl \
    maybe-install-tcl \
    maybe-install-itcl \
    maybe-install-ld \
    maybe-install-libbacktrace \
    maybe-install-libcpp \
    maybe-install-libdecnumber \
    maybe-install-libgui \
    maybe-install-libiberty \
    maybe-install-libiconv \
    maybe-install-m4 \
    maybe-install-readline \
    maybe-install-sid \
    maybe-install-sim \
    maybe-install-texinfo \
    maybe-install-zlib \
    maybe-install-gdb \
    maybe-install-expect \
    maybe-install-guile \
    maybe-install-tk \
    maybe-install-libtermcap \
    maybe-install-utils \
    maybe-install-gnattools \
    maybe-install-lto-plugin

.PHONY: install-host
install-host:  \
    maybe-install-bfd \
    maybe-install-opcodes \
    maybe-install-binutils \
    maybe-install-bison \
    maybe-install-cgen \
    maybe-install-dejagnu \
    maybe-install-etc \
    maybe-install-fastjar \
    maybe-install-fixincludes \
    maybe-install-flex \
    maybe-install-gas \
    maybe-install-gcc \
    maybe-install-gmp \
    maybe-install-mpfr \
    maybe-install-mpc \
    maybe-install-isl \
    maybe-install-cloog \
    maybe-install-libelf \
    maybe-install-gold \
    maybe-install-gprof \
    maybe-install-intl \
    maybe-install-tcl \
    maybe-install-itcl \
    maybe-install-ld \
    maybe-install-libbacktrace \
    maybe-install-libcpp \
    maybe-install-libdecnumber \
    maybe-install-libgui \
    maybe-install-libiberty \
    maybe-install-libiconv \
    maybe-install-m4 \
    maybe-install-readline \
    maybe-install-sid \
    maybe-install-sim \
    maybe-install-texinfo \
    maybe-install-zlib \
    maybe-install-gdb \
    maybe-install-expect \
    maybe-install-guile \
    maybe-install-tk \
    maybe-install-libtermcap \
    maybe-install-utils \
    maybe-install-gnattools \
    maybe-install-lto-plugin

.PHONY: install-target
install-target:  \
    maybe-install-target-libstdc++-v3 \
    maybe-install-target-libmudflap \
    maybe-install-target-libsanitizer \
    maybe-install-target-libvtv \
    maybe-install-target-libssp \
    maybe-install-target-newlib \
    maybe-install-target-libgcc \
    maybe-install-target-libbacktrace \
    maybe-install-target-libquadmath \
    maybe-install-target-libgfortran \
    maybe-install-target-libobjc \
    maybe-install-target-libgo \
    maybe-install-target-libtermcap \
    maybe-install-target-winsup \
    maybe-install-target-libgloss \
    maybe-install-target-libffi \
    maybe-install-target-libjava \
    maybe-install-target-zlib \
    maybe-install-target-boehm-gc \
    maybe-install-target-rda \
    maybe-install-target-libada \
    maybe-install-target-libgomp \
    maybe-install-target-libitm \
    maybe-install-target-libatomic

uninstall:
	@echo "the uninstall target is not supported in this tree"

.PHONY: install.all
install.all: install-no-fixedincludes
	@if [ -f ./gcc/Makefile ] ; then \
		r=`${PWD_COMMAND}` ; export r ; \
		s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
		$(HOST_EXPORTS) \
		(cd ./gcc && \
		$(MAKE) $(FLAGS_TO_PASS) install-headers) ; \
	else \
		true ; \
	fi

# install-no-fixedincludes is used to allow the elaboration of binary packages
# suitable for distribution, where we cannot include the fixed system header
# files.
.PHONY: install-no-fixedincludes
install-no-fixedincludes: installdirs install-host-nogcc \
	install-target gcc-install-no-fixedincludes

.PHONY: install-strip
install-strip:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) installdirs install-strip-host install-strip-target

.PHONY: install-strip-host
install-strip-host:  \
    maybe-install-strip-bfd \
    maybe-install-strip-opcodes \
    maybe-install-strip-binutils \
    maybe-install-strip-bison \
    maybe-install-strip-cgen \
    maybe-install-strip-dejagnu \
    maybe-install-strip-etc \
    maybe-install-strip-fastjar \
    maybe-install-strip-fixincludes \
    maybe-install-strip-flex \
    maybe-install-strip-gas \
    maybe-install-strip-gcc \
    maybe-install-strip-gmp \
    maybe-install-strip-mpfr \
    maybe-install-strip-mpc \
    maybe-install-strip-isl \
    maybe-install-strip-cloog \
    maybe-install-strip-libelf \
    maybe-install-strip-gold \
    maybe-install-strip-gprof \
    maybe-install-strip-intl \
    maybe-install-strip-tcl \
    maybe-install-strip-itcl \
    maybe-install-strip-ld \
    maybe-install-strip-libbacktrace \
    maybe-install-strip-libcpp \
    maybe-install-strip-libdecnumber \
    maybe-install-strip-libgui \
    maybe-install-strip-libiberty \
    maybe-install-strip-libiconv \
    maybe-install-strip-m4 \
    maybe-install-strip-readline \
    maybe-install-strip-sid \
    maybe-install-strip-sim \
    maybe-install-strip-texinfo \
    maybe-install-strip-zlib \
    maybe-install-strip-gdb \
    maybe-install-strip-expect \
    maybe-install-strip-guile \
    maybe-install-strip-tk \
    maybe-install-strip-libtermcap \
    maybe-install-strip-utils \
    maybe-install-strip-gnattools \
    maybe-install-strip-lto-plugin

.PHONY: install-strip-target
install-strip-target:  \
    maybe-install-strip-target-libstdc++-v3 \
    maybe-install-strip-target-libmudflap \
    maybe-install-strip-target-libsanitizer \
    maybe-install-strip-target-libvtv \
    maybe-install-strip-target-libssp \
    maybe-install-strip-target-newlib \
    maybe-install-strip-target-libgcc \
    maybe-install-strip-target-libbacktrace \
    maybe-install-strip-target-libquadmath \
    maybe-install-strip-target-libgfortran \
    maybe-install-strip-target-libobjc \
    maybe-install-strip-target-libgo \
    maybe-install-strip-target-libtermcap \
    maybe-install-strip-target-winsup \
    maybe-install-strip-target-libgloss \
    maybe-install-strip-target-libffi \
    maybe-install-strip-target-libjava \
    maybe-install-strip-target-zlib \
    maybe-install-strip-target-boehm-gc \
    maybe-install-strip-target-rda \
    maybe-install-strip-target-libada \
    maybe-install-strip-target-libgomp \
    maybe-install-strip-target-libitm \
    maybe-install-strip-target-libatomic


### other supporting targets

MAKEDIRS= \
	$(DESTDIR)$(prefix) \
	$(DESTDIR)$(exec_prefix)
.PHONY: installdirs
installdirs: mkinstalldirs
	$(SHELL) $(srcdir)/mkinstalldirs $(MAKEDIRS)

dir.info: do-install-info
	if [ -f $(srcdir)/texinfo/gen-info-dir ] ; then \
	  $(srcdir)/texinfo/gen-info-dir $(DESTDIR)$(infodir) $(srcdir)/texinfo/dir.info-template > dir.info.new ; \
	  mv -f dir.info.new dir.info ; \
	else true ; \
	fi

dist:
	@echo "Building a full distribution of this tree isn't done"
	@echo "via 'make dist'.  Check out the etc/ subdirectory" 

etags tags: TAGS

# Right now this just builds TAGS in each subdirectory.  emacs19 has the
# ability to use several tags files at once, so there is probably no need
# to combine them into one big TAGS file (like CVS 1.3 does).  We could
# (if we felt like it) have this Makefile write a piece of elisp which
# the user could load to tell emacs19 where all the TAGS files we just
# built are.
TAGS: do-TAGS

# ------------------------------------
# Macros for configure and all targets
# ------------------------------------





# --------------------------------------
# Modules which run on the build machine
# --------------------------------------


.PHONY: configure-build-libiberty maybe-configure-build-libiberty
maybe-configure-build-libiberty:





.PHONY: all-build-libiberty maybe-all-build-libiberty
maybe-all-build-libiberty:





.PHONY: configure-build-bison maybe-configure-build-bison
maybe-configure-build-bison:





.PHONY: all-build-bison maybe-all-build-bison
maybe-all-build-bison:





.PHONY: configure-build-flex maybe-configure-build-flex
maybe-configure-build-flex:





.PHONY: all-build-flex maybe-all-build-flex
maybe-all-build-flex:





.PHONY: configure-build-m4 maybe-configure-build-m4
maybe-configure-build-m4:





.PHONY: all-build-m4 maybe-all-build-m4
maybe-all-build-m4:





.PHONY: configure-build-texinfo maybe-configure-build-texinfo
maybe-configure-build-texinfo:





.PHONY: all-build-texinfo maybe-all-build-texinfo
maybe-all-build-texinfo:





.PHONY: configure-build-fixincludes maybe-configure-build-fixincludes
maybe-configure-build-fixincludes:





.PHONY: all-build-fixincludes maybe-all-build-fixincludes
maybe-all-build-fixincludes:





# --------------------------------------
# Modules which run on the host machine
# --------------------------------------


.PHONY: configure-bfd maybe-configure-bfd
maybe-configure-bfd:



.PHONY: configure-stage1-bfd maybe-configure-stage1-bfd
maybe-configure-stage1-bfd:

.PHONY: configure-stage2-bfd maybe-configure-stage2-bfd
maybe-configure-stage2-bfd:

.PHONY: configure-stage3-bfd maybe-configure-stage3-bfd
maybe-configure-stage3-bfd:

.PHONY: configure-stage4-bfd maybe-configure-stage4-bfd
maybe-configure-stage4-bfd:

.PHONY: configure-stageprofile-bfd maybe-configure-stageprofile-bfd
maybe-configure-stageprofile-bfd:

.PHONY: configure-stagefeedback-bfd maybe-configure-stagefeedback-bfd
maybe-configure-stagefeedback-bfd:





.PHONY: all-bfd maybe-all-bfd
maybe-all-bfd:



.PHONY: all-stage1-bfd maybe-all-stage1-bfd
.PHONY: clean-stage1-bfd maybe-clean-stage1-bfd
maybe-all-stage1-bfd:
maybe-clean-stage1-bfd:


.PHONY: all-stage2-bfd maybe-all-stage2-bfd
.PHONY: clean-stage2-bfd maybe-clean-stage2-bfd
maybe-all-stage2-bfd:
maybe-clean-stage2-bfd:


.PHONY: all-stage3-bfd maybe-all-stage3-bfd
.PHONY: clean-stage3-bfd maybe-clean-stage3-bfd
maybe-all-stage3-bfd:
maybe-clean-stage3-bfd:


.PHONY: all-stage4-bfd maybe-all-stage4-bfd
.PHONY: clean-stage4-bfd maybe-clean-stage4-bfd
maybe-all-stage4-bfd:
maybe-clean-stage4-bfd:


.PHONY: all-stageprofile-bfd maybe-all-stageprofile-bfd
.PHONY: clean-stageprofile-bfd maybe-clean-stageprofile-bfd
maybe-all-stageprofile-bfd:
maybe-clean-stageprofile-bfd:


.PHONY: all-stagefeedback-bfd maybe-all-stagefeedback-bfd
.PHONY: clean-stagefeedback-bfd maybe-clean-stagefeedback-bfd
maybe-all-stagefeedback-bfd:
maybe-clean-stagefeedback-bfd:





.PHONY: check-bfd maybe-check-bfd
maybe-check-bfd:

.PHONY: install-bfd maybe-install-bfd
maybe-install-bfd:

.PHONY: install-strip-bfd maybe-install-strip-bfd
maybe-install-strip-bfd:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-bfd info-bfd
maybe-info-bfd:

.PHONY: maybe-dvi-bfd dvi-bfd
maybe-dvi-bfd:

.PHONY: maybe-pdf-bfd pdf-bfd
maybe-pdf-bfd:

.PHONY: maybe-html-bfd html-bfd
maybe-html-bfd:

.PHONY: maybe-TAGS-bfd TAGS-bfd
maybe-TAGS-bfd:

.PHONY: maybe-install-info-bfd install-info-bfd
maybe-install-info-bfd:

.PHONY: maybe-install-pdf-bfd install-pdf-bfd
maybe-install-pdf-bfd:

.PHONY: maybe-install-html-bfd install-html-bfd
maybe-install-html-bfd:

.PHONY: maybe-installcheck-bfd installcheck-bfd
maybe-installcheck-bfd:

.PHONY: maybe-mostlyclean-bfd mostlyclean-bfd
maybe-mostlyclean-bfd:

.PHONY: maybe-clean-bfd clean-bfd
maybe-clean-bfd:

.PHONY: maybe-distclean-bfd distclean-bfd
maybe-distclean-bfd:

.PHONY: maybe-maintainer-clean-bfd maintainer-clean-bfd
maybe-maintainer-clean-bfd:



.PHONY: configure-opcodes maybe-configure-opcodes
maybe-configure-opcodes:



.PHONY: configure-stage1-opcodes maybe-configure-stage1-opcodes
maybe-configure-stage1-opcodes:

.PHONY: configure-stage2-opcodes maybe-configure-stage2-opcodes
maybe-configure-stage2-opcodes:

.PHONY: configure-stage3-opcodes maybe-configure-stage3-opcodes
maybe-configure-stage3-opcodes:

.PHONY: configure-stage4-opcodes maybe-configure-stage4-opcodes
maybe-configure-stage4-opcodes:

.PHONY: configure-stageprofile-opcodes maybe-configure-stageprofile-opcodes
maybe-configure-stageprofile-opcodes:

.PHONY: configure-stagefeedback-opcodes maybe-configure-stagefeedback-opcodes
maybe-configure-stagefeedback-opcodes:





.PHONY: all-opcodes maybe-all-opcodes
maybe-all-opcodes:



.PHONY: all-stage1-opcodes maybe-all-stage1-opcodes
.PHONY: clean-stage1-opcodes maybe-clean-stage1-opcodes
maybe-all-stage1-opcodes:
maybe-clean-stage1-opcodes:


.PHONY: all-stage2-opcodes maybe-all-stage2-opcodes
.PHONY: clean-stage2-opcodes maybe-clean-stage2-opcodes
maybe-all-stage2-opcodes:
maybe-clean-stage2-opcodes:


.PHONY: all-stage3-opcodes maybe-all-stage3-opcodes
.PHONY: clean-stage3-opcodes maybe-clean-stage3-opcodes
maybe-all-stage3-opcodes:
maybe-clean-stage3-opcodes:


.PHONY: all-stage4-opcodes maybe-all-stage4-opcodes
.PHONY: clean-stage4-opcodes maybe-clean-stage4-opcodes
maybe-all-stage4-opcodes:
maybe-clean-stage4-opcodes:


.PHONY: all-stageprofile-opcodes maybe-all-stageprofile-opcodes
.PHONY: clean-stageprofile-opcodes maybe-clean-stageprofile-opcodes
maybe-all-stageprofile-opcodes:
maybe-clean-stageprofile-opcodes:


.PHONY: all-stagefeedback-opcodes maybe-all-stagefeedback-opcodes
.PHONY: clean-stagefeedback-opcodes maybe-clean-stagefeedback-opcodes
maybe-all-stagefeedback-opcodes:
maybe-clean-stagefeedback-opcodes:





.PHONY: check-opcodes maybe-check-opcodes
maybe-check-opcodes:

.PHONY: install-opcodes maybe-install-opcodes
maybe-install-opcodes:

.PHONY: install-strip-opcodes maybe-install-strip-opcodes
maybe-install-strip-opcodes:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-opcodes info-opcodes
maybe-info-opcodes:

.PHONY: maybe-dvi-opcodes dvi-opcodes
maybe-dvi-opcodes:

.PHONY: maybe-pdf-opcodes pdf-opcodes
maybe-pdf-opcodes:

.PHONY: maybe-html-opcodes html-opcodes
maybe-html-opcodes:

.PHONY: maybe-TAGS-opcodes TAGS-opcodes
maybe-TAGS-opcodes:

.PHONY: maybe-install-info-opcodes install-info-opcodes
maybe-install-info-opcodes:

.PHONY: maybe-install-pdf-opcodes install-pdf-opcodes
maybe-install-pdf-opcodes:

.PHONY: maybe-install-html-opcodes install-html-opcodes
maybe-install-html-opcodes:

.PHONY: maybe-installcheck-opcodes installcheck-opcodes
maybe-installcheck-opcodes:

.PHONY: maybe-mostlyclean-opcodes mostlyclean-opcodes
maybe-mostlyclean-opcodes:

.PHONY: maybe-clean-opcodes clean-opcodes
maybe-clean-opcodes:

.PHONY: maybe-distclean-opcodes distclean-opcodes
maybe-distclean-opcodes:

.PHONY: maybe-maintainer-clean-opcodes maintainer-clean-opcodes
maybe-maintainer-clean-opcodes:



.PHONY: configure-binutils maybe-configure-binutils
maybe-configure-binutils:



.PHONY: configure-stage1-binutils maybe-configure-stage1-binutils
maybe-configure-stage1-binutils:

.PHONY: configure-stage2-binutils maybe-configure-stage2-binutils
maybe-configure-stage2-binutils:

.PHONY: configure-stage3-binutils maybe-configure-stage3-binutils
maybe-configure-stage3-binutils:

.PHONY: configure-stage4-binutils maybe-configure-stage4-binutils
maybe-configure-stage4-binutils:

.PHONY: configure-stageprofile-binutils maybe-configure-stageprofile-binutils
maybe-configure-stageprofile-binutils:

.PHONY: configure-stagefeedback-binutils maybe-configure-stagefeedback-binutils
maybe-configure-stagefeedback-binutils:





.PHONY: all-binutils maybe-all-binutils
maybe-all-binutils:



.PHONY: all-stage1-binutils maybe-all-stage1-binutils
.PHONY: clean-stage1-binutils maybe-clean-stage1-binutils
maybe-all-stage1-binutils:
maybe-clean-stage1-binutils:


.PHONY: all-stage2-binutils maybe-all-stage2-binutils
.PHONY: clean-stage2-binutils maybe-clean-stage2-binutils
maybe-all-stage2-binutils:
maybe-clean-stage2-binutils:


.PHONY: all-stage3-binutils maybe-all-stage3-binutils
.PHONY: clean-stage3-binutils maybe-clean-stage3-binutils
maybe-all-stage3-binutils:
maybe-clean-stage3-binutils:


.PHONY: all-stage4-binutils maybe-all-stage4-binutils
.PHONY: clean-stage4-binutils maybe-clean-stage4-binutils
maybe-all-stage4-binutils:
maybe-clean-stage4-binutils:


.PHONY: all-stageprofile-binutils maybe-all-stageprofile-binutils
.PHONY: clean-stageprofile-binutils maybe-clean-stageprofile-binutils
maybe-all-stageprofile-binutils:
maybe-clean-stageprofile-binutils:


.PHONY: all-stagefeedback-binutils maybe-all-stagefeedback-binutils
.PHONY: clean-stagefeedback-binutils maybe-clean-stagefeedback-binutils
maybe-all-stagefeedback-binutils:
maybe-clean-stagefeedback-binutils:





.PHONY: check-binutils maybe-check-binutils
maybe-check-binutils:

.PHONY: install-binutils maybe-install-binutils
maybe-install-binutils:

.PHONY: install-strip-binutils maybe-install-strip-binutils
maybe-install-strip-binutils:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-binutils info-binutils
maybe-info-binutils:

.PHONY: maybe-dvi-binutils dvi-binutils
maybe-dvi-binutils:

.PHONY: maybe-pdf-binutils pdf-binutils
maybe-pdf-binutils:

.PHONY: maybe-html-binutils html-binutils
maybe-html-binutils:

.PHONY: maybe-TAGS-binutils TAGS-binutils
maybe-TAGS-binutils:

.PHONY: maybe-install-info-binutils install-info-binutils
maybe-install-info-binutils:

.PHONY: maybe-install-pdf-binutils install-pdf-binutils
maybe-install-pdf-binutils:

.PHONY: maybe-install-html-binutils install-html-binutils
maybe-install-html-binutils:

.PHONY: maybe-installcheck-binutils installcheck-binutils
maybe-installcheck-binutils:

.PHONY: maybe-mostlyclean-binutils mostlyclean-binutils
maybe-mostlyclean-binutils:

.PHONY: maybe-clean-binutils clean-binutils
maybe-clean-binutils:

.PHONY: maybe-distclean-binutils distclean-binutils
maybe-distclean-binutils:

.PHONY: maybe-maintainer-clean-binutils maintainer-clean-binutils
maybe-maintainer-clean-binutils:



.PHONY: configure-bison maybe-configure-bison
maybe-configure-bison:





.PHONY: all-bison maybe-all-bison
maybe-all-bison:




.PHONY: check-bison maybe-check-bison
maybe-check-bison:

.PHONY: install-bison maybe-install-bison
maybe-install-bison:

.PHONY: install-strip-bison maybe-install-strip-bison
maybe-install-strip-bison:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-bison info-bison
maybe-info-bison:

.PHONY: maybe-dvi-bison dvi-bison
maybe-dvi-bison:

.PHONY: maybe-pdf-bison pdf-bison
maybe-pdf-bison:

.PHONY: maybe-html-bison html-bison
maybe-html-bison:

.PHONY: maybe-TAGS-bison TAGS-bison
maybe-TAGS-bison:

.PHONY: maybe-install-info-bison install-info-bison
maybe-install-info-bison:

.PHONY: maybe-install-pdf-bison install-pdf-bison
maybe-install-pdf-bison:

.PHONY: maybe-install-html-bison install-html-bison
maybe-install-html-bison:

.PHONY: maybe-installcheck-bison installcheck-bison
maybe-installcheck-bison:

.PHONY: maybe-mostlyclean-bison mostlyclean-bison
maybe-mostlyclean-bison:

.PHONY: maybe-clean-bison clean-bison
maybe-clean-bison:

.PHONY: maybe-distclean-bison distclean-bison
maybe-distclean-bison:

.PHONY: maybe-maintainer-clean-bison maintainer-clean-bison
maybe-maintainer-clean-bison:



.PHONY: configure-cgen maybe-configure-cgen
maybe-configure-cgen:





.PHONY: all-cgen maybe-all-cgen
maybe-all-cgen:




.PHONY: check-cgen maybe-check-cgen
maybe-check-cgen:

.PHONY: install-cgen maybe-install-cgen
maybe-install-cgen:

.PHONY: install-strip-cgen maybe-install-strip-cgen
maybe-install-strip-cgen:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-cgen info-cgen
maybe-info-cgen:

.PHONY: maybe-dvi-cgen dvi-cgen
maybe-dvi-cgen:

.PHONY: maybe-pdf-cgen pdf-cgen
maybe-pdf-cgen:

.PHONY: maybe-html-cgen html-cgen
maybe-html-cgen:

.PHONY: maybe-TAGS-cgen TAGS-cgen
maybe-TAGS-cgen:

.PHONY: maybe-install-info-cgen install-info-cgen
maybe-install-info-cgen:

.PHONY: maybe-install-pdf-cgen install-pdf-cgen
maybe-install-pdf-cgen:

.PHONY: maybe-install-html-cgen install-html-cgen
maybe-install-html-cgen:

.PHONY: maybe-installcheck-cgen installcheck-cgen
maybe-installcheck-cgen:

.PHONY: maybe-mostlyclean-cgen mostlyclean-cgen
maybe-mostlyclean-cgen:

.PHONY: maybe-clean-cgen clean-cgen
maybe-clean-cgen:

.PHONY: maybe-distclean-cgen distclean-cgen
maybe-distclean-cgen:

.PHONY: maybe-maintainer-clean-cgen maintainer-clean-cgen
maybe-maintainer-clean-cgen:



.PHONY: configure-dejagnu maybe-configure-dejagnu
maybe-configure-dejagnu:





.PHONY: all-dejagnu maybe-all-dejagnu
maybe-all-dejagnu:




.PHONY: check-dejagnu maybe-check-dejagnu
maybe-check-dejagnu:

.PHONY: install-dejagnu maybe-install-dejagnu
maybe-install-dejagnu:

.PHONY: install-strip-dejagnu maybe-install-strip-dejagnu
maybe-install-strip-dejagnu:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-dejagnu info-dejagnu
maybe-info-dejagnu:

.PHONY: maybe-dvi-dejagnu dvi-dejagnu
maybe-dvi-dejagnu:

.PHONY: maybe-pdf-dejagnu pdf-dejagnu
maybe-pdf-dejagnu:

.PHONY: maybe-html-dejagnu html-dejagnu
maybe-html-dejagnu:

.PHONY: maybe-TAGS-dejagnu TAGS-dejagnu
maybe-TAGS-dejagnu:

.PHONY: maybe-install-info-dejagnu install-info-dejagnu
maybe-install-info-dejagnu:

.PHONY: maybe-install-pdf-dejagnu install-pdf-dejagnu
maybe-install-pdf-dejagnu:

.PHONY: maybe-install-html-dejagnu install-html-dejagnu
maybe-install-html-dejagnu:

.PHONY: maybe-installcheck-dejagnu installcheck-dejagnu
maybe-installcheck-dejagnu:

.PHONY: maybe-mostlyclean-dejagnu mostlyclean-dejagnu
maybe-mostlyclean-dejagnu:

.PHONY: maybe-clean-dejagnu clean-dejagnu
maybe-clean-dejagnu:

.PHONY: maybe-distclean-dejagnu distclean-dejagnu
maybe-distclean-dejagnu:

.PHONY: maybe-maintainer-clean-dejagnu maintainer-clean-dejagnu
maybe-maintainer-clean-dejagnu:



.PHONY: configure-etc maybe-configure-etc
maybe-configure-etc:
maybe-configure-etc: configure-etc
configure-etc: 
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	test ! -f $(HOST_SUBDIR)/etc/Makefile || exit 0; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)/etc ; \
	$(HOST_EXPORTS)  \
	echo Configuring in $(HOST_SUBDIR)/etc; \
	cd "$(HOST_SUBDIR)/etc" || exit 1; \
	case $(srcdir) in \
	  /* | [A-Za-z]:[\\/]*) topdir=$(srcdir) ;; \
	  *) topdir=`echo $(HOST_SUBDIR)/etc/ | \
		sed -e 's,\./,,g' -e 's,[^/]*/,../,g' `$(srcdir) ;; \
	esac; \
	srcdiroption="--srcdir=$${topdir}/etc"; \
	libsrcdir="$$s/etc"; \
	$(SHELL) $${libsrcdir}/configure \
	  $(HOST_CONFIGARGS) --build=${build_alias} --host=${host_alias} \
	  --target=${target_alias} $${srcdiroption}  \
	  || exit 1





.PHONY: all-etc maybe-all-etc
maybe-all-etc:
TARGET-etc=all
maybe-all-etc: all-etc
all-etc: configure-etc
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS)  \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) $(EXTRA_HOST_FLAGS) $(STAGE1_FLAGS_TO_PASS)  \
		$(TARGET-etc))




.PHONY: check-etc maybe-check-etc
maybe-check-etc:
maybe-check-etc: check-etc

check-etc:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(FLAGS_TO_PASS)  check)


.PHONY: install-etc maybe-install-etc
maybe-install-etc:
maybe-install-etc: install-etc

install-etc: installdirs
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(FLAGS_TO_PASS)  install)


.PHONY: install-strip-etc maybe-install-strip-etc
maybe-install-strip-etc:
maybe-install-strip-etc: install-strip-etc

install-strip-etc: installdirs
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(FLAGS_TO_PASS)  install-strip)


# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-etc info-etc
maybe-info-etc:
maybe-info-etc: info-etc

info-etc: \
    configure-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing info in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          info) \
	  || exit 1


.PHONY: maybe-dvi-etc dvi-etc
maybe-dvi-etc:
maybe-dvi-etc: dvi-etc

dvi-etc: \
    configure-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing dvi in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          dvi) \
	  || exit 1


.PHONY: maybe-pdf-etc pdf-etc
maybe-pdf-etc:
maybe-pdf-etc: pdf-etc

pdf-etc: \
    configure-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing pdf in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          pdf) \
	  || exit 1


.PHONY: maybe-html-etc html-etc
maybe-html-etc:
maybe-html-etc: html-etc

html-etc: \
    configure-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing html in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          html) \
	  || exit 1


.PHONY: maybe-TAGS-etc TAGS-etc
maybe-TAGS-etc:
maybe-TAGS-etc: TAGS-etc

TAGS-etc: \
    configure-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing TAGS in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          TAGS) \
	  || exit 1


.PHONY: maybe-install-info-etc install-info-etc
maybe-install-info-etc:
maybe-install-info-etc: install-info-etc

install-info-etc: \
    configure-etc \
    info-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing install-info in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          install-info) \
	  || exit 1


.PHONY: maybe-install-pdf-etc install-pdf-etc
maybe-install-pdf-etc:
maybe-install-pdf-etc: install-pdf-etc

install-pdf-etc: \
    configure-etc \
    pdf-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing install-pdf in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          install-pdf) \
	  || exit 1


.PHONY: maybe-install-html-etc install-html-etc
maybe-install-html-etc:
maybe-install-html-etc: install-html-etc

install-html-etc: \
    configure-etc \
    html-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing install-html in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          install-html) \
	  || exit 1


.PHONY: maybe-installcheck-etc installcheck-etc
maybe-installcheck-etc:
maybe-installcheck-etc: installcheck-etc

installcheck-etc: \
    configure-etc 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing installcheck in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          installcheck) \
	  || exit 1


.PHONY: maybe-mostlyclean-etc mostlyclean-etc
maybe-mostlyclean-etc:
maybe-mostlyclean-etc: mostlyclean-etc

mostlyclean-etc: 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing mostlyclean in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          mostlyclean) \
	  || exit 1


.PHONY: maybe-clean-etc clean-etc
maybe-clean-etc:
maybe-clean-etc: clean-etc

clean-etc: 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing clean in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          clean) \
	  || exit 1


.PHONY: maybe-distclean-etc distclean-etc
maybe-distclean-etc:
maybe-distclean-etc: distclean-etc

distclean-etc: 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing distclean in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          distclean) \
	  || exit 1


.PHONY: maybe-maintainer-clean-etc maintainer-clean-etc
maybe-maintainer-clean-etc:
maybe-maintainer-clean-etc: maintainer-clean-etc

maintainer-clean-etc: 
	@: $(MAKE); $(unstage)
	@[ -f ./etc/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) ; do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing maintainer-clean in etc" ; \
	(cd $(HOST_SUBDIR)/etc && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	          maintainer-clean) \
	  || exit 1




.PHONY: configure-fastjar maybe-configure-fastjar
maybe-configure-fastjar:





.PHONY: all-fastjar maybe-all-fastjar
maybe-all-fastjar:




.PHONY: check-fastjar maybe-check-fastjar
maybe-check-fastjar:

.PHONY: install-fastjar maybe-install-fastjar
maybe-install-fastjar:

.PHONY: install-strip-fastjar maybe-install-strip-fastjar
maybe-install-strip-fastjar:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-fastjar info-fastjar
maybe-info-fastjar:

.PHONY: maybe-dvi-fastjar dvi-fastjar
maybe-dvi-fastjar:

.PHONY: maybe-pdf-fastjar pdf-fastjar
maybe-pdf-fastjar:

.PHONY: maybe-html-fastjar html-fastjar
maybe-html-fastjar:

.PHONY: maybe-TAGS-fastjar TAGS-fastjar
maybe-TAGS-fastjar:

.PHONY: maybe-install-info-fastjar install-info-fastjar
maybe-install-info-fastjar:

.PHONY: maybe-install-pdf-fastjar install-pdf-fastjar
maybe-install-pdf-fastjar:

.PHONY: maybe-install-html-fastjar install-html-fastjar
maybe-install-html-fastjar:

.PHONY: maybe-installcheck-fastjar installcheck-fastjar
maybe-installcheck-fastjar:

.PHONY: maybe-mostlyclean-fastjar mostlyclean-fastjar
maybe-mostlyclean-fastjar:

.PHONY: maybe-clean-fastjar clean-fastjar
maybe-clean-fastjar:

.PHONY: maybe-distclean-fastjar distclean-fastjar
maybe-distclean-fastjar:

.PHONY: maybe-maintainer-clean-fastjar maintainer-clean-fastjar
maybe-maintainer-clean-fastjar:



.PHONY: configure-fixincludes maybe-configure-fixincludes
maybe-configure-fixincludes:





.PHONY: all-fixincludes maybe-all-fixincludes
maybe-all-fixincludes:




.PHONY: check-fixincludes maybe-check-fixincludes
maybe-check-fixincludes:

.PHONY: install-fixincludes maybe-install-fixincludes
maybe-install-fixincludes:

.PHONY: install-strip-fixincludes maybe-install-strip-fixincludes
maybe-install-strip-fixincludes:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-fixincludes info-fixincludes
maybe-info-fixincludes:

.PHONY: maybe-dvi-fixincludes dvi-fixincludes
maybe-dvi-fixincludes:

.PHONY: maybe-pdf-fixincludes pdf-fixincludes
maybe-pdf-fixincludes:

.PHONY: maybe-html-fixincludes html-fixincludes
maybe-html-fixincludes:

.PHONY: maybe-TAGS-fixincludes TAGS-fixincludes
maybe-TAGS-fixincludes:

.PHONY: maybe-install-info-fixincludes install-info-fixincludes
maybe-install-info-fixincludes:

.PHONY: maybe-install-pdf-fixincludes install-pdf-fixincludes
maybe-install-pdf-fixincludes:

.PHONY: maybe-install-html-fixincludes install-html-fixincludes
maybe-install-html-fixincludes:

.PHONY: maybe-installcheck-fixincludes installcheck-fixincludes
maybe-installcheck-fixincludes:

.PHONY: maybe-mostlyclean-fixincludes mostlyclean-fixincludes
maybe-mostlyclean-fixincludes:

.PHONY: maybe-clean-fixincludes clean-fixincludes
maybe-clean-fixincludes:

.PHONY: maybe-distclean-fixincludes distclean-fixincludes
maybe-distclean-fixincludes:

.PHONY: maybe-maintainer-clean-fixincludes maintainer-clean-fixincludes
maybe-maintainer-clean-fixincludes:



.PHONY: configure-flex maybe-configure-flex
maybe-configure-flex:





.PHONY: all-flex maybe-all-flex
maybe-all-flex:




.PHONY: check-flex maybe-check-flex
maybe-check-flex:

.PHONY: install-flex maybe-install-flex
maybe-install-flex:

.PHONY: install-strip-flex maybe-install-strip-flex
maybe-install-strip-flex:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-flex info-flex
maybe-info-flex:

.PHONY: maybe-dvi-flex dvi-flex
maybe-dvi-flex:

.PHONY: maybe-pdf-flex pdf-flex
maybe-pdf-flex:

.PHONY: maybe-html-flex html-flex
maybe-html-flex:

.PHONY: maybe-TAGS-flex TAGS-flex
maybe-TAGS-flex:

.PHONY: maybe-install-info-flex install-info-flex
maybe-install-info-flex:

.PHONY: maybe-install-pdf-flex install-pdf-flex
maybe-install-pdf-flex:

.PHONY: maybe-install-html-flex install-html-flex
maybe-install-html-flex:

.PHONY: maybe-installcheck-flex installcheck-flex
maybe-installcheck-flex:

.PHONY: maybe-mostlyclean-flex mostlyclean-flex
maybe-mostlyclean-flex:

.PHONY: maybe-clean-flex clean-flex
maybe-clean-flex:

.PHONY: maybe-distclean-flex distclean-flex
maybe-distclean-flex:

.PHONY: maybe-maintainer-clean-flex maintainer-clean-flex
maybe-maintainer-clean-flex:



.PHONY: configure-gas maybe-configure-gas
maybe-configure-gas:



.PHONY: configure-stage1-gas maybe-configure-stage1-gas
maybe-configure-stage1-gas:

.PHONY: configure-stage2-gas maybe-configure-stage2-gas
maybe-configure-stage2-gas:

.PHONY: configure-stage3-gas maybe-configure-stage3-gas
maybe-configure-stage3-gas:

.PHONY: configure-stage4-gas maybe-configure-stage4-gas
maybe-configure-stage4-gas:

.PHONY: configure-stageprofile-gas maybe-configure-stageprofile-gas
maybe-configure-stageprofile-gas:

.PHONY: configure-stagefeedback-gas maybe-configure-stagefeedback-gas
maybe-configure-stagefeedback-gas:





.PHONY: all-gas maybe-all-gas
maybe-all-gas:



.PHONY: all-stage1-gas maybe-all-stage1-gas
.PHONY: clean-stage1-gas maybe-clean-stage1-gas
maybe-all-stage1-gas:
maybe-clean-stage1-gas:


.PHONY: all-stage2-gas maybe-all-stage2-gas
.PHONY: clean-stage2-gas maybe-clean-stage2-gas
maybe-all-stage2-gas:
maybe-clean-stage2-gas:


.PHONY: all-stage3-gas maybe-all-stage3-gas
.PHONY: clean-stage3-gas maybe-clean-stage3-gas
maybe-all-stage3-gas:
maybe-clean-stage3-gas:


.PHONY: all-stage4-gas maybe-all-stage4-gas
.PHONY: clean-stage4-gas maybe-clean-stage4-gas
maybe-all-stage4-gas:
maybe-clean-stage4-gas:


.PHONY: all-stageprofile-gas maybe-all-stageprofile-gas
.PHONY: clean-stageprofile-gas maybe-clean-stageprofile-gas
maybe-all-stageprofile-gas:
maybe-clean-stageprofile-gas:


.PHONY: all-stagefeedback-gas maybe-all-stagefeedback-gas
.PHONY: clean-stagefeedback-gas maybe-clean-stagefeedback-gas
maybe-all-stagefeedback-gas:
maybe-clean-stagefeedback-gas:





.PHONY: check-gas maybe-check-gas
maybe-check-gas:

.PHONY: install-gas maybe-install-gas
maybe-install-gas:

.PHONY: install-strip-gas maybe-install-strip-gas
maybe-install-strip-gas:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gas info-gas
maybe-info-gas:

.PHONY: maybe-dvi-gas dvi-gas
maybe-dvi-gas:

.PHONY: maybe-pdf-gas pdf-gas
maybe-pdf-gas:

.PHONY: maybe-html-gas html-gas
maybe-html-gas:

.PHONY: maybe-TAGS-gas TAGS-gas
maybe-TAGS-gas:

.PHONY: maybe-install-info-gas install-info-gas
maybe-install-info-gas:

.PHONY: maybe-install-pdf-gas install-pdf-gas
maybe-install-pdf-gas:

.PHONY: maybe-install-html-gas install-html-gas
maybe-install-html-gas:

.PHONY: maybe-installcheck-gas installcheck-gas
maybe-installcheck-gas:

.PHONY: maybe-mostlyclean-gas mostlyclean-gas
maybe-mostlyclean-gas:

.PHONY: maybe-clean-gas clean-gas
maybe-clean-gas:

.PHONY: maybe-distclean-gas distclean-gas
maybe-distclean-gas:

.PHONY: maybe-maintainer-clean-gas maintainer-clean-gas
maybe-maintainer-clean-gas:



.PHONY: configure-gcc maybe-configure-gcc
maybe-configure-gcc:



.PHONY: configure-stage1-gcc maybe-configure-stage1-gcc
maybe-configure-stage1-gcc:

.PHONY: configure-stage2-gcc maybe-configure-stage2-gcc
maybe-configure-stage2-gcc:

.PHONY: configure-stage3-gcc maybe-configure-stage3-gcc
maybe-configure-stage3-gcc:

.PHONY: configure-stage4-gcc maybe-configure-stage4-gcc
maybe-configure-stage4-gcc:

.PHONY: configure-stageprofile-gcc maybe-configure-stageprofile-gcc
maybe-configure-stageprofile-gcc:

.PHONY: configure-stagefeedback-gcc maybe-configure-stagefeedback-gcc
maybe-configure-stagefeedback-gcc:





.PHONY: all-gcc maybe-all-gcc
maybe-all-gcc:



.PHONY: all-stage1-gcc maybe-all-stage1-gcc
.PHONY: clean-stage1-gcc maybe-clean-stage1-gcc
maybe-all-stage1-gcc:
maybe-clean-stage1-gcc:


.PHONY: all-stage2-gcc maybe-all-stage2-gcc
.PHONY: clean-stage2-gcc maybe-clean-stage2-gcc
maybe-all-stage2-gcc:
maybe-clean-stage2-gcc:


.PHONY: all-stage3-gcc maybe-all-stage3-gcc
.PHONY: clean-stage3-gcc maybe-clean-stage3-gcc
maybe-all-stage3-gcc:
maybe-clean-stage3-gcc:


.PHONY: all-stage4-gcc maybe-all-stage4-gcc
.PHONY: clean-stage4-gcc maybe-clean-stage4-gcc
maybe-all-stage4-gcc:
maybe-clean-stage4-gcc:


.PHONY: all-stageprofile-gcc maybe-all-stageprofile-gcc
.PHONY: clean-stageprofile-gcc maybe-clean-stageprofile-gcc
maybe-all-stageprofile-gcc:
maybe-clean-stageprofile-gcc:


.PHONY: all-stagefeedback-gcc maybe-all-stagefeedback-gcc
.PHONY: clean-stagefeedback-gcc maybe-clean-stagefeedback-gcc
maybe-all-stagefeedback-gcc:
maybe-clean-stagefeedback-gcc:





.PHONY: check-gcc maybe-check-gcc
maybe-check-gcc:

.PHONY: install-gcc maybe-install-gcc
maybe-install-gcc:

.PHONY: install-strip-gcc maybe-install-strip-gcc
maybe-install-strip-gcc:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gcc info-gcc
maybe-info-gcc:

.PHONY: maybe-dvi-gcc dvi-gcc
maybe-dvi-gcc:

.PHONY: maybe-pdf-gcc pdf-gcc
maybe-pdf-gcc:

.PHONY: maybe-html-gcc html-gcc
maybe-html-gcc:

.PHONY: maybe-TAGS-gcc TAGS-gcc
maybe-TAGS-gcc:

.PHONY: maybe-install-info-gcc install-info-gcc
maybe-install-info-gcc:

.PHONY: maybe-install-pdf-gcc install-pdf-gcc
maybe-install-pdf-gcc:

.PHONY: maybe-install-html-gcc install-html-gcc
maybe-install-html-gcc:

.PHONY: maybe-installcheck-gcc installcheck-gcc
maybe-installcheck-gcc:

.PHONY: maybe-mostlyclean-gcc mostlyclean-gcc
maybe-mostlyclean-gcc:

.PHONY: maybe-clean-gcc clean-gcc
maybe-clean-gcc:

.PHONY: maybe-distclean-gcc distclean-gcc
maybe-distclean-gcc:

.PHONY: maybe-maintainer-clean-gcc maintainer-clean-gcc
maybe-maintainer-clean-gcc:



.PHONY: configure-gmp maybe-configure-gmp
maybe-configure-gmp:



.PHONY: configure-stage1-gmp maybe-configure-stage1-gmp
maybe-configure-stage1-gmp:

.PHONY: configure-stage2-gmp maybe-configure-stage2-gmp
maybe-configure-stage2-gmp:

.PHONY: configure-stage3-gmp maybe-configure-stage3-gmp
maybe-configure-stage3-gmp:

.PHONY: configure-stage4-gmp maybe-configure-stage4-gmp
maybe-configure-stage4-gmp:

.PHONY: configure-stageprofile-gmp maybe-configure-stageprofile-gmp
maybe-configure-stageprofile-gmp:

.PHONY: configure-stagefeedback-gmp maybe-configure-stagefeedback-gmp
maybe-configure-stagefeedback-gmp:





.PHONY: all-gmp maybe-all-gmp
maybe-all-gmp:



.PHONY: all-stage1-gmp maybe-all-stage1-gmp
.PHONY: clean-stage1-gmp maybe-clean-stage1-gmp
maybe-all-stage1-gmp:
maybe-clean-stage1-gmp:


.PHONY: all-stage2-gmp maybe-all-stage2-gmp
.PHONY: clean-stage2-gmp maybe-clean-stage2-gmp
maybe-all-stage2-gmp:
maybe-clean-stage2-gmp:


.PHONY: all-stage3-gmp maybe-all-stage3-gmp
.PHONY: clean-stage3-gmp maybe-clean-stage3-gmp
maybe-all-stage3-gmp:
maybe-clean-stage3-gmp:


.PHONY: all-stage4-gmp maybe-all-stage4-gmp
.PHONY: clean-stage4-gmp maybe-clean-stage4-gmp
maybe-all-stage4-gmp:
maybe-clean-stage4-gmp:


.PHONY: all-stageprofile-gmp maybe-all-stageprofile-gmp
.PHONY: clean-stageprofile-gmp maybe-clean-stageprofile-gmp
maybe-all-stageprofile-gmp:
maybe-clean-stageprofile-gmp:


.PHONY: all-stagefeedback-gmp maybe-all-stagefeedback-gmp
.PHONY: clean-stagefeedback-gmp maybe-clean-stagefeedback-gmp
maybe-all-stagefeedback-gmp:
maybe-clean-stagefeedback-gmp:





.PHONY: check-gmp maybe-check-gmp
maybe-check-gmp:

.PHONY: install-gmp maybe-install-gmp
maybe-install-gmp:

.PHONY: install-strip-gmp maybe-install-strip-gmp
maybe-install-strip-gmp:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gmp info-gmp
maybe-info-gmp:

.PHONY: maybe-dvi-gmp dvi-gmp
maybe-dvi-gmp:

.PHONY: maybe-pdf-gmp pdf-gmp
maybe-pdf-gmp:

.PHONY: maybe-html-gmp html-gmp
maybe-html-gmp:

.PHONY: maybe-TAGS-gmp TAGS-gmp
maybe-TAGS-gmp:

.PHONY: maybe-install-info-gmp install-info-gmp
maybe-install-info-gmp:

.PHONY: maybe-install-pdf-gmp install-pdf-gmp
maybe-install-pdf-gmp:

.PHONY: maybe-install-html-gmp install-html-gmp
maybe-install-html-gmp:

.PHONY: maybe-installcheck-gmp installcheck-gmp
maybe-installcheck-gmp:

.PHONY: maybe-mostlyclean-gmp mostlyclean-gmp
maybe-mostlyclean-gmp:

.PHONY: maybe-clean-gmp clean-gmp
maybe-clean-gmp:

.PHONY: maybe-distclean-gmp distclean-gmp
maybe-distclean-gmp:

.PHONY: maybe-maintainer-clean-gmp maintainer-clean-gmp
maybe-maintainer-clean-gmp:



.PHONY: configure-mpfr maybe-configure-mpfr
maybe-configure-mpfr:



.PHONY: configure-stage1-mpfr maybe-configure-stage1-mpfr
maybe-configure-stage1-mpfr:

.PHONY: configure-stage2-mpfr maybe-configure-stage2-mpfr
maybe-configure-stage2-mpfr:

.PHONY: configure-stage3-mpfr maybe-configure-stage3-mpfr
maybe-configure-stage3-mpfr:

.PHONY: configure-stage4-mpfr maybe-configure-stage4-mpfr
maybe-configure-stage4-mpfr:

.PHONY: configure-stageprofile-mpfr maybe-configure-stageprofile-mpfr
maybe-configure-stageprofile-mpfr:

.PHONY: configure-stagefeedback-mpfr maybe-configure-stagefeedback-mpfr
maybe-configure-stagefeedback-mpfr:





.PHONY: all-mpfr maybe-all-mpfr
maybe-all-mpfr:



.PHONY: all-stage1-mpfr maybe-all-stage1-mpfr
.PHONY: clean-stage1-mpfr maybe-clean-stage1-mpfr
maybe-all-stage1-mpfr:
maybe-clean-stage1-mpfr:


.PHONY: all-stage2-mpfr maybe-all-stage2-mpfr
.PHONY: clean-stage2-mpfr maybe-clean-stage2-mpfr
maybe-all-stage2-mpfr:
maybe-clean-stage2-mpfr:


.PHONY: all-stage3-mpfr maybe-all-stage3-mpfr
.PHONY: clean-stage3-mpfr maybe-clean-stage3-mpfr
maybe-all-stage3-mpfr:
maybe-clean-stage3-mpfr:


.PHONY: all-stage4-mpfr maybe-all-stage4-mpfr
.PHONY: clean-stage4-mpfr maybe-clean-stage4-mpfr
maybe-all-stage4-mpfr:
maybe-clean-stage4-mpfr:


.PHONY: all-stageprofile-mpfr maybe-all-stageprofile-mpfr
.PHONY: clean-stageprofile-mpfr maybe-clean-stageprofile-mpfr
maybe-all-stageprofile-mpfr:
maybe-clean-stageprofile-mpfr:


.PHONY: all-stagefeedback-mpfr maybe-all-stagefeedback-mpfr
.PHONY: clean-stagefeedback-mpfr maybe-clean-stagefeedback-mpfr
maybe-all-stagefeedback-mpfr:
maybe-clean-stagefeedback-mpfr:





.PHONY: check-mpfr maybe-check-mpfr
maybe-check-mpfr:

.PHONY: install-mpfr maybe-install-mpfr
maybe-install-mpfr:

.PHONY: install-strip-mpfr maybe-install-strip-mpfr
maybe-install-strip-mpfr:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-mpfr info-mpfr
maybe-info-mpfr:

.PHONY: maybe-dvi-mpfr dvi-mpfr
maybe-dvi-mpfr:

.PHONY: maybe-pdf-mpfr pdf-mpfr
maybe-pdf-mpfr:

.PHONY: maybe-html-mpfr html-mpfr
maybe-html-mpfr:

.PHONY: maybe-TAGS-mpfr TAGS-mpfr
maybe-TAGS-mpfr:

.PHONY: maybe-install-info-mpfr install-info-mpfr
maybe-install-info-mpfr:

.PHONY: maybe-install-pdf-mpfr install-pdf-mpfr
maybe-install-pdf-mpfr:

.PHONY: maybe-install-html-mpfr install-html-mpfr
maybe-install-html-mpfr:

.PHONY: maybe-installcheck-mpfr installcheck-mpfr
maybe-installcheck-mpfr:

.PHONY: maybe-mostlyclean-mpfr mostlyclean-mpfr
maybe-mostlyclean-mpfr:

.PHONY: maybe-clean-mpfr clean-mpfr
maybe-clean-mpfr:

.PHONY: maybe-distclean-mpfr distclean-mpfr
maybe-distclean-mpfr:

.PHONY: maybe-maintainer-clean-mpfr maintainer-clean-mpfr
maybe-maintainer-clean-mpfr:



.PHONY: configure-mpc maybe-configure-mpc
maybe-configure-mpc:



.PHONY: configure-stage1-mpc maybe-configure-stage1-mpc
maybe-configure-stage1-mpc:

.PHONY: configure-stage2-mpc maybe-configure-stage2-mpc
maybe-configure-stage2-mpc:

.PHONY: configure-stage3-mpc maybe-configure-stage3-mpc
maybe-configure-stage3-mpc:

.PHONY: configure-stage4-mpc maybe-configure-stage4-mpc
maybe-configure-stage4-mpc:

.PHONY: configure-stageprofile-mpc maybe-configure-stageprofile-mpc
maybe-configure-stageprofile-mpc:

.PHONY: configure-stagefeedback-mpc maybe-configure-stagefeedback-mpc
maybe-configure-stagefeedback-mpc:





.PHONY: all-mpc maybe-all-mpc
maybe-all-mpc:



.PHONY: all-stage1-mpc maybe-all-stage1-mpc
.PHONY: clean-stage1-mpc maybe-clean-stage1-mpc
maybe-all-stage1-mpc:
maybe-clean-stage1-mpc:


.PHONY: all-stage2-mpc maybe-all-stage2-mpc
.PHONY: clean-stage2-mpc maybe-clean-stage2-mpc
maybe-all-stage2-mpc:
maybe-clean-stage2-mpc:


.PHONY: all-stage3-mpc maybe-all-stage3-mpc
.PHONY: clean-stage3-mpc maybe-clean-stage3-mpc
maybe-all-stage3-mpc:
maybe-clean-stage3-mpc:


.PHONY: all-stage4-mpc maybe-all-stage4-mpc
.PHONY: clean-stage4-mpc maybe-clean-stage4-mpc
maybe-all-stage4-mpc:
maybe-clean-stage4-mpc:


.PHONY: all-stageprofile-mpc maybe-all-stageprofile-mpc
.PHONY: clean-stageprofile-mpc maybe-clean-stageprofile-mpc
maybe-all-stageprofile-mpc:
maybe-clean-stageprofile-mpc:


.PHONY: all-stagefeedback-mpc maybe-all-stagefeedback-mpc
.PHONY: clean-stagefeedback-mpc maybe-clean-stagefeedback-mpc
maybe-all-stagefeedback-mpc:
maybe-clean-stagefeedback-mpc:





.PHONY: check-mpc maybe-check-mpc
maybe-check-mpc:

.PHONY: install-mpc maybe-install-mpc
maybe-install-mpc:

.PHONY: install-strip-mpc maybe-install-strip-mpc
maybe-install-strip-mpc:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-mpc info-mpc
maybe-info-mpc:

.PHONY: maybe-dvi-mpc dvi-mpc
maybe-dvi-mpc:

.PHONY: maybe-pdf-mpc pdf-mpc
maybe-pdf-mpc:

.PHONY: maybe-html-mpc html-mpc
maybe-html-mpc:

.PHONY: maybe-TAGS-mpc TAGS-mpc
maybe-TAGS-mpc:

.PHONY: maybe-install-info-mpc install-info-mpc
maybe-install-info-mpc:

.PHONY: maybe-install-pdf-mpc install-pdf-mpc
maybe-install-pdf-mpc:

.PHONY: maybe-install-html-mpc install-html-mpc
maybe-install-html-mpc:

.PHONY: maybe-installcheck-mpc installcheck-mpc
maybe-installcheck-mpc:

.PHONY: maybe-mostlyclean-mpc mostlyclean-mpc
maybe-mostlyclean-mpc:

.PHONY: maybe-clean-mpc clean-mpc
maybe-clean-mpc:

.PHONY: maybe-distclean-mpc distclean-mpc
maybe-distclean-mpc:

.PHONY: maybe-maintainer-clean-mpc maintainer-clean-mpc
maybe-maintainer-clean-mpc:



.PHONY: configure-isl maybe-configure-isl
maybe-configure-isl:



.PHONY: configure-stage1-isl maybe-configure-stage1-isl
maybe-configure-stage1-isl:

.PHONY: configure-stage2-isl maybe-configure-stage2-isl
maybe-configure-stage2-isl:

.PHONY: configure-stage3-isl maybe-configure-stage3-isl
maybe-configure-stage3-isl:

.PHONY: configure-stage4-isl maybe-configure-stage4-isl
maybe-configure-stage4-isl:

.PHONY: configure-stageprofile-isl maybe-configure-stageprofile-isl
maybe-configure-stageprofile-isl:

.PHONY: configure-stagefeedback-isl maybe-configure-stagefeedback-isl
maybe-configure-stagefeedback-isl:





.PHONY: all-isl maybe-all-isl
maybe-all-isl:



.PHONY: all-stage1-isl maybe-all-stage1-isl
.PHONY: clean-stage1-isl maybe-clean-stage1-isl
maybe-all-stage1-isl:
maybe-clean-stage1-isl:


.PHONY: all-stage2-isl maybe-all-stage2-isl
.PHONY: clean-stage2-isl maybe-clean-stage2-isl
maybe-all-stage2-isl:
maybe-clean-stage2-isl:


.PHONY: all-stage3-isl maybe-all-stage3-isl
.PHONY: clean-stage3-isl maybe-clean-stage3-isl
maybe-all-stage3-isl:
maybe-clean-stage3-isl:


.PHONY: all-stage4-isl maybe-all-stage4-isl
.PHONY: clean-stage4-isl maybe-clean-stage4-isl
maybe-all-stage4-isl:
maybe-clean-stage4-isl:


.PHONY: all-stageprofile-isl maybe-all-stageprofile-isl
.PHONY: clean-stageprofile-isl maybe-clean-stageprofile-isl
maybe-all-stageprofile-isl:
maybe-clean-stageprofile-isl:


.PHONY: all-stagefeedback-isl maybe-all-stagefeedback-isl
.PHONY: clean-stagefeedback-isl maybe-clean-stagefeedback-isl
maybe-all-stagefeedback-isl:
maybe-clean-stagefeedback-isl:





.PHONY: check-isl maybe-check-isl
maybe-check-isl:

.PHONY: install-isl maybe-install-isl
maybe-install-isl:

.PHONY: install-strip-isl maybe-install-strip-isl
maybe-install-strip-isl:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-isl info-isl
maybe-info-isl:

.PHONY: maybe-dvi-isl dvi-isl
maybe-dvi-isl:

.PHONY: maybe-pdf-isl pdf-isl
maybe-pdf-isl:

.PHONY: maybe-html-isl html-isl
maybe-html-isl:

.PHONY: maybe-TAGS-isl TAGS-isl
maybe-TAGS-isl:

.PHONY: maybe-install-info-isl install-info-isl
maybe-install-info-isl:

.PHONY: maybe-install-pdf-isl install-pdf-isl
maybe-install-pdf-isl:

.PHONY: maybe-install-html-isl install-html-isl
maybe-install-html-isl:

.PHONY: maybe-installcheck-isl installcheck-isl
maybe-installcheck-isl:

.PHONY: maybe-mostlyclean-isl mostlyclean-isl
maybe-mostlyclean-isl:

.PHONY: maybe-clean-isl clean-isl
maybe-clean-isl:

.PHONY: maybe-distclean-isl distclean-isl
maybe-distclean-isl:

.PHONY: maybe-maintainer-clean-isl maintainer-clean-isl
maybe-maintainer-clean-isl:



.PHONY: configure-cloog maybe-configure-cloog
maybe-configure-cloog:



.PHONY: configure-stage1-cloog maybe-configure-stage1-cloog
maybe-configure-stage1-cloog:

.PHONY: configure-stage2-cloog maybe-configure-stage2-cloog
maybe-configure-stage2-cloog:

.PHONY: configure-stage3-cloog maybe-configure-stage3-cloog
maybe-configure-stage3-cloog:

.PHONY: configure-stage4-cloog maybe-configure-stage4-cloog
maybe-configure-stage4-cloog:

.PHONY: configure-stageprofile-cloog maybe-configure-stageprofile-cloog
maybe-configure-stageprofile-cloog:

.PHONY: configure-stagefeedback-cloog maybe-configure-stagefeedback-cloog
maybe-configure-stagefeedback-cloog:





.PHONY: all-cloog maybe-all-cloog
maybe-all-cloog:



.PHONY: all-stage1-cloog maybe-all-stage1-cloog
.PHONY: clean-stage1-cloog maybe-clean-stage1-cloog
maybe-all-stage1-cloog:
maybe-clean-stage1-cloog:


.PHONY: all-stage2-cloog maybe-all-stage2-cloog
.PHONY: clean-stage2-cloog maybe-clean-stage2-cloog
maybe-all-stage2-cloog:
maybe-clean-stage2-cloog:


.PHONY: all-stage3-cloog maybe-all-stage3-cloog
.PHONY: clean-stage3-cloog maybe-clean-stage3-cloog
maybe-all-stage3-cloog:
maybe-clean-stage3-cloog:


.PHONY: all-stage4-cloog maybe-all-stage4-cloog
.PHONY: clean-stage4-cloog maybe-clean-stage4-cloog
maybe-all-stage4-cloog:
maybe-clean-stage4-cloog:


.PHONY: all-stageprofile-cloog maybe-all-stageprofile-cloog
.PHONY: clean-stageprofile-cloog maybe-clean-stageprofile-cloog
maybe-all-stageprofile-cloog:
maybe-clean-stageprofile-cloog:


.PHONY: all-stagefeedback-cloog maybe-all-stagefeedback-cloog
.PHONY: clean-stagefeedback-cloog maybe-clean-stagefeedback-cloog
maybe-all-stagefeedback-cloog:
maybe-clean-stagefeedback-cloog:





.PHONY: check-cloog maybe-check-cloog
maybe-check-cloog:

.PHONY: install-cloog maybe-install-cloog
maybe-install-cloog:

.PHONY: install-strip-cloog maybe-install-strip-cloog
maybe-install-strip-cloog:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-cloog info-cloog
maybe-info-cloog:

.PHONY: maybe-dvi-cloog dvi-cloog
maybe-dvi-cloog:

.PHONY: maybe-pdf-cloog pdf-cloog
maybe-pdf-cloog:

.PHONY: maybe-html-cloog html-cloog
maybe-html-cloog:

.PHONY: maybe-TAGS-cloog TAGS-cloog
maybe-TAGS-cloog:

.PHONY: maybe-install-info-cloog install-info-cloog
maybe-install-info-cloog:

.PHONY: maybe-install-pdf-cloog install-pdf-cloog
maybe-install-pdf-cloog:

.PHONY: maybe-install-html-cloog install-html-cloog
maybe-install-html-cloog:

.PHONY: maybe-installcheck-cloog installcheck-cloog
maybe-installcheck-cloog:

.PHONY: maybe-mostlyclean-cloog mostlyclean-cloog
maybe-mostlyclean-cloog:

.PHONY: maybe-clean-cloog clean-cloog
maybe-clean-cloog:

.PHONY: maybe-distclean-cloog distclean-cloog
maybe-distclean-cloog:

.PHONY: maybe-maintainer-clean-cloog maintainer-clean-cloog
maybe-maintainer-clean-cloog:



.PHONY: configure-libelf maybe-configure-libelf
maybe-configure-libelf:



.PHONY: configure-stage1-libelf maybe-configure-stage1-libelf
maybe-configure-stage1-libelf:

.PHONY: configure-stage2-libelf maybe-configure-stage2-libelf
maybe-configure-stage2-libelf:

.PHONY: configure-stage3-libelf maybe-configure-stage3-libelf
maybe-configure-stage3-libelf:

.PHONY: configure-stage4-libelf maybe-configure-stage4-libelf
maybe-configure-stage4-libelf:

.PHONY: configure-stageprofile-libelf maybe-configure-stageprofile-libelf
maybe-configure-stageprofile-libelf:

.PHONY: configure-stagefeedback-libelf maybe-configure-stagefeedback-libelf
maybe-configure-stagefeedback-libelf:





.PHONY: all-libelf maybe-all-libelf
maybe-all-libelf:



.PHONY: all-stage1-libelf maybe-all-stage1-libelf
.PHONY: clean-stage1-libelf maybe-clean-stage1-libelf
maybe-all-stage1-libelf:
maybe-clean-stage1-libelf:


.PHONY: all-stage2-libelf maybe-all-stage2-libelf
.PHONY: clean-stage2-libelf maybe-clean-stage2-libelf
maybe-all-stage2-libelf:
maybe-clean-stage2-libelf:


.PHONY: all-stage3-libelf maybe-all-stage3-libelf
.PHONY: clean-stage3-libelf maybe-clean-stage3-libelf
maybe-all-stage3-libelf:
maybe-clean-stage3-libelf:


.PHONY: all-stage4-libelf maybe-all-stage4-libelf
.PHONY: clean-stage4-libelf maybe-clean-stage4-libelf
maybe-all-stage4-libelf:
maybe-clean-stage4-libelf:


.PHONY: all-stageprofile-libelf maybe-all-stageprofile-libelf
.PHONY: clean-stageprofile-libelf maybe-clean-stageprofile-libelf
maybe-all-stageprofile-libelf:
maybe-clean-stageprofile-libelf:


.PHONY: all-stagefeedback-libelf maybe-all-stagefeedback-libelf
.PHONY: clean-stagefeedback-libelf maybe-clean-stagefeedback-libelf
maybe-all-stagefeedback-libelf:
maybe-clean-stagefeedback-libelf:





.PHONY: check-libelf maybe-check-libelf
maybe-check-libelf:

.PHONY: install-libelf maybe-install-libelf
maybe-install-libelf:

.PHONY: install-strip-libelf maybe-install-strip-libelf
maybe-install-strip-libelf:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libelf info-libelf
maybe-info-libelf:

.PHONY: maybe-dvi-libelf dvi-libelf
maybe-dvi-libelf:

.PHONY: maybe-pdf-libelf pdf-libelf
maybe-pdf-libelf:

.PHONY: maybe-html-libelf html-libelf
maybe-html-libelf:

.PHONY: maybe-TAGS-libelf TAGS-libelf
maybe-TAGS-libelf:

.PHONY: maybe-install-info-libelf install-info-libelf
maybe-install-info-libelf:

.PHONY: maybe-install-pdf-libelf install-pdf-libelf
maybe-install-pdf-libelf:

.PHONY: maybe-install-html-libelf install-html-libelf
maybe-install-html-libelf:

.PHONY: maybe-installcheck-libelf installcheck-libelf
maybe-installcheck-libelf:

.PHONY: maybe-mostlyclean-libelf mostlyclean-libelf
maybe-mostlyclean-libelf:

.PHONY: maybe-clean-libelf clean-libelf
maybe-clean-libelf:

.PHONY: maybe-distclean-libelf distclean-libelf
maybe-distclean-libelf:

.PHONY: maybe-maintainer-clean-libelf maintainer-clean-libelf
maybe-maintainer-clean-libelf:



.PHONY: configure-gold maybe-configure-gold
maybe-configure-gold:



.PHONY: configure-stage1-gold maybe-configure-stage1-gold
maybe-configure-stage1-gold:

.PHONY: configure-stage2-gold maybe-configure-stage2-gold
maybe-configure-stage2-gold:

.PHONY: configure-stage3-gold maybe-configure-stage3-gold
maybe-configure-stage3-gold:

.PHONY: configure-stage4-gold maybe-configure-stage4-gold
maybe-configure-stage4-gold:

.PHONY: configure-stageprofile-gold maybe-configure-stageprofile-gold
maybe-configure-stageprofile-gold:

.PHONY: configure-stagefeedback-gold maybe-configure-stagefeedback-gold
maybe-configure-stagefeedback-gold:





.PHONY: all-gold maybe-all-gold
maybe-all-gold:



.PHONY: all-stage1-gold maybe-all-stage1-gold
.PHONY: clean-stage1-gold maybe-clean-stage1-gold
maybe-all-stage1-gold:
maybe-clean-stage1-gold:


.PHONY: all-stage2-gold maybe-all-stage2-gold
.PHONY: clean-stage2-gold maybe-clean-stage2-gold
maybe-all-stage2-gold:
maybe-clean-stage2-gold:


.PHONY: all-stage3-gold maybe-all-stage3-gold
.PHONY: clean-stage3-gold maybe-clean-stage3-gold
maybe-all-stage3-gold:
maybe-clean-stage3-gold:


.PHONY: all-stage4-gold maybe-all-stage4-gold
.PHONY: clean-stage4-gold maybe-clean-stage4-gold
maybe-all-stage4-gold:
maybe-clean-stage4-gold:


.PHONY: all-stageprofile-gold maybe-all-stageprofile-gold
.PHONY: clean-stageprofile-gold maybe-clean-stageprofile-gold
maybe-all-stageprofile-gold:
maybe-clean-stageprofile-gold:


.PHONY: all-stagefeedback-gold maybe-all-stagefeedback-gold
.PHONY: clean-stagefeedback-gold maybe-clean-stagefeedback-gold
maybe-all-stagefeedback-gold:
maybe-clean-stagefeedback-gold:





.PHONY: check-gold maybe-check-gold
maybe-check-gold:

.PHONY: install-gold maybe-install-gold
maybe-install-gold:

.PHONY: install-strip-gold maybe-install-strip-gold
maybe-install-strip-gold:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gold info-gold
maybe-info-gold:

.PHONY: maybe-dvi-gold dvi-gold
maybe-dvi-gold:

.PHONY: maybe-pdf-gold pdf-gold
maybe-pdf-gold:

.PHONY: maybe-html-gold html-gold
maybe-html-gold:

.PHONY: maybe-TAGS-gold TAGS-gold
maybe-TAGS-gold:

.PHONY: maybe-install-info-gold install-info-gold
maybe-install-info-gold:

.PHONY: maybe-install-pdf-gold install-pdf-gold
maybe-install-pdf-gold:

.PHONY: maybe-install-html-gold install-html-gold
maybe-install-html-gold:

.PHONY: maybe-installcheck-gold installcheck-gold
maybe-installcheck-gold:

.PHONY: maybe-mostlyclean-gold mostlyclean-gold
maybe-mostlyclean-gold:

.PHONY: maybe-clean-gold clean-gold
maybe-clean-gold:

.PHONY: maybe-distclean-gold distclean-gold
maybe-distclean-gold:

.PHONY: maybe-maintainer-clean-gold maintainer-clean-gold
maybe-maintainer-clean-gold:



.PHONY: configure-gprof maybe-configure-gprof
maybe-configure-gprof:





.PHONY: all-gprof maybe-all-gprof
maybe-all-gprof:




.PHONY: check-gprof maybe-check-gprof
maybe-check-gprof:

.PHONY: install-gprof maybe-install-gprof
maybe-install-gprof:

.PHONY: install-strip-gprof maybe-install-strip-gprof
maybe-install-strip-gprof:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gprof info-gprof
maybe-info-gprof:

.PHONY: maybe-dvi-gprof dvi-gprof
maybe-dvi-gprof:

.PHONY: maybe-pdf-gprof pdf-gprof
maybe-pdf-gprof:

.PHONY: maybe-html-gprof html-gprof
maybe-html-gprof:

.PHONY: maybe-TAGS-gprof TAGS-gprof
maybe-TAGS-gprof:

.PHONY: maybe-install-info-gprof install-info-gprof
maybe-install-info-gprof:

.PHONY: maybe-install-pdf-gprof install-pdf-gprof
maybe-install-pdf-gprof:

.PHONY: maybe-install-html-gprof install-html-gprof
maybe-install-html-gprof:

.PHONY: maybe-installcheck-gprof installcheck-gprof
maybe-installcheck-gprof:

.PHONY: maybe-mostlyclean-gprof mostlyclean-gprof
maybe-mostlyclean-gprof:

.PHONY: maybe-clean-gprof clean-gprof
maybe-clean-gprof:

.PHONY: maybe-distclean-gprof distclean-gprof
maybe-distclean-gprof:

.PHONY: maybe-maintainer-clean-gprof maintainer-clean-gprof
maybe-maintainer-clean-gprof:



.PHONY: configure-intl maybe-configure-intl
maybe-configure-intl:



.PHONY: configure-stage1-intl maybe-configure-stage1-intl
maybe-configure-stage1-intl:

.PHONY: configure-stage2-intl maybe-configure-stage2-intl
maybe-configure-stage2-intl:

.PHONY: configure-stage3-intl maybe-configure-stage3-intl
maybe-configure-stage3-intl:

.PHONY: configure-stage4-intl maybe-configure-stage4-intl
maybe-configure-stage4-intl:

.PHONY: configure-stageprofile-intl maybe-configure-stageprofile-intl
maybe-configure-stageprofile-intl:

.PHONY: configure-stagefeedback-intl maybe-configure-stagefeedback-intl
maybe-configure-stagefeedback-intl:





.PHONY: all-intl maybe-all-intl
maybe-all-intl:



.PHONY: all-stage1-intl maybe-all-stage1-intl
.PHONY: clean-stage1-intl maybe-clean-stage1-intl
maybe-all-stage1-intl:
maybe-clean-stage1-intl:


.PHONY: all-stage2-intl maybe-all-stage2-intl
.PHONY: clean-stage2-intl maybe-clean-stage2-intl
maybe-all-stage2-intl:
maybe-clean-stage2-intl:


.PHONY: all-stage3-intl maybe-all-stage3-intl
.PHONY: clean-stage3-intl maybe-clean-stage3-intl
maybe-all-stage3-intl:
maybe-clean-stage3-intl:


.PHONY: all-stage4-intl maybe-all-stage4-intl
.PHONY: clean-stage4-intl maybe-clean-stage4-intl
maybe-all-stage4-intl:
maybe-clean-stage4-intl:


.PHONY: all-stageprofile-intl maybe-all-stageprofile-intl
.PHONY: clean-stageprofile-intl maybe-clean-stageprofile-intl
maybe-all-stageprofile-intl:
maybe-clean-stageprofile-intl:


.PHONY: all-stagefeedback-intl maybe-all-stagefeedback-intl
.PHONY: clean-stagefeedback-intl maybe-clean-stagefeedback-intl
maybe-all-stagefeedback-intl:
maybe-clean-stagefeedback-intl:





.PHONY: check-intl maybe-check-intl
maybe-check-intl:

.PHONY: install-intl maybe-install-intl
maybe-install-intl:

.PHONY: install-strip-intl maybe-install-strip-intl
maybe-install-strip-intl:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-intl info-intl
maybe-info-intl:

.PHONY: maybe-dvi-intl dvi-intl
maybe-dvi-intl:

.PHONY: maybe-pdf-intl pdf-intl
maybe-pdf-intl:

.PHONY: maybe-html-intl html-intl
maybe-html-intl:

.PHONY: maybe-TAGS-intl TAGS-intl
maybe-TAGS-intl:

.PHONY: maybe-install-info-intl install-info-intl
maybe-install-info-intl:

.PHONY: maybe-install-pdf-intl install-pdf-intl
maybe-install-pdf-intl:

.PHONY: maybe-install-html-intl install-html-intl
maybe-install-html-intl:

.PHONY: maybe-installcheck-intl installcheck-intl
maybe-installcheck-intl:

.PHONY: maybe-mostlyclean-intl mostlyclean-intl
maybe-mostlyclean-intl:

.PHONY: maybe-clean-intl clean-intl
maybe-clean-intl:

.PHONY: maybe-distclean-intl distclean-intl
maybe-distclean-intl:

.PHONY: maybe-maintainer-clean-intl maintainer-clean-intl
maybe-maintainer-clean-intl:



.PHONY: configure-tcl maybe-configure-tcl
maybe-configure-tcl:





.PHONY: all-tcl maybe-all-tcl
maybe-all-tcl:




.PHONY: check-tcl maybe-check-tcl
maybe-check-tcl:

.PHONY: install-tcl maybe-install-tcl
maybe-install-tcl:

.PHONY: install-strip-tcl maybe-install-strip-tcl
maybe-install-strip-tcl:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-tcl info-tcl
maybe-info-tcl:

.PHONY: maybe-dvi-tcl dvi-tcl
maybe-dvi-tcl:

.PHONY: maybe-pdf-tcl pdf-tcl
maybe-pdf-tcl:

.PHONY: maybe-html-tcl html-tcl
maybe-html-tcl:

.PHONY: maybe-TAGS-tcl TAGS-tcl
maybe-TAGS-tcl:

.PHONY: maybe-install-info-tcl install-info-tcl
maybe-install-info-tcl:

.PHONY: maybe-install-pdf-tcl install-pdf-tcl
maybe-install-pdf-tcl:

.PHONY: maybe-install-html-tcl install-html-tcl
maybe-install-html-tcl:

.PHONY: maybe-installcheck-tcl installcheck-tcl
maybe-installcheck-tcl:

.PHONY: maybe-mostlyclean-tcl mostlyclean-tcl
maybe-mostlyclean-tcl:

.PHONY: maybe-clean-tcl clean-tcl
maybe-clean-tcl:

.PHONY: maybe-distclean-tcl distclean-tcl
maybe-distclean-tcl:

.PHONY: maybe-maintainer-clean-tcl maintainer-clean-tcl
maybe-maintainer-clean-tcl:



.PHONY: configure-itcl maybe-configure-itcl
maybe-configure-itcl:





.PHONY: all-itcl maybe-all-itcl
maybe-all-itcl:




.PHONY: check-itcl maybe-check-itcl
maybe-check-itcl:

.PHONY: install-itcl maybe-install-itcl
maybe-install-itcl:

.PHONY: install-strip-itcl maybe-install-strip-itcl
maybe-install-strip-itcl:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-itcl info-itcl
maybe-info-itcl:

.PHONY: maybe-dvi-itcl dvi-itcl
maybe-dvi-itcl:

.PHONY: maybe-pdf-itcl pdf-itcl
maybe-pdf-itcl:

.PHONY: maybe-html-itcl html-itcl
maybe-html-itcl:

.PHONY: maybe-TAGS-itcl TAGS-itcl
maybe-TAGS-itcl:

.PHONY: maybe-install-info-itcl install-info-itcl
maybe-install-info-itcl:

.PHONY: maybe-install-pdf-itcl install-pdf-itcl
maybe-install-pdf-itcl:

.PHONY: maybe-install-html-itcl install-html-itcl
maybe-install-html-itcl:

.PHONY: maybe-installcheck-itcl installcheck-itcl
maybe-installcheck-itcl:

.PHONY: maybe-mostlyclean-itcl mostlyclean-itcl
maybe-mostlyclean-itcl:

.PHONY: maybe-clean-itcl clean-itcl
maybe-clean-itcl:

.PHONY: maybe-distclean-itcl distclean-itcl
maybe-distclean-itcl:

.PHONY: maybe-maintainer-clean-itcl maintainer-clean-itcl
maybe-maintainer-clean-itcl:



.PHONY: configure-ld maybe-configure-ld
maybe-configure-ld:



.PHONY: configure-stage1-ld maybe-configure-stage1-ld
maybe-configure-stage1-ld:

.PHONY: configure-stage2-ld maybe-configure-stage2-ld
maybe-configure-stage2-ld:

.PHONY: configure-stage3-ld maybe-configure-stage3-ld
maybe-configure-stage3-ld:

.PHONY: configure-stage4-ld maybe-configure-stage4-ld
maybe-configure-stage4-ld:

.PHONY: configure-stageprofile-ld maybe-configure-stageprofile-ld
maybe-configure-stageprofile-ld:

.PHONY: configure-stagefeedback-ld maybe-configure-stagefeedback-ld
maybe-configure-stagefeedback-ld:





.PHONY: all-ld maybe-all-ld
maybe-all-ld:



.PHONY: all-stage1-ld maybe-all-stage1-ld
.PHONY: clean-stage1-ld maybe-clean-stage1-ld
maybe-all-stage1-ld:
maybe-clean-stage1-ld:


.PHONY: all-stage2-ld maybe-all-stage2-ld
.PHONY: clean-stage2-ld maybe-clean-stage2-ld
maybe-all-stage2-ld:
maybe-clean-stage2-ld:


.PHONY: all-stage3-ld maybe-all-stage3-ld
.PHONY: clean-stage3-ld maybe-clean-stage3-ld
maybe-all-stage3-ld:
maybe-clean-stage3-ld:


.PHONY: all-stage4-ld maybe-all-stage4-ld
.PHONY: clean-stage4-ld maybe-clean-stage4-ld
maybe-all-stage4-ld:
maybe-clean-stage4-ld:


.PHONY: all-stageprofile-ld maybe-all-stageprofile-ld
.PHONY: clean-stageprofile-ld maybe-clean-stageprofile-ld
maybe-all-stageprofile-ld:
maybe-clean-stageprofile-ld:


.PHONY: all-stagefeedback-ld maybe-all-stagefeedback-ld
.PHONY: clean-stagefeedback-ld maybe-clean-stagefeedback-ld
maybe-all-stagefeedback-ld:
maybe-clean-stagefeedback-ld:





.PHONY: check-ld maybe-check-ld
maybe-check-ld:

.PHONY: install-ld maybe-install-ld
maybe-install-ld:

.PHONY: install-strip-ld maybe-install-strip-ld
maybe-install-strip-ld:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-ld info-ld
maybe-info-ld:

.PHONY: maybe-dvi-ld dvi-ld
maybe-dvi-ld:

.PHONY: maybe-pdf-ld pdf-ld
maybe-pdf-ld:

.PHONY: maybe-html-ld html-ld
maybe-html-ld:

.PHONY: maybe-TAGS-ld TAGS-ld
maybe-TAGS-ld:

.PHONY: maybe-install-info-ld install-info-ld
maybe-install-info-ld:

.PHONY: maybe-install-pdf-ld install-pdf-ld
maybe-install-pdf-ld:

.PHONY: maybe-install-html-ld install-html-ld
maybe-install-html-ld:

.PHONY: maybe-installcheck-ld installcheck-ld
maybe-installcheck-ld:

.PHONY: maybe-mostlyclean-ld mostlyclean-ld
maybe-mostlyclean-ld:

.PHONY: maybe-clean-ld clean-ld
maybe-clean-ld:

.PHONY: maybe-distclean-ld distclean-ld
maybe-distclean-ld:

.PHONY: maybe-maintainer-clean-ld maintainer-clean-ld
maybe-maintainer-clean-ld:



.PHONY: configure-libbacktrace maybe-configure-libbacktrace
maybe-configure-libbacktrace:



.PHONY: configure-stage1-libbacktrace maybe-configure-stage1-libbacktrace
maybe-configure-stage1-libbacktrace:

.PHONY: configure-stage2-libbacktrace maybe-configure-stage2-libbacktrace
maybe-configure-stage2-libbacktrace:

.PHONY: configure-stage3-libbacktrace maybe-configure-stage3-libbacktrace
maybe-configure-stage3-libbacktrace:

.PHONY: configure-stage4-libbacktrace maybe-configure-stage4-libbacktrace
maybe-configure-stage4-libbacktrace:

.PHONY: configure-stageprofile-libbacktrace maybe-configure-stageprofile-libbacktrace
maybe-configure-stageprofile-libbacktrace:

.PHONY: configure-stagefeedback-libbacktrace maybe-configure-stagefeedback-libbacktrace
maybe-configure-stagefeedback-libbacktrace:





.PHONY: all-libbacktrace maybe-all-libbacktrace
maybe-all-libbacktrace:



.PHONY: all-stage1-libbacktrace maybe-all-stage1-libbacktrace
.PHONY: clean-stage1-libbacktrace maybe-clean-stage1-libbacktrace
maybe-all-stage1-libbacktrace:
maybe-clean-stage1-libbacktrace:


.PHONY: all-stage2-libbacktrace maybe-all-stage2-libbacktrace
.PHONY: clean-stage2-libbacktrace maybe-clean-stage2-libbacktrace
maybe-all-stage2-libbacktrace:
maybe-clean-stage2-libbacktrace:


.PHONY: all-stage3-libbacktrace maybe-all-stage3-libbacktrace
.PHONY: clean-stage3-libbacktrace maybe-clean-stage3-libbacktrace
maybe-all-stage3-libbacktrace:
maybe-clean-stage3-libbacktrace:


.PHONY: all-stage4-libbacktrace maybe-all-stage4-libbacktrace
.PHONY: clean-stage4-libbacktrace maybe-clean-stage4-libbacktrace
maybe-all-stage4-libbacktrace:
maybe-clean-stage4-libbacktrace:


.PHONY: all-stageprofile-libbacktrace maybe-all-stageprofile-libbacktrace
.PHONY: clean-stageprofile-libbacktrace maybe-clean-stageprofile-libbacktrace
maybe-all-stageprofile-libbacktrace:
maybe-clean-stageprofile-libbacktrace:


.PHONY: all-stagefeedback-libbacktrace maybe-all-stagefeedback-libbacktrace
.PHONY: clean-stagefeedback-libbacktrace maybe-clean-stagefeedback-libbacktrace
maybe-all-stagefeedback-libbacktrace:
maybe-clean-stagefeedback-libbacktrace:





.PHONY: check-libbacktrace maybe-check-libbacktrace
maybe-check-libbacktrace:

.PHONY: install-libbacktrace maybe-install-libbacktrace
maybe-install-libbacktrace:

.PHONY: install-strip-libbacktrace maybe-install-strip-libbacktrace
maybe-install-strip-libbacktrace:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libbacktrace info-libbacktrace
maybe-info-libbacktrace:

.PHONY: maybe-dvi-libbacktrace dvi-libbacktrace
maybe-dvi-libbacktrace:

.PHONY: maybe-pdf-libbacktrace pdf-libbacktrace
maybe-pdf-libbacktrace:

.PHONY: maybe-html-libbacktrace html-libbacktrace
maybe-html-libbacktrace:

.PHONY: maybe-TAGS-libbacktrace TAGS-libbacktrace
maybe-TAGS-libbacktrace:

.PHONY: maybe-install-info-libbacktrace install-info-libbacktrace
maybe-install-info-libbacktrace:

.PHONY: maybe-install-pdf-libbacktrace install-pdf-libbacktrace
maybe-install-pdf-libbacktrace:

.PHONY: maybe-install-html-libbacktrace install-html-libbacktrace
maybe-install-html-libbacktrace:

.PHONY: maybe-installcheck-libbacktrace installcheck-libbacktrace
maybe-installcheck-libbacktrace:

.PHONY: maybe-mostlyclean-libbacktrace mostlyclean-libbacktrace
maybe-mostlyclean-libbacktrace:

.PHONY: maybe-clean-libbacktrace clean-libbacktrace
maybe-clean-libbacktrace:

.PHONY: maybe-distclean-libbacktrace distclean-libbacktrace
maybe-distclean-libbacktrace:

.PHONY: maybe-maintainer-clean-libbacktrace maintainer-clean-libbacktrace
maybe-maintainer-clean-libbacktrace:



.PHONY: configure-libcpp maybe-configure-libcpp
maybe-configure-libcpp:



.PHONY: configure-stage1-libcpp maybe-configure-stage1-libcpp
maybe-configure-stage1-libcpp:

.PHONY: configure-stage2-libcpp maybe-configure-stage2-libcpp
maybe-configure-stage2-libcpp:

.PHONY: configure-stage3-libcpp maybe-configure-stage3-libcpp
maybe-configure-stage3-libcpp:

.PHONY: configure-stage4-libcpp maybe-configure-stage4-libcpp
maybe-configure-stage4-libcpp:

.PHONY: configure-stageprofile-libcpp maybe-configure-stageprofile-libcpp
maybe-configure-stageprofile-libcpp:

.PHONY: configure-stagefeedback-libcpp maybe-configure-stagefeedback-libcpp
maybe-configure-stagefeedback-libcpp:





.PHONY: all-libcpp maybe-all-libcpp
maybe-all-libcpp:



.PHONY: all-stage1-libcpp maybe-all-stage1-libcpp
.PHONY: clean-stage1-libcpp maybe-clean-stage1-libcpp
maybe-all-stage1-libcpp:
maybe-clean-stage1-libcpp:


.PHONY: all-stage2-libcpp maybe-all-stage2-libcpp
.PHONY: clean-stage2-libcpp maybe-clean-stage2-libcpp
maybe-all-stage2-libcpp:
maybe-clean-stage2-libcpp:


.PHONY: all-stage3-libcpp maybe-all-stage3-libcpp
.PHONY: clean-stage3-libcpp maybe-clean-stage3-libcpp
maybe-all-stage3-libcpp:
maybe-clean-stage3-libcpp:


.PHONY: all-stage4-libcpp maybe-all-stage4-libcpp
.PHONY: clean-stage4-libcpp maybe-clean-stage4-libcpp
maybe-all-stage4-libcpp:
maybe-clean-stage4-libcpp:


.PHONY: all-stageprofile-libcpp maybe-all-stageprofile-libcpp
.PHONY: clean-stageprofile-libcpp maybe-clean-stageprofile-libcpp
maybe-all-stageprofile-libcpp:
maybe-clean-stageprofile-libcpp:


.PHONY: all-stagefeedback-libcpp maybe-all-stagefeedback-libcpp
.PHONY: clean-stagefeedback-libcpp maybe-clean-stagefeedback-libcpp
maybe-all-stagefeedback-libcpp:
maybe-clean-stagefeedback-libcpp:





.PHONY: check-libcpp maybe-check-libcpp
maybe-check-libcpp:

.PHONY: install-libcpp maybe-install-libcpp
maybe-install-libcpp:

.PHONY: install-strip-libcpp maybe-install-strip-libcpp
maybe-install-strip-libcpp:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libcpp info-libcpp
maybe-info-libcpp:

.PHONY: maybe-dvi-libcpp dvi-libcpp
maybe-dvi-libcpp:

.PHONY: maybe-pdf-libcpp pdf-libcpp
maybe-pdf-libcpp:

.PHONY: maybe-html-libcpp html-libcpp
maybe-html-libcpp:

.PHONY: maybe-TAGS-libcpp TAGS-libcpp
maybe-TAGS-libcpp:

.PHONY: maybe-install-info-libcpp install-info-libcpp
maybe-install-info-libcpp:

.PHONY: maybe-install-pdf-libcpp install-pdf-libcpp
maybe-install-pdf-libcpp:

.PHONY: maybe-install-html-libcpp install-html-libcpp
maybe-install-html-libcpp:

.PHONY: maybe-installcheck-libcpp installcheck-libcpp
maybe-installcheck-libcpp:

.PHONY: maybe-mostlyclean-libcpp mostlyclean-libcpp
maybe-mostlyclean-libcpp:

.PHONY: maybe-clean-libcpp clean-libcpp
maybe-clean-libcpp:

.PHONY: maybe-distclean-libcpp distclean-libcpp
maybe-distclean-libcpp:

.PHONY: maybe-maintainer-clean-libcpp maintainer-clean-libcpp
maybe-maintainer-clean-libcpp:



.PHONY: configure-libdecnumber maybe-configure-libdecnumber
maybe-configure-libdecnumber:



.PHONY: configure-stage1-libdecnumber maybe-configure-stage1-libdecnumber
maybe-configure-stage1-libdecnumber:

.PHONY: configure-stage2-libdecnumber maybe-configure-stage2-libdecnumber
maybe-configure-stage2-libdecnumber:

.PHONY: configure-stage3-libdecnumber maybe-configure-stage3-libdecnumber
maybe-configure-stage3-libdecnumber:

.PHONY: configure-stage4-libdecnumber maybe-configure-stage4-libdecnumber
maybe-configure-stage4-libdecnumber:

.PHONY: configure-stageprofile-libdecnumber maybe-configure-stageprofile-libdecnumber
maybe-configure-stageprofile-libdecnumber:

.PHONY: configure-stagefeedback-libdecnumber maybe-configure-stagefeedback-libdecnumber
maybe-configure-stagefeedback-libdecnumber:





.PHONY: all-libdecnumber maybe-all-libdecnumber
maybe-all-libdecnumber:



.PHONY: all-stage1-libdecnumber maybe-all-stage1-libdecnumber
.PHONY: clean-stage1-libdecnumber maybe-clean-stage1-libdecnumber
maybe-all-stage1-libdecnumber:
maybe-clean-stage1-libdecnumber:


.PHONY: all-stage2-libdecnumber maybe-all-stage2-libdecnumber
.PHONY: clean-stage2-libdecnumber maybe-clean-stage2-libdecnumber
maybe-all-stage2-libdecnumber:
maybe-clean-stage2-libdecnumber:


.PHONY: all-stage3-libdecnumber maybe-all-stage3-libdecnumber
.PHONY: clean-stage3-libdecnumber maybe-clean-stage3-libdecnumber
maybe-all-stage3-libdecnumber:
maybe-clean-stage3-libdecnumber:


.PHONY: all-stage4-libdecnumber maybe-all-stage4-libdecnumber
.PHONY: clean-stage4-libdecnumber maybe-clean-stage4-libdecnumber
maybe-all-stage4-libdecnumber:
maybe-clean-stage4-libdecnumber:


.PHONY: all-stageprofile-libdecnumber maybe-all-stageprofile-libdecnumber
.PHONY: clean-stageprofile-libdecnumber maybe-clean-stageprofile-libdecnumber
maybe-all-stageprofile-libdecnumber:
maybe-clean-stageprofile-libdecnumber:


.PHONY: all-stagefeedback-libdecnumber maybe-all-stagefeedback-libdecnumber
.PHONY: clean-stagefeedback-libdecnumber maybe-clean-stagefeedback-libdecnumber
maybe-all-stagefeedback-libdecnumber:
maybe-clean-stagefeedback-libdecnumber:





.PHONY: check-libdecnumber maybe-check-libdecnumber
maybe-check-libdecnumber:

.PHONY: install-libdecnumber maybe-install-libdecnumber
maybe-install-libdecnumber:

.PHONY: install-strip-libdecnumber maybe-install-strip-libdecnumber
maybe-install-strip-libdecnumber:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libdecnumber info-libdecnumber
maybe-info-libdecnumber:

.PHONY: maybe-dvi-libdecnumber dvi-libdecnumber
maybe-dvi-libdecnumber:

.PHONY: maybe-pdf-libdecnumber pdf-libdecnumber
maybe-pdf-libdecnumber:

.PHONY: maybe-html-libdecnumber html-libdecnumber
maybe-html-libdecnumber:

.PHONY: maybe-TAGS-libdecnumber TAGS-libdecnumber
maybe-TAGS-libdecnumber:

.PHONY: maybe-install-info-libdecnumber install-info-libdecnumber
maybe-install-info-libdecnumber:

.PHONY: maybe-install-pdf-libdecnumber install-pdf-libdecnumber
maybe-install-pdf-libdecnumber:

.PHONY: maybe-install-html-libdecnumber install-html-libdecnumber
maybe-install-html-libdecnumber:

.PHONY: maybe-installcheck-libdecnumber installcheck-libdecnumber
maybe-installcheck-libdecnumber:

.PHONY: maybe-mostlyclean-libdecnumber mostlyclean-libdecnumber
maybe-mostlyclean-libdecnumber:

.PHONY: maybe-clean-libdecnumber clean-libdecnumber
maybe-clean-libdecnumber:

.PHONY: maybe-distclean-libdecnumber distclean-libdecnumber
maybe-distclean-libdecnumber:

.PHONY: maybe-maintainer-clean-libdecnumber maintainer-clean-libdecnumber
maybe-maintainer-clean-libdecnumber:



.PHONY: configure-libgui maybe-configure-libgui
maybe-configure-libgui:





.PHONY: all-libgui maybe-all-libgui
maybe-all-libgui:




.PHONY: check-libgui maybe-check-libgui
maybe-check-libgui:

.PHONY: install-libgui maybe-install-libgui
maybe-install-libgui:

.PHONY: install-strip-libgui maybe-install-strip-libgui
maybe-install-strip-libgui:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libgui info-libgui
maybe-info-libgui:

.PHONY: maybe-dvi-libgui dvi-libgui
maybe-dvi-libgui:

.PHONY: maybe-pdf-libgui pdf-libgui
maybe-pdf-libgui:

.PHONY: maybe-html-libgui html-libgui
maybe-html-libgui:

.PHONY: maybe-TAGS-libgui TAGS-libgui
maybe-TAGS-libgui:

.PHONY: maybe-install-info-libgui install-info-libgui
maybe-install-info-libgui:

.PHONY: maybe-install-pdf-libgui install-pdf-libgui
maybe-install-pdf-libgui:

.PHONY: maybe-install-html-libgui install-html-libgui
maybe-install-html-libgui:

.PHONY: maybe-installcheck-libgui installcheck-libgui
maybe-installcheck-libgui:

.PHONY: maybe-mostlyclean-libgui mostlyclean-libgui
maybe-mostlyclean-libgui:

.PHONY: maybe-clean-libgui clean-libgui
maybe-clean-libgui:

.PHONY: maybe-distclean-libgui distclean-libgui
maybe-distclean-libgui:

.PHONY: maybe-maintainer-clean-libgui maintainer-clean-libgui
maybe-maintainer-clean-libgui:



.PHONY: configure-libiberty maybe-configure-libiberty
maybe-configure-libiberty:



.PHONY: configure-stage1-libiberty maybe-configure-stage1-libiberty
maybe-configure-stage1-libiberty:

.PHONY: configure-stage2-libiberty maybe-configure-stage2-libiberty
maybe-configure-stage2-libiberty:

.PHONY: configure-stage3-libiberty maybe-configure-stage3-libiberty
maybe-configure-stage3-libiberty:

.PHONY: configure-stage4-libiberty maybe-configure-stage4-libiberty
maybe-configure-stage4-libiberty:

.PHONY: configure-stageprofile-libiberty maybe-configure-stageprofile-libiberty
maybe-configure-stageprofile-libiberty:

.PHONY: configure-stagefeedback-libiberty maybe-configure-stagefeedback-libiberty
maybe-configure-stagefeedback-libiberty:





.PHONY: all-libiberty maybe-all-libiberty
maybe-all-libiberty:



.PHONY: all-stage1-libiberty maybe-all-stage1-libiberty
.PHONY: clean-stage1-libiberty maybe-clean-stage1-libiberty
maybe-all-stage1-libiberty:
maybe-clean-stage1-libiberty:


.PHONY: all-stage2-libiberty maybe-all-stage2-libiberty
.PHONY: clean-stage2-libiberty maybe-clean-stage2-libiberty
maybe-all-stage2-libiberty:
maybe-clean-stage2-libiberty:


.PHONY: all-stage3-libiberty maybe-all-stage3-libiberty
.PHONY: clean-stage3-libiberty maybe-clean-stage3-libiberty
maybe-all-stage3-libiberty:
maybe-clean-stage3-libiberty:


.PHONY: all-stage4-libiberty maybe-all-stage4-libiberty
.PHONY: clean-stage4-libiberty maybe-clean-stage4-libiberty
maybe-all-stage4-libiberty:
maybe-clean-stage4-libiberty:


.PHONY: all-stageprofile-libiberty maybe-all-stageprofile-libiberty
.PHONY: clean-stageprofile-libiberty maybe-clean-stageprofile-libiberty
maybe-all-stageprofile-libiberty:
maybe-clean-stageprofile-libiberty:


.PHONY: all-stagefeedback-libiberty maybe-all-stagefeedback-libiberty
.PHONY: clean-stagefeedback-libiberty maybe-clean-stagefeedback-libiberty
maybe-all-stagefeedback-libiberty:
maybe-clean-stagefeedback-libiberty:





.PHONY: check-libiberty maybe-check-libiberty
maybe-check-libiberty:

.PHONY: install-libiberty maybe-install-libiberty
maybe-install-libiberty:

.PHONY: install-strip-libiberty maybe-install-strip-libiberty
maybe-install-strip-libiberty:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libiberty info-libiberty
maybe-info-libiberty:

.PHONY: maybe-dvi-libiberty dvi-libiberty
maybe-dvi-libiberty:

.PHONY: maybe-pdf-libiberty pdf-libiberty
maybe-pdf-libiberty:

.PHONY: maybe-html-libiberty html-libiberty
maybe-html-libiberty:

.PHONY: maybe-TAGS-libiberty TAGS-libiberty
maybe-TAGS-libiberty:

.PHONY: maybe-install-info-libiberty install-info-libiberty
maybe-install-info-libiberty:

.PHONY: maybe-install-pdf-libiberty install-pdf-libiberty
maybe-install-pdf-libiberty:

.PHONY: maybe-install-html-libiberty install-html-libiberty
maybe-install-html-libiberty:

.PHONY: maybe-installcheck-libiberty installcheck-libiberty
maybe-installcheck-libiberty:

.PHONY: maybe-mostlyclean-libiberty mostlyclean-libiberty
maybe-mostlyclean-libiberty:

.PHONY: maybe-clean-libiberty clean-libiberty
maybe-clean-libiberty:

.PHONY: maybe-distclean-libiberty distclean-libiberty
maybe-distclean-libiberty:

.PHONY: maybe-maintainer-clean-libiberty maintainer-clean-libiberty
maybe-maintainer-clean-libiberty:



.PHONY: configure-libiconv maybe-configure-libiconv
maybe-configure-libiconv:





.PHONY: all-libiconv maybe-all-libiconv
maybe-all-libiconv:




.PHONY: check-libiconv maybe-check-libiconv
maybe-check-libiconv:

.PHONY: install-libiconv maybe-install-libiconv
maybe-install-libiconv:

.PHONY: install-strip-libiconv maybe-install-strip-libiconv
maybe-install-strip-libiconv:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libiconv info-libiconv
maybe-info-libiconv:

.PHONY: maybe-dvi-libiconv dvi-libiconv
maybe-dvi-libiconv:

.PHONY: maybe-pdf-libiconv pdf-libiconv
maybe-pdf-libiconv:

.PHONY: maybe-html-libiconv html-libiconv
maybe-html-libiconv:

.PHONY: maybe-TAGS-libiconv TAGS-libiconv
maybe-TAGS-libiconv:

.PHONY: maybe-install-info-libiconv install-info-libiconv
maybe-install-info-libiconv:

.PHONY: maybe-install-pdf-libiconv install-pdf-libiconv
maybe-install-pdf-libiconv:

.PHONY: maybe-install-html-libiconv install-html-libiconv
maybe-install-html-libiconv:

.PHONY: maybe-installcheck-libiconv installcheck-libiconv
maybe-installcheck-libiconv:

.PHONY: maybe-mostlyclean-libiconv mostlyclean-libiconv
maybe-mostlyclean-libiconv:

.PHONY: maybe-clean-libiconv clean-libiconv
maybe-clean-libiconv:

.PHONY: maybe-distclean-libiconv distclean-libiconv
maybe-distclean-libiconv:

.PHONY: maybe-maintainer-clean-libiconv maintainer-clean-libiconv
maybe-maintainer-clean-libiconv:



.PHONY: configure-m4 maybe-configure-m4
maybe-configure-m4:





.PHONY: all-m4 maybe-all-m4
maybe-all-m4:




.PHONY: check-m4 maybe-check-m4
maybe-check-m4:

.PHONY: install-m4 maybe-install-m4
maybe-install-m4:

.PHONY: install-strip-m4 maybe-install-strip-m4
maybe-install-strip-m4:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-m4 info-m4
maybe-info-m4:

.PHONY: maybe-dvi-m4 dvi-m4
maybe-dvi-m4:

.PHONY: maybe-pdf-m4 pdf-m4
maybe-pdf-m4:

.PHONY: maybe-html-m4 html-m4
maybe-html-m4:

.PHONY: maybe-TAGS-m4 TAGS-m4
maybe-TAGS-m4:

.PHONY: maybe-install-info-m4 install-info-m4
maybe-install-info-m4:

.PHONY: maybe-install-pdf-m4 install-pdf-m4
maybe-install-pdf-m4:

.PHONY: maybe-install-html-m4 install-html-m4
maybe-install-html-m4:

.PHONY: maybe-installcheck-m4 installcheck-m4
maybe-installcheck-m4:

.PHONY: maybe-mostlyclean-m4 mostlyclean-m4
maybe-mostlyclean-m4:

.PHONY: maybe-clean-m4 clean-m4
maybe-clean-m4:

.PHONY: maybe-distclean-m4 distclean-m4
maybe-distclean-m4:

.PHONY: maybe-maintainer-clean-m4 maintainer-clean-m4
maybe-maintainer-clean-m4:



.PHONY: configure-readline maybe-configure-readline
maybe-configure-readline:





.PHONY: all-readline maybe-all-readline
maybe-all-readline:




.PHONY: check-readline maybe-check-readline
maybe-check-readline:

.PHONY: install-readline maybe-install-readline
maybe-install-readline:

.PHONY: install-strip-readline maybe-install-strip-readline
maybe-install-strip-readline:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-readline info-readline
maybe-info-readline:

.PHONY: maybe-dvi-readline dvi-readline
maybe-dvi-readline:

.PHONY: maybe-pdf-readline pdf-readline
maybe-pdf-readline:

.PHONY: maybe-html-readline html-readline
maybe-html-readline:

.PHONY: maybe-TAGS-readline TAGS-readline
maybe-TAGS-readline:

.PHONY: maybe-install-info-readline install-info-readline
maybe-install-info-readline:

.PHONY: maybe-install-pdf-readline install-pdf-readline
maybe-install-pdf-readline:

.PHONY: maybe-install-html-readline install-html-readline
maybe-install-html-readline:

.PHONY: maybe-installcheck-readline installcheck-readline
maybe-installcheck-readline:

.PHONY: maybe-mostlyclean-readline mostlyclean-readline
maybe-mostlyclean-readline:

.PHONY: maybe-clean-readline clean-readline
maybe-clean-readline:

.PHONY: maybe-distclean-readline distclean-readline
maybe-distclean-readline:

.PHONY: maybe-maintainer-clean-readline maintainer-clean-readline
maybe-maintainer-clean-readline:



.PHONY: configure-sid maybe-configure-sid
maybe-configure-sid:





.PHONY: all-sid maybe-all-sid
maybe-all-sid:




.PHONY: check-sid maybe-check-sid
maybe-check-sid:

.PHONY: install-sid maybe-install-sid
maybe-install-sid:

.PHONY: install-strip-sid maybe-install-strip-sid
maybe-install-strip-sid:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-sid info-sid
maybe-info-sid:

.PHONY: maybe-dvi-sid dvi-sid
maybe-dvi-sid:

.PHONY: maybe-pdf-sid pdf-sid
maybe-pdf-sid:

.PHONY: maybe-html-sid html-sid
maybe-html-sid:

.PHONY: maybe-TAGS-sid TAGS-sid
maybe-TAGS-sid:

.PHONY: maybe-install-info-sid install-info-sid
maybe-install-info-sid:

.PHONY: maybe-install-pdf-sid install-pdf-sid
maybe-install-pdf-sid:

.PHONY: maybe-install-html-sid install-html-sid
maybe-install-html-sid:

.PHONY: maybe-installcheck-sid installcheck-sid
maybe-installcheck-sid:

.PHONY: maybe-mostlyclean-sid mostlyclean-sid
maybe-mostlyclean-sid:

.PHONY: maybe-clean-sid clean-sid
maybe-clean-sid:

.PHONY: maybe-distclean-sid distclean-sid
maybe-distclean-sid:

.PHONY: maybe-maintainer-clean-sid maintainer-clean-sid
maybe-maintainer-clean-sid:



.PHONY: configure-sim maybe-configure-sim
maybe-configure-sim:





.PHONY: all-sim maybe-all-sim
maybe-all-sim:




.PHONY: check-sim maybe-check-sim
maybe-check-sim:

.PHONY: install-sim maybe-install-sim
maybe-install-sim:

.PHONY: install-strip-sim maybe-install-strip-sim
maybe-install-strip-sim:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-sim info-sim
maybe-info-sim:

.PHONY: maybe-dvi-sim dvi-sim
maybe-dvi-sim:

.PHONY: maybe-pdf-sim pdf-sim
maybe-pdf-sim:

.PHONY: maybe-html-sim html-sim
maybe-html-sim:

.PHONY: maybe-TAGS-sim TAGS-sim
maybe-TAGS-sim:

.PHONY: maybe-install-info-sim install-info-sim
maybe-install-info-sim:

.PHONY: maybe-install-pdf-sim install-pdf-sim
maybe-install-pdf-sim:

.PHONY: maybe-install-html-sim install-html-sim
maybe-install-html-sim:

.PHONY: maybe-installcheck-sim installcheck-sim
maybe-installcheck-sim:

.PHONY: maybe-mostlyclean-sim mostlyclean-sim
maybe-mostlyclean-sim:

.PHONY: maybe-clean-sim clean-sim
maybe-clean-sim:

.PHONY: maybe-distclean-sim distclean-sim
maybe-distclean-sim:

.PHONY: maybe-maintainer-clean-sim maintainer-clean-sim
maybe-maintainer-clean-sim:



.PHONY: configure-texinfo maybe-configure-texinfo
maybe-configure-texinfo:





.PHONY: all-texinfo maybe-all-texinfo
maybe-all-texinfo:




.PHONY: check-texinfo maybe-check-texinfo
maybe-check-texinfo:

.PHONY: install-texinfo maybe-install-texinfo
maybe-install-texinfo:

.PHONY: install-strip-texinfo maybe-install-strip-texinfo
maybe-install-strip-texinfo:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-texinfo info-texinfo
maybe-info-texinfo:

.PHONY: maybe-dvi-texinfo dvi-texinfo
maybe-dvi-texinfo:

.PHONY: maybe-pdf-texinfo pdf-texinfo
maybe-pdf-texinfo:

.PHONY: maybe-html-texinfo html-texinfo
maybe-html-texinfo:

.PHONY: maybe-TAGS-texinfo TAGS-texinfo
maybe-TAGS-texinfo:

.PHONY: maybe-install-info-texinfo install-info-texinfo
maybe-install-info-texinfo:

.PHONY: maybe-install-pdf-texinfo install-pdf-texinfo
maybe-install-pdf-texinfo:

.PHONY: maybe-install-html-texinfo install-html-texinfo
maybe-install-html-texinfo:

.PHONY: maybe-installcheck-texinfo installcheck-texinfo
maybe-installcheck-texinfo:

.PHONY: maybe-mostlyclean-texinfo mostlyclean-texinfo
maybe-mostlyclean-texinfo:

.PHONY: maybe-clean-texinfo clean-texinfo
maybe-clean-texinfo:

.PHONY: maybe-distclean-texinfo distclean-texinfo
maybe-distclean-texinfo:

.PHONY: maybe-maintainer-clean-texinfo maintainer-clean-texinfo
maybe-maintainer-clean-texinfo:



.PHONY: configure-zlib maybe-configure-zlib
maybe-configure-zlib:



.PHONY: configure-stage1-zlib maybe-configure-stage1-zlib
maybe-configure-stage1-zlib:

.PHONY: configure-stage2-zlib maybe-configure-stage2-zlib
maybe-configure-stage2-zlib:

.PHONY: configure-stage3-zlib maybe-configure-stage3-zlib
maybe-configure-stage3-zlib:

.PHONY: configure-stage4-zlib maybe-configure-stage4-zlib
maybe-configure-stage4-zlib:

.PHONY: configure-stageprofile-zlib maybe-configure-stageprofile-zlib
maybe-configure-stageprofile-zlib:

.PHONY: configure-stagefeedback-zlib maybe-configure-stagefeedback-zlib
maybe-configure-stagefeedback-zlib:





.PHONY: all-zlib maybe-all-zlib
maybe-all-zlib:



.PHONY: all-stage1-zlib maybe-all-stage1-zlib
.PHONY: clean-stage1-zlib maybe-clean-stage1-zlib
maybe-all-stage1-zlib:
maybe-clean-stage1-zlib:


.PHONY: all-stage2-zlib maybe-all-stage2-zlib
.PHONY: clean-stage2-zlib maybe-clean-stage2-zlib
maybe-all-stage2-zlib:
maybe-clean-stage2-zlib:


.PHONY: all-stage3-zlib maybe-all-stage3-zlib
.PHONY: clean-stage3-zlib maybe-clean-stage3-zlib
maybe-all-stage3-zlib:
maybe-clean-stage3-zlib:


.PHONY: all-stage4-zlib maybe-all-stage4-zlib
.PHONY: clean-stage4-zlib maybe-clean-stage4-zlib
maybe-all-stage4-zlib:
maybe-clean-stage4-zlib:


.PHONY: all-stageprofile-zlib maybe-all-stageprofile-zlib
.PHONY: clean-stageprofile-zlib maybe-clean-stageprofile-zlib
maybe-all-stageprofile-zlib:
maybe-clean-stageprofile-zlib:


.PHONY: all-stagefeedback-zlib maybe-all-stagefeedback-zlib
.PHONY: clean-stagefeedback-zlib maybe-clean-stagefeedback-zlib
maybe-all-stagefeedback-zlib:
maybe-clean-stagefeedback-zlib:





.PHONY: check-zlib maybe-check-zlib
maybe-check-zlib:

.PHONY: install-zlib maybe-install-zlib
maybe-install-zlib:

.PHONY: install-strip-zlib maybe-install-strip-zlib
maybe-install-strip-zlib:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-zlib info-zlib
maybe-info-zlib:

.PHONY: maybe-dvi-zlib dvi-zlib
maybe-dvi-zlib:

.PHONY: maybe-pdf-zlib pdf-zlib
maybe-pdf-zlib:

.PHONY: maybe-html-zlib html-zlib
maybe-html-zlib:

.PHONY: maybe-TAGS-zlib TAGS-zlib
maybe-TAGS-zlib:

.PHONY: maybe-install-info-zlib install-info-zlib
maybe-install-info-zlib:

.PHONY: maybe-install-pdf-zlib install-pdf-zlib
maybe-install-pdf-zlib:

.PHONY: maybe-install-html-zlib install-html-zlib
maybe-install-html-zlib:

.PHONY: maybe-installcheck-zlib installcheck-zlib
maybe-installcheck-zlib:

.PHONY: maybe-mostlyclean-zlib mostlyclean-zlib
maybe-mostlyclean-zlib:

.PHONY: maybe-clean-zlib clean-zlib
maybe-clean-zlib:

.PHONY: maybe-distclean-zlib distclean-zlib
maybe-distclean-zlib:

.PHONY: maybe-maintainer-clean-zlib maintainer-clean-zlib
maybe-maintainer-clean-zlib:



.PHONY: configure-gdb maybe-configure-gdb
maybe-configure-gdb:





.PHONY: all-gdb maybe-all-gdb
maybe-all-gdb:




.PHONY: check-gdb maybe-check-gdb
maybe-check-gdb:

.PHONY: install-gdb maybe-install-gdb
maybe-install-gdb:

.PHONY: install-strip-gdb maybe-install-strip-gdb
maybe-install-strip-gdb:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gdb info-gdb
maybe-info-gdb:

.PHONY: maybe-dvi-gdb dvi-gdb
maybe-dvi-gdb:

.PHONY: maybe-pdf-gdb pdf-gdb
maybe-pdf-gdb:

.PHONY: maybe-html-gdb html-gdb
maybe-html-gdb:

.PHONY: maybe-TAGS-gdb TAGS-gdb
maybe-TAGS-gdb:

.PHONY: maybe-install-info-gdb install-info-gdb
maybe-install-info-gdb:

.PHONY: maybe-install-pdf-gdb install-pdf-gdb
maybe-install-pdf-gdb:

.PHONY: maybe-install-html-gdb install-html-gdb
maybe-install-html-gdb:

.PHONY: maybe-installcheck-gdb installcheck-gdb
maybe-installcheck-gdb:

.PHONY: maybe-mostlyclean-gdb mostlyclean-gdb
maybe-mostlyclean-gdb:

.PHONY: maybe-clean-gdb clean-gdb
maybe-clean-gdb:

.PHONY: maybe-distclean-gdb distclean-gdb
maybe-distclean-gdb:

.PHONY: maybe-maintainer-clean-gdb maintainer-clean-gdb
maybe-maintainer-clean-gdb:



.PHONY: configure-expect maybe-configure-expect
maybe-configure-expect:





.PHONY: all-expect maybe-all-expect
maybe-all-expect:




.PHONY: check-expect maybe-check-expect
maybe-check-expect:

.PHONY: install-expect maybe-install-expect
maybe-install-expect:

.PHONY: install-strip-expect maybe-install-strip-expect
maybe-install-strip-expect:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-expect info-expect
maybe-info-expect:

.PHONY: maybe-dvi-expect dvi-expect
maybe-dvi-expect:

.PHONY: maybe-pdf-expect pdf-expect
maybe-pdf-expect:

.PHONY: maybe-html-expect html-expect
maybe-html-expect:

.PHONY: maybe-TAGS-expect TAGS-expect
maybe-TAGS-expect:

.PHONY: maybe-install-info-expect install-info-expect
maybe-install-info-expect:

.PHONY: maybe-install-pdf-expect install-pdf-expect
maybe-install-pdf-expect:

.PHONY: maybe-install-html-expect install-html-expect
maybe-install-html-expect:

.PHONY: maybe-installcheck-expect installcheck-expect
maybe-installcheck-expect:

.PHONY: maybe-mostlyclean-expect mostlyclean-expect
maybe-mostlyclean-expect:

.PHONY: maybe-clean-expect clean-expect
maybe-clean-expect:

.PHONY: maybe-distclean-expect distclean-expect
maybe-distclean-expect:

.PHONY: maybe-maintainer-clean-expect maintainer-clean-expect
maybe-maintainer-clean-expect:



.PHONY: configure-guile maybe-configure-guile
maybe-configure-guile:





.PHONY: all-guile maybe-all-guile
maybe-all-guile:




.PHONY: check-guile maybe-check-guile
maybe-check-guile:

.PHONY: install-guile maybe-install-guile
maybe-install-guile:

.PHONY: install-strip-guile maybe-install-strip-guile
maybe-install-strip-guile:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-guile info-guile
maybe-info-guile:

.PHONY: maybe-dvi-guile dvi-guile
maybe-dvi-guile:

.PHONY: maybe-pdf-guile pdf-guile
maybe-pdf-guile:

.PHONY: maybe-html-guile html-guile
maybe-html-guile:

.PHONY: maybe-TAGS-guile TAGS-guile
maybe-TAGS-guile:

.PHONY: maybe-install-info-guile install-info-guile
maybe-install-info-guile:

.PHONY: maybe-install-pdf-guile install-pdf-guile
maybe-install-pdf-guile:

.PHONY: maybe-install-html-guile install-html-guile
maybe-install-html-guile:

.PHONY: maybe-installcheck-guile installcheck-guile
maybe-installcheck-guile:

.PHONY: maybe-mostlyclean-guile mostlyclean-guile
maybe-mostlyclean-guile:

.PHONY: maybe-clean-guile clean-guile
maybe-clean-guile:

.PHONY: maybe-distclean-guile distclean-guile
maybe-distclean-guile:

.PHONY: maybe-maintainer-clean-guile maintainer-clean-guile
maybe-maintainer-clean-guile:



.PHONY: configure-tk maybe-configure-tk
maybe-configure-tk:





.PHONY: all-tk maybe-all-tk
maybe-all-tk:




.PHONY: check-tk maybe-check-tk
maybe-check-tk:

.PHONY: install-tk maybe-install-tk
maybe-install-tk:

.PHONY: install-strip-tk maybe-install-strip-tk
maybe-install-strip-tk:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-tk info-tk
maybe-info-tk:

.PHONY: maybe-dvi-tk dvi-tk
maybe-dvi-tk:

.PHONY: maybe-pdf-tk pdf-tk
maybe-pdf-tk:

.PHONY: maybe-html-tk html-tk
maybe-html-tk:

.PHONY: maybe-TAGS-tk TAGS-tk
maybe-TAGS-tk:

.PHONY: maybe-install-info-tk install-info-tk
maybe-install-info-tk:

.PHONY: maybe-install-pdf-tk install-pdf-tk
maybe-install-pdf-tk:

.PHONY: maybe-install-html-tk install-html-tk
maybe-install-html-tk:

.PHONY: maybe-installcheck-tk installcheck-tk
maybe-installcheck-tk:

.PHONY: maybe-mostlyclean-tk mostlyclean-tk
maybe-mostlyclean-tk:

.PHONY: maybe-clean-tk clean-tk
maybe-clean-tk:

.PHONY: maybe-distclean-tk distclean-tk
maybe-distclean-tk:

.PHONY: maybe-maintainer-clean-tk maintainer-clean-tk
maybe-maintainer-clean-tk:



.PHONY: configure-libtermcap maybe-configure-libtermcap
maybe-configure-libtermcap:





.PHONY: all-libtermcap maybe-all-libtermcap
maybe-all-libtermcap:




.PHONY: check-libtermcap maybe-check-libtermcap
maybe-check-libtermcap:

.PHONY: install-libtermcap maybe-install-libtermcap
maybe-install-libtermcap:

.PHONY: install-strip-libtermcap maybe-install-strip-libtermcap
maybe-install-strip-libtermcap:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-libtermcap info-libtermcap
maybe-info-libtermcap:

.PHONY: maybe-dvi-libtermcap dvi-libtermcap
maybe-dvi-libtermcap:

.PHONY: maybe-pdf-libtermcap pdf-libtermcap
maybe-pdf-libtermcap:

.PHONY: maybe-html-libtermcap html-libtermcap
maybe-html-libtermcap:

.PHONY: maybe-TAGS-libtermcap TAGS-libtermcap
maybe-TAGS-libtermcap:

.PHONY: maybe-install-info-libtermcap install-info-libtermcap
maybe-install-info-libtermcap:

.PHONY: maybe-install-pdf-libtermcap install-pdf-libtermcap
maybe-install-pdf-libtermcap:

.PHONY: maybe-install-html-libtermcap install-html-libtermcap
maybe-install-html-libtermcap:

.PHONY: maybe-installcheck-libtermcap installcheck-libtermcap
maybe-installcheck-libtermcap:

.PHONY: maybe-mostlyclean-libtermcap mostlyclean-libtermcap
maybe-mostlyclean-libtermcap:

.PHONY: maybe-clean-libtermcap clean-libtermcap
maybe-clean-libtermcap:

.PHONY: maybe-distclean-libtermcap distclean-libtermcap
maybe-distclean-libtermcap:

.PHONY: maybe-maintainer-clean-libtermcap maintainer-clean-libtermcap
maybe-maintainer-clean-libtermcap:



.PHONY: configure-utils maybe-configure-utils
maybe-configure-utils:





.PHONY: all-utils maybe-all-utils
maybe-all-utils:




.PHONY: check-utils maybe-check-utils
maybe-check-utils:

.PHONY: install-utils maybe-install-utils
maybe-install-utils:

.PHONY: install-strip-utils maybe-install-strip-utils
maybe-install-strip-utils:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-utils info-utils
maybe-info-utils:

.PHONY: maybe-dvi-utils dvi-utils
maybe-dvi-utils:

.PHONY: maybe-pdf-utils pdf-utils
maybe-pdf-utils:

.PHONY: maybe-html-utils html-utils
maybe-html-utils:

.PHONY: maybe-TAGS-utils TAGS-utils
maybe-TAGS-utils:

.PHONY: maybe-install-info-utils install-info-utils
maybe-install-info-utils:

.PHONY: maybe-install-pdf-utils install-pdf-utils
maybe-install-pdf-utils:

.PHONY: maybe-install-html-utils install-html-utils
maybe-install-html-utils:

.PHONY: maybe-installcheck-utils installcheck-utils
maybe-installcheck-utils:

.PHONY: maybe-mostlyclean-utils mostlyclean-utils
maybe-mostlyclean-utils:

.PHONY: maybe-clean-utils clean-utils
maybe-clean-utils:

.PHONY: maybe-distclean-utils distclean-utils
maybe-distclean-utils:

.PHONY: maybe-maintainer-clean-utils maintainer-clean-utils
maybe-maintainer-clean-utils:



.PHONY: configure-gnattools maybe-configure-gnattools
maybe-configure-gnattools:





.PHONY: all-gnattools maybe-all-gnattools
maybe-all-gnattools:




.PHONY: check-gnattools maybe-check-gnattools
maybe-check-gnattools:

.PHONY: install-gnattools maybe-install-gnattools
maybe-install-gnattools:

.PHONY: install-strip-gnattools maybe-install-strip-gnattools
maybe-install-strip-gnattools:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-gnattools info-gnattools
maybe-info-gnattools:

.PHONY: maybe-dvi-gnattools dvi-gnattools
maybe-dvi-gnattools:

.PHONY: maybe-pdf-gnattools pdf-gnattools
maybe-pdf-gnattools:

.PHONY: maybe-html-gnattools html-gnattools
maybe-html-gnattools:

.PHONY: maybe-TAGS-gnattools TAGS-gnattools
maybe-TAGS-gnattools:

.PHONY: maybe-install-info-gnattools install-info-gnattools
maybe-install-info-gnattools:

.PHONY: maybe-install-pdf-gnattools install-pdf-gnattools
maybe-install-pdf-gnattools:

.PHONY: maybe-install-html-gnattools install-html-gnattools
maybe-install-html-gnattools:

.PHONY: maybe-installcheck-gnattools installcheck-gnattools
maybe-installcheck-gnattools:

.PHONY: maybe-mostlyclean-gnattools mostlyclean-gnattools
maybe-mostlyclean-gnattools:

.PHONY: maybe-clean-gnattools clean-gnattools
maybe-clean-gnattools:

.PHONY: maybe-distclean-gnattools distclean-gnattools
maybe-distclean-gnattools:

.PHONY: maybe-maintainer-clean-gnattools maintainer-clean-gnattools
maybe-maintainer-clean-gnattools:



.PHONY: configure-lto-plugin maybe-configure-lto-plugin
maybe-configure-lto-plugin:



.PHONY: configure-stage1-lto-plugin maybe-configure-stage1-lto-plugin
maybe-configure-stage1-lto-plugin:

.PHONY: configure-stage2-lto-plugin maybe-configure-stage2-lto-plugin
maybe-configure-stage2-lto-plugin:

.PHONY: configure-stage3-lto-plugin maybe-configure-stage3-lto-plugin
maybe-configure-stage3-lto-plugin:

.PHONY: configure-stage4-lto-plugin maybe-configure-stage4-lto-plugin
maybe-configure-stage4-lto-plugin:

.PHONY: configure-stageprofile-lto-plugin maybe-configure-stageprofile-lto-plugin
maybe-configure-stageprofile-lto-plugin:

.PHONY: configure-stagefeedback-lto-plugin maybe-configure-stagefeedback-lto-plugin
maybe-configure-stagefeedback-lto-plugin:





.PHONY: all-lto-plugin maybe-all-lto-plugin
maybe-all-lto-plugin:



.PHONY: all-stage1-lto-plugin maybe-all-stage1-lto-plugin
.PHONY: clean-stage1-lto-plugin maybe-clean-stage1-lto-plugin
maybe-all-stage1-lto-plugin:
maybe-clean-stage1-lto-plugin:


.PHONY: all-stage2-lto-plugin maybe-all-stage2-lto-plugin
.PHONY: clean-stage2-lto-plugin maybe-clean-stage2-lto-plugin
maybe-all-stage2-lto-plugin:
maybe-clean-stage2-lto-plugin:


.PHONY: all-stage3-lto-plugin maybe-all-stage3-lto-plugin
.PHONY: clean-stage3-lto-plugin maybe-clean-stage3-lto-plugin
maybe-all-stage3-lto-plugin:
maybe-clean-stage3-lto-plugin:


.PHONY: all-stage4-lto-plugin maybe-all-stage4-lto-plugin
.PHONY: clean-stage4-lto-plugin maybe-clean-stage4-lto-plugin
maybe-all-stage4-lto-plugin:
maybe-clean-stage4-lto-plugin:


.PHONY: all-stageprofile-lto-plugin maybe-all-stageprofile-lto-plugin
.PHONY: clean-stageprofile-lto-plugin maybe-clean-stageprofile-lto-plugin
maybe-all-stageprofile-lto-plugin:
maybe-clean-stageprofile-lto-plugin:


.PHONY: all-stagefeedback-lto-plugin maybe-all-stagefeedback-lto-plugin
.PHONY: clean-stagefeedback-lto-plugin maybe-clean-stagefeedback-lto-plugin
maybe-all-stagefeedback-lto-plugin:
maybe-clean-stagefeedback-lto-plugin:





.PHONY: check-lto-plugin maybe-check-lto-plugin
maybe-check-lto-plugin:

.PHONY: install-lto-plugin maybe-install-lto-plugin
maybe-install-lto-plugin:

.PHONY: install-strip-lto-plugin maybe-install-strip-lto-plugin
maybe-install-strip-lto-plugin:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-lto-plugin info-lto-plugin
maybe-info-lto-plugin:

.PHONY: maybe-dvi-lto-plugin dvi-lto-plugin
maybe-dvi-lto-plugin:

.PHONY: maybe-pdf-lto-plugin pdf-lto-plugin
maybe-pdf-lto-plugin:

.PHONY: maybe-html-lto-plugin html-lto-plugin
maybe-html-lto-plugin:

.PHONY: maybe-TAGS-lto-plugin TAGS-lto-plugin
maybe-TAGS-lto-plugin:

.PHONY: maybe-install-info-lto-plugin install-info-lto-plugin
maybe-install-info-lto-plugin:

.PHONY: maybe-install-pdf-lto-plugin install-pdf-lto-plugin
maybe-install-pdf-lto-plugin:

.PHONY: maybe-install-html-lto-plugin install-html-lto-plugin
maybe-install-html-lto-plugin:

.PHONY: maybe-installcheck-lto-plugin installcheck-lto-plugin
maybe-installcheck-lto-plugin:

.PHONY: maybe-mostlyclean-lto-plugin mostlyclean-lto-plugin
maybe-mostlyclean-lto-plugin:

.PHONY: maybe-clean-lto-plugin clean-lto-plugin
maybe-clean-lto-plugin:

.PHONY: maybe-distclean-lto-plugin distclean-lto-plugin
maybe-distclean-lto-plugin:

.PHONY: maybe-maintainer-clean-lto-plugin maintainer-clean-lto-plugin
maybe-maintainer-clean-lto-plugin:



# ---------------------------------------
# Modules which run on the target machine
# ---------------------------------------




.PHONY: configure-target-libstdc++-v3 maybe-configure-target-libstdc++-v3
maybe-configure-target-libstdc++-v3:



.PHONY: configure-stage1-target-libstdc++-v3 maybe-configure-stage1-target-libstdc++-v3
maybe-configure-stage1-target-libstdc++-v3:

.PHONY: configure-stage2-target-libstdc++-v3 maybe-configure-stage2-target-libstdc++-v3
maybe-configure-stage2-target-libstdc++-v3:

.PHONY: configure-stage3-target-libstdc++-v3 maybe-configure-stage3-target-libstdc++-v3
maybe-configure-stage3-target-libstdc++-v3:

.PHONY: configure-stage4-target-libstdc++-v3 maybe-configure-stage4-target-libstdc++-v3
maybe-configure-stage4-target-libstdc++-v3:

.PHONY: configure-stageprofile-target-libstdc++-v3 maybe-configure-stageprofile-target-libstdc++-v3
maybe-configure-stageprofile-target-libstdc++-v3:

.PHONY: configure-stagefeedback-target-libstdc++-v3 maybe-configure-stagefeedback-target-libstdc++-v3
maybe-configure-stagefeedback-target-libstdc++-v3:





.PHONY: all-target-libstdc++-v3 maybe-all-target-libstdc++-v3
maybe-all-target-libstdc++-v3:



.PHONY: all-stage1-target-libstdc++-v3 maybe-all-stage1-target-libstdc++-v3
.PHONY: clean-stage1-target-libstdc++-v3 maybe-clean-stage1-target-libstdc++-v3
maybe-all-stage1-target-libstdc++-v3:
maybe-clean-stage1-target-libstdc++-v3:


.PHONY: all-stage2-target-libstdc++-v3 maybe-all-stage2-target-libstdc++-v3
.PHONY: clean-stage2-target-libstdc++-v3 maybe-clean-stage2-target-libstdc++-v3
maybe-all-stage2-target-libstdc++-v3:
maybe-clean-stage2-target-libstdc++-v3:


.PHONY: all-stage3-target-libstdc++-v3 maybe-all-stage3-target-libstdc++-v3
.PHONY: clean-stage3-target-libstdc++-v3 maybe-clean-stage3-target-libstdc++-v3
maybe-all-stage3-target-libstdc++-v3:
maybe-clean-stage3-target-libstdc++-v3:


.PHONY: all-stage4-target-libstdc++-v3 maybe-all-stage4-target-libstdc++-v3
.PHONY: clean-stage4-target-libstdc++-v3 maybe-clean-stage4-target-libstdc++-v3
maybe-all-stage4-target-libstdc++-v3:
maybe-clean-stage4-target-libstdc++-v3:


.PHONY: all-stageprofile-target-libstdc++-v3 maybe-all-stageprofile-target-libstdc++-v3
.PHONY: clean-stageprofile-target-libstdc++-v3 maybe-clean-stageprofile-target-libstdc++-v3
maybe-all-stageprofile-target-libstdc++-v3:
maybe-clean-stageprofile-target-libstdc++-v3:


.PHONY: all-stagefeedback-target-libstdc++-v3 maybe-all-stagefeedback-target-libstdc++-v3
.PHONY: clean-stagefeedback-target-libstdc++-v3 maybe-clean-stagefeedback-target-libstdc++-v3
maybe-all-stagefeedback-target-libstdc++-v3:
maybe-clean-stagefeedback-target-libstdc++-v3:






.PHONY: check-target-libstdc++-v3 maybe-check-target-libstdc++-v3
maybe-check-target-libstdc++-v3:

.PHONY: install-target-libstdc++-v3 maybe-install-target-libstdc++-v3
maybe-install-target-libstdc++-v3:

.PHONY: install-strip-target-libstdc++-v3 maybe-install-strip-target-libstdc++-v3
maybe-install-strip-target-libstdc++-v3:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libstdc++-v3 info-target-libstdc++-v3
maybe-info-target-libstdc++-v3:

.PHONY: maybe-dvi-target-libstdc++-v3 dvi-target-libstdc++-v3
maybe-dvi-target-libstdc++-v3:

.PHONY: maybe-pdf-target-libstdc++-v3 pdf-target-libstdc++-v3
maybe-pdf-target-libstdc++-v3:

.PHONY: maybe-html-target-libstdc++-v3 html-target-libstdc++-v3
maybe-html-target-libstdc++-v3:

.PHONY: maybe-TAGS-target-libstdc++-v3 TAGS-target-libstdc++-v3
maybe-TAGS-target-libstdc++-v3:

.PHONY: maybe-install-info-target-libstdc++-v3 install-info-target-libstdc++-v3
maybe-install-info-target-libstdc++-v3:

.PHONY: maybe-install-pdf-target-libstdc++-v3 install-pdf-target-libstdc++-v3
maybe-install-pdf-target-libstdc++-v3:

.PHONY: maybe-install-html-target-libstdc++-v3 install-html-target-libstdc++-v3
maybe-install-html-target-libstdc++-v3:

.PHONY: maybe-installcheck-target-libstdc++-v3 installcheck-target-libstdc++-v3
maybe-installcheck-target-libstdc++-v3:

.PHONY: maybe-mostlyclean-target-libstdc++-v3 mostlyclean-target-libstdc++-v3
maybe-mostlyclean-target-libstdc++-v3:

.PHONY: maybe-clean-target-libstdc++-v3 clean-target-libstdc++-v3
maybe-clean-target-libstdc++-v3:

.PHONY: maybe-distclean-target-libstdc++-v3 distclean-target-libstdc++-v3
maybe-distclean-target-libstdc++-v3:

.PHONY: maybe-maintainer-clean-target-libstdc++-v3 maintainer-clean-target-libstdc++-v3
maybe-maintainer-clean-target-libstdc++-v3:





.PHONY: configure-target-libmudflap maybe-configure-target-libmudflap
maybe-configure-target-libmudflap:





.PHONY: all-target-libmudflap maybe-all-target-libmudflap
maybe-all-target-libmudflap:





.PHONY: check-target-libmudflap maybe-check-target-libmudflap
maybe-check-target-libmudflap:

.PHONY: install-target-libmudflap maybe-install-target-libmudflap
maybe-install-target-libmudflap:

.PHONY: install-strip-target-libmudflap maybe-install-strip-target-libmudflap
maybe-install-strip-target-libmudflap:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libmudflap info-target-libmudflap
maybe-info-target-libmudflap:

.PHONY: maybe-dvi-target-libmudflap dvi-target-libmudflap
maybe-dvi-target-libmudflap:

.PHONY: maybe-pdf-target-libmudflap pdf-target-libmudflap
maybe-pdf-target-libmudflap:

.PHONY: maybe-html-target-libmudflap html-target-libmudflap
maybe-html-target-libmudflap:

.PHONY: maybe-TAGS-target-libmudflap TAGS-target-libmudflap
maybe-TAGS-target-libmudflap:

.PHONY: maybe-install-info-target-libmudflap install-info-target-libmudflap
maybe-install-info-target-libmudflap:

.PHONY: maybe-install-pdf-target-libmudflap install-pdf-target-libmudflap
maybe-install-pdf-target-libmudflap:

.PHONY: maybe-install-html-target-libmudflap install-html-target-libmudflap
maybe-install-html-target-libmudflap:

.PHONY: maybe-installcheck-target-libmudflap installcheck-target-libmudflap
maybe-installcheck-target-libmudflap:

.PHONY: maybe-mostlyclean-target-libmudflap mostlyclean-target-libmudflap
maybe-mostlyclean-target-libmudflap:

.PHONY: maybe-clean-target-libmudflap clean-target-libmudflap
maybe-clean-target-libmudflap:

.PHONY: maybe-distclean-target-libmudflap distclean-target-libmudflap
maybe-distclean-target-libmudflap:

.PHONY: maybe-maintainer-clean-target-libmudflap maintainer-clean-target-libmudflap
maybe-maintainer-clean-target-libmudflap:





.PHONY: configure-target-libsanitizer maybe-configure-target-libsanitizer
maybe-configure-target-libsanitizer:



.PHONY: configure-stage1-target-libsanitizer maybe-configure-stage1-target-libsanitizer
maybe-configure-stage1-target-libsanitizer:

.PHONY: configure-stage2-target-libsanitizer maybe-configure-stage2-target-libsanitizer
maybe-configure-stage2-target-libsanitizer:

.PHONY: configure-stage3-target-libsanitizer maybe-configure-stage3-target-libsanitizer
maybe-configure-stage3-target-libsanitizer:

.PHONY: configure-stage4-target-libsanitizer maybe-configure-stage4-target-libsanitizer
maybe-configure-stage4-target-libsanitizer:

.PHONY: configure-stageprofile-target-libsanitizer maybe-configure-stageprofile-target-libsanitizer
maybe-configure-stageprofile-target-libsanitizer:

.PHONY: configure-stagefeedback-target-libsanitizer maybe-configure-stagefeedback-target-libsanitizer
maybe-configure-stagefeedback-target-libsanitizer:





.PHONY: all-target-libsanitizer maybe-all-target-libsanitizer
maybe-all-target-libsanitizer:



.PHONY: all-stage1-target-libsanitizer maybe-all-stage1-target-libsanitizer
.PHONY: clean-stage1-target-libsanitizer maybe-clean-stage1-target-libsanitizer
maybe-all-stage1-target-libsanitizer:
maybe-clean-stage1-target-libsanitizer:


.PHONY: all-stage2-target-libsanitizer maybe-all-stage2-target-libsanitizer
.PHONY: clean-stage2-target-libsanitizer maybe-clean-stage2-target-libsanitizer
maybe-all-stage2-target-libsanitizer:
maybe-clean-stage2-target-libsanitizer:


.PHONY: all-stage3-target-libsanitizer maybe-all-stage3-target-libsanitizer
.PHONY: clean-stage3-target-libsanitizer maybe-clean-stage3-target-libsanitizer
maybe-all-stage3-target-libsanitizer:
maybe-clean-stage3-target-libsanitizer:


.PHONY: all-stage4-target-libsanitizer maybe-all-stage4-target-libsanitizer
.PHONY: clean-stage4-target-libsanitizer maybe-clean-stage4-target-libsanitizer
maybe-all-stage4-target-libsanitizer:
maybe-clean-stage4-target-libsanitizer:


.PHONY: all-stageprofile-target-libsanitizer maybe-all-stageprofile-target-libsanitizer
.PHONY: clean-stageprofile-target-libsanitizer maybe-clean-stageprofile-target-libsanitizer
maybe-all-stageprofile-target-libsanitizer:
maybe-clean-stageprofile-target-libsanitizer:


.PHONY: all-stagefeedback-target-libsanitizer maybe-all-stagefeedback-target-libsanitizer
.PHONY: clean-stagefeedback-target-libsanitizer maybe-clean-stagefeedback-target-libsanitizer
maybe-all-stagefeedback-target-libsanitizer:
maybe-clean-stagefeedback-target-libsanitizer:






.PHONY: check-target-libsanitizer maybe-check-target-libsanitizer
maybe-check-target-libsanitizer:

.PHONY: install-target-libsanitizer maybe-install-target-libsanitizer
maybe-install-target-libsanitizer:

.PHONY: install-strip-target-libsanitizer maybe-install-strip-target-libsanitizer
maybe-install-strip-target-libsanitizer:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libsanitizer info-target-libsanitizer
maybe-info-target-libsanitizer:

.PHONY: maybe-dvi-target-libsanitizer dvi-target-libsanitizer
maybe-dvi-target-libsanitizer:

.PHONY: maybe-pdf-target-libsanitizer pdf-target-libsanitizer
maybe-pdf-target-libsanitizer:

.PHONY: maybe-html-target-libsanitizer html-target-libsanitizer
maybe-html-target-libsanitizer:

.PHONY: maybe-TAGS-target-libsanitizer TAGS-target-libsanitizer
maybe-TAGS-target-libsanitizer:

.PHONY: maybe-install-info-target-libsanitizer install-info-target-libsanitizer
maybe-install-info-target-libsanitizer:

.PHONY: maybe-install-pdf-target-libsanitizer install-pdf-target-libsanitizer
maybe-install-pdf-target-libsanitizer:

.PHONY: maybe-install-html-target-libsanitizer install-html-target-libsanitizer
maybe-install-html-target-libsanitizer:

.PHONY: maybe-installcheck-target-libsanitizer installcheck-target-libsanitizer
maybe-installcheck-target-libsanitizer:

.PHONY: maybe-mostlyclean-target-libsanitizer mostlyclean-target-libsanitizer
maybe-mostlyclean-target-libsanitizer:

.PHONY: maybe-clean-target-libsanitizer clean-target-libsanitizer
maybe-clean-target-libsanitizer:

.PHONY: maybe-distclean-target-libsanitizer distclean-target-libsanitizer
maybe-distclean-target-libsanitizer:

.PHONY: maybe-maintainer-clean-target-libsanitizer maintainer-clean-target-libsanitizer
maybe-maintainer-clean-target-libsanitizer:





.PHONY: configure-target-libvtv maybe-configure-target-libvtv
maybe-configure-target-libvtv:



.PHONY: configure-stage1-target-libvtv maybe-configure-stage1-target-libvtv
maybe-configure-stage1-target-libvtv:

.PHONY: configure-stage2-target-libvtv maybe-configure-stage2-target-libvtv
maybe-configure-stage2-target-libvtv:

.PHONY: configure-stage3-target-libvtv maybe-configure-stage3-target-libvtv
maybe-configure-stage3-target-libvtv:

.PHONY: configure-stage4-target-libvtv maybe-configure-stage4-target-libvtv
maybe-configure-stage4-target-libvtv:

.PHONY: configure-stageprofile-target-libvtv maybe-configure-stageprofile-target-libvtv
maybe-configure-stageprofile-target-libvtv:

.PHONY: configure-stagefeedback-target-libvtv maybe-configure-stagefeedback-target-libvtv
maybe-configure-stagefeedback-target-libvtv:





.PHONY: all-target-libvtv maybe-all-target-libvtv
maybe-all-target-libvtv:



.PHONY: all-stage1-target-libvtv maybe-all-stage1-target-libvtv
.PHONY: clean-stage1-target-libvtv maybe-clean-stage1-target-libvtv
maybe-all-stage1-target-libvtv:
maybe-clean-stage1-target-libvtv:


.PHONY: all-stage2-target-libvtv maybe-all-stage2-target-libvtv
.PHONY: clean-stage2-target-libvtv maybe-clean-stage2-target-libvtv
maybe-all-stage2-target-libvtv:
maybe-clean-stage2-target-libvtv:


.PHONY: all-stage3-target-libvtv maybe-all-stage3-target-libvtv
.PHONY: clean-stage3-target-libvtv maybe-clean-stage3-target-libvtv
maybe-all-stage3-target-libvtv:
maybe-clean-stage3-target-libvtv:


.PHONY: all-stage4-target-libvtv maybe-all-stage4-target-libvtv
.PHONY: clean-stage4-target-libvtv maybe-clean-stage4-target-libvtv
maybe-all-stage4-target-libvtv:
maybe-clean-stage4-target-libvtv:


.PHONY: all-stageprofile-target-libvtv maybe-all-stageprofile-target-libvtv
.PHONY: clean-stageprofile-target-libvtv maybe-clean-stageprofile-target-libvtv
maybe-all-stageprofile-target-libvtv:
maybe-clean-stageprofile-target-libvtv:


.PHONY: all-stagefeedback-target-libvtv maybe-all-stagefeedback-target-libvtv
.PHONY: clean-stagefeedback-target-libvtv maybe-clean-stagefeedback-target-libvtv
maybe-all-stagefeedback-target-libvtv:
maybe-clean-stagefeedback-target-libvtv:






.PHONY: check-target-libvtv maybe-check-target-libvtv
maybe-check-target-libvtv:

.PHONY: install-target-libvtv maybe-install-target-libvtv
maybe-install-target-libvtv:

.PHONY: install-strip-target-libvtv maybe-install-strip-target-libvtv
maybe-install-strip-target-libvtv:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libvtv info-target-libvtv
maybe-info-target-libvtv:

.PHONY: maybe-dvi-target-libvtv dvi-target-libvtv
maybe-dvi-target-libvtv:

.PHONY: maybe-pdf-target-libvtv pdf-target-libvtv
maybe-pdf-target-libvtv:

.PHONY: maybe-html-target-libvtv html-target-libvtv
maybe-html-target-libvtv:

.PHONY: maybe-TAGS-target-libvtv TAGS-target-libvtv
maybe-TAGS-target-libvtv:

.PHONY: maybe-install-info-target-libvtv install-info-target-libvtv
maybe-install-info-target-libvtv:

.PHONY: maybe-install-pdf-target-libvtv install-pdf-target-libvtv
maybe-install-pdf-target-libvtv:

.PHONY: maybe-install-html-target-libvtv install-html-target-libvtv
maybe-install-html-target-libvtv:

.PHONY: maybe-installcheck-target-libvtv installcheck-target-libvtv
maybe-installcheck-target-libvtv:

.PHONY: maybe-mostlyclean-target-libvtv mostlyclean-target-libvtv
maybe-mostlyclean-target-libvtv:

.PHONY: maybe-clean-target-libvtv clean-target-libvtv
maybe-clean-target-libvtv:

.PHONY: maybe-distclean-target-libvtv distclean-target-libvtv
maybe-distclean-target-libvtv:

.PHONY: maybe-maintainer-clean-target-libvtv maintainer-clean-target-libvtv
maybe-maintainer-clean-target-libvtv:





.PHONY: configure-target-libssp maybe-configure-target-libssp
maybe-configure-target-libssp:





.PHONY: all-target-libssp maybe-all-target-libssp
maybe-all-target-libssp:





.PHONY: check-target-libssp maybe-check-target-libssp
maybe-check-target-libssp:

.PHONY: install-target-libssp maybe-install-target-libssp
maybe-install-target-libssp:

.PHONY: install-strip-target-libssp maybe-install-strip-target-libssp
maybe-install-strip-target-libssp:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libssp info-target-libssp
maybe-info-target-libssp:

.PHONY: maybe-dvi-target-libssp dvi-target-libssp
maybe-dvi-target-libssp:

.PHONY: maybe-pdf-target-libssp pdf-target-libssp
maybe-pdf-target-libssp:

.PHONY: maybe-html-target-libssp html-target-libssp
maybe-html-target-libssp:

.PHONY: maybe-TAGS-target-libssp TAGS-target-libssp
maybe-TAGS-target-libssp:

.PHONY: maybe-install-info-target-libssp install-info-target-libssp
maybe-install-info-target-libssp:

.PHONY: maybe-install-pdf-target-libssp install-pdf-target-libssp
maybe-install-pdf-target-libssp:

.PHONY: maybe-install-html-target-libssp install-html-target-libssp
maybe-install-html-target-libssp:

.PHONY: maybe-installcheck-target-libssp installcheck-target-libssp
maybe-installcheck-target-libssp:

.PHONY: maybe-mostlyclean-target-libssp mostlyclean-target-libssp
maybe-mostlyclean-target-libssp:

.PHONY: maybe-clean-target-libssp clean-target-libssp
maybe-clean-target-libssp:

.PHONY: maybe-distclean-target-libssp distclean-target-libssp
maybe-distclean-target-libssp:

.PHONY: maybe-maintainer-clean-target-libssp maintainer-clean-target-libssp
maybe-maintainer-clean-target-libssp:





.PHONY: configure-target-newlib maybe-configure-target-newlib
maybe-configure-target-newlib:
maybe-configure-target-newlib: configure-target-newlib
configure-target-newlib: 
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	echo "Checking multilib configuration for newlib..."; \
	$(SHELL) $(srcdir)/mkinstalldirs $(TARGET_SUBDIR)/newlib ; \
	$(CC_FOR_TARGET) --print-multi-lib > $(TARGET_SUBDIR)/newlib/multilib.tmp 2> /dev/null ; \
	if test -r $(TARGET_SUBDIR)/newlib/multilib.out; then \
	  if cmp -s $(TARGET_SUBDIR)/newlib/multilib.tmp $(TARGET_SUBDIR)/newlib/multilib.out; then \
	    rm -f $(TARGET_SUBDIR)/newlib/multilib.tmp; \
	  else \
	    rm -f $(TARGET_SUBDIR)/newlib/Makefile; \
	    mv $(TARGET_SUBDIR)/newlib/multilib.tmp $(TARGET_SUBDIR)/newlib/multilib.out; \
	  fi; \
	else \
	  mv $(TARGET_SUBDIR)/newlib/multilib.tmp $(TARGET_SUBDIR)/newlib/multilib.out; \
	fi; \
	test ! -f $(TARGET_SUBDIR)/newlib/Makefile || exit 0; \
	$(SHELL) $(srcdir)/mkinstalldirs $(TARGET_SUBDIR)/newlib ; \
	$(NORMAL_TARGET_EXPORTS)  \
	echo Configuring in $(TARGET_SUBDIR)/newlib; \
	cd "$(TARGET_SUBDIR)/newlib" || exit 1; \
	case $(srcdir) in \
	  /* | [A-Za-z]:[\\/]*) topdir=$(srcdir) ;; \
	  *) topdir=`echo $(TARGET_SUBDIR)/newlib/ | \
		sed -e 's,\./,,g' -e 's,[^/]*/,../,g' `$(srcdir) ;; \
	esac; \
	srcdiroption="--srcdir=$${topdir}/newlib"; \
	libsrcdir="$$s/newlib"; \
	rm -f no-such-file || : ; \
	CONFIG_SITE=no-such-file $(SHELL) $${libsrcdir}/configure \
	  $(TARGET_CONFIGARGS) --build=${build_alias} --host=${target_alias} \
	  --target=${target_alias} $${srcdiroption}  \
	  || exit 1





.PHONY: all-target-newlib maybe-all-target-newlib
maybe-all-target-newlib:
TARGET-target-newlib=all
maybe-all-target-newlib: all-target-newlib
all-target-newlib: configure-target-newlib
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS)  \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) $(EXTRA_TARGET_FLAGS)   \
		$(TARGET-target-newlib))





.PHONY: check-target-newlib maybe-check-target-newlib
maybe-check-target-newlib:
maybe-check-target-newlib: check-target-newlib

check-target-newlib:
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(TARGET_FLAGS_TO_PASS)   check)


.PHONY: install-target-newlib maybe-install-target-newlib
maybe-install-target-newlib:
maybe-install-target-newlib: install-target-newlib

install-target-newlib: installdirs
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(TARGET_FLAGS_TO_PASS)  install)


.PHONY: install-strip-target-newlib maybe-install-strip-target-newlib
maybe-install-strip-target-newlib:
maybe-install-strip-target-newlib: install-strip-target-newlib

install-strip-target-newlib: installdirs
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(TARGET_FLAGS_TO_PASS)  install-strip)


# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-newlib info-target-newlib
maybe-info-target-newlib:
maybe-info-target-newlib: info-target-newlib

info-target-newlib: \
    configure-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing info in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           info) \
	  || exit 1


.PHONY: maybe-dvi-target-newlib dvi-target-newlib
maybe-dvi-target-newlib:
maybe-dvi-target-newlib: dvi-target-newlib

dvi-target-newlib: \
    configure-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing dvi in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           dvi) \
	  || exit 1


.PHONY: maybe-pdf-target-newlib pdf-target-newlib
maybe-pdf-target-newlib:
maybe-pdf-target-newlib: pdf-target-newlib

pdf-target-newlib: \
    configure-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing pdf in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           pdf) \
	  || exit 1


.PHONY: maybe-html-target-newlib html-target-newlib
maybe-html-target-newlib:
maybe-html-target-newlib: html-target-newlib

html-target-newlib: \
    configure-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing html in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           html) \
	  || exit 1


.PHONY: maybe-TAGS-target-newlib TAGS-target-newlib
maybe-TAGS-target-newlib:
maybe-TAGS-target-newlib: TAGS-target-newlib

TAGS-target-newlib: \
    configure-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing TAGS in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           TAGS) \
	  || exit 1


.PHONY: maybe-install-info-target-newlib install-info-target-newlib
maybe-install-info-target-newlib:
maybe-install-info-target-newlib: install-info-target-newlib

install-info-target-newlib: \
    configure-target-newlib \
    info-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing install-info in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           install-info) \
	  || exit 1


.PHONY: maybe-install-pdf-target-newlib install-pdf-target-newlib
maybe-install-pdf-target-newlib:
maybe-install-pdf-target-newlib: install-pdf-target-newlib

install-pdf-target-newlib: \
    configure-target-newlib \
    pdf-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing install-pdf in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           install-pdf) \
	  || exit 1


.PHONY: maybe-install-html-target-newlib install-html-target-newlib
maybe-install-html-target-newlib:
maybe-install-html-target-newlib: install-html-target-newlib

install-html-target-newlib: \
    configure-target-newlib \
    html-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing install-html in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           install-html) \
	  || exit 1


.PHONY: maybe-installcheck-target-newlib installcheck-target-newlib
maybe-installcheck-target-newlib:
maybe-installcheck-target-newlib: installcheck-target-newlib

installcheck-target-newlib: \
    configure-target-newlib 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing installcheck in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           installcheck) \
	  || exit 1


.PHONY: maybe-mostlyclean-target-newlib mostlyclean-target-newlib
maybe-mostlyclean-target-newlib:
maybe-mostlyclean-target-newlib: mostlyclean-target-newlib

mostlyclean-target-newlib: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing mostlyclean in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           mostlyclean) \
	  || exit 1


.PHONY: maybe-clean-target-newlib clean-target-newlib
maybe-clean-target-newlib:
maybe-clean-target-newlib: clean-target-newlib

clean-target-newlib: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing clean in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           clean) \
	  || exit 1


.PHONY: maybe-distclean-target-newlib distclean-target-newlib
maybe-distclean-target-newlib:
maybe-distclean-target-newlib: distclean-target-newlib

distclean-target-newlib: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing distclean in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           distclean) \
	  || exit 1


.PHONY: maybe-maintainer-clean-target-newlib maintainer-clean-target-newlib
maybe-maintainer-clean-target-newlib:
maybe-maintainer-clean-target-newlib: maintainer-clean-target-newlib

maintainer-clean-target-newlib: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/newlib/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing maintainer-clean in $(TARGET_SUBDIR)/newlib" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/newlib && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           maintainer-clean) \
	  || exit 1






.PHONY: configure-target-libgcc maybe-configure-target-libgcc
maybe-configure-target-libgcc:



.PHONY: configure-stage1-target-libgcc maybe-configure-stage1-target-libgcc
maybe-configure-stage1-target-libgcc:

.PHONY: configure-stage2-target-libgcc maybe-configure-stage2-target-libgcc
maybe-configure-stage2-target-libgcc:

.PHONY: configure-stage3-target-libgcc maybe-configure-stage3-target-libgcc
maybe-configure-stage3-target-libgcc:

.PHONY: configure-stage4-target-libgcc maybe-configure-stage4-target-libgcc
maybe-configure-stage4-target-libgcc:

.PHONY: configure-stageprofile-target-libgcc maybe-configure-stageprofile-target-libgcc
maybe-configure-stageprofile-target-libgcc:

.PHONY: configure-stagefeedback-target-libgcc maybe-configure-stagefeedback-target-libgcc
maybe-configure-stagefeedback-target-libgcc:





.PHONY: all-target-libgcc maybe-all-target-libgcc
maybe-all-target-libgcc:



.PHONY: all-stage1-target-libgcc maybe-all-stage1-target-libgcc
.PHONY: clean-stage1-target-libgcc maybe-clean-stage1-target-libgcc
maybe-all-stage1-target-libgcc:
maybe-clean-stage1-target-libgcc:


.PHONY: all-stage2-target-libgcc maybe-all-stage2-target-libgcc
.PHONY: clean-stage2-target-libgcc maybe-clean-stage2-target-libgcc
maybe-all-stage2-target-libgcc:
maybe-clean-stage2-target-libgcc:


.PHONY: all-stage3-target-libgcc maybe-all-stage3-target-libgcc
.PHONY: clean-stage3-target-libgcc maybe-clean-stage3-target-libgcc
maybe-all-stage3-target-libgcc:
maybe-clean-stage3-target-libgcc:


.PHONY: all-stage4-target-libgcc maybe-all-stage4-target-libgcc
.PHONY: clean-stage4-target-libgcc maybe-clean-stage4-target-libgcc
maybe-all-stage4-target-libgcc:
maybe-clean-stage4-target-libgcc:


.PHONY: all-stageprofile-target-libgcc maybe-all-stageprofile-target-libgcc
.PHONY: clean-stageprofile-target-libgcc maybe-clean-stageprofile-target-libgcc
maybe-all-stageprofile-target-libgcc:
maybe-clean-stageprofile-target-libgcc:


.PHONY: all-stagefeedback-target-libgcc maybe-all-stagefeedback-target-libgcc
.PHONY: clean-stagefeedback-target-libgcc maybe-clean-stagefeedback-target-libgcc
maybe-all-stagefeedback-target-libgcc:
maybe-clean-stagefeedback-target-libgcc:






.PHONY: check-target-libgcc maybe-check-target-libgcc
maybe-check-target-libgcc:

.PHONY: install-target-libgcc maybe-install-target-libgcc
maybe-install-target-libgcc:

.PHONY: install-strip-target-libgcc maybe-install-strip-target-libgcc
maybe-install-strip-target-libgcc:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libgcc info-target-libgcc
maybe-info-target-libgcc:

.PHONY: maybe-dvi-target-libgcc dvi-target-libgcc
maybe-dvi-target-libgcc:

.PHONY: maybe-pdf-target-libgcc pdf-target-libgcc
maybe-pdf-target-libgcc:

.PHONY: maybe-html-target-libgcc html-target-libgcc
maybe-html-target-libgcc:

.PHONY: maybe-TAGS-target-libgcc TAGS-target-libgcc
maybe-TAGS-target-libgcc:

.PHONY: maybe-install-info-target-libgcc install-info-target-libgcc
maybe-install-info-target-libgcc:

.PHONY: maybe-install-pdf-target-libgcc install-pdf-target-libgcc
maybe-install-pdf-target-libgcc:

.PHONY: maybe-install-html-target-libgcc install-html-target-libgcc
maybe-install-html-target-libgcc:

.PHONY: maybe-installcheck-target-libgcc installcheck-target-libgcc
maybe-installcheck-target-libgcc:

.PHONY: maybe-mostlyclean-target-libgcc mostlyclean-target-libgcc
maybe-mostlyclean-target-libgcc:

.PHONY: maybe-clean-target-libgcc clean-target-libgcc
maybe-clean-target-libgcc:

.PHONY: maybe-distclean-target-libgcc distclean-target-libgcc
maybe-distclean-target-libgcc:

.PHONY: maybe-maintainer-clean-target-libgcc maintainer-clean-target-libgcc
maybe-maintainer-clean-target-libgcc:





.PHONY: configure-target-libbacktrace maybe-configure-target-libbacktrace
maybe-configure-target-libbacktrace:





.PHONY: all-target-libbacktrace maybe-all-target-libbacktrace
maybe-all-target-libbacktrace:





.PHONY: check-target-libbacktrace maybe-check-target-libbacktrace
maybe-check-target-libbacktrace:

.PHONY: install-target-libbacktrace maybe-install-target-libbacktrace
maybe-install-target-libbacktrace:

.PHONY: install-strip-target-libbacktrace maybe-install-strip-target-libbacktrace
maybe-install-strip-target-libbacktrace:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libbacktrace info-target-libbacktrace
maybe-info-target-libbacktrace:

.PHONY: maybe-dvi-target-libbacktrace dvi-target-libbacktrace
maybe-dvi-target-libbacktrace:

.PHONY: maybe-pdf-target-libbacktrace pdf-target-libbacktrace
maybe-pdf-target-libbacktrace:

.PHONY: maybe-html-target-libbacktrace html-target-libbacktrace
maybe-html-target-libbacktrace:

.PHONY: maybe-TAGS-target-libbacktrace TAGS-target-libbacktrace
maybe-TAGS-target-libbacktrace:

.PHONY: maybe-install-info-target-libbacktrace install-info-target-libbacktrace
maybe-install-info-target-libbacktrace:

.PHONY: maybe-install-pdf-target-libbacktrace install-pdf-target-libbacktrace
maybe-install-pdf-target-libbacktrace:

.PHONY: maybe-install-html-target-libbacktrace install-html-target-libbacktrace
maybe-install-html-target-libbacktrace:

.PHONY: maybe-installcheck-target-libbacktrace installcheck-target-libbacktrace
maybe-installcheck-target-libbacktrace:

.PHONY: maybe-mostlyclean-target-libbacktrace mostlyclean-target-libbacktrace
maybe-mostlyclean-target-libbacktrace:

.PHONY: maybe-clean-target-libbacktrace clean-target-libbacktrace
maybe-clean-target-libbacktrace:

.PHONY: maybe-distclean-target-libbacktrace distclean-target-libbacktrace
maybe-distclean-target-libbacktrace:

.PHONY: maybe-maintainer-clean-target-libbacktrace maintainer-clean-target-libbacktrace
maybe-maintainer-clean-target-libbacktrace:





.PHONY: configure-target-libquadmath maybe-configure-target-libquadmath
maybe-configure-target-libquadmath:





.PHONY: all-target-libquadmath maybe-all-target-libquadmath
maybe-all-target-libquadmath:





.PHONY: check-target-libquadmath maybe-check-target-libquadmath
maybe-check-target-libquadmath:

.PHONY: install-target-libquadmath maybe-install-target-libquadmath
maybe-install-target-libquadmath:

.PHONY: install-strip-target-libquadmath maybe-install-strip-target-libquadmath
maybe-install-strip-target-libquadmath:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libquadmath info-target-libquadmath
maybe-info-target-libquadmath:

.PHONY: maybe-dvi-target-libquadmath dvi-target-libquadmath
maybe-dvi-target-libquadmath:

.PHONY: maybe-pdf-target-libquadmath pdf-target-libquadmath
maybe-pdf-target-libquadmath:

.PHONY: maybe-html-target-libquadmath html-target-libquadmath
maybe-html-target-libquadmath:

.PHONY: maybe-TAGS-target-libquadmath TAGS-target-libquadmath
maybe-TAGS-target-libquadmath:

.PHONY: maybe-install-info-target-libquadmath install-info-target-libquadmath
maybe-install-info-target-libquadmath:

.PHONY: maybe-install-pdf-target-libquadmath install-pdf-target-libquadmath
maybe-install-pdf-target-libquadmath:

.PHONY: maybe-install-html-target-libquadmath install-html-target-libquadmath
maybe-install-html-target-libquadmath:

.PHONY: maybe-installcheck-target-libquadmath installcheck-target-libquadmath
maybe-installcheck-target-libquadmath:

.PHONY: maybe-mostlyclean-target-libquadmath mostlyclean-target-libquadmath
maybe-mostlyclean-target-libquadmath:

.PHONY: maybe-clean-target-libquadmath clean-target-libquadmath
maybe-clean-target-libquadmath:

.PHONY: maybe-distclean-target-libquadmath distclean-target-libquadmath
maybe-distclean-target-libquadmath:

.PHONY: maybe-maintainer-clean-target-libquadmath maintainer-clean-target-libquadmath
maybe-maintainer-clean-target-libquadmath:





.PHONY: configure-target-libgfortran maybe-configure-target-libgfortran
maybe-configure-target-libgfortran:





.PHONY: all-target-libgfortran maybe-all-target-libgfortran
maybe-all-target-libgfortran:





.PHONY: check-target-libgfortran maybe-check-target-libgfortran
maybe-check-target-libgfortran:

.PHONY: install-target-libgfortran maybe-install-target-libgfortran
maybe-install-target-libgfortran:

.PHONY: install-strip-target-libgfortran maybe-install-strip-target-libgfortran
maybe-install-strip-target-libgfortran:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libgfortran info-target-libgfortran
maybe-info-target-libgfortran:

.PHONY: maybe-dvi-target-libgfortran dvi-target-libgfortran
maybe-dvi-target-libgfortran:

.PHONY: maybe-pdf-target-libgfortran pdf-target-libgfortran
maybe-pdf-target-libgfortran:

.PHONY: maybe-html-target-libgfortran html-target-libgfortran
maybe-html-target-libgfortran:

.PHONY: maybe-TAGS-target-libgfortran TAGS-target-libgfortran
maybe-TAGS-target-libgfortran:

.PHONY: maybe-install-info-target-libgfortran install-info-target-libgfortran
maybe-install-info-target-libgfortran:

.PHONY: maybe-install-pdf-target-libgfortran install-pdf-target-libgfortran
maybe-install-pdf-target-libgfortran:

.PHONY: maybe-install-html-target-libgfortran install-html-target-libgfortran
maybe-install-html-target-libgfortran:

.PHONY: maybe-installcheck-target-libgfortran installcheck-target-libgfortran
maybe-installcheck-target-libgfortran:

.PHONY: maybe-mostlyclean-target-libgfortran mostlyclean-target-libgfortran
maybe-mostlyclean-target-libgfortran:

.PHONY: maybe-clean-target-libgfortran clean-target-libgfortran
maybe-clean-target-libgfortran:

.PHONY: maybe-distclean-target-libgfortran distclean-target-libgfortran
maybe-distclean-target-libgfortran:

.PHONY: maybe-maintainer-clean-target-libgfortran maintainer-clean-target-libgfortran
maybe-maintainer-clean-target-libgfortran:





.PHONY: configure-target-libobjc maybe-configure-target-libobjc
maybe-configure-target-libobjc:





.PHONY: all-target-libobjc maybe-all-target-libobjc
maybe-all-target-libobjc:





.PHONY: check-target-libobjc maybe-check-target-libobjc
maybe-check-target-libobjc:

.PHONY: install-target-libobjc maybe-install-target-libobjc
maybe-install-target-libobjc:

.PHONY: install-strip-target-libobjc maybe-install-strip-target-libobjc
maybe-install-strip-target-libobjc:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libobjc info-target-libobjc
maybe-info-target-libobjc:

.PHONY: maybe-dvi-target-libobjc dvi-target-libobjc
maybe-dvi-target-libobjc:

.PHONY: maybe-pdf-target-libobjc pdf-target-libobjc
maybe-pdf-target-libobjc:

.PHONY: maybe-html-target-libobjc html-target-libobjc
maybe-html-target-libobjc:

.PHONY: maybe-TAGS-target-libobjc TAGS-target-libobjc
maybe-TAGS-target-libobjc:

.PHONY: maybe-install-info-target-libobjc install-info-target-libobjc
maybe-install-info-target-libobjc:

.PHONY: maybe-install-pdf-target-libobjc install-pdf-target-libobjc
maybe-install-pdf-target-libobjc:

.PHONY: maybe-install-html-target-libobjc install-html-target-libobjc
maybe-install-html-target-libobjc:

.PHONY: maybe-installcheck-target-libobjc installcheck-target-libobjc
maybe-installcheck-target-libobjc:

.PHONY: maybe-mostlyclean-target-libobjc mostlyclean-target-libobjc
maybe-mostlyclean-target-libobjc:

.PHONY: maybe-clean-target-libobjc clean-target-libobjc
maybe-clean-target-libobjc:

.PHONY: maybe-distclean-target-libobjc distclean-target-libobjc
maybe-distclean-target-libobjc:

.PHONY: maybe-maintainer-clean-target-libobjc maintainer-clean-target-libobjc
maybe-maintainer-clean-target-libobjc:





.PHONY: configure-target-libgo maybe-configure-target-libgo
maybe-configure-target-libgo:





.PHONY: all-target-libgo maybe-all-target-libgo
maybe-all-target-libgo:





.PHONY: check-target-libgo maybe-check-target-libgo
maybe-check-target-libgo:

.PHONY: install-target-libgo maybe-install-target-libgo
maybe-install-target-libgo:

.PHONY: install-strip-target-libgo maybe-install-strip-target-libgo
maybe-install-strip-target-libgo:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libgo info-target-libgo
maybe-info-target-libgo:

.PHONY: maybe-dvi-target-libgo dvi-target-libgo
maybe-dvi-target-libgo:

.PHONY: maybe-pdf-target-libgo pdf-target-libgo
maybe-pdf-target-libgo:

.PHONY: maybe-html-target-libgo html-target-libgo
maybe-html-target-libgo:

.PHONY: maybe-TAGS-target-libgo TAGS-target-libgo
maybe-TAGS-target-libgo:

.PHONY: maybe-install-info-target-libgo install-info-target-libgo
maybe-install-info-target-libgo:

.PHONY: maybe-install-pdf-target-libgo install-pdf-target-libgo
maybe-install-pdf-target-libgo:

.PHONY: maybe-install-html-target-libgo install-html-target-libgo
maybe-install-html-target-libgo:

.PHONY: maybe-installcheck-target-libgo installcheck-target-libgo
maybe-installcheck-target-libgo:

.PHONY: maybe-mostlyclean-target-libgo mostlyclean-target-libgo
maybe-mostlyclean-target-libgo:

.PHONY: maybe-clean-target-libgo clean-target-libgo
maybe-clean-target-libgo:

.PHONY: maybe-distclean-target-libgo distclean-target-libgo
maybe-distclean-target-libgo:

.PHONY: maybe-maintainer-clean-target-libgo maintainer-clean-target-libgo
maybe-maintainer-clean-target-libgo:





.PHONY: configure-target-libtermcap maybe-configure-target-libtermcap
maybe-configure-target-libtermcap:





.PHONY: all-target-libtermcap maybe-all-target-libtermcap
maybe-all-target-libtermcap:





.PHONY: check-target-libtermcap maybe-check-target-libtermcap
maybe-check-target-libtermcap:

.PHONY: install-target-libtermcap maybe-install-target-libtermcap
maybe-install-target-libtermcap:

.PHONY: install-strip-target-libtermcap maybe-install-strip-target-libtermcap
maybe-install-strip-target-libtermcap:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libtermcap info-target-libtermcap
maybe-info-target-libtermcap:

.PHONY: maybe-dvi-target-libtermcap dvi-target-libtermcap
maybe-dvi-target-libtermcap:

.PHONY: maybe-pdf-target-libtermcap pdf-target-libtermcap
maybe-pdf-target-libtermcap:

.PHONY: maybe-html-target-libtermcap html-target-libtermcap
maybe-html-target-libtermcap:

.PHONY: maybe-TAGS-target-libtermcap TAGS-target-libtermcap
maybe-TAGS-target-libtermcap:

.PHONY: maybe-install-info-target-libtermcap install-info-target-libtermcap
maybe-install-info-target-libtermcap:

.PHONY: maybe-install-pdf-target-libtermcap install-pdf-target-libtermcap
maybe-install-pdf-target-libtermcap:

.PHONY: maybe-install-html-target-libtermcap install-html-target-libtermcap
maybe-install-html-target-libtermcap:

.PHONY: maybe-installcheck-target-libtermcap installcheck-target-libtermcap
maybe-installcheck-target-libtermcap:

.PHONY: maybe-mostlyclean-target-libtermcap mostlyclean-target-libtermcap
maybe-mostlyclean-target-libtermcap:

.PHONY: maybe-clean-target-libtermcap clean-target-libtermcap
maybe-clean-target-libtermcap:

.PHONY: maybe-distclean-target-libtermcap distclean-target-libtermcap
maybe-distclean-target-libtermcap:

.PHONY: maybe-maintainer-clean-target-libtermcap maintainer-clean-target-libtermcap
maybe-maintainer-clean-target-libtermcap:





.PHONY: configure-target-winsup maybe-configure-target-winsup
maybe-configure-target-winsup:





.PHONY: all-target-winsup maybe-all-target-winsup
maybe-all-target-winsup:





.PHONY: check-target-winsup maybe-check-target-winsup
maybe-check-target-winsup:

.PHONY: install-target-winsup maybe-install-target-winsup
maybe-install-target-winsup:

.PHONY: install-strip-target-winsup maybe-install-strip-target-winsup
maybe-install-strip-target-winsup:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-winsup info-target-winsup
maybe-info-target-winsup:

.PHONY: maybe-dvi-target-winsup dvi-target-winsup
maybe-dvi-target-winsup:

.PHONY: maybe-pdf-target-winsup pdf-target-winsup
maybe-pdf-target-winsup:

.PHONY: maybe-html-target-winsup html-target-winsup
maybe-html-target-winsup:

.PHONY: maybe-TAGS-target-winsup TAGS-target-winsup
maybe-TAGS-target-winsup:

.PHONY: maybe-install-info-target-winsup install-info-target-winsup
maybe-install-info-target-winsup:

.PHONY: maybe-install-pdf-target-winsup install-pdf-target-winsup
maybe-install-pdf-target-winsup:

.PHONY: maybe-install-html-target-winsup install-html-target-winsup
maybe-install-html-target-winsup:

.PHONY: maybe-installcheck-target-winsup installcheck-target-winsup
maybe-installcheck-target-winsup:

.PHONY: maybe-mostlyclean-target-winsup mostlyclean-target-winsup
maybe-mostlyclean-target-winsup:

.PHONY: maybe-clean-target-winsup clean-target-winsup
maybe-clean-target-winsup:

.PHONY: maybe-distclean-target-winsup distclean-target-winsup
maybe-distclean-target-winsup:

.PHONY: maybe-maintainer-clean-target-winsup maintainer-clean-target-winsup
maybe-maintainer-clean-target-winsup:





.PHONY: configure-target-libgloss maybe-configure-target-libgloss
maybe-configure-target-libgloss:
maybe-configure-target-libgloss: configure-target-libgloss
configure-target-libgloss: 
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	echo "Checking multilib configuration for libgloss..."; \
	$(SHELL) $(srcdir)/mkinstalldirs $(TARGET_SUBDIR)/libgloss ; \
	$(CC_FOR_TARGET) --print-multi-lib > $(TARGET_SUBDIR)/libgloss/multilib.tmp 2> /dev/null ; \
	if test -r $(TARGET_SUBDIR)/libgloss/multilib.out; then \
	  if cmp -s $(TARGET_SUBDIR)/libgloss/multilib.tmp $(TARGET_SUBDIR)/libgloss/multilib.out; then \
	    rm -f $(TARGET_SUBDIR)/libgloss/multilib.tmp; \
	  else \
	    rm -f $(TARGET_SUBDIR)/libgloss/Makefile; \
	    mv $(TARGET_SUBDIR)/libgloss/multilib.tmp $(TARGET_SUBDIR)/libgloss/multilib.out; \
	  fi; \
	else \
	  mv $(TARGET_SUBDIR)/libgloss/multilib.tmp $(TARGET_SUBDIR)/libgloss/multilib.out; \
	fi; \
	test ! -f $(TARGET_SUBDIR)/libgloss/Makefile || exit 0; \
	$(SHELL) $(srcdir)/mkinstalldirs $(TARGET_SUBDIR)/libgloss ; \
	$(NORMAL_TARGET_EXPORTS)  \
	echo Configuring in $(TARGET_SUBDIR)/libgloss; \
	cd "$(TARGET_SUBDIR)/libgloss" || exit 1; \
	case $(srcdir) in \
	  /* | [A-Za-z]:[\\/]*) topdir=$(srcdir) ;; \
	  *) topdir=`echo $(TARGET_SUBDIR)/libgloss/ | \
		sed -e 's,\./,,g' -e 's,[^/]*/,../,g' `$(srcdir) ;; \
	esac; \
	srcdiroption="--srcdir=$${topdir}/libgloss"; \
	libsrcdir="$$s/libgloss"; \
	rm -f no-such-file || : ; \
	CONFIG_SITE=no-such-file $(SHELL) $${libsrcdir}/configure \
	  $(TARGET_CONFIGARGS) --build=${build_alias} --host=${target_alias} \
	  --target=${target_alias} $${srcdiroption}  \
	  || exit 1





.PHONY: all-target-libgloss maybe-all-target-libgloss
maybe-all-target-libgloss:
TARGET-target-libgloss=all
maybe-all-target-libgloss: all-target-libgloss
all-target-libgloss: configure-target-libgloss
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS)  \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) $(EXTRA_TARGET_FLAGS)   \
		$(TARGET-target-libgloss))





.PHONY: check-target-libgloss maybe-check-target-libgloss
maybe-check-target-libgloss:
maybe-check-target-libgloss: check-target-libgloss

# Dummy target for uncheckable module.
check-target-libgloss:


.PHONY: install-target-libgloss maybe-install-target-libgloss
maybe-install-target-libgloss:
maybe-install-target-libgloss: install-target-libgloss

install-target-libgloss: installdirs
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(TARGET_FLAGS_TO_PASS)  install)


.PHONY: install-strip-target-libgloss maybe-install-strip-target-libgloss
maybe-install-strip-target-libgloss:
maybe-install-strip-target-libgloss: install-strip-target-libgloss

install-strip-target-libgloss: installdirs
	@: $(MAKE); $(unstage)
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(TARGET_FLAGS_TO_PASS)  install-strip)


# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libgloss info-target-libgloss
maybe-info-target-libgloss:
maybe-info-target-libgloss: info-target-libgloss

info-target-libgloss: \
    configure-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing info in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           info) \
	  || exit 1


.PHONY: maybe-dvi-target-libgloss dvi-target-libgloss
maybe-dvi-target-libgloss:
maybe-dvi-target-libgloss: dvi-target-libgloss

dvi-target-libgloss: \
    configure-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing dvi in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           dvi) \
	  || exit 1


.PHONY: maybe-pdf-target-libgloss pdf-target-libgloss
maybe-pdf-target-libgloss:
maybe-pdf-target-libgloss: pdf-target-libgloss

pdf-target-libgloss: \
    configure-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing pdf in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           pdf) \
	  || exit 1


.PHONY: maybe-html-target-libgloss html-target-libgloss
maybe-html-target-libgloss:
maybe-html-target-libgloss: html-target-libgloss

html-target-libgloss: \
    configure-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing html in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           html) \
	  || exit 1


.PHONY: maybe-TAGS-target-libgloss TAGS-target-libgloss
maybe-TAGS-target-libgloss:
maybe-TAGS-target-libgloss: TAGS-target-libgloss

TAGS-target-libgloss: \
    configure-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing TAGS in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           TAGS) \
	  || exit 1


.PHONY: maybe-install-info-target-libgloss install-info-target-libgloss
maybe-install-info-target-libgloss:
maybe-install-info-target-libgloss: install-info-target-libgloss

install-info-target-libgloss: \
    configure-target-libgloss \
    info-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing install-info in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           install-info) \
	  || exit 1


.PHONY: maybe-install-pdf-target-libgloss install-pdf-target-libgloss
maybe-install-pdf-target-libgloss:
maybe-install-pdf-target-libgloss: install-pdf-target-libgloss

install-pdf-target-libgloss: \
    configure-target-libgloss \
    pdf-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing install-pdf in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           install-pdf) \
	  || exit 1


.PHONY: maybe-install-html-target-libgloss install-html-target-libgloss
maybe-install-html-target-libgloss:
maybe-install-html-target-libgloss: install-html-target-libgloss

install-html-target-libgloss: \
    configure-target-libgloss \
    html-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing install-html in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           install-html) \
	  || exit 1


.PHONY: maybe-installcheck-target-libgloss installcheck-target-libgloss
maybe-installcheck-target-libgloss:
maybe-installcheck-target-libgloss: installcheck-target-libgloss

installcheck-target-libgloss: \
    configure-target-libgloss 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing installcheck in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           installcheck) \
	  || exit 1


.PHONY: maybe-mostlyclean-target-libgloss mostlyclean-target-libgloss
maybe-mostlyclean-target-libgloss:
maybe-mostlyclean-target-libgloss: mostlyclean-target-libgloss

mostlyclean-target-libgloss: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing mostlyclean in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           mostlyclean) \
	  || exit 1


.PHONY: maybe-clean-target-libgloss clean-target-libgloss
maybe-clean-target-libgloss:
maybe-clean-target-libgloss: clean-target-libgloss

clean-target-libgloss: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing clean in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           clean) \
	  || exit 1


.PHONY: maybe-distclean-target-libgloss distclean-target-libgloss
maybe-distclean-target-libgloss:
maybe-distclean-target-libgloss: distclean-target-libgloss

distclean-target-libgloss: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing distclean in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           distclean) \
	  || exit 1


.PHONY: maybe-maintainer-clean-target-libgloss maintainer-clean-target-libgloss
maybe-maintainer-clean-target-libgloss:
maybe-maintainer-clean-target-libgloss: maintainer-clean-target-libgloss

maintainer-clean-target-libgloss: 
	@: $(MAKE); $(unstage)
	@[ -f $(TARGET_SUBDIR)/libgloss/Makefile ] || exit 0 ; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(NORMAL_TARGET_EXPORTS) \
	echo "Doing maintainer-clean in $(TARGET_SUBDIR)/libgloss" ; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$$flag" | sed -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/libgloss && \
	  $(MAKE) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" "WINDMC=$${WINDMC}" \
	           maintainer-clean) \
	  || exit 1






.PHONY: configure-target-libffi maybe-configure-target-libffi
maybe-configure-target-libffi:





.PHONY: all-target-libffi maybe-all-target-libffi
maybe-all-target-libffi:





.PHONY: check-target-libffi maybe-check-target-libffi
maybe-check-target-libffi:

.PHONY: install-target-libffi maybe-install-target-libffi
maybe-install-target-libffi:

.PHONY: install-strip-target-libffi maybe-install-strip-target-libffi
maybe-install-strip-target-libffi:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libffi info-target-libffi
maybe-info-target-libffi:

.PHONY: maybe-dvi-target-libffi dvi-target-libffi
maybe-dvi-target-libffi:

.PHONY: maybe-pdf-target-libffi pdf-target-libffi
maybe-pdf-target-libffi:

.PHONY: maybe-html-target-libffi html-target-libffi
maybe-html-target-libffi:

.PHONY: maybe-TAGS-target-libffi TAGS-target-libffi
maybe-TAGS-target-libffi:

.PHONY: maybe-install-info-target-libffi install-info-target-libffi
maybe-install-info-target-libffi:

.PHONY: maybe-install-pdf-target-libffi install-pdf-target-libffi
maybe-install-pdf-target-libffi:

.PHONY: maybe-install-html-target-libffi install-html-target-libffi
maybe-install-html-target-libffi:

.PHONY: maybe-installcheck-target-libffi installcheck-target-libffi
maybe-installcheck-target-libffi:

.PHONY: maybe-mostlyclean-target-libffi mostlyclean-target-libffi
maybe-mostlyclean-target-libffi:

.PHONY: maybe-clean-target-libffi clean-target-libffi
maybe-clean-target-libffi:

.PHONY: maybe-distclean-target-libffi distclean-target-libffi
maybe-distclean-target-libffi:

.PHONY: maybe-maintainer-clean-target-libffi maintainer-clean-target-libffi
maybe-maintainer-clean-target-libffi:





.PHONY: configure-target-libjava maybe-configure-target-libjava
maybe-configure-target-libjava:





.PHONY: all-target-libjava maybe-all-target-libjava
maybe-all-target-libjava:





.PHONY: check-target-libjava maybe-check-target-libjava
maybe-check-target-libjava:

.PHONY: install-target-libjava maybe-install-target-libjava
maybe-install-target-libjava:

.PHONY: install-strip-target-libjava maybe-install-strip-target-libjava
maybe-install-strip-target-libjava:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libjava info-target-libjava
maybe-info-target-libjava:

.PHONY: maybe-dvi-target-libjava dvi-target-libjava
maybe-dvi-target-libjava:

.PHONY: maybe-pdf-target-libjava pdf-target-libjava
maybe-pdf-target-libjava:

.PHONY: maybe-html-target-libjava html-target-libjava
maybe-html-target-libjava:

.PHONY: maybe-TAGS-target-libjava TAGS-target-libjava
maybe-TAGS-target-libjava:

.PHONY: maybe-install-info-target-libjava install-info-target-libjava
maybe-install-info-target-libjava:

.PHONY: maybe-install-pdf-target-libjava install-pdf-target-libjava
maybe-install-pdf-target-libjava:

.PHONY: maybe-install-html-target-libjava install-html-target-libjava
maybe-install-html-target-libjava:

.PHONY: maybe-installcheck-target-libjava installcheck-target-libjava
maybe-installcheck-target-libjava:

.PHONY: maybe-mostlyclean-target-libjava mostlyclean-target-libjava
maybe-mostlyclean-target-libjava:

.PHONY: maybe-clean-target-libjava clean-target-libjava
maybe-clean-target-libjava:

.PHONY: maybe-distclean-target-libjava distclean-target-libjava
maybe-distclean-target-libjava:

.PHONY: maybe-maintainer-clean-target-libjava maintainer-clean-target-libjava
maybe-maintainer-clean-target-libjava:





.PHONY: configure-target-zlib maybe-configure-target-zlib
maybe-configure-target-zlib:





.PHONY: all-target-zlib maybe-all-target-zlib
maybe-all-target-zlib:





.PHONY: check-target-zlib maybe-check-target-zlib
maybe-check-target-zlib:

.PHONY: install-target-zlib maybe-install-target-zlib
maybe-install-target-zlib:

.PHONY: install-strip-target-zlib maybe-install-strip-target-zlib
maybe-install-strip-target-zlib:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-zlib info-target-zlib
maybe-info-target-zlib:

.PHONY: maybe-dvi-target-zlib dvi-target-zlib
maybe-dvi-target-zlib:

.PHONY: maybe-pdf-target-zlib pdf-target-zlib
maybe-pdf-target-zlib:

.PHONY: maybe-html-target-zlib html-target-zlib
maybe-html-target-zlib:

.PHONY: maybe-TAGS-target-zlib TAGS-target-zlib
maybe-TAGS-target-zlib:

.PHONY: maybe-install-info-target-zlib install-info-target-zlib
maybe-install-info-target-zlib:

.PHONY: maybe-install-pdf-target-zlib install-pdf-target-zlib
maybe-install-pdf-target-zlib:

.PHONY: maybe-install-html-target-zlib install-html-target-zlib
maybe-install-html-target-zlib:

.PHONY: maybe-installcheck-target-zlib installcheck-target-zlib
maybe-installcheck-target-zlib:

.PHONY: maybe-mostlyclean-target-zlib mostlyclean-target-zlib
maybe-mostlyclean-target-zlib:

.PHONY: maybe-clean-target-zlib clean-target-zlib
maybe-clean-target-zlib:

.PHONY: maybe-distclean-target-zlib distclean-target-zlib
maybe-distclean-target-zlib:

.PHONY: maybe-maintainer-clean-target-zlib maintainer-clean-target-zlib
maybe-maintainer-clean-target-zlib:





.PHONY: configure-target-boehm-gc maybe-configure-target-boehm-gc
maybe-configure-target-boehm-gc:





.PHONY: all-target-boehm-gc maybe-all-target-boehm-gc
maybe-all-target-boehm-gc:





.PHONY: check-target-boehm-gc maybe-check-target-boehm-gc
maybe-check-target-boehm-gc:

.PHONY: install-target-boehm-gc maybe-install-target-boehm-gc
maybe-install-target-boehm-gc:

.PHONY: install-strip-target-boehm-gc maybe-install-strip-target-boehm-gc
maybe-install-strip-target-boehm-gc:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-boehm-gc info-target-boehm-gc
maybe-info-target-boehm-gc:

.PHONY: maybe-dvi-target-boehm-gc dvi-target-boehm-gc
maybe-dvi-target-boehm-gc:

.PHONY: maybe-pdf-target-boehm-gc pdf-target-boehm-gc
maybe-pdf-target-boehm-gc:

.PHONY: maybe-html-target-boehm-gc html-target-boehm-gc
maybe-html-target-boehm-gc:

.PHONY: maybe-TAGS-target-boehm-gc TAGS-target-boehm-gc
maybe-TAGS-target-boehm-gc:

.PHONY: maybe-install-info-target-boehm-gc install-info-target-boehm-gc
maybe-install-info-target-boehm-gc:

.PHONY: maybe-install-pdf-target-boehm-gc install-pdf-target-boehm-gc
maybe-install-pdf-target-boehm-gc:

.PHONY: maybe-install-html-target-boehm-gc install-html-target-boehm-gc
maybe-install-html-target-boehm-gc:

.PHONY: maybe-installcheck-target-boehm-gc installcheck-target-boehm-gc
maybe-installcheck-target-boehm-gc:

.PHONY: maybe-mostlyclean-target-boehm-gc mostlyclean-target-boehm-gc
maybe-mostlyclean-target-boehm-gc:

.PHONY: maybe-clean-target-boehm-gc clean-target-boehm-gc
maybe-clean-target-boehm-gc:

.PHONY: maybe-distclean-target-boehm-gc distclean-target-boehm-gc
maybe-distclean-target-boehm-gc:

.PHONY: maybe-maintainer-clean-target-boehm-gc maintainer-clean-target-boehm-gc
maybe-maintainer-clean-target-boehm-gc:





.PHONY: configure-target-rda maybe-configure-target-rda
maybe-configure-target-rda:





.PHONY: all-target-rda maybe-all-target-rda
maybe-all-target-rda:





.PHONY: check-target-rda maybe-check-target-rda
maybe-check-target-rda:

.PHONY: install-target-rda maybe-install-target-rda
maybe-install-target-rda:

.PHONY: install-strip-target-rda maybe-install-strip-target-rda
maybe-install-strip-target-rda:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-rda info-target-rda
maybe-info-target-rda:

.PHONY: maybe-dvi-target-rda dvi-target-rda
maybe-dvi-target-rda:

.PHONY: maybe-pdf-target-rda pdf-target-rda
maybe-pdf-target-rda:

.PHONY: maybe-html-target-rda html-target-rda
maybe-html-target-rda:

.PHONY: maybe-TAGS-target-rda TAGS-target-rda
maybe-TAGS-target-rda:

.PHONY: maybe-install-info-target-rda install-info-target-rda
maybe-install-info-target-rda:

.PHONY: maybe-install-pdf-target-rda install-pdf-target-rda
maybe-install-pdf-target-rda:

.PHONY: maybe-install-html-target-rda install-html-target-rda
maybe-install-html-target-rda:

.PHONY: maybe-installcheck-target-rda installcheck-target-rda
maybe-installcheck-target-rda:

.PHONY: maybe-mostlyclean-target-rda mostlyclean-target-rda
maybe-mostlyclean-target-rda:

.PHONY: maybe-clean-target-rda clean-target-rda
maybe-clean-target-rda:

.PHONY: maybe-distclean-target-rda distclean-target-rda
maybe-distclean-target-rda:

.PHONY: maybe-maintainer-clean-target-rda maintainer-clean-target-rda
maybe-maintainer-clean-target-rda:





.PHONY: configure-target-libada maybe-configure-target-libada
maybe-configure-target-libada:





.PHONY: all-target-libada maybe-all-target-libada
maybe-all-target-libada:





.PHONY: check-target-libada maybe-check-target-libada
maybe-check-target-libada:

.PHONY: install-target-libada maybe-install-target-libada
maybe-install-target-libada:

.PHONY: install-strip-target-libada maybe-install-strip-target-libada
maybe-install-strip-target-libada:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libada info-target-libada
maybe-info-target-libada:

.PHONY: maybe-dvi-target-libada dvi-target-libada
maybe-dvi-target-libada:

.PHONY: maybe-pdf-target-libada pdf-target-libada
maybe-pdf-target-libada:

.PHONY: maybe-html-target-libada html-target-libada
maybe-html-target-libada:

.PHONY: maybe-TAGS-target-libada TAGS-target-libada
maybe-TAGS-target-libada:

.PHONY: maybe-install-info-target-libada install-info-target-libada
maybe-install-info-target-libada:

.PHONY: maybe-install-pdf-target-libada install-pdf-target-libada
maybe-install-pdf-target-libada:

.PHONY: maybe-install-html-target-libada install-html-target-libada
maybe-install-html-target-libada:

.PHONY: maybe-installcheck-target-libada installcheck-target-libada
maybe-installcheck-target-libada:

.PHONY: maybe-mostlyclean-target-libada mostlyclean-target-libada
maybe-mostlyclean-target-libada:

.PHONY: maybe-clean-target-libada clean-target-libada
maybe-clean-target-libada:

.PHONY: maybe-distclean-target-libada distclean-target-libada
maybe-distclean-target-libada:

.PHONY: maybe-maintainer-clean-target-libada maintainer-clean-target-libada
maybe-maintainer-clean-target-libada:





.PHONY: configure-target-libgomp maybe-configure-target-libgomp
maybe-configure-target-libgomp:



.PHONY: configure-stage1-target-libgomp maybe-configure-stage1-target-libgomp
maybe-configure-stage1-target-libgomp:

.PHONY: configure-stage2-target-libgomp maybe-configure-stage2-target-libgomp
maybe-configure-stage2-target-libgomp:

.PHONY: configure-stage3-target-libgomp maybe-configure-stage3-target-libgomp
maybe-configure-stage3-target-libgomp:

.PHONY: configure-stage4-target-libgomp maybe-configure-stage4-target-libgomp
maybe-configure-stage4-target-libgomp:

.PHONY: configure-stageprofile-target-libgomp maybe-configure-stageprofile-target-libgomp
maybe-configure-stageprofile-target-libgomp:

.PHONY: configure-stagefeedback-target-libgomp maybe-configure-stagefeedback-target-libgomp
maybe-configure-stagefeedback-target-libgomp:





.PHONY: all-target-libgomp maybe-all-target-libgomp
maybe-all-target-libgomp:



.PHONY: all-stage1-target-libgomp maybe-all-stage1-target-libgomp
.PHONY: clean-stage1-target-libgomp maybe-clean-stage1-target-libgomp
maybe-all-stage1-target-libgomp:
maybe-clean-stage1-target-libgomp:


.PHONY: all-stage2-target-libgomp maybe-all-stage2-target-libgomp
.PHONY: clean-stage2-target-libgomp maybe-clean-stage2-target-libgomp
maybe-all-stage2-target-libgomp:
maybe-clean-stage2-target-libgomp:


.PHONY: all-stage3-target-libgomp maybe-all-stage3-target-libgomp
.PHONY: clean-stage3-target-libgomp maybe-clean-stage3-target-libgomp
maybe-all-stage3-target-libgomp:
maybe-clean-stage3-target-libgomp:


.PHONY: all-stage4-target-libgomp maybe-all-stage4-target-libgomp
.PHONY: clean-stage4-target-libgomp maybe-clean-stage4-target-libgomp
maybe-all-stage4-target-libgomp:
maybe-clean-stage4-target-libgomp:


.PHONY: all-stageprofile-target-libgomp maybe-all-stageprofile-target-libgomp
.PHONY: clean-stageprofile-target-libgomp maybe-clean-stageprofile-target-libgomp
maybe-all-stageprofile-target-libgomp:
maybe-clean-stageprofile-target-libgomp:


.PHONY: all-stagefeedback-target-libgomp maybe-all-stagefeedback-target-libgomp
.PHONY: clean-stagefeedback-target-libgomp maybe-clean-stagefeedback-target-libgomp
maybe-all-stagefeedback-target-libgomp:
maybe-clean-stagefeedback-target-libgomp:






.PHONY: check-target-libgomp maybe-check-target-libgomp
maybe-check-target-libgomp:

.PHONY: install-target-libgomp maybe-install-target-libgomp
maybe-install-target-libgomp:

.PHONY: install-strip-target-libgomp maybe-install-strip-target-libgomp
maybe-install-strip-target-libgomp:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libgomp info-target-libgomp
maybe-info-target-libgomp:

.PHONY: maybe-dvi-target-libgomp dvi-target-libgomp
maybe-dvi-target-libgomp:

.PHONY: maybe-pdf-target-libgomp pdf-target-libgomp
maybe-pdf-target-libgomp:

.PHONY: maybe-html-target-libgomp html-target-libgomp
maybe-html-target-libgomp:

.PHONY: maybe-TAGS-target-libgomp TAGS-target-libgomp
maybe-TAGS-target-libgomp:

.PHONY: maybe-install-info-target-libgomp install-info-target-libgomp
maybe-install-info-target-libgomp:

.PHONY: maybe-install-pdf-target-libgomp install-pdf-target-libgomp
maybe-install-pdf-target-libgomp:

.PHONY: maybe-install-html-target-libgomp install-html-target-libgomp
maybe-install-html-target-libgomp:

.PHONY: maybe-installcheck-target-libgomp installcheck-target-libgomp
maybe-installcheck-target-libgomp:

.PHONY: maybe-mostlyclean-target-libgomp mostlyclean-target-libgomp
maybe-mostlyclean-target-libgomp:

.PHONY: maybe-clean-target-libgomp clean-target-libgomp
maybe-clean-target-libgomp:

.PHONY: maybe-distclean-target-libgomp distclean-target-libgomp
maybe-distclean-target-libgomp:

.PHONY: maybe-maintainer-clean-target-libgomp maintainer-clean-target-libgomp
maybe-maintainer-clean-target-libgomp:





.PHONY: configure-target-libitm maybe-configure-target-libitm
maybe-configure-target-libitm:





.PHONY: all-target-libitm maybe-all-target-libitm
maybe-all-target-libitm:





.PHONY: check-target-libitm maybe-check-target-libitm
maybe-check-target-libitm:

.PHONY: install-target-libitm maybe-install-target-libitm
maybe-install-target-libitm:

.PHONY: install-strip-target-libitm maybe-install-strip-target-libitm
maybe-install-strip-target-libitm:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libitm info-target-libitm
maybe-info-target-libitm:

.PHONY: maybe-dvi-target-libitm dvi-target-libitm
maybe-dvi-target-libitm:

.PHONY: maybe-pdf-target-libitm pdf-target-libitm
maybe-pdf-target-libitm:

.PHONY: maybe-html-target-libitm html-target-libitm
maybe-html-target-libitm:

.PHONY: maybe-TAGS-target-libitm TAGS-target-libitm
maybe-TAGS-target-libitm:

.PHONY: maybe-install-info-target-libitm install-info-target-libitm
maybe-install-info-target-libitm:

.PHONY: maybe-install-pdf-target-libitm install-pdf-target-libitm
maybe-install-pdf-target-libitm:

.PHONY: maybe-install-html-target-libitm install-html-target-libitm
maybe-install-html-target-libitm:

.PHONY: maybe-installcheck-target-libitm installcheck-target-libitm
maybe-installcheck-target-libitm:

.PHONY: maybe-mostlyclean-target-libitm mostlyclean-target-libitm
maybe-mostlyclean-target-libitm:

.PHONY: maybe-clean-target-libitm clean-target-libitm
maybe-clean-target-libitm:

.PHONY: maybe-distclean-target-libitm distclean-target-libitm
maybe-distclean-target-libitm:

.PHONY: maybe-maintainer-clean-target-libitm maintainer-clean-target-libitm
maybe-maintainer-clean-target-libitm:





.PHONY: configure-target-libatomic maybe-configure-target-libatomic
maybe-configure-target-libatomic:





.PHONY: all-target-libatomic maybe-all-target-libatomic
maybe-all-target-libatomic:





.PHONY: check-target-libatomic maybe-check-target-libatomic
maybe-check-target-libatomic:

.PHONY: install-target-libatomic maybe-install-target-libatomic
maybe-install-target-libatomic:

.PHONY: install-strip-target-libatomic maybe-install-strip-target-libatomic
maybe-install-strip-target-libatomic:

# Other targets (info, dvi, pdf, etc.)

.PHONY: maybe-info-target-libatomic info-target-libatomic
maybe-info-target-libatomic:

.PHONY: maybe-dvi-target-libatomic dvi-target-libatomic
maybe-dvi-target-libatomic:

.PHONY: maybe-pdf-target-libatomic pdf-target-libatomic
maybe-pdf-target-libatomic:

.PHONY: maybe-html-target-libatomic html-target-libatomic
maybe-html-target-libatomic:

.PHONY: maybe-TAGS-target-libatomic TAGS-target-libatomic
maybe-TAGS-target-libatomic:

.PHONY: maybe-install-info-target-libatomic install-info-target-libatomic
maybe-install-info-target-libatomic:

.PHONY: maybe-install-pdf-target-libatomic install-pdf-target-libatomic
maybe-install-pdf-target-libatomic:

.PHONY: maybe-install-html-target-libatomic install-html-target-libatomic
maybe-install-html-target-libatomic:

.PHONY: maybe-installcheck-target-libatomic installcheck-target-libatomic
maybe-installcheck-target-libatomic:

.PHONY: maybe-mostlyclean-target-libatomic mostlyclean-target-libatomic
maybe-mostlyclean-target-libatomic:

.PHONY: maybe-clean-target-libatomic clean-target-libatomic
maybe-clean-target-libatomic:

.PHONY: maybe-distclean-target-libatomic distclean-target-libatomic
maybe-distclean-target-libatomic:

.PHONY: maybe-maintainer-clean-target-libatomic maintainer-clean-target-libatomic
maybe-maintainer-clean-target-libatomic:






# ----------
# GCC module
# ----------



# ---------------------
# GCC bootstrap support
# ---------------------

# We track the current stage (the one in 'gcc') in the stage_current file.
# stage_last instead tracks the stage that was built last.  These targets
# are dummy when toplevel bootstrap is not active.

# While making host and target tools, symlinks to the final stage must be
# there, so $(unstage) should be run at various points.  To avoid excessive
# recursive invocations of make, we "inline" them using a variable.  These
# must be referenced as ": $(MAKE) ; $(unstage)" rather than "$(unstage)"
# to avoid warnings from the GNU Make job server.

unstage = :
stage = :
current_stage = ""


.PHONY: unstage stage
unstage:
	@: $(MAKE); $(unstage)
stage:
	@: $(MAKE); $(stage)

# Disable commands for lean bootstrap.
LEAN = false

# We name the build directories for the various stages "stage1-gcc",
# "stage2-gcc","stage3-gcc", etc.

# Since the 'compare' process will fail (on debugging information) if any
# directory names are different, we need to link the gcc directory for
# the previous stage to a constant name ('prev-gcc'), and to make the name of
# the build directories constant as well. For the latter, we use naked names
# like 'gcc', because the scripts in that directory assume it.  We use
# mv on platforms where symlinks to directories do not work or are not
# reliable.

# 'touch' doesn't work right on some platforms.
STAMP = echo timestamp > 

# We only want to compare .o files, so set this!
objext = .o


.PHONY: stage1-start stage1-end

stage1-start::
	@: $(MAKE); $(stage); \
	echo stage1 > stage_current ; \
	echo stage1 > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)
	@[ -d stage1-$(TARGET_SUBDIR) ] || \
	  mkdir stage1-$(TARGET_SUBDIR); \
	mv stage1-$(TARGET_SUBDIR) $(TARGET_SUBDIR) 

stage1-end:: 
	@if test -d $(TARGET_SUBDIR) ; then \
	  mv $(TARGET_SUBDIR) stage1-$(TARGET_SUBDIR)  ; \
	fi
	rm -f stage_current

# Bubble a bug fix through all the stages up to stage 1.  They are
# remade, but not reconfigured.  The next stage (if any) will not be
# reconfigured either.
.PHONY: stage1-bubble
stage1-bubble:: 
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if test -f stage1-lean  ; then \
	  echo Skipping rebuild of stage1 ; \
	else \
	  $(MAKE) stage1-start; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-stage1; \
	fi

.PHONY: all-stage1 clean-stage1
do-clean: clean-stage1

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules


.PHONY: stage2-start stage2-end

stage2-start::
	@: $(MAKE); $(stage); \
	echo stage2 > stage_current ; \
	echo stage2 > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)
	@[ -d stage2-$(TARGET_SUBDIR) ] || \
	  mkdir stage2-$(TARGET_SUBDIR); \
	mv stage2-$(TARGET_SUBDIR) $(TARGET_SUBDIR)  ; \
	mv stage1-$(TARGET_SUBDIR) prev-$(TARGET_SUBDIR) || test -f stage1-lean 

stage2-end:: 
	@if test -d $(TARGET_SUBDIR) ; then \
	  mv $(TARGET_SUBDIR) stage2-$(TARGET_SUBDIR)  ; \
	  mv prev-$(TARGET_SUBDIR) stage1-$(TARGET_SUBDIR) ; :  ; \
	fi
	rm -f stage_current

# Bubble a bug fix through all the stages up to stage 2.  They are
# remade, but not reconfigured.  The next stage (if any) will not be
# reconfigured either.
.PHONY: stage2-bubble
stage2-bubble:: stage1-bubble
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if test -f stage2-lean || test -f stage1-lean  ; then \
	  echo Skipping rebuild of stage2 ; \
	else \
	  $(MAKE) stage2-start; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-stage2; \
	fi

.PHONY: all-stage2 clean-stage2
do-clean: clean-stage2

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules


.PHONY: stage3-start stage3-end

stage3-start::
	@: $(MAKE); $(stage); \
	echo stage3 > stage_current ; \
	echo stage3 > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)
	@[ -d stage3-$(TARGET_SUBDIR) ] || \
	  mkdir stage3-$(TARGET_SUBDIR); \
	mv stage3-$(TARGET_SUBDIR) $(TARGET_SUBDIR)  ; \
	mv stage2-$(TARGET_SUBDIR) prev-$(TARGET_SUBDIR) || test -f stage2-lean 

stage3-end:: 
	@if test -d $(TARGET_SUBDIR) ; then \
	  mv $(TARGET_SUBDIR) stage3-$(TARGET_SUBDIR)  ; \
	  mv prev-$(TARGET_SUBDIR) stage2-$(TARGET_SUBDIR) ; :  ; \
	fi
	rm -f stage_current

# Bubble a bug fix through all the stages up to stage 3.  They are
# remade, but not reconfigured.  The next stage (if any) will not be
# reconfigured either.
.PHONY: stage3-bubble
stage3-bubble:: stage2-bubble
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if test -f stage3-lean || test -f stage2-lean  ; then \
	  echo Skipping rebuild of stage3 ; \
	else \
	  $(MAKE) stage3-start; \
	  if $(LEAN); then \
	    rm -rf stage1-* ; \
	    $(STAMP) stage1-lean ; \
	  fi; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-stage3; \
	fi
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) compare

.PHONY: all-stage3 clean-stage3
do-clean: clean-stage3

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules


.PHONY: stage4-start stage4-end

stage4-start::
	@: $(MAKE); $(stage); \
	echo stage4 > stage_current ; \
	echo stage4 > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)
	@[ -d stage4-$(TARGET_SUBDIR) ] || \
	  mkdir stage4-$(TARGET_SUBDIR); \
	mv stage4-$(TARGET_SUBDIR) $(TARGET_SUBDIR)  ; \
	mv stage3-$(TARGET_SUBDIR) prev-$(TARGET_SUBDIR) || test -f stage3-lean 

stage4-end:: 
	@if test -d $(TARGET_SUBDIR) ; then \
	  mv $(TARGET_SUBDIR) stage4-$(TARGET_SUBDIR)  ; \
	  mv prev-$(TARGET_SUBDIR) stage3-$(TARGET_SUBDIR) ; :  ; \
	fi
	rm -f stage_current

# Bubble a bug fix through all the stages up to stage 4.  They are
# remade, but not reconfigured.  The next stage (if any) will not be
# reconfigured either.
.PHONY: stage4-bubble
stage4-bubble:: stage3-bubble
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if test -f stage4-lean || test -f stage3-lean  ; then \
	  echo Skipping rebuild of stage4 ; \
	else \
	  $(MAKE) stage4-start; \
	  if $(LEAN); then \
	    rm -rf stage2-* ; \
	    $(STAMP) stage2-lean ; \
	  fi; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-stage4; \
	fi
	$(MAKE) $(RECURSE_FLAGS_TO_PASS) compare3

.PHONY: all-stage4 clean-stage4
do-clean: clean-stage4

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules


.PHONY: stageprofile-start stageprofile-end

stageprofile-start::
	@: $(MAKE); $(stage); \
	echo stageprofile > stage_current ; \
	echo stageprofile > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)
	@[ -d stageprofile-$(TARGET_SUBDIR) ] || \
	  mkdir stageprofile-$(TARGET_SUBDIR); \
	mv stageprofile-$(TARGET_SUBDIR) $(TARGET_SUBDIR)  ; \
	mv stage1-$(TARGET_SUBDIR) prev-$(TARGET_SUBDIR) || test -f stage1-lean 

stageprofile-end:: 
	@if test -d $(TARGET_SUBDIR) ; then \
	  mv $(TARGET_SUBDIR) stageprofile-$(TARGET_SUBDIR)  ; \
	  mv prev-$(TARGET_SUBDIR) stage1-$(TARGET_SUBDIR) ; :  ; \
	fi
	rm -f stage_current

# Bubble a bug fix through all the stages up to stage profile.  They are
# remade, but not reconfigured.  The next stage (if any) will not be
# reconfigured either.
.PHONY: stageprofile-bubble
stageprofile-bubble:: stage1-bubble
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if test -f stageprofile-lean || test -f stage1-lean  ; then \
	  echo Skipping rebuild of stageprofile ; \
	else \
	  $(MAKE) stageprofile-start; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-stageprofile; \
	fi

.PHONY: all-stageprofile clean-stageprofile
do-clean: clean-stageprofile

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules


.PHONY: stagefeedback-start stagefeedback-end

stagefeedback-start::
	@: $(MAKE); $(stage); \
	echo stagefeedback > stage_current ; \
	echo stagefeedback > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR)
	@[ -d stagefeedback-$(TARGET_SUBDIR) ] || \
	  mkdir stagefeedback-$(TARGET_SUBDIR); \
	mv stagefeedback-$(TARGET_SUBDIR) $(TARGET_SUBDIR)  ; \
	mv stageprofile-$(TARGET_SUBDIR) prev-$(TARGET_SUBDIR) || test -f stageprofile-lean 

stagefeedback-end:: 
	@if test -d $(TARGET_SUBDIR) ; then \
	  mv $(TARGET_SUBDIR) stagefeedback-$(TARGET_SUBDIR)  ; \
	  mv prev-$(TARGET_SUBDIR) stageprofile-$(TARGET_SUBDIR) ; :  ; \
	fi
	rm -f stage_current

# Bubble a bug fix through all the stages up to stage feedback.  They are
# remade, but not reconfigured.  The next stage (if any) will not be
# reconfigured either.
.PHONY: stagefeedback-bubble
stagefeedback-bubble:: stageprofile-bubble
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if test -f stagefeedback-lean || test -f stageprofile-lean  ; then \
	  echo Skipping rebuild of stagefeedback ; \
	else \
	  $(MAKE) stagefeedback-start; \
	  $(MAKE) $(RECURSE_FLAGS_TO_PASS) all-stagefeedback; \
	fi

.PHONY: all-stagefeedback clean-stagefeedback
do-clean: clean-stagefeedback

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules



stageprofile-end::
	$(MAKE) distclean-stagefeedback

stagefeedback-start::
	@r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	for i in prev-*; do \
	  j=`echo $$i | sed s/^prev-//` ; \
	  cd $$r/$$i && \
	  { find . -type d | sort | sed 's,.*,$(SHELL) '"$$s"'/mkinstalldirs "../'$$j'/&",' | $(SHELL) ; } && \
	  { find . -name '*.*da' | sed 's,.*,$(LN) -f "&" "../'$$j'/&",' | $(SHELL) ; } ; \
	done


# --------------------------------------
# Dependencies between different modules
# --------------------------------------

# Generic dependencies for target modules on host stuff, especially gcc



# There are two types of dependencies here: 'hard' dependencies, where one
# module simply won't build without the other; and 'soft' dependencies, where
# if the depended-on module is missing, the depending module will do without
# or find a substitute somewhere (perhaps installed).  Soft dependencies
# are made here to depend on a 'maybe-' target.  If you're not sure,
# it's safer to use a soft dependency.






# With all the machinery above in place, it is pretty easy to generate
# dependencies.  Host dependencies are a bit more complex because we have
# to check for bootstrap/prebootstrap dependencies.  To resolve
# prebootstrap dependencies, prebootstrap modules are gathered in
# a hash table.
all-build-bison: maybe-all-build-texinfo
all-build-flex: maybe-all-build-texinfo
all-build-flex: maybe-all-build-bison
all-build-flex: maybe-all-build-m4
all-build-libiberty: maybe-all-build-texinfo
all-build-m4: maybe-all-build-texinfo
all-build-fixincludes: maybe-all-build-libiberty
configure-gcc: maybe-configure-intl

configure-stage1-gcc: maybe-configure-stage1-intl
configure-stage2-gcc: maybe-configure-stage2-intl
configure-stage3-gcc: maybe-configure-stage3-intl
configure-stage4-gcc: maybe-configure-stage4-intl
configure-stageprofile-gcc: maybe-configure-stageprofile-intl
configure-stagefeedback-gcc: maybe-configure-stagefeedback-intl
configure-gcc: maybe-all-gmp

configure-stage1-gcc: maybe-all-stage1-gmp
configure-stage2-gcc: maybe-all-stage2-gmp
configure-stage3-gcc: maybe-all-stage3-gmp
configure-stage4-gcc: maybe-all-stage4-gmp
configure-stageprofile-gcc: maybe-all-stageprofile-gmp
configure-stagefeedback-gcc: maybe-all-stagefeedback-gmp
configure-gcc: maybe-all-lto-plugin

configure-stage1-gcc: maybe-all-stage1-lto-plugin
configure-stage2-gcc: maybe-all-stage2-lto-plugin
configure-stage3-gcc: maybe-all-stage3-lto-plugin
configure-stage4-gcc: maybe-all-stage4-lto-plugin
configure-stageprofile-gcc: maybe-all-stageprofile-lto-plugin
configure-stagefeedback-gcc: maybe-all-stagefeedback-lto-plugin
configure-gcc: maybe-all-binutils

configure-stage1-gcc: maybe-all-stage1-binutils
configure-stage2-gcc: maybe-all-stage2-binutils
configure-stage3-gcc: maybe-all-stage3-binutils
configure-stage4-gcc: maybe-all-stage4-binutils
configure-stageprofile-gcc: maybe-all-stageprofile-binutils
configure-stagefeedback-gcc: maybe-all-stagefeedback-binutils
configure-gcc: maybe-all-gas

configure-stage1-gcc: maybe-all-stage1-gas
configure-stage2-gcc: maybe-all-stage2-gas
configure-stage3-gcc: maybe-all-stage3-gas
configure-stage4-gcc: maybe-all-stage4-gas
configure-stageprofile-gcc: maybe-all-stageprofile-gas
configure-stagefeedback-gcc: maybe-all-stagefeedback-gas
configure-gcc: maybe-all-ld

configure-stage1-gcc: maybe-all-stage1-ld
configure-stage2-gcc: maybe-all-stage2-ld
configure-stage3-gcc: maybe-all-stage3-ld
configure-stage4-gcc: maybe-all-stage4-ld
configure-stageprofile-gcc: maybe-all-stageprofile-ld
configure-stagefeedback-gcc: maybe-all-stagefeedback-ld
configure-gcc: maybe-all-gold

configure-stage1-gcc: maybe-all-stage1-gold
configure-stage2-gcc: maybe-all-stage2-gold
configure-stage3-gcc: maybe-all-stage3-gold
configure-stage4-gcc: maybe-all-stage4-gold
configure-stageprofile-gcc: maybe-all-stageprofile-gold
configure-stagefeedback-gcc: maybe-all-stagefeedback-gold
configure-gcc: maybe-all-libelf

configure-stage1-gcc: maybe-all-stage1-libelf
configure-stage2-gcc: maybe-all-stage2-libelf
configure-stage3-gcc: maybe-all-stage3-libelf
configure-stage4-gcc: maybe-all-stage4-libelf
configure-stageprofile-gcc: maybe-all-stageprofile-libelf
configure-stagefeedback-gcc: maybe-all-stagefeedback-libelf
all-gcc: all-libiberty

all-stage1-gcc: all-stage1-libiberty
all-stage2-gcc: all-stage2-libiberty
all-stage3-gcc: all-stage3-libiberty
all-stage4-gcc: all-stage4-libiberty
all-stageprofile-gcc: all-stageprofile-libiberty
all-stagefeedback-gcc: all-stagefeedback-libiberty
all-gcc: maybe-all-intl

all-stage1-gcc: maybe-all-stage1-intl
all-stage2-gcc: maybe-all-stage2-intl
all-stage3-gcc: maybe-all-stage3-intl
all-stage4-gcc: maybe-all-stage4-intl
all-stageprofile-gcc: maybe-all-stageprofile-intl
all-stagefeedback-gcc: maybe-all-stagefeedback-intl
all-gcc: maybe-all-mpfr

all-stage1-gcc: maybe-all-stage1-mpfr
all-stage2-gcc: maybe-all-stage2-mpfr
all-stage3-gcc: maybe-all-stage3-mpfr
all-stage4-gcc: maybe-all-stage4-mpfr
all-stageprofile-gcc: maybe-all-stageprofile-mpfr
all-stagefeedback-gcc: maybe-all-stagefeedback-mpfr
all-gcc: maybe-all-mpc

all-stage1-gcc: maybe-all-stage1-mpc
all-stage2-gcc: maybe-all-stage2-mpc
all-stage3-gcc: maybe-all-stage3-mpc
all-stage4-gcc: maybe-all-stage4-mpc
all-stageprofile-gcc: maybe-all-stageprofile-mpc
all-stagefeedback-gcc: maybe-all-stagefeedback-mpc
all-gcc: maybe-all-cloog

all-stage1-gcc: maybe-all-stage1-cloog
all-stage2-gcc: maybe-all-stage2-cloog
all-stage3-gcc: maybe-all-stage3-cloog
all-stage4-gcc: maybe-all-stage4-cloog
all-stageprofile-gcc: maybe-all-stageprofile-cloog
all-stagefeedback-gcc: maybe-all-stagefeedback-cloog
all-gcc: maybe-all-build-texinfo

all-stage1-gcc: maybe-all-build-texinfo
all-stage2-gcc: maybe-all-build-texinfo
all-stage3-gcc: maybe-all-build-texinfo
all-stage4-gcc: maybe-all-build-texinfo
all-stageprofile-gcc: maybe-all-build-texinfo
all-stagefeedback-gcc: maybe-all-build-texinfo
all-gcc: maybe-all-build-bison

all-stage1-gcc: maybe-all-build-bison
all-stage2-gcc: maybe-all-build-bison
all-stage3-gcc: maybe-all-build-bison
all-stage4-gcc: maybe-all-build-bison
all-stageprofile-gcc: maybe-all-build-bison
all-stagefeedback-gcc: maybe-all-build-bison
all-gcc: maybe-all-build-flex

all-stage1-gcc: maybe-all-build-flex
all-stage2-gcc: maybe-all-build-flex
all-stage3-gcc: maybe-all-build-flex
all-stage4-gcc: maybe-all-build-flex
all-stageprofile-gcc: maybe-all-build-flex
all-stagefeedback-gcc: maybe-all-build-flex
all-gcc: maybe-all-build-libiberty

all-stage1-gcc: maybe-all-build-libiberty
all-stage2-gcc: maybe-all-build-libiberty
all-stage3-gcc: maybe-all-build-libiberty
all-stage4-gcc: maybe-all-build-libiberty
all-stageprofile-gcc: maybe-all-build-libiberty
all-stagefeedback-gcc: maybe-all-build-libiberty
all-gcc: maybe-all-build-fixincludes

all-stage1-gcc: maybe-all-build-fixincludes
all-stage2-gcc: maybe-all-build-fixincludes
all-stage3-gcc: maybe-all-build-fixincludes
all-stage4-gcc: maybe-all-build-fixincludes
all-stageprofile-gcc: maybe-all-build-fixincludes
all-stagefeedback-gcc: maybe-all-build-fixincludes
all-gcc: maybe-all-zlib

all-stage1-gcc: maybe-all-stage1-zlib
all-stage2-gcc: maybe-all-stage2-zlib
all-stage3-gcc: maybe-all-stage3-zlib
all-stage4-gcc: maybe-all-stage4-zlib
all-stageprofile-gcc: maybe-all-stageprofile-zlib
all-stagefeedback-gcc: maybe-all-stagefeedback-zlib
all-gcc: all-libbacktrace

all-stage1-gcc: all-stage1-libbacktrace
all-stage2-gcc: all-stage2-libbacktrace
all-stage3-gcc: all-stage3-libbacktrace
all-stage4-gcc: all-stage4-libbacktrace
all-stageprofile-gcc: all-stageprofile-libbacktrace
all-stagefeedback-gcc: all-stagefeedback-libbacktrace
all-gcc: all-libcpp

all-stage1-gcc: all-stage1-libcpp
all-stage2-gcc: all-stage2-libcpp
all-stage3-gcc: all-stage3-libcpp
all-stage4-gcc: all-stage4-libcpp
all-stageprofile-gcc: all-stageprofile-libcpp
all-stagefeedback-gcc: all-stagefeedback-libcpp
all-gcc: all-libdecnumber

all-stage1-gcc: all-stage1-libdecnumber
all-stage2-gcc: all-stage2-libdecnumber
all-stage3-gcc: all-stage3-libdecnumber
all-stage4-gcc: all-stage4-libdecnumber
all-stageprofile-gcc: all-stageprofile-libdecnumber
all-stagefeedback-gcc: all-stagefeedback-libdecnumber
all-gcc: maybe-all-libiberty

all-stage1-gcc: maybe-all-stage1-libiberty
all-stage2-gcc: maybe-all-stage2-libiberty
all-stage3-gcc: maybe-all-stage3-libiberty
all-stage4-gcc: maybe-all-stage4-libiberty
all-stageprofile-gcc: maybe-all-stageprofile-libiberty
all-stagefeedback-gcc: maybe-all-stagefeedback-libiberty
all-gcc: maybe-all-fixincludes
all-gcc: maybe-all-lto-plugin

all-stage1-gcc: maybe-all-stage1-lto-plugin
all-stage2-gcc: maybe-all-stage2-lto-plugin
all-stage3-gcc: maybe-all-stage3-lto-plugin
all-stage4-gcc: maybe-all-stage4-lto-plugin
all-stageprofile-gcc: maybe-all-stageprofile-lto-plugin
all-stagefeedback-gcc: maybe-all-stagefeedback-lto-plugin
info-gcc: maybe-all-build-libiberty

info-stage1-gcc: maybe-all-build-libiberty
info-stage2-gcc: maybe-all-build-libiberty
info-stage3-gcc: maybe-all-build-libiberty
info-stage4-gcc: maybe-all-build-libiberty
info-stageprofile-gcc: maybe-all-build-libiberty
info-stagefeedback-gcc: maybe-all-build-libiberty
dvi-gcc: maybe-all-build-libiberty

dvi-stage1-gcc: maybe-all-build-libiberty
dvi-stage2-gcc: maybe-all-build-libiberty
dvi-stage3-gcc: maybe-all-build-libiberty
dvi-stage4-gcc: maybe-all-build-libiberty
dvi-stageprofile-gcc: maybe-all-build-libiberty
dvi-stagefeedback-gcc: maybe-all-build-libiberty
pdf-gcc: maybe-all-build-libiberty

pdf-stage1-gcc: maybe-all-build-libiberty
pdf-stage2-gcc: maybe-all-build-libiberty
pdf-stage3-gcc: maybe-all-build-libiberty
pdf-stage4-gcc: maybe-all-build-libiberty
pdf-stageprofile-gcc: maybe-all-build-libiberty
pdf-stagefeedback-gcc: maybe-all-build-libiberty
html-gcc: maybe-all-build-libiberty

html-stage1-gcc: maybe-all-build-libiberty
html-stage2-gcc: maybe-all-build-libiberty
html-stage3-gcc: maybe-all-build-libiberty
html-stage4-gcc: maybe-all-build-libiberty
html-stageprofile-gcc: maybe-all-build-libiberty
html-stagefeedback-gcc: maybe-all-build-libiberty
install-gcc: maybe-install-fixincludes
install-gcc: maybe-install-lto-plugin
install-strip-gcc: maybe-install-strip-fixincludes
install-strip-gcc: maybe-install-strip-lto-plugin
configure-libcpp: configure-libiberty

configure-stage1-libcpp: configure-stage1-libiberty
configure-stage2-libcpp: configure-stage2-libiberty
configure-stage3-libcpp: configure-stage3-libiberty
configure-stage4-libcpp: configure-stage4-libiberty
configure-stageprofile-libcpp: configure-stageprofile-libiberty
configure-stagefeedback-libcpp: configure-stagefeedback-libiberty
configure-libcpp: maybe-configure-intl

configure-stage1-libcpp: maybe-configure-stage1-intl
configure-stage2-libcpp: maybe-configure-stage2-intl
configure-stage3-libcpp: maybe-configure-stage3-intl
configure-stage4-libcpp: maybe-configure-stage4-intl
configure-stageprofile-libcpp: maybe-configure-stageprofile-intl
configure-stagefeedback-libcpp: maybe-configure-stagefeedback-intl
all-libcpp: all-libiberty

all-stage1-libcpp: all-stage1-libiberty
all-stage2-libcpp: all-stage2-libiberty
all-stage3-libcpp: all-stage3-libiberty
all-stage4-libcpp: all-stage4-libiberty
all-stageprofile-libcpp: all-stageprofile-libiberty
all-stagefeedback-libcpp: all-stagefeedback-libiberty
all-libcpp: maybe-all-intl

all-stage1-libcpp: maybe-all-stage1-intl
all-stage2-libcpp: maybe-all-stage2-intl
all-stage3-libcpp: maybe-all-stage3-intl
all-stage4-libcpp: maybe-all-stage4-intl
all-stageprofile-libcpp: maybe-all-stageprofile-intl
all-stagefeedback-libcpp: maybe-all-stagefeedback-intl
all-fixincludes: maybe-all-libiberty
all-gnattools: maybe-all-target-libada
all-lto-plugin: maybe-all-libiberty

all-stage1-lto-plugin: maybe-all-stage1-libiberty
all-stage2-lto-plugin: maybe-all-stage2-libiberty
all-stage3-lto-plugin: maybe-all-stage3-libiberty
all-stage4-lto-plugin: maybe-all-stage4-libiberty
all-stageprofile-lto-plugin: maybe-all-stageprofile-libiberty
all-stagefeedback-lto-plugin: maybe-all-stagefeedback-libiberty
all-utils: maybe-all-libiberty
configure-mpfr: maybe-all-gmp

configure-stage1-mpfr: maybe-all-stage1-gmp
configure-stage2-mpfr: maybe-all-stage2-gmp
configure-stage3-mpfr: maybe-all-stage3-gmp
configure-stage4-mpfr: maybe-all-stage4-gmp
configure-stageprofile-mpfr: maybe-all-stageprofile-gmp
configure-stagefeedback-mpfr: maybe-all-stagefeedback-gmp
configure-mpc: maybe-all-mpfr

configure-stage1-mpc: maybe-all-stage1-mpfr
configure-stage2-mpc: maybe-all-stage2-mpfr
configure-stage3-mpc: maybe-all-stage3-mpfr
configure-stage4-mpc: maybe-all-stage4-mpfr
configure-stageprofile-mpc: maybe-all-stageprofile-mpfr
configure-stagefeedback-mpc: maybe-all-stagefeedback-mpfr
configure-isl: maybe-all-gmp

configure-stage1-isl: maybe-all-stage1-gmp
configure-stage2-isl: maybe-all-stage2-gmp
configure-stage3-isl: maybe-all-stage3-gmp
configure-stage4-isl: maybe-all-stage4-gmp
configure-stageprofile-isl: maybe-all-stageprofile-gmp
configure-stagefeedback-isl: maybe-all-stagefeedback-gmp
configure-cloog: maybe-all-isl

configure-stage1-cloog: maybe-all-stage1-isl
configure-stage2-cloog: maybe-all-stage2-isl
configure-stage3-cloog: maybe-all-stage3-isl
configure-stage4-cloog: maybe-all-stage4-isl
configure-stageprofile-cloog: maybe-all-stageprofile-isl
configure-stagefeedback-cloog: maybe-all-stagefeedback-isl
configure-cloog: maybe-all-gmp

configure-stage1-cloog: maybe-all-stage1-gmp
configure-stage2-cloog: maybe-all-stage2-gmp
configure-stage3-cloog: maybe-all-stage3-gmp
configure-stage4-cloog: maybe-all-stage4-gmp
configure-stageprofile-cloog: maybe-all-stageprofile-gmp
configure-stagefeedback-cloog: maybe-all-stagefeedback-gmp
configure-gdb: maybe-all-intl
configure-gdb: maybe-configure-sim
configure-gdb: maybe-all-bfd
configure-gdb: maybe-all-libiconv
all-gdb: maybe-all-libiberty
all-gdb: maybe-all-libiconv
all-gdb: maybe-all-opcodes
all-gdb: maybe-all-readline
all-gdb: maybe-all-build-bison
all-gdb: maybe-all-sim
all-gdb: maybe-all-libdecnumber
all-gdb: maybe-all-libtermcap
configure-libgui: maybe-configure-tcl
configure-libgui: maybe-configure-tk
all-libgui: maybe-all-tcl
all-libgui: maybe-all-tk
all-libgui: maybe-all-itcl
configure-bfd: configure-libiberty

configure-stage1-bfd: configure-stage1-libiberty
configure-stage2-bfd: configure-stage2-libiberty
configure-stage3-bfd: configure-stage3-libiberty
configure-stage4-bfd: configure-stage4-libiberty
configure-stageprofile-bfd: configure-stageprofile-libiberty
configure-stagefeedback-bfd: configure-stagefeedback-libiberty
configure-bfd: maybe-configure-intl

configure-stage1-bfd: maybe-configure-stage1-intl
configure-stage2-bfd: maybe-configure-stage2-intl
configure-stage3-bfd: maybe-configure-stage3-intl
configure-stage4-bfd: maybe-configure-stage4-intl
configure-stageprofile-bfd: maybe-configure-stageprofile-intl
configure-stagefeedback-bfd: maybe-configure-stagefeedback-intl
all-bfd: maybe-all-libiberty

all-stage1-bfd: maybe-all-stage1-libiberty
all-stage2-bfd: maybe-all-stage2-libiberty
all-stage3-bfd: maybe-all-stage3-libiberty
all-stage4-bfd: maybe-all-stage4-libiberty
all-stageprofile-bfd: maybe-all-stageprofile-libiberty
all-stagefeedback-bfd: maybe-all-stagefeedback-libiberty
all-bfd: maybe-all-intl

all-stage1-bfd: maybe-all-stage1-intl
all-stage2-bfd: maybe-all-stage2-intl
all-stage3-bfd: maybe-all-stage3-intl
all-stage4-bfd: maybe-all-stage4-intl
all-stageprofile-bfd: maybe-all-stageprofile-intl
all-stagefeedback-bfd: maybe-all-stagefeedback-intl
configure-opcodes: configure-libiberty

configure-stage1-opcodes: configure-stage1-libiberty
configure-stage2-opcodes: configure-stage2-libiberty
configure-stage3-opcodes: configure-stage3-libiberty
configure-stage4-opcodes: configure-stage4-libiberty
configure-stageprofile-opcodes: configure-stageprofile-libiberty
configure-stagefeedback-opcodes: configure-stagefeedback-libiberty
all-opcodes: maybe-all-libiberty

all-stage1-opcodes: maybe-all-stage1-libiberty
all-stage2-opcodes: maybe-all-stage2-libiberty
all-stage3-opcodes: maybe-all-stage3-libiberty
all-stage4-opcodes: maybe-all-stage4-libiberty
all-stageprofile-opcodes: maybe-all-stageprofile-libiberty
all-stagefeedback-opcodes: maybe-all-stagefeedback-libiberty
configure-binutils: maybe-configure-intl

configure-stage1-binutils: maybe-configure-stage1-intl
configure-stage2-binutils: maybe-configure-stage2-intl
configure-stage3-binutils: maybe-configure-stage3-intl
configure-stage4-binutils: maybe-configure-stage4-intl
configure-stageprofile-binutils: maybe-configure-stageprofile-intl
configure-stagefeedback-binutils: maybe-configure-stagefeedback-intl
all-binutils: maybe-all-libiberty

all-stage1-binutils: maybe-all-stage1-libiberty
all-stage2-binutils: maybe-all-stage2-libiberty
all-stage3-binutils: maybe-all-stage3-libiberty
all-stage4-binutils: maybe-all-stage4-libiberty
all-stageprofile-binutils: maybe-all-stageprofile-libiberty
all-stagefeedback-binutils: maybe-all-stagefeedback-libiberty
all-binutils: maybe-all-opcodes

all-stage1-binutils: maybe-all-stage1-opcodes
all-stage2-binutils: maybe-all-stage2-opcodes
all-stage3-binutils: maybe-all-stage3-opcodes
all-stage4-binutils: maybe-all-stage4-opcodes
all-stageprofile-binutils: maybe-all-stageprofile-opcodes
all-stagefeedback-binutils: maybe-all-stagefeedback-opcodes
all-binutils: maybe-all-bfd

all-stage1-binutils: maybe-all-stage1-bfd
all-stage2-binutils: maybe-all-stage2-bfd
all-stage3-binutils: maybe-all-stage3-bfd
all-stage4-binutils: maybe-all-stage4-bfd
all-stageprofile-binutils: maybe-all-stageprofile-bfd
all-stagefeedback-binutils: maybe-all-stagefeedback-bfd
all-binutils: maybe-all-build-flex

all-stage1-binutils: maybe-all-build-flex
all-stage2-binutils: maybe-all-build-flex
all-stage3-binutils: maybe-all-build-flex
all-stage4-binutils: maybe-all-build-flex
all-stageprofile-binutils: maybe-all-build-flex
all-stagefeedback-binutils: maybe-all-build-flex
all-binutils: maybe-all-build-bison

all-stage1-binutils: maybe-all-build-bison
all-stage2-binutils: maybe-all-build-bison
all-stage3-binutils: maybe-all-build-bison
all-stage4-binutils: maybe-all-build-bison
all-stageprofile-binutils: maybe-all-build-bison
all-stagefeedback-binutils: maybe-all-build-bison
all-binutils: maybe-all-intl

all-stage1-binutils: maybe-all-stage1-intl
all-stage2-binutils: maybe-all-stage2-intl
all-stage3-binutils: maybe-all-stage3-intl
all-stage4-binutils: maybe-all-stage4-intl
all-stageprofile-binutils: maybe-all-stageprofile-intl
all-stagefeedback-binutils: maybe-all-stagefeedback-intl
all-binutils: maybe-all-gas

all-stage1-binutils: maybe-all-stage1-gas
all-stage2-binutils: maybe-all-stage2-gas
all-stage3-binutils: maybe-all-stage3-gas
all-stage4-binutils: maybe-all-stage4-gas
all-stageprofile-binutils: maybe-all-stageprofile-gas
all-stagefeedback-binutils: maybe-all-stagefeedback-gas
install-binutils: maybe-install-opcodes
install-strip-binutils: maybe-install-strip-opcodes
install-opcodes: maybe-install-bfd
install-strip-opcodes: maybe-install-strip-bfd
configure-gas: maybe-configure-intl

configure-stage1-gas: maybe-configure-stage1-intl
configure-stage2-gas: maybe-configure-stage2-intl
configure-stage3-gas: maybe-configure-stage3-intl
configure-stage4-gas: maybe-configure-stage4-intl
configure-stageprofile-gas: maybe-configure-stageprofile-intl
configure-stagefeedback-gas: maybe-configure-stagefeedback-intl
all-gas: maybe-all-libiberty

all-stage1-gas: maybe-all-stage1-libiberty
all-stage2-gas: maybe-all-stage2-libiberty
all-stage3-gas: maybe-all-stage3-libiberty
all-stage4-gas: maybe-all-stage4-libiberty
all-stageprofile-gas: maybe-all-stageprofile-libiberty
all-stagefeedback-gas: maybe-all-stagefeedback-libiberty
all-gas: maybe-all-opcodes

all-stage1-gas: maybe-all-stage1-opcodes
all-stage2-gas: maybe-all-stage2-opcodes
all-stage3-gas: maybe-all-stage3-opcodes
all-stage4-gas: maybe-all-stage4-opcodes
all-stageprofile-gas: maybe-all-stageprofile-opcodes
all-stagefeedback-gas: maybe-all-stagefeedback-opcodes
all-gas: maybe-all-bfd

all-stage1-gas: maybe-all-stage1-bfd
all-stage2-gas: maybe-all-stage2-bfd
all-stage3-gas: maybe-all-stage3-bfd
all-stage4-gas: maybe-all-stage4-bfd
all-stageprofile-gas: maybe-all-stageprofile-bfd
all-stagefeedback-gas: maybe-all-stagefeedback-bfd
all-gas: maybe-all-intl

all-stage1-gas: maybe-all-stage1-intl
all-stage2-gas: maybe-all-stage2-intl
all-stage3-gas: maybe-all-stage3-intl
all-stage4-gas: maybe-all-stage4-intl
all-stageprofile-gas: maybe-all-stageprofile-intl
all-stagefeedback-gas: maybe-all-stagefeedback-intl
configure-gprof: maybe-configure-intl
all-gprof: maybe-all-libiberty
all-gprof: maybe-all-bfd
all-gprof: maybe-all-opcodes
all-gprof: maybe-all-intl
all-gprof: maybe-all-gas
configure-ld: maybe-configure-intl

configure-stage1-ld: maybe-configure-stage1-intl
configure-stage2-ld: maybe-configure-stage2-intl
configure-stage3-ld: maybe-configure-stage3-intl
configure-stage4-ld: maybe-configure-stage4-intl
configure-stageprofile-ld: maybe-configure-stageprofile-intl
configure-stagefeedback-ld: maybe-configure-stagefeedback-intl
all-ld: maybe-all-libiberty

all-stage1-ld: maybe-all-stage1-libiberty
all-stage2-ld: maybe-all-stage2-libiberty
all-stage3-ld: maybe-all-stage3-libiberty
all-stage4-ld: maybe-all-stage4-libiberty
all-stageprofile-ld: maybe-all-stageprofile-libiberty
all-stagefeedback-ld: maybe-all-stagefeedback-libiberty
all-ld: maybe-all-bfd

all-stage1-ld: maybe-all-stage1-bfd
all-stage2-ld: maybe-all-stage2-bfd
all-stage3-ld: maybe-all-stage3-bfd
all-stage4-ld: maybe-all-stage4-bfd
all-stageprofile-ld: maybe-all-stageprofile-bfd
all-stagefeedback-ld: maybe-all-stagefeedback-bfd
all-ld: maybe-all-opcodes

all-stage1-ld: maybe-all-stage1-opcodes
all-stage2-ld: maybe-all-stage2-opcodes
all-stage3-ld: maybe-all-stage3-opcodes
all-stage4-ld: maybe-all-stage4-opcodes
all-stageprofile-ld: maybe-all-stageprofile-opcodes
all-stagefeedback-ld: maybe-all-stagefeedback-opcodes
all-ld: maybe-all-build-bison

all-stage1-ld: maybe-all-build-bison
all-stage2-ld: maybe-all-build-bison
all-stage3-ld: maybe-all-build-bison
all-stage4-ld: maybe-all-build-bison
all-stageprofile-ld: maybe-all-build-bison
all-stagefeedback-ld: maybe-all-build-bison
all-ld: maybe-all-build-flex

all-stage1-ld: maybe-all-build-flex
all-stage2-ld: maybe-all-build-flex
all-stage3-ld: maybe-all-build-flex
all-stage4-ld: maybe-all-build-flex
all-stageprofile-ld: maybe-all-build-flex
all-stagefeedback-ld: maybe-all-build-flex
all-ld: maybe-all-intl

all-stage1-ld: maybe-all-stage1-intl
all-stage2-ld: maybe-all-stage2-intl
all-stage3-ld: maybe-all-stage3-intl
all-stage4-ld: maybe-all-stage4-intl
all-stageprofile-ld: maybe-all-stageprofile-intl
all-stagefeedback-ld: maybe-all-stagefeedback-intl
all-ld: maybe-all-gas

all-stage1-ld: maybe-all-stage1-gas
all-stage2-ld: maybe-all-stage2-gas
all-stage3-ld: maybe-all-stage3-gas
all-stage4-ld: maybe-all-stage4-gas
all-stageprofile-ld: maybe-all-stageprofile-gas
all-stagefeedback-ld: maybe-all-stagefeedback-gas
install-ld: maybe-install-gold
install-strip-ld: maybe-install-strip-gold
configure-gold: maybe-configure-intl

configure-stage1-gold: maybe-configure-stage1-intl
configure-stage2-gold: maybe-configure-stage2-intl
configure-stage3-gold: maybe-configure-stage3-intl
configure-stage4-gold: maybe-configure-stage4-intl
configure-stageprofile-gold: maybe-configure-stageprofile-intl
configure-stagefeedback-gold: maybe-configure-stagefeedback-intl
all-gold: maybe-all-libiberty

all-stage1-gold: maybe-all-stage1-libiberty
all-stage2-gold: maybe-all-stage2-libiberty
all-stage3-gold: maybe-all-stage3-libiberty
all-stage4-gold: maybe-all-stage4-libiberty
all-stageprofile-gold: maybe-all-stageprofile-libiberty
all-stagefeedback-gold: maybe-all-stagefeedback-libiberty
all-gold: maybe-all-intl

all-stage1-gold: maybe-all-stage1-intl
all-stage2-gold: maybe-all-stage2-intl
all-stage3-gold: maybe-all-stage3-intl
all-stage4-gold: maybe-all-stage4-intl
all-stageprofile-gold: maybe-all-stageprofile-intl
all-stagefeedback-gold: maybe-all-stagefeedback-intl
all-gold: maybe-all-bfd

all-stage1-gold: maybe-all-stage1-bfd
all-stage2-gold: maybe-all-stage2-bfd
all-stage3-gold: maybe-all-stage3-bfd
all-stage4-gold: maybe-all-stage4-bfd
all-stageprofile-gold: maybe-all-stageprofile-bfd
all-stagefeedback-gold: maybe-all-stagefeedback-bfd
all-gold: maybe-all-build-bison

all-stage1-gold: maybe-all-build-bison
all-stage2-gold: maybe-all-build-bison
all-stage3-gold: maybe-all-build-bison
all-stage4-gold: maybe-all-build-bison
all-stageprofile-gold: maybe-all-build-bison
all-stagefeedback-gold: maybe-all-build-bison
all-gold: maybe-all-gas

all-stage1-gold: maybe-all-stage1-gas
all-stage2-gold: maybe-all-stage2-gas
all-stage3-gold: maybe-all-stage3-gas
all-stage4-gold: maybe-all-stage4-gas
all-stageprofile-gold: maybe-all-stageprofile-gas
all-stagefeedback-gold: maybe-all-stagefeedback-gas
check-gold: maybe-all-binutils

check-stage1-gold: maybe-all-stage1-binutils
check-stage2-gold: maybe-all-stage2-binutils
check-stage3-gold: maybe-all-stage3-binutils
check-stage4-gold: maybe-all-stage4-binutils
check-stageprofile-gold: maybe-all-stageprofile-binutils
check-stagefeedback-gold: maybe-all-stagefeedback-binutils
check-gold: maybe-all-gas

check-stage1-gold: maybe-all-stage1-gas
check-stage2-gold: maybe-all-stage2-gas
check-stage3-gold: maybe-all-stage3-gas
check-stage4-gold: maybe-all-stage4-gas
check-stageprofile-gold: maybe-all-stageprofile-gas
check-stagefeedback-gold: maybe-all-stagefeedback-gas
configure-opcodes: maybe-configure-intl

configure-stage1-opcodes: maybe-configure-stage1-intl
configure-stage2-opcodes: maybe-configure-stage2-intl
configure-stage3-opcodes: maybe-configure-stage3-intl
configure-stage4-opcodes: maybe-configure-stage4-intl
configure-stageprofile-opcodes: maybe-configure-stageprofile-intl
configure-stagefeedback-opcodes: maybe-configure-stagefeedback-intl
all-opcodes: maybe-all-bfd

all-stage1-opcodes: maybe-all-stage1-bfd
all-stage2-opcodes: maybe-all-stage2-bfd
all-stage3-opcodes: maybe-all-stage3-bfd
all-stage4-opcodes: maybe-all-stage4-bfd
all-stageprofile-opcodes: maybe-all-stageprofile-bfd
all-stagefeedback-opcodes: maybe-all-stagefeedback-bfd
all-opcodes: maybe-all-libiberty

all-stage1-opcodes: maybe-all-stage1-libiberty
all-stage2-opcodes: maybe-all-stage2-libiberty
all-stage3-opcodes: maybe-all-stage3-libiberty
all-stage4-opcodes: maybe-all-stage4-libiberty
all-stageprofile-opcodes: maybe-all-stageprofile-libiberty
all-stagefeedback-opcodes: maybe-all-stagefeedback-libiberty
all-opcodes: maybe-all-intl

all-stage1-opcodes: maybe-all-stage1-intl
all-stage2-opcodes: maybe-all-stage2-intl
all-stage3-opcodes: maybe-all-stage3-intl
all-stage4-opcodes: maybe-all-stage4-intl
all-stageprofile-opcodes: maybe-all-stageprofile-intl
all-stagefeedback-opcodes: maybe-all-stagefeedback-intl
all-dejagnu: maybe-all-tcl
all-dejagnu: maybe-all-expect
all-dejagnu: maybe-all-tk
configure-expect: maybe-configure-tcl
configure-expect: maybe-configure-tk
all-expect: maybe-all-tcl
all-expect: maybe-all-tk
configure-itcl: maybe-configure-tcl
configure-itcl: maybe-configure-tk
all-itcl: maybe-all-tcl
all-itcl: maybe-all-tk
install-itcl: maybe-install-tcl
install-strip-itcl: maybe-install-strip-tcl
configure-tk: maybe-configure-tcl
all-tk: maybe-all-tcl
all-sid: maybe-all-libiberty
all-sid: maybe-all-bfd
all-sid: maybe-all-opcodes
all-sid: maybe-all-tcl
all-sid: maybe-all-tk
install-sid: maybe-install-tcl
install-strip-sid: maybe-install-strip-tcl
install-sid: maybe-install-tk
install-strip-sid: maybe-install-strip-tk
configure-sim: maybe-configure-intl
all-sim: maybe-all-intl
all-sim: maybe-all-libiberty
all-sim: maybe-all-bfd
all-sim: maybe-all-opcodes
all-sim: maybe-all-readline
all-sim: maybe-configure-gdb
all-fastjar: maybe-all-zlib
all-fastjar: maybe-all-build-texinfo
all-fastjar: maybe-all-libiberty
all-bison: maybe-all-intl
all-bison: maybe-all-build-texinfo
all-flex: maybe-all-build-bison
all-flex: maybe-all-intl
all-flex: maybe-all-m4
all-flex: maybe-all-build-texinfo
all-m4: maybe-all-intl
all-m4: maybe-all-build-texinfo
configure-target-boehm-gc: maybe-all-target-libstdc++-v3
configure-target-fastjar: maybe-configure-target-zlib
all-target-fastjar: maybe-all-target-zlib
configure-target-libgo: maybe-configure-target-libffi
configure-target-libgo: maybe-all-target-libstdc++-v3
all-target-libgo: maybe-all-target-libbacktrace
all-target-libgo: maybe-all-target-libffi
all-target-libgo: maybe-all-target-libatomic
configure-target-libjava: maybe-configure-target-zlib
configure-target-libjava: maybe-configure-target-boehm-gc
configure-target-libjava: maybe-configure-target-libffi
all-target-libjava: maybe-all-fastjar
all-target-libjava: maybe-all-target-zlib
all-target-libjava: maybe-all-target-boehm-gc
all-target-libjava: maybe-all-target-libffi
configure-target-libobjc: maybe-configure-target-boehm-gc
all-target-libobjc: maybe-all-target-boehm-gc
configure-target-libstdc++-v3: maybe-configure-target-libgomp

configure-stage1-target-libstdc++-v3: maybe-configure-stage1-target-libgomp
configure-stage2-target-libstdc++-v3: maybe-configure-stage2-target-libgomp
configure-stage3-target-libstdc++-v3: maybe-configure-stage3-target-libgomp
configure-stage4-target-libstdc++-v3: maybe-configure-stage4-target-libgomp
configure-stageprofile-target-libstdc++-v3: maybe-configure-stageprofile-target-libgomp
configure-stagefeedback-target-libstdc++-v3: maybe-configure-stagefeedback-target-libgomp
configure-target-libsanitizer: maybe-all-target-libstdc++-v3

configure-stage1-target-libsanitizer: maybe-all-stage1-target-libstdc++-v3
configure-stage2-target-libsanitizer: maybe-all-stage2-target-libstdc++-v3
configure-stage3-target-libsanitizer: maybe-all-stage3-target-libstdc++-v3
configure-stage4-target-libsanitizer: maybe-all-stage4-target-libstdc++-v3
configure-stageprofile-target-libsanitizer: maybe-all-stageprofile-target-libstdc++-v3
configure-stagefeedback-target-libsanitizer: maybe-all-stagefeedback-target-libstdc++-v3
configure-target-libvtv: maybe-all-target-libstdc++-v3

configure-stage1-target-libvtv: maybe-all-stage1-target-libstdc++-v3
configure-stage2-target-libvtv: maybe-all-stage2-target-libstdc++-v3
configure-stage3-target-libvtv: maybe-all-stage3-target-libstdc++-v3
configure-stage4-target-libvtv: maybe-all-stage4-target-libstdc++-v3
configure-stageprofile-target-libvtv: maybe-all-stageprofile-target-libstdc++-v3
configure-stagefeedback-target-libvtv: maybe-all-stagefeedback-target-libstdc++-v3
all-target-libstdc++-v3: maybe-configure-target-libgomp

all-stage1-target-libstdc++-v3: maybe-configure-stage1-target-libgomp
all-stage2-target-libstdc++-v3: maybe-configure-stage2-target-libgomp
all-stage3-target-libstdc++-v3: maybe-configure-stage3-target-libgomp
all-stage4-target-libstdc++-v3: maybe-configure-stage4-target-libgomp
all-stageprofile-target-libstdc++-v3: maybe-configure-stageprofile-target-libgomp
all-stagefeedback-target-libstdc++-v3: maybe-configure-stagefeedback-target-libgomp
install-target-libgo: maybe-install-target-libatomic
install-target-libgfortran: maybe-install-target-libquadmath
install-target-libgfortran: maybe-install-target-libgcc
install-target-libsanitizer: maybe-install-target-libstdc++-v3
install-target-libsanitizer: maybe-install-target-libgcc
install-target-libvtv: maybe-install-target-libstdc++-v3
install-target-libvtv: maybe-install-target-libgcc
install-target-libjava: maybe-install-target-libgcc
install-target-libitm: maybe-install-target-libgcc
install-target-libobjc: maybe-install-target-libgcc
install-target-libstdc++-v3: maybe-install-target-libgcc
all-target-libgloss: maybe-all-target-newlib
all-target-winsup: maybe-all-target-libtermcap
configure-target-newlib: maybe-all-binutils
configure-target-newlib: maybe-all-ld
configure-target-libgfortran: maybe-all-target-libquadmath


# Dependencies for target modules on other target modules are
# described by lang_env_dependencies; the defaults apply to anything
# not mentioned there.





configure-target-libstdc++-v3: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libmudflap: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libsanitizer: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libvtv: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libssp: maybe-all-target-newlib maybe-all-target-libgloss



configure-target-libbacktrace: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libquadmath: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libgfortran: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libobjc: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libgo: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libtermcap: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-winsup: maybe-all-target-newlib maybe-all-target-libgloss


configure-target-libffi: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libjava: maybe-all-target-newlib maybe-all-target-libgloss
configure-target-libjava: maybe-all-target-libstdc++-v3

configure-target-zlib: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-boehm-gc: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-rda: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libada: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libgomp: maybe-all-target-newlib maybe-all-target-libgloss

configure-target-libitm: maybe-all-target-newlib maybe-all-target-libgloss
configure-target-libitm: maybe-all-target-libstdc++-v3

configure-target-libatomic: maybe-all-target-newlib maybe-all-target-libgloss


CONFIGURE_GDB_TK = 
GDB_TK = 
INSTALL_GDB_TK = 
configure-gdb: $(CONFIGURE_GDB_TK)
all-gdb: $(gdbnlmrequirements) $(GDB_TK)
install-gdb: $(INSTALL_GDB_TK)

# Serialization dependencies.  Host configures don't work well in parallel to
# each other, due to contention over config.cache.  Target configures and 
# build configures are similar.
# serdep.tmp

# --------------------------------
# Regenerating top level configury
# --------------------------------

# Rebuilding Makefile.in, using autogen.
AUTOGEN = autogen
$(srcdir)/Makefile.in: # $(srcdir)/Makefile.tpl $(srcdir)/Makefile.def
	cd $(srcdir) && $(AUTOGEN) Makefile.def

# Rebuilding Makefile.
Makefile: $(srcdir)/Makefile.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

config.status: configure
	CONFIG_SHELL="$(SHELL)" $(SHELL) ./config.status --recheck

# Rebuilding configure.
AUTOCONF = autoconf
$(srcdir)/configure: # $(srcdir)/configure.ac $(srcdir)/config/acx.m4 \
	$(srcdir)/config/override.m4 $(srcdir)/config/proginstall.m4
	cd $(srcdir) && $(AUTOCONF)

# ------------------------------
# Special directives to GNU Make
# ------------------------------

# Don't pass command-line variables to submakes.
.NOEXPORT:
MAKEOVERRIDES=

# end of Makefile.in
