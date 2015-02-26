# Pure64 -- The BareMetal OS kernel loader
Copyright (C) 2007-2014 Return Infinity -- see LICENSE.TXT

Pure64 is a 64-bit software loader for BareMetal OS. The loader gets the computer into a full 64-bit state with no legacy compatibility layers and also enables all available CPU Cores in the computer. Pure64 keeps an information table in memory that stores important details about the computer (Amount of RAM and memory layout, number of CPU cores and their APIC IDs, etc).

See LICENSE.TXT for redistribution/modification rights, and CREDITS.TXT for a list of people involved.

Ian Seyler (ian.seyler@returninfinity.com)


## Building

The only requirement for building Pure64 is [NASM](http://www.nasm.us/) (The Netwide Assembler). In Linux you can probably download it from the distro's repository. If you are using Windows or Mac OS X you can grab pre-compiled binaries [here](http://www.nasm.us/pub/nasm/releasebuilds/2.10.07/) in the `macosx` and `win32` directories, respectively.

Build scripts are included for Unix/Linux and Windows systems.

Mac OS X/Linux: `./build.sh`

Windows: `build.bat`

## Notes

Building Pure64 from source requires NASM v2.10 or higher; the version included in the OS X 10.10 Developer Tools is not recent enough. - *Seriously, Apple? NASM v0.98 is from 2007!!*

If you use [MacPorts](http://www.macports.org), you can install NASM v2.10+ by executing: `sudo port install nasm`
