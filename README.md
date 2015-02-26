Hello
-----

Bomberjacket OS is a radical new operating system with a focus on minimalism. It steals conceptually from Lisp machines and Plan 9.

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

Licenses
--------

Bomberjacket OS: MIT, (C)2015 Lukas F. Hartmann / @mntmn

Pure64/BareMetal: 3-clause BSD (C)ReturnInfinity / Ian Seyler

GNU Lightning: GNU GPL v3

newlib: GNU GPL v2
