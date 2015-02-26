mnt sledge
----------

sledge compiles a minimal subset of LISP to machine code using GNU lightning.

dependencies
------------

GNU lightning: https://www.gnu.org/software/lightning/

lightning needs to be compiled with --enable-disassembler, this in turn requires 
libiberty.

building (debian)
-----------------

1. First, install package libiberty-dev with apt: ```sudo apt-get libiberty-dev```

2. Lightning is now bundled with sledge. 
build it using: ```./build-deps.sh```

3. For graphics, install SDL2: ```sudo apt-get install libsdl2-dev```.

4. Build using ```./build.sh```

usage
-----

```
./sledge

sledge> (do (def a 23) (* (+ 2500 2500) a))

    0x7f44724ef000	sub    $0x30,%rsp
    0x7f44724ef004	mov    %rbp,(%rsp)
    0x7f44724ef008	mov    %rsp,%rbp
    0x7f44724ef00b	sub    $0x1018,%rsp
    0x7f44724ef012	mov    $0x17,%eax
    0x7f44724ef017	mov    %rax,-0x1008(%rbp)
    0x7f44724ef01e	movabs $0x7f446949f34b,%rdi
    0x7f44724ef028	mov    %rax,%rsi
    0x7f44724ef02b	mov    $0x6066b0,%edx
    0x7f44724ef030	mov    $0x401ea0,%r11d
    0x7f44724ef036	callq  *%r11
    0x7f44724ef039	mov    -0x1008(%rbp),%rax
    0x7f44724ef040	mov    $0x9c4,%eax
    0x7f44724ef045	mov    $0x9c4,%r10d
    0x7f44724ef04b	add    %r10,%rax
    0x7f44724ef04e	mov    %rax,-0x1008(%rbp)
    0x7f44724ef055	movabs $0x7f446949f4fb,%rdi
    0x7f44724ef05f	mov    $0x6066b0,%esi
    0x7f44724ef064	mov    %rax,%r10
    0x7f44724ef067	mov    $0x4018f0,%r10d
    0x7f44724ef06d	callq  *%r10
    0x7f44724ef070	mov    %rax,%r10
    0x7f44724ef073	mov    -0x1008(%rbp),%rax
    0x7f44724ef07a	imul   %r10,%rax
    0x7f44724ef07e	mov    %rbp,%rsp
    0x7f44724ef081	mov    (%rsp),%rbp
    0x7f44724ef085	add    $0x30,%rsp
    0x7f44724ef089	retq   

115000
0 ms
```

Option "-f" enables full screen:

```
cat test.l | ./sledge -f
```

```
cat goa.l | ./sledge -f
```
