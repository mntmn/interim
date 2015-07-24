.global khrn_hw_full_memory_barrier

khrn_hw_full_memory_barrier:

   ldr  r0, =khrn_hw_full_memory_barrier // any harmless address

   orr  r0, r0, #0xc0000000
   ldr  r1, [r0] // load from direct alias
   mov  r1, r1 // kill lazy load

   // todo: we're assuming v3d is turned on!
   mov  r0, #0x3fc00000          // v3d_ident0
   ldr  r1, [r0] // load from v3d
   mov  r1, r1 // kill lazy load

   mov  pc, lr
  
