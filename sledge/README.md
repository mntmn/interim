mnt sledge
----------

sledge JIT-compiles a minimal subset of LISP to a minimal subset of x64 or armv5 machine code.

dependencies
------------

- gcc (for building and for x64 JIT. not required for ARM JIT.)
- optionally SDL or SDL2 for windowed framebuffer.

building (x64)
--------------

1. Build using ```./build.sh```

building (rpi/ARM)
------------------

1. Build using ```./build_rpi.sh```

usage
-----

```
./sledge

sledge> (def my-function (fn a b (* a (+ a b))))

JIT ---------------------
push %rdi
jmp f1_0x7f6a0144ca98
f0_0x7f6a0144ca98:
push %rdi
movq %r12, %rdi
mov (%rdi), %rdi
movq %r13, %rsi
mov (%rsi), %rsi
addq %rsi, %rdi
movq %rdi, %rsi
pop %rdi
movq %r12, %rdi
mov (%rdi), %rdi
imulq %rsi, %rdi
mov $0x406f02, %rax
callq *%rax # alloc_int
ret
f1_0x7f6a0144ca98:
mov $0x7f6a0144ca98, %rax
movq %rax, %rsi
pop %rdi
movq %rax, %rsi
mov $0x7f6a0144c750, %rdi
mov $0x401b21, %rax
callq *%rax # insert_global_symbol
ret
-------------------------

sledge> (my-function 35 36)

JIT ---------------------
mov $0x7f6a0144cb28, %r12
mov $0x7f6a0144cb58, %r13
mov $0x7f6a16a8b003, %rax
callq *%rax # lambda
ret
-------------------------

2485
```
