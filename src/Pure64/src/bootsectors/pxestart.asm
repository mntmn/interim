; =============================================================================
; Pure64 PXE Start -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2015 Return Infinity -- see LICENSE.TXT
;
; This is a stub file for loading Pure64 and a kernel via PXE.
;
; Windows - copy /b pxestart.bin + pure64.sys + kernel64.sys pxeboot.bin
; Unix - cat pxestart.bin pure64.sys kernel64.sys > pxeboot.bin
;
; Max size of the resulting pxeboot.bin is 33792 bytes. 1K for the PXE loader
; stub and up to 32KiB for the code/data. PXE loads the file to address
; 0x00007C00 (Just like a boot sector).
;
; File Sizes
; pxestart.bin	 1024 bytes
; pure64.sys	 6144 bytes
; kernel64.sys	16384 bytes (or so)
; =============================================================================


USE16
org 0x7C00

start:
	cli				; Disable interrupts
	xor eax, eax
	mov ss, ax
	mov es, ax
	mov ds, ax
	mov sp, 0x7C00
	sti				; Enable interrupts

; Make sure the screen is set to 80x25 color text mode
	mov ax, 0x0003			; Set to normal (80x25 text) video mode
	int 0x10

; Print message
	mov si, msg_Load
	call print_string_16

	jmp 0x0000:0x8000

;------------------------------------------------------------------------------
; 16-bit Function to print a string to the screen
; input: SI - Address of start of string
print_string_16:			; Output string in SI to screen
	pusha
	mov ah, 0x0E			; int 0x10 teletype function
.repeat:
	lodsb				; Get char from string
	test al, al
	jz .done			; If char is zero, end of string
	int 0x10			; Otherwise, print it
	jmp short .repeat
.done:
	popa
	ret
;------------------------------------------------------------------------------


msg_Load db "Loading via PXE... ", 0

times 510-$+$$ db 0			; Pad out for a normal boot sector

sign dw 0xAA55

times 1024-$+$$ db 0			; Padding so that Pure64 will be aligned at 0x8000
