; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; INIT ISA
; =============================================================================


init_isa:
	mov edi, 0x00004000		; Clear out memory for the E820 map
	xor eax, eax
	mov ecx, 2048
	rep stosd

; Get the BIOS E820 Memory Map
; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
do_e820:
	mov edi, 0x00004000		; location that memory map will be stored to
	xor ebx, ebx			; ebx must be 0 to start
	xor bp, bp			; keep an entry count in bp
	mov edx, 0x0534D4150		; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24			; ask for 24 bytes
	int 0x15
	jc nomemmap			; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150		; Some BIOSes apparently trash this register?
	cmp eax, edx			; on success, eax must have been reset to "SMAP"
	jne nomemmap
	test ebx, ebx			; ebx = 0 implies list is only 1 entry long (worthless)
	je nomemmap
	jmp jmpin
e820lp:
	mov eax, 0xe820			; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24			; ask for 24 bytes again
	int 0x15
	jc memmapend			; carry set means "end of list already reached"
	mov edx, 0x0534D4150		; repair potentially trashed register
jmpin:
	jcxz skipent			; skip any 0 length entries
	cmp cl, 20			; got a 24 byte ACPI 3.X response?
	jbe notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je skipent
notext:
	mov ecx, [es:di + 8]		; get lower dword of memory region length
	test ecx, ecx			; is the qword == 0?
	jne goodent
	mov ecx, [es:di + 12]		; get upper dword of memory region length
	jecxz skipent			; if length qword is 0, skip entry
goodent:
	inc bp				; got a good entry: ++count, move to next storage spot
	add di, 32
skipent:
	test ebx, ebx			; if ebx resets to 0, list is complete
	jne e820lp
nomemmap:
	mov byte [cfg_e820], 0		; No memory map function
memmapend:
	xor eax, eax			; Create a blank record for termination (32 bytes)
	mov ecx, 8
	rep stosd

; Enable the A20 gate
set_A20:
	in al, 0x64
	test al, 0x02
	jnz set_A20
	mov al, 0xD1
	out 0x64, al
check_A20:
	in al, 0x64
	test al, 0x02
	jnz check_A20
	mov al, 0xDF
	out 0x60, al

; Set up RTC
; Port 0x70 is RTC Address, and 0x71 is RTC Data
; http://www.nondot.org/sabre/os/files/MiscHW/RealtimeClockFAQ.txt
rtc_poll:
	mov al, 0x0A			; Status Register A
	out 0x70, al			; Select the address
	in al, 0x71			; Read the data
	test al, 0x80			; Is there an update in process?
	jne rtc_poll			; If so then keep polling
	mov al, 0x0A			; Status Register A
	out 0x70, al			; Select the address
	mov al, 00100110b		; UIP (0), RTC@32.768KHz (010), Rate@1024Hz (0110)
	out 0x71, al			; Write the data

	; Remap PIC IRQ's
	mov al, 00010001b		; begin PIC 1 initialization
	out 0x20, al
	mov al, 00010001b		; begin PIC 2 initialization
	out 0xA0, al
	mov al, 0x20			; IRQ 0-7: interrupts 20h-27h
	out 0x21, al
	mov al, 0x28			; IRQ 8-15: interrupts 28h-2Fh
	out 0xA1, al
	mov al, 4
	out 0x21, al
	mov al, 2
	out 0xA1, al
	mov al, 1
	out 0x21, al
	out 0xA1, al

	; Mask all PIC interrupts
	mov al, 0xFF
	out 0x21, al
	out 0xA1, al

	; Configure graphics if requested
	;cmp byte [cfg_vesa], 1		; Check if VESA should be enabled
	;jne VBEdone			; If not then skip VESA init

  
	; CX queries the mode, it should be in the form 0x41XX as bit 14 is set for LFB and bit 8 is set for VESA mode
	; 0x4112 is 640x480x24bit, 0x4129 should be 32bit
	; 0x4115 is 800x600x24bit, 0x412E should be 32bit
	; 0x4118 is 1024x768x24bit, 0x4138 should be 32bit
	                              ; 0x411B is 1280x1024x24bit, 0x413D should be 32bit


VBE1024: 
	mov edi, VBEModeInfoBlock	; VBE data will be stored at this address
	mov ax, 0x4F01			; GET SuperVGA MODE INFORMATION - http://www.ctyme.com/intr/rb-0274.htm
	mov cx, 0x4138			; Put your desired mode here
	mov bx, cx			; Mode is saved to BX for the set command later
	int 0x10        ; do it
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if command supported and successful
	jne VBE800
  jmp VBEsetmode

  
VBE800: 
	mov edi, VBEModeInfoBlock	; VBE data will be stored at this address
	mov ax, 0x4F01			; GET SuperVGA MODE INFORMATION - http://www.ctyme.com/intr/rb-0274.htm
	mov cx, 0x412e			; Put your desired mode here
	mov bx, cx			; Mode is saved to BX for the set command later
	int 0x10        ; do it
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if command supported and successful
	jne VBE640
  jmp VBEsetmode

  
VBE640: 
	mov edi, VBEModeInfoBlock	; VBE data will be stored at this address
	mov ax, 0x4F01			; GET SuperVGA MODE INFORMATION - http://www.ctyme.com/intr/rb-0274.htm
	mov cx, 0x4129			; Put your desired mode here
	mov bx, cx			; Mode is saved to BX for the set command later
	int 0x10        ; do it
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if command supported and successful
	jne VBE1024b
  jmp VBEsetmode

  
VBE1024b: 
	mov edi, VBEModeInfoBlock	; VBE data will be stored at this address
	mov ax, 0x4F01			; GET SuperVGA MODE INFORMATION - http://www.ctyme.com/intr/rb-0274.htm
	mov cx, 0x4118			; Put your desired mode here
	mov bx, cx			; Mode is saved to BX for the set command later
	int 0x10        ; do it
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if command supported and successful
	jne VBE800b
  jmp VBEsetmode

  
VBE800b: 
	mov edi, VBEModeInfoBlock	; VBE data will be stored at this address
	mov ax, 0x4F01			; GET SuperVGA MODE INFORMATION - http://www.ctyme.com/intr/rb-0274.htm
	mov cx, 0x4115			; Put your desired mode here
	mov bx, cx			; Mode is saved to BX for the set command later
	int 0x10        ; do it
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if command supported and successful
	jne VBE640b
  jmp VBEsetmode

  
VBE640b: 
	mov edi, VBEModeInfoBlock	; VBE data will be stored at this address
	mov ax, 0x4F01			; GET SuperVGA MODE INFORMATION - http://www.ctyme.com/intr/rb-0274.htm
	mov cx, 0x4112			; Put your desired mode here
	mov bx, cx			; Mode is saved to BX for the set command later
	int 0x10        ; do it
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if command supported and successful
	jne VBEfail
  jmp VBEsetmode


  
VBEsetmode:
  or bx, 0x4000			; Use linear/flat frame buffer model (set bit 14)
	mov ax, 0x4F02			; SET SuperVGA VIDEO MODE - http://www.ctyme.com/intr/rb-0275.htm
	int 0x10
	cmp ax, 0x004F			; Return value in AX should equal 0x004F if supported and successful
	jne VBEfail
	jmp VBEdone

VBEfail:
	mov si, msg_novesa
	call print_string_16
	mov byte [cfg_vesa], 0		; Clear the VESA config as it was not successful

VBEdone:

ret


; =============================================================================
; EOF
