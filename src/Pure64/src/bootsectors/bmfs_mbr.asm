; =============================================================================
; Pure64 MBR -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2015 Return Infinity -- see LICENSE.TXT
;
; This Master Boot Record will load Pure64 from a pre-defined location on the
; hard drive without making use of the file system.
;
; In this code we are expecting a BMFS-formatted drive. With BMFS the Pure64
; binary is required to start at sector 16 (8192 bytes from the start). A small
; ckeck is made to make sure Pure64 was loaded by comparing a signiture.
; =============================================================================

USE16
org 0x7C00

entry:
	cli				; Disable interrupts
;	xchg bx, bx			; Bochs magic debug
	xor eax, eax
	mov ss, ax
	mov es, ax
	mov ds, ax
	mov sp, 0x7C00
	sti				; Enable interrupts

	mov [DriveNumber], dl		; BIOS passes drive number in DL

	mov si, msg_Load
	call print_string_16

	mov eax, 64			; Number of sectors to load. 64 sectors = 32768 bytes
	mov ebx, 16			; Start immediately after directory (offset 8192)
	mov cx, 0x8000			; Pure64 expects to be loaded at 0x8000

load_nextsector:
	call readsector			; Load 512 bytes
	dec eax
	cmp eax, 0
	jnz load_nextsector

	mov eax, [0x8000]
	cmp eax, 0xC03166FA		; Match against the Pure64 binary
	jne magic_fail

	mov si, msg_LoadDone
	call print_string_16

	jmp 0x0000:0x8000

magic_fail:
	mov si, msg_MagicFail
	call print_string_16
halt:
	hlt
	jmp halt

;------------------------------------------------------------------------------
; Read a sector from a disk, using LBA
; IN:	EAX - High word of 64-bit DOS sector number
;	EBX - Low word of 64-bit DOS sector number
;	ES:CX - destination buffer
; OUT:	ES:CX points one byte after the last byte read
;	EAX - High word of next sector
;	EBX - Low word of sector
readsector:
	push eax
	xor eax, eax			; We don't need to load from sectors > 32-bit
	push dx
	push si
	push di

read_it:
	push eax			; Save the sector number
	push ebx
	mov di, sp			; remember parameter block end

	push eax			; [C] sector number high 32bit
	push ebx			; [8] sector number low 32bit
	push es				; [6] buffer segment
	push cx				; [4] buffer offset
	push byte 1			; [2] 1 sector (word)
	push byte 16			; [0] size of parameter block (word)

	mov si, sp
	mov dl, [DriveNumber]
	mov ah, 42h			; EXTENDED READ
	int 0x13			; http://hdebruijn.soo.dto.tudelft.nl/newpage/interupt/out-0700.htm#0651

	mov sp, di			; remove parameter block from stack
	pop ebx
	pop eax				; Restore the sector number

	jnc read_ok			; jump if no error

	push ax
	xor ah, ah			; else, reset and retry
	int 0x13
	pop ax
	jmp read_it

read_ok:
	add ebx, 1			; increment next sector with carry
	adc eax, 0
	add cx, 512			; Add bytes per sector
	jnc no_incr_es			; if overflow...

incr_es:
	mov dx, es
	add dh, 0x10			; ...add 1000h to ES
	mov es, dx

no_incr_es:
	pop di
	pop si
	pop dx
	pop eax

	ret
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
; 16-bit function to print a string to the screen
; IN:	SI - Address of start of string
print_string_16:			; Output string in SI to screen
	pusha
	mov ah, 0x0E			; int 0x10 teletype function
.repeat:
	lodsb				; Get char from string
	cmp al, 0
	je .done			; If char is zero, end of string
	int 0x10			; Otherwise, print it
	jmp short .repeat
.done:
	popa
	ret
;------------------------------------------------------------------------------


msg_Load db "BMFS MBR v1.0 - Loading Pure64", 0
msg_LoadDone db " - done.", 13, 10, "Executing...", 0
msg_MagicFail db " - Not found!", 0
DriveNumber db 0x00

times 446-$+$$ db 0

; False partition table entry required by some BIOS vendors.
db 0x80, 0x00, 0x01, 0x00, 0xEB, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF

times 510-$+$$ db 0

sign dw 0xAA55
