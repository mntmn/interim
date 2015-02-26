; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; INIT SMP AP
; =============================================================================


USE16

init_smp_ap:
	jmp 0x0000:clearcs_ap

clearcs_ap:

; Enable the A20 gate
set_A20_ap:
	in al, 0x64
	test al, 0x02
	jnz set_A20_ap
	mov al, 0xD1
	out 0x64, al
check_A20_ap:
	in al, 0x64
	test al, 0x02
	jnz check_A20_ap
	mov al, 0xDF
	out 0x60, al

; At this point we are done with real mode and BIOS interrupts. Jump to 32-bit mode.
	lgdt [cs:GDTR32]		; load GDT register

	mov eax, cr0 			; switch to 32-bit protected mode
	or al, 1
	mov cr0, eax

	jmp 8:startap32

align 16


; =============================================================================
; 32-bit mode
USE32

startap32:
	mov eax, 16			; load 4 GB data descriptor
	mov ds, ax			; to all data segment registers
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	xor ebp, ebp
	mov esp, 0x8000			; Set a known free location for the stack

; Load the GDT
	lgdt [GDTR64]

; Enable extended properties
	mov eax, cr4
	or eax, 0x0000000B0		; PGE (Bit 7), PAE (Bit 5), and PSE (Bit 4)
	mov cr4, eax

; Point cr3 at PML4
	mov eax, 0x00002008		; Write-thru (Bit 3)
	mov cr3, eax

; Enable long mode and SYSCALL/SYSRET
	mov ecx, 0xC0000080		; EFER MSR number
	rdmsr				; Read EFER
	or eax, 0x00000101 		; LME (Bit 8)
	wrmsr				; Write EFER

; Enable paging to activate long mode
	mov eax, cr0
	or eax, 0x80000000		; PG (Bit 31)
	mov cr0, eax

; Make the jump directly from 16-bit real mode to 64-bit long mode
	jmp SYS64_CODE_SEL:startap64

align 16


; =============================================================================
; 64-bit mode
USE64

startap64:
	xor rax, rax			; aka r0
	xor rbx, rbx			; aka r3
	xor rcx, rcx			; aka r1
	xor rdx, rdx			; aka r2
	xor rsi, rsi			; aka r6
	xor rdi, rdi			; aka r7
	xor rbp, rbp			; aka r5
	xor rsp, rsp			; aka r4
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15

	mov ds, ax			; Clear the legacy segment registers
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	mov rax, clearcs64_ap
	jmp rax
	nop
clearcs64_ap:
	xor rax, rax

	; Reset the stack. Each CPU gets a 1024-byte unique stack location
	mov rsi, [os_LocalAPICAddress]	; We would call os_smp_get_id here but the stack is not ...
	add rsi, 0x20			; ... yet defined. It is safer to find the value directly.
	lodsd				; Load a 32-bit value. We only want the high 8 bits
	shr rax, 24			; Shift to the right and AL now holds the CPU's APIC ID
	shl rax, 10			; shift left 10 bits for a 1024byte stack
	add rax, 0x0000000000050400	; stacks decrement when you "push", start at 1024 bytes in
	mov rsp, rax			; Pure64 leaves 0x50000-0x9FFFF free so we use that

	lgdt [GDTR64]			; Load the GDT
	lidt [IDTR64]			; load IDT register

; Enable Local APIC on AP
	mov rsi, [os_LocalAPICAddress]
	add rsi, 0x00f0			; Offset to Spurious Interrupt Register
	mov rdi, rsi
	lodsd
	or eax, 0000000100000000b
	stosd

	call init_cpu			; Setup CPU

; Make sure exceptions are working.
;	xor rax, rax
;	xor rbx, rbx
;	xor rcx, rcx
;	xor rdx, rdx
;	div rax

	lock inc word [cpu_activated]
	xor eax, eax
	mov rsi, [os_LocalAPICAddress]
	add rsi, 0x20			; Add the offset for the APIC ID location
	lodsd				; APIC ID is stored in bits 31:24
	shr rax, 24			; AL now holds the CPU's APIC ID (0 - 255)
	mov rdi, 0x00005700		; The location where the cpu values are stored
	add rdi, rax			; RDI points to infomap CPU area + APIC ID. ex F701 would be APIC ID 1
	mov al, 1
	stosb
	sti				; Activate interrupts for SMP
	jmp ap_sleep


align 16

ap_sleep:
	hlt				; Suspend CPU until an interrupt is received. opcode for hlt is 0xF4
	jmp ap_sleep			; just-in-case of an NMI


; =============================================================================
; EOF
