# Pure64 - v0.6.1 Manual

Pure64 must be loaded to the 16-bit memory address `0x0000:0x8000`

Pure64 expects that the up to 26KiB of data after it is the software that will be loaded to address `0x0000000000100000`.


## System Requirements

A computer with at least one 64-bit Intel or AMD CPU (or anything else that uses the x86-64 architecture)

At least 2 MiB of RAM

The ability to boot via a hard drive, USB stick, or the network


## Hard disk / USB boot

`bmfs_mbr.asm` in the bootsectors folder shows how to boot via a BMFS formatted drive.

*Note*: Once Pure64 has executed you will lose access the the USB drive unless you have written a driver for it. The BIOS was used to load from it and you can't use the BIOS in 64-bit mode.


## Network boot

`pxestart.asm` in the bootsectors folder shows how to build a PXE image.


## Memory Map

This memory map shows how physical memory looks after Pure64 is finished.

<table border="1" cellpadding="2" cellspacing="0">
<tr><th>Start Address</th><th>End Address</th><th>Size</th><th>Description</th></tr>
<tr><td>0x0000000000000000</td><td>0x0000000000000FFF</td><td>4 KiB</td><td>IDT - 256 descriptors (each descriptor is 16 bytes)</td></tr>
<tr><td>0x0000000000001000</td><td>0x0000000000001FFF</td><td>4 KiB</td><td>GDT - 256 descriptors (each descriptor is 16 bytes)</td></tr>
<tr><td>0x0000000000002000</td><td>0x0000000000002FFF</td><td>4 KiB</td><td>PML4 - 512 entries, first entry points to PDP at 0x3000</td></tr>
<tr><td>0x0000000000003000</td><td>0x0000000000003FFF</td><td>4 KiB</td><td>PDP - 512 enties</td></tr>
<tr><td>0x0000000000004000</td><td>0x0000000000007FFF</td><td>16 KiB</td><td>Pure64 Data</td></tr>
<tr><td>0x0000000000008000</td><td>0x000000000000FFFF</td><td>32 KiB</td><td>Pure64 - After the OS is loaded and running this memory is free again</td></tr>
<tr><td>0x0000000000010000</td><td>0x000000000004FFFF</td><td>256 KiB</td><td>PD - Room to map 64 GiB</td></tr>
<tr><td>0x0000000000050000</td><td>0x000000000009FFFF</td><td>320 KiB</td><td>Free</td></tr>
<tr><td>0x00000000000A0000</td><td>0x00000000000FFFFF</td><td>384 KiB</td><td>ROM Area</td></tr>
<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>VGA mem at 0xA0000 (128 KiB) Color text starts at 0xB8000</td></tr>
<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Video BIOS at 0xC0000 (64 KiB)</td></tr>
<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Motherboard BIOS at F0000 (64 KiB)</td></tr>
<tr><td>0x0000000000100000</td><td>0xFFFFFFFFFFFFFFFF</td><td>1+ MiB</td><td>Your software is loaded here</td></tr>
</table>

When creating your Operating System or Demo you can use the sections marked free, however it is the safest to use memory above 1 MiB.


## Information Table

Pure64 stores an information table in memory that contains various pieces of data about the computer before it passes control over to the software you want it to load.

The Pure64 information table is located at `0x0000000000005000` and ends at `0x00000000000057FF` (2048 bytes).

<table border="1" cellpadding="2" cellspacing="0">
<tr><th>Memory Address</th><th>Variable Size</th><th>Name</th><th>Description</th></tr>
<tr><td>0x5000</td><td>64-bit</td><td>ACPI</td><td>Address of the ACPI tables</td></tr>
<tr><td>0x5008</td><td>32-bit</td><td>BSP_ID</td><td>APIC ID of the BSP</td></tr>
<tr><td>0x5010</td><td>16-bit</td><td>CPUSPEED</td><td>Speed of the CPUs in MegaHertz (<a href="http://en.wikipedia.org/wiki/Mhz#Computing">MHz</a>)</td></tr>
<tr><td>0x5012</td><td>16-bit</td><td>CORES_ACTIVE</td><td>The number of CPU cores that were activated in the system</td></tr>
<tr><td>0x5014</td><td>16-bit</td><td>CORES_DETECT</td><td>The number of CPU cores that were detected in the system</td></tr>
<tr><td>0x5016 - 0x501F</td><td>&nbsp;</td><td>&nbsp;</td><td>For future use</td></tr>
<tr><td>0x5020</td><td>32-bit</td><td>RAMAMOUNT</td><td>Amount of system RAM in Mebibytes (<a href="http://en.wikipedia.org/wiki/Mebibyte">MiB</a>)</td></tr>
<tr><td>0x5022 - 0x502F</td><td>&nbsp;</td><td>&nbsp;</td><td>For future use</td></tr>
<tr><td>0x5030</td><td>8-bit</td><td>IOAPIC_COUNT</td><td>Number of IO-APICs in the system</td></tr>
<tr><td>0x5031 - 0x503F</td><td>&nbsp;</td><td>&nbsp;</td><td>For future use</td></tr>
<tr><td>0x5040</td><td>64-bit</td><td>HPET</td><td>Base memory address for the High Precision Event Timer</td></tr>
<tr><td>0x5048 - 0x505F</td><td>&nbsp;</td><td>&nbsp;</td><td>For future use</td></tr>
<tr><td>0x5060</td><td>64-bit</td><td>LAPIC</td><td>Local APIC address</td></tr>
<tr><td>0x5068 - 0x507F</td><td>64-bit</td><td>IOAPIC</td><td>IO-APIC addresses (based on IOAPIC_COUNT)</td></tr>
<tr><td>0x5080</td><td>32-bit</td><td>VIDEO_BASE</td><td>Base memory for video (if graphics mode set)</td></tr>
<tr><td>0x5084</td><td>16-bit</td><td>VIDEO_X</td><td>X resolution</td></tr>
<tr><td>0x5086</td><td>16-bit</td><td>VIDEO_Y</td><td>Y resolution</td></tr>
<tr><td>0x5088</td><td>8-bit</td><td>VIDEO_DEPTH</td><td>Color depth</td></tr>
<tr><td>0x5100...</td><td>8-bit</td><td>APIC_ID</td><td>APIC ID's for valid CPU cores (based on CORES_ACTIVE)</td></tr>
</table>

A copy of the E820 System Memory Map is stored at memory address `0x0000000000004000`. Each E820 record is 32 bytes in length and the memory map is terminated by a blank record.<p />
<table border="1" cellpadding="2" cellspacing="0">
<tr><th>Variable</th><th>Variable Size</th><th>Description</th></tr>
<tr><td>Starting Address</td><td>64-bit</td><td>The starting address for this record</td></tr>
<tr><td>Length</td><td>64-bit</td><td>The length of memory for this record</td></tr>
<tr><td>Memory Type</td><td>32-bit</td><td>Type 1 is usable memory, Type 2 is not usable</td></tr>
<tr><td>Extended Attributes</td><td>32-bit</td><td>ACPI 3.0 Extended Attributes bitfield</td></tr>
<tr><td>Padding</td><td>64-bit</td><td>Padding for 32-byte alignment</td></tr>
</table>
For more information on the E820 Memory Map: <a href="http://wiki.osdev.org/Detecting_Memory_%28x86%29#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820">OSDev wiki on E820</a><p />
