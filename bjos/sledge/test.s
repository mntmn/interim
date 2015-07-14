push {r1}
ldr r1, =0x7f0baab61bc0
ldr r1, [r1]
ldr r2, =0x7f0baab61bf0
ldr r2, [r2]
add r1, r2, r2
ldr lr, =0x408b58 // alloc_int
bx lr
mov r2, r1
pop {r1}
ldr r1, =0x7f0baab61b48
ldr r1, [r1]
add r1, r2, r2
ldr lr, =0x408b58 // alloc_int
bx lr
