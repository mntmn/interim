.global _start
.global _get_stack_pointer

// https://github.com/dwelch67/raspberrypi/blob/master/float02/vectors.s

_start:
  ldr pc,reset_handler
  ldr pc,undefined_handler
  ldr pc,swi_handler
  ldr pc,prefetch_handler
  ldr pc,=data_handler
  ldr pc,unused_handler
  ldr pc,irq_handler
  ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word undef
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word hang
fiq_handler:        .word hang

reset:
  // copy vectors to #0000
  mov r0,#0x8000
  mov r1,#0x0000
  ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
  stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
  ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
  stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

  // init stack
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
  mrc p15, #0, r1, c1, c0, #2	
	orr r1, r1, #(0xf << 20)
	mcr p15, #0, r1, c1, c0, #2
	mov r1, #0
	mcr p15, #0, r1, c7, c5, #4
	mov r0,#0x40000000
	fmxr FPEXC, r0

  // enable unaligned access
  /*mrc p15, #0, r0, c1, c0, #0
  bic r0, r0, #2 // clear A bit
  orr r0, #0x400000         // 1<<22 set U bit
	mcr p15, #0, r0, c1, c0, #0*/
  
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

hang:
    b hang
    .word 0
    .word 0

undef:
    b .
    .word 0
    .word 0

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
