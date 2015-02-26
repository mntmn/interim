; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; Interrupts
; =============================================================================


; -----------------------------------------------------------------------------
; Default exception handler
exception_gate:
	mov rsi, int_string
	call os_print_string
	mov rsi, exc_string
	call os_print_string
exception_gate_halt:
	cli				; Disable interrupts
	hlt				; Halt the system
	jmp exception_gate_halt
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; Default interrupt handler
interrupt_gate:				; handler for all other interrupts
	iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; Keyboard interrupt. IRQ 0x01, INT 0x21
; This IRQ runs whenever there is input on the keyboard
align 16
keyboard:
	push rdi
	push rax

	xor eax, eax

	in al, 0x60			; Get the scancode from the keyboard
	test al, 0x80
	jnz keyboard_done

	mov [0x000B8088], al		; Dump the scancode to the screen

	mov rax, [os_Counter_RTC]
	add rax, 10
	mov [os_Counter_RTC], rax

keyboard_done:
	mov al, 0x20			; Acknowledge the IRQ
	out 0x20, al

	pop rax
	pop rdi
	iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; Cascade interrupt. IRQ 0x02, INT 0x22
cascade:
	push rax

	mov al, 0x20			; Acknowledge the IRQ
	out 0x20, al

	pop rax
	iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; Real-time clock interrupt. IRQ 0x08, INT 0x28
align 16
rtc:
	push rdi
	push rax

	add qword [os_Counter_RTC], 1	; 64-bit counter started at bootup

	mov al, 'R'
	mov [0x000B8092], al
	mov rax, [os_Counter_RTC]
	and al, 1			; Clear all but lowest bit (Can only be 0 or 1)
	add al, 48
	mov [0x000B8094], al
	mov al, 0x0C			; Select RTC register C
	out 0x70, al			; Port 0x70 is the RTC index, and 0x71 is the RTC data
	in al, 0x71			; Read the value in register C

	mov al, 0x20			; Acknowledge the IRQ
	out 0xA0, al
	out 0x20, al

	pop rax
	pop rdi
	iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; Spurious interrupt. INT 0xFF
align 16
spurious:				; handler for spurious interrupts
	mov al, 'S'
	mov [0x000B8080], al
	iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; CPU Exception Gates
exception_gate_00:
	mov al, 0x00
	jmp exception_gate_main

exception_gate_01:
	mov al, 0x01
	jmp exception_gate_main

exception_gate_02:
	mov al, 0x02
	jmp exception_gate_main

exception_gate_03:
	mov al, 0x03
	jmp exception_gate_main

exception_gate_04:
	mov al, 0x04
	jmp exception_gate_main

exception_gate_05:
	mov al, 0x05
	jmp exception_gate_main

exception_gate_06:
	mov al, 0x06
	jmp exception_gate_main

exception_gate_07:
	mov al, 0x07
	jmp exception_gate_main

exception_gate_08:
	mov al, 0x08
	jmp exception_gate_main

exception_gate_09:
	mov al, 0x09
	jmp exception_gate_main

exception_gate_10:
	mov al, 0x0A
	jmp exception_gate_main

exception_gate_11:
	mov al, 0x0B
	jmp exception_gate_main

exception_gate_12:
	mov al, 0x0C
	jmp exception_gate_main

exception_gate_13:
	mov al, 0x0D
	jmp exception_gate_main

exception_gate_14:
	mov al, 0x0E
	jmp exception_gate_main

exception_gate_15:
	mov al, 0x0F
	jmp exception_gate_main

exception_gate_16:
	mov al, 0x10
	jmp exception_gate_main

exception_gate_17:
	mov al, 0x11
	jmp exception_gate_main

exception_gate_18:
	mov al, 0x12
	jmp exception_gate_main

exception_gate_19:
	mov al, 0x13
	jmp exception_gate_main

exception_gate_main:
	call os_print_newline
	mov rsi, int_string
	call os_print_string
	mov rsi, exc_string00
	and rax, 0xFF			; Clear out everything in RAX except for AL
	shl eax, 3				; Quick multiply by 3
	add rsi, rax				; Use the value in RAX as an offset to get to the right message
	call os_print_string
	mov rsi, adr_string
	call os_print_string
	mov rax, [rsp]
	call os_debug_dump_rax
	call os_print_newline
	call os_dump_regs

exception_gate_main_hang:
	nop
	jmp exception_gate_main_hang	; Hang. User must reset machine at this point

; Strings for the error messages
int_string db 'Pure64 - Exception ', 0
adr_string db ' @ 0x', 0
exc_string db '?? - Unknown', 0
align 16
exc_string00 db '00 - DE', 0
exc_string01 db '01 - DB', 0
exc_string02 db '02     ', 0
exc_string03 db '03 - BP', 0
exc_string04 db '04 - OF', 0
exc_string05 db '05 - BR', 0
exc_string06 db '06 - UD', 0
exc_string07 db '07 - NM', 0
exc_string08 db '08 - DF', 0
exc_string09 db '09     ', 0		; No longer generated on new CPU's
exc_string10 db '10 - TS', 0
exc_string11 db '11 - NP', 0
exc_string12 db '12 - SS', 0
exc_string13 db '13 - GP', 0
exc_string14 db '14 - PF', 0
exc_string15 db '15     ', 0
exc_string16 db '16 - MF', 0
exc_string17 db '17 - AC', 0
exc_string18 db '18 - MC', 0
exc_string19 db '19 - XM', 0


; =============================================================================
; EOF
