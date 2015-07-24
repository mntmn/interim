USPi
====

> If you read this file in an editor you should switch line wrapping on.

Overview
--------

USPi is a bare metal USB driver for the Raspberry Pi written in C. It was ported from the Circle USB library. Using C allows it to be used from bare metal C code for the Raspberry Pi. Like the Circle USB library it supports control (synchronous), bulk and interrupt (synchronous and asynchronous) transfers. Function drivers are available for USB keyboards, mice, gamepads, mass storage devices (e.g. USB flash devices) and the on-board Ethernet controller. USPi should run on all existing Raspberry Pi models.

USPi comes with an environment library (int the *env/* subdirectory) which provides all required functions to get USPi running. Furthermore there are some sample programs (in the *sample/* subdirectory) which demonstrate the use of USPi and which rely on the environment library. If you provide your own application and environment both are not needed.

Please note that this USB library was developed in a hobby project. There are known issues with it (e.g. no dynamic attachments, no error recovery, limited split support). For me it works well but that need not be the case with any device and in any situation. Also the focus for development was function not performance.

USPi was "mechanically" ported from the Circle USB library which is written in C++. That's why the source code may look a little bit strange. But it was faster to do so.

Directories
-----------

* include: USPi header files
* lib: USPi source code
* sample: Some sample programs
* env: The environment used by the sample programs (not needed by the USPi library itself)
* doc: Additional documentation files

Interface
---------

The USPi library provides functions to be used by the bare metal environment to access USB devices. There are seven groups of functions which are declared in *include/uspi.h*:

* USPi initialization
* Keyboard
* Mouse
* GamePad/Joystick
* USB Mass storage device
* Ethernet controller
* USB device information

The bare metal environment has to provide some functions to the USPi library which are declared in *include/uspios.h*. There are the six groups of functions:

* Memory allocation
* Timer
* Interrupt handling
* Property tags
* Logging
* Debug support

Configuration
-------------

Before you build the USPi library you have to configure it to meet your system configuration in the file *include/uspios.h* (top section).

Another option (NDEBUG) can be defined in Rules.mk to build the release version. In the test phase it is better to use the debug version which contains many additional checks.

Building
--------

Building is normally done on PC Linux. If building for the Raspberry Pi 1 you need a [toolchain](http://elinux.org/Rpi_Software#ARM) for the ARM1176JZF core. For Raspberry Pi 2 you need a toolchain with Cortex-A7 support. [This one](https://github.com/raspberrypi/tools/tree/master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64) should work for both. You can also build USPi on the Raspberry Pi itself on Raspbian (Debian wheezy).

First edit the file *Rules.mk* and set the Raspberry Pi version (*RASPPI*, 1 or 2) and the *PREFIX* of your toolchain commands. Alternatively you can create a *Config.mk* file (which is ignored by git) and set the Raspberry Pi version and the *PREFIX* variable to the prefix of your compiler like this (don't forget the dash at the end):

`RASPPI = 1`  
`PREFIX = arm-none-eabi-`

Then go to the lib/ directory of USPi and do:

`make clean`  
`make`

The ready build *libuspi.a* file should be in the lib/ directory.

Using
-----

Add the USPi include/ directory to the include path in your Makefile and specify the *libuspi.a* library to *ld*. Include the file *uspi.h* where you want to access USPi functions.

Samples
-------

The sample programs in the *sample/* subdirectory and all required libraries can be build from USPi root by:

`./makeall clean`  
`./makeall`

The ready build *kernel.img* image file is in the same directory where its source code is. Copy it on a SD(HC) card along with the firmware files *bootcode.bin*, *fixup.dat* and *start.elf* which can be get [here](https://github.com/raspberrypi/firmware/tree/master/boot). Put the SD(HC) card into your Raspberry Pi and start it.
