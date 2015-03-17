Hello
-----

Bomberjacket OS is a radical new operating system with a focus on minimalism. It steals conceptually from Lisp machines and Plan 9.

![Bomberjacket OS Logo](http://dump.mntmn.com/bjos.png)

![Bomberjacket OS Screenshot](http://dump.mntmn.com/bjos-mar17-2015.png)

Architecture
------------

Layer 2: Jockey Graphical Terminal / Editor

Layer 1: Sledge Lisp JIT Compiler

Layer 0: Platform Interface. On X86-64, this is Pure64/BareMetal. The "alien hosted" variant uses SDL.

Rationale
---------

- The shell is the editor is the REPL is the language is the compiler.
- Namespacing allows sandboxing and network transparency.
- You need only one text encoding: UTF-8.
- All facilities must be navigable by keyboard.

Building
--------

1. Install the dependencies: `qemu`, `nasm`
2. Create a fresh disk image: ````./init-disk.sh````
3. Build and install the bootloader & kernel ("1" option rebuilds libraries): ````./build-kernel.sh 1````
4. Build the OS: ````./build-os.sh````

Optionally, you can build newlib yourselves if you don't trust my included 64-bit binary: ````./build-newlib.sh````

Running
-------

1. Initialise a network bridge if you haven't done so before. Read and modify this script before running! ````./setup-bridge.sh```` 

2. Launch QEMU: ````./run.sh````

You can evaluate LISP-expressions in the editor by pressing Ctrl-E (or Super-E). Example: type ````(buf-load "goa")```` and press Ctrl-E. This will load a graphics demo into the editor. Start it by pressing Ctrl-E again.

Debugging
---------

Remote kernel debugging using QEMU und gdb:

1. Add -s -S options to qemu commandline (in run.sh). -s will setup remote debugging. -S starts QEMU in halted state.
2. Start gdb and enter ````target remote localhost:1234````
3. Enter ````continue````
4. The OS will boot. You can get a function's address by doing ````(write my-function eval-buf)````.
5. Press Ctrl-C in gdb to stop the machine.
5b. If you get strange errors, set up architecture: ````set architecture i386:x86-64:intel````
6. Disassemble the function, example: ````disassemble 0xd26d1f,+100````
7. Choose your breakpoint address and set it up: ````hbreak 0xd26d1f````
8. Enter ````continue```` to continue.
9. Call your function in bomberjacket, e.g. ````(my-function)````
10. gdb should stop execution in your function. You can now inspect everything, example: ````info registers````.

Licenses
--------

Bomberjacket OS: GNU GPLv3 or later, (C) 2015 Lukas F. Hartmann / @mntmn

Bomberjacket OS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Pure64/BareMetal: 3-clause BSD, (C) ReturnInfinity / Ian Seyler

GNU Lightning: GNU GPL v3

newlib: GNU GPL v2
