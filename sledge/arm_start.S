.global _start
.global _get_stack_pointer
 
_start:
  ldr     sp, =stack_top
  sub     sp, sp, #0x4

  // clear bss segment
	ldr r3, =_bss_start - 1 // -1 because of the strltb below
	ldr r4, =_bss_end - 1
1:
	cmp	r3, r4 // already done?
	strltb r0, [r3, #1]! // if not store 0 byte and advance r3
	blt 1b	// if not repeat
  
  // enable FPU (VFP/NEON)
  /*mrc p15, #0, r1, c1, c0, #2	
	orr r1, r1, #(0xf << 20)
	mcr p15, #0, r1, c1, c0, #2
	mov r1, #0
	mcr p15, #0, r1, c7, c5, #4
	mov r0,#0x40000000
	fmxr FPEXC, r0*/
  
	// jump to kernel_main
	ldr r3, =_cstartup
	blx r3
 
_inf_loop:
  b       _inf_loop
 
_get_stack_pointer:
  // Return the stack pointer value
  str     sp, [sp]
  ldr     r0, [sp]
  mov     pc, lr
  
// constants for ldr macro
constants:
.ltorg
.section ".bss"
// 16k stack aligned to 4 byte
.balign 4
.global stack
stack:
.fill 64000, 4, 0
.global stack_top
stack_top:
