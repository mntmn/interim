# What I Learned Writing an Operating System (and Building a Keyboard)

## Roots, Bloody Roots

I don't recall the exact moment when I decided to start the project that turned out to be much, much more work and patience then I ever expected, but it must have been around 1 or 2 years ago.

It has its roots in my experiments with turning AVR-based microcontrollers (Arduino, Teensy) into something resembling a PC or terminal. Back then, the first step was that I tried to generate a VGA signal and display characters on a VGA monitor directly from the MCU without specialized "graphics hardware". Surprisingly, after a lot of glitches and noise, it worked. I connected the pins of an old PS/2 keyboard to the same MCU and a few lines of code later, I could type letters on the screen and had a minimal command line. These early little successes gave me a lot of motivation. 

*When entering new territory, first make small attempts at something you never did before to get some first feelings of success.*

When I moved to the Teensy, this was my first hands-on encounter with an ARM processor; although I quickly found out that it doesn't really understand the ARM instruction set, but a mini version of it called Thumb. I pasted some code into my bare-bones character generator that could disassemble Thumb, and later wrote a very basic text editor in C that had an "assemble" function that generated Thumb code from assembler source. It didn't support all opcodes, nor macros or other fancy assembler features, but it worked. I soldered an SD card directly to this little computer and linking Arduino's SDFat library, I could read and write files! Later I found out that I could generate up to 256 colors by constructing a resistor ladder that would turn 8 output bits into blazing RRGGGBB encoded pixels, and a similar approach works for 8-bit sound. I spent some nights programming rotating 3D cubes, loading pixel images from SD and even streaming music from a WAV file. It was good. This "bare metal" kind of directness was what I had been missing from computers since the end of the C64 and Amiga times. 

But it was extremely limited because MCUs typically only have a few kilobytes (!) of RAM. Very fast, on-die SRAM, but most have something like 8, 16 or (Teensy 3.1) 64kB. Not enough to store a display framebuffer in a satisfying resolution (>320x256). Also, MCUs typically operate at speeds of less than 100 MHz.

Since these experiments, I really craved a computer system that was dead simple, "brutal" and direct, but at the same time in the GHz/GB arena so that I could manipulate interesting audiovisual content and make use of the internet in a meaningful way.

I still haven't found a system like that, but I have an approximation.

## A Simpler OS

At the same time, I had a nagging feeling that I wanted a simpler operating system that had less layers between me and the hardware, and whose complexity was low enough so someone with limited time could wrap their head around it and become productive quickly. By productive I mean "get graphics and text on the screen, sound from the speakers and packets on the network" and not "run 100 mysterious background services, fill your RAM with a bloated office package and load advertising from the web". 

I figured I needed a few components to make the system do something interesting:

1. Load my code and make it run (setup)
2. Get a pointer to the display contents so I can put pixels on the screen (framebuffer)
3. Load stuff – like a font – from disk so I can display text (filesystem)
4. Receive characters from a keyboard
5. Have some kind of main loop that interprets keyboard input as commands (a REPL)
7. Write data to a disk
8. Receive and send packets from/to ethernet (network)

The platform I chose to implement these things was the Raspberry Pi 2 (actually first the Pi 1 because I had several lying around, but this is a differnt story). The Raspberry Pi 2 fulfills, in theory, mostly, the requirements I had for the hardware:

- (almost) 1 GHz RISC CPU (actually _four_ of them + "QPU")
- 1 GB RAM
- Full-HD graphics output
- Stereo CD quality audio output
- "bare-metal" programmable

What I dislike about the RPi 2:

- Requires closed-source binary blobs to boot
- Un(der)documented Videocore IV "QPU" has boot control of system. ARM CPU is a peripheral device
- Unnecessary features on the board (MIPI, CSI) that seem like left-overs from mobile SoC
- Ethernet is an internal USB device; USB is overly complex
- After working with it intensively for several months, it doesn't seem at all like a "clean and simple" system to me
- More detailled criticism at https://wiki.debian.org/RaspberryPi

## Bare-Metal Setup (RPi2)

Loading a bare-metal kernel on the RPi 2 is relatively easy. You put a file "kernel7.img" on the first FAT partition of an MBR partioned SD card. You also put the required binary blobs and a "config.txt" file on the card. Then you plug it in and pray. 

How do you create a "kernel" file?

kernel7.img is an ARM binary file that you can generate from an ELF executable using objcopy from the ARM version of GNU binutils:

    arm-none-eabi-objcopy build/interim-arm.elf -O binary build/kernel7.img

The secret sauce in the RPi2 loads your binary not at address 0x0, but at address 0x8000. To make everything work, if you compile your binary with GCC, for example, you need a linker file (a file with the extension .ld, passed to gcc with the -T parameter) like this that specifies the start address at the right place:

    ENTRY(_start)
    SECTIONS
    {
    . = 0x8000;
    .text : {
    KEEP(*(.text.boot))
    *(.text)
    }
    .rodata : {
    *(.rodata)
    }
    .data : {
    *(.data)
    }
    .bss : {
    _bss_start = ALIGN(4096);
    bss = ALIGN(4096);
    *(.bss)
    }
    _bss_end = .;
    }

Linker files look weird, no? Some lessons I learned here:

- .text is the section that contains your actual machine code
- .rodata and .data contain constants like constant numbers and strings
- .bss contains non-stack variables that must be initialized to zero at program startup (you have to do this by yourself, or you will get weird behaviour because of randomly initialized variables)

If your kernel is written in a high level language like C, this language needs some setup before code written in it can run reliably on the target platform. Which is at least:

- Clear (zero) the .bss section (see above)
- Set up a stack (without a stack, function calls in C will crash)

Usually, and I also did it like this, you do this setup in an assembler fragment that is linked in at the start address of your kernel. This performs the critical setup -- on the way probably switching on some CPU features -- and then jump to the "main" function of your C code; then you are in C-land.

How to set up the stack?

Setting up the stack requires you to think about the general memory layout of the system. I decided to have a flat memory layout (virtual addresses = physical addresses, to avoid indirection / knots in my brain) and make the stack and heap (memory that is allocated by the language; explanation further down below) grow in opposite direction from each other.

I did it like this:

- The heap ("main working memory") starts at 0x1000000, which is 16 MB, and allocations grow the heap with increasing addresses.
- The stack (a temporary storage used mainly for function calls) starts at 0xfffffc, which is 4 bytes (1 word = size of a 32-bit pointer) below the heap and grows "down". This means for every new item on the stack, the pointer to the stack (SP) is decremented by 4.
- The kernel code and data occupy the first MB of RAM (so the stack cannot be larger than 15 MB, or it would overwrite the kernel).

What else to do in the assembler setup fragment?

- Switch on VFP/NEON, the floating point and vector units of the ARM, respectively. Without this, if you use any floating point code, the CPU will crash and you will wonder why.

- In the ARM Architecture Reference Manual (ARM ARM), they also set up the MMU (memory management unit) in assembler, but I found this too cumbersome and adapted mvrn's C code to do this instead.

Most hardware configuration that is ARM-related, like switching on the VFP, is done via the MRC and MCR instructions. First you read from the control/coprocessor registers using MRC to a temporary register, then you twiddle the bits and then you write everything back with MCR. Example:

    mrc p15, #0, r1, c1, c0, #2	
    orr r1, r1, #(0xf << 20)
    mcr p15, #0, r1, c1, c0, #2

## Setup in the kernel

    ldr r3, =main // load address of main to R3
    blx r3        // branch to R3 and store PC in link register

The main() function does more hardware configuration:

- UART, the "serial port", so we can do early debug printing over the serial line, and see what is going on during boot (invaluable!)
- MMU, the memory management unit, a complicated topic that helped me boost performace by 100x or so (and I discovered it very late)
- QPU, the 3D/shader "quad" processor
- Framebuffer address (graphics output)
- USB (I integrated parts of the "USPi" library for this)
- USB keyboard
- USB ethernet

Then I launch the REPL, which is the system's main loop.

## Setup in the REPL

- Init the compiler (which in turn inits the allocator, which is necessary to allocate Interim Lisp objects on the heap)
- Mount filesystems (like /framebuffer, /keyboard, /console, /mouse, /sound and /sd, the SD card interface)

Finally, the parts of the OS written in Interim Lisp are loaded by evaluating:

    (eval (read (recv (open "/sd/shell.l"))))

## Reader

File: reader.c

The Reader parses ASCII/UTF-8 strings into S-Expressions, which are internally represented as trees of the Cell struct (defined in minilisp.h).

A Cell consists of 3 machine words: value/addr (CAR), size/next (CDR) and the tag, which specifies the type of the Cell.

    typedef struct Cell {
      union {
        jit_word_t value;
        void* addr;
      };
      union {
        jit_word_t size;
        void* next;
      };
      jit_word_t tag;
    } Cell;

## Allocator

Carves cells from the heap.

    Cell* cell_alloc() {
      if (free_list_avail>free_list_consumed) {
        // serve from free list
        int idx = free_list_consumed;
        free_list_consumed++;
        Cell* res = free_list[idx];
        return res;
      } else {
        // grow heap
        Cell* res = &cell_heap[cells_used];
        cells_used++;
        if (cells_used>MAX_CELLS) {
          // out of memory
          printf("[cell_alloc] failed: MAX_CELLS used.\n");
          exit(1);
        }
        return res;
      }
    }

## Compiler

File: compiler.c

- Opcode list
- Function signatures

## JIT

File: jit_arm_raw.c

Generates ARM instructions.

    void jit_movr(int dreg, int sreg) {
      uint32_t op = 0xe1a00000;
      op |= (sreg<<0);
      op |= (dreg<<12);
      
      code[code_idx++] = op;
    }


## Execute

## Writer

File: writer.c

Turns Cell trees into human- (and Reader-) readable strings.

## Garbage Collector

File: alloc.c

- Mark and sweep

Finds all reachable objects and frees the rest.

## "Multitasking"

Interim is single-tasked. Cooperative multi-tasking is emulated by round-robin executing a list of functions in the global *tasks* list. Tasks can be spawned by appending a function to this list.

## MMU/L1/L2 Caches setup

## QPU/3D graphics setup

## Filesystem: /framebuffer

## Filesystem: /keyboard

## Filesystem: /mouse

    (def mouse (open "/mouse"))

    (let mouse-info (recv mouse))
    (def mouse-dx (car (car mouse-info)))
    (def mouse-dy (cdr (car mouse-info)))

## Filesystem: /sound

## Filesystem: /sd

## Filesystem: /net

    (def freenode (open "/net/tcp/62.231.75.133/6667"))

    (let packet (recv net))
    (if (size packet) (do
      …
    ))

## Interim Lisp

Interim's syntax is defined as follows.

An atom is one of the following:

- an integer: 1234
- an ASCII symbol: hello
- a UTF-8 string: "こんにちは"
- a hexadecimal byte string: [deadbeef01020304]
- a pair: (123 . 456)
- a "lambda", which is an applicable function
- nothing

A pair is a link between two atoms; it has a left-hand side (traditionally called "car", "contents of address register") and a right-hand side (traditionally called "cdr", "contents of data register").

A list atom is a pair whose left side points to an atom of any type and whose right side points to the next list atom or an "empty list" atom (a pair of two "nothings"). Lists are entered and displayed simply by separating atoms with whitespace and wrapping the result in parentheses: (1 2 3 4).

Interim programs are simply Interim lists. The first item of a progam list must be the symbol of the function to apply to the following parameters; the remaining items of the list are the parameters:

    (+ 1 2)    ; evaluates to 3.
    (- 5 4)    ; evaluates to 1.
    (def a 23) ; evaluates to 23. as a side-effect, the symbol "a" is defined as the number 23.

    (def double (fn x (+ x x))) ; defines the function "double" that takes one parameter called "x"
    (double a)                  ; now evaluates to 46.

A number of built-in symbols are available at system startup.

Symbol definition and functions
-------------------------------

    (def <symbol> <atom>)
Definition. Binds an atom to a symbol (globally).

    (let <symbol> <atom>)
Definition. Binds an atom to a symbol (function-locally).

    (fn <arg1 argn …> <body>)
Function. Defines a function with parameter symbols ("arguments") which can be used as placeholders anywhere in the function body.

    (<symbol> <arg1 argn…>)
Application. Apply arguments to the function bound to the symbol.

Arithmetic
----------

    (+), (-), (*), (/), (%)
Add, subtract, multiply, divide, modulus. These operate on integers. A non-nothing non-integer is interpreted as 1. Nothing is interpreted as 0. This allows logic to be constructed from arithmetic.

    (gt <a> <b>), (lt <a> <b>), (eq <a> <b>)
Compare two values (a and b). Gt returns 1 if a is greater than b and 0 if not. Lt does the opposite. Eq returns 1 if a and b are equal (they represent the same number or object) and 0 if not.

Flow control
------------

    (if <condition> <then> <else>)
Conditional branch. If condition evaluates to non-zero, <then> is evaluated, or else <else> is evaluted. The value of the evaluated branch is returned.

    (while <condition> <then>)
Conditional loop. As long as condition evaluates to non-zero, <then> is evaluated over and over again. The value of the last evaluation is returned.

    (do <expr1> <exprn> …)
Sequencing. Expressions are evaluated in the given order; The value of the last evaluation is returned.

Pairs & Lists
-------------

    (cons <a> <b>)
Returns a pair of a and b: (a.b). 

    (car <pair>)
Returns the left side of a pair.

    (cdr <pair>)
Returns the right side of a pair.

    (quote <s-expression>)
Returns the given S-expression unmodified without evaluating it.

    (list <a> <b> …)
Constructs a list from the given parameters.

Homoiconicity & Reflection
--------------------------

    (read <string>)
Deserialization. Parses a string (text) into atoms.

    (write <op>)
Serialization. Returns a string representation of the given atom.

    (symbols)
Returns a list of all visible symbols.

Strings & Buffers
-----------------

Strings (text), byte strings (binary data) and files have a uniform interface in Interim OS. All system resources (see Section 3.6) in Interim OS are exposed as file systems which can be addressed using (open <path>). The opened "file" behaves exactly like a byte string; thus, a string created at run-time is a nameless file in memory.

    (alloc 65536)
Returns a 64-kilobyte string of zeroes.

    (size <str>)
Returns the length of a string in bytes.

    (substr <str> <offset> <length>)
Returns a substring of length <length> that starts at <offset> bytes in the given string.

    (put <str> <offset> <byte>)
Replaces the byte at position <offset> in the given string with a copy of <byte>.

    (get <str> <offset>)
Returns the byte at position <offset> in the given string.

Filesystem
----------

    (open "/path/to/file")
Returns a stream that is an interface to the file at the specified path.

    (send <sym> <str2>)
Sends (appends) a string to the given symbol representing a stream.

    (recv <sym> <length>)
Receives (consumes) up to <length> bytes from the stream represented by <sym>.
