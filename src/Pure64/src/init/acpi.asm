; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; INIT ACPI
; =============================================================================


init_acpi:
	mov rsi, 0x00000000000E0000	; Start looking for the Root System Description Pointer Structure
	mov rbx, 'RSD PTR '		; This in the Signature for the ACPI Structure Table (0x2052545020445352)
searchingforACPI:
	lodsq				; Load a quad word from RSI and store in RAX, then increment RSI by 8
	cmp rax, rbx
	je foundACPI
	cmp rsi, 0x00000000000FFFFF	; Keep looking until we get here
	jge noACPI			; ACPI tables couldn't be found, Fail.
	jmp searchingforACPI

foundACPI:				; Found a Pointer Structure, verify the checksum
	push rsi
	xor ebx, ebx
	mov ecx, 20			; As per the spec only the first 20 bytes matter
	sub rsi, 8			; Bytes 0 thru 19 must sum to zero
nextchecksum:
	lodsb				; Get a byte
	add bl, al			; Add it to the running total
	sub cl, 1
	cmp cl, 0
	jne nextchecksum
	pop rsi
	cmp bl, 0
	jne searchingforACPI		; Checksum didn't check out? Then keep looking.

	lodsb				; Checksum
	lodsd				; OEMID (First 4 bytes)
	lodsw				; OEMID (Last 2 bytes)
	lodsb				; Grab the Revision value (0 is v1.0, 1 is v2.0, 2 is v3.0, etc)
	add al, 49
	mov [0x000B8098], al		; Print the ACPI version number
	sub al, 49
	cmp al, 0
	je foundACPIv1			; If AL is 0 then the system is using ACPI v1.0
	jmp foundACPIv2			; Otherwise it is v2.0 or higher

foundACPIv1:
	xor eax, eax
	lodsd				; Grab the 32 bit physical address of the RSDT (Offset 16).
	mov rsi, rax			; RSI now points to the RSDT
	lodsd				; Grab the Signiture
	cmp eax, 'RSDT'			; Make sure the signiture is valid
	jne novalidacpi			; Not the same? Bail out
	sub rsi, 4
	mov [os_ACPITableAddress], rsi	; Save the RSDT Table Address
	add rsi, 4
	xor eax, eax
	lodsd				; Length
	add rsi, 28			; Skip to the Entry offset
	sub eax, 36			; EAX holds the table size. Subtract the preamble
	shr eax, 2			; Divide by 4
	mov rdx, rax			; RDX is the entry count
	xor ecx, ecx
foundACPIv1_nextentry:
	lodsd
	push rax
	add ecx, 1
	cmp ecx, edx
	je findACPITables
	jmp foundACPIv1_nextentry

foundACPIv2:
	lodsd				; RSDT Address
	lodsd				; Length
	lodsq				; Grab the 64 bit physical address of the XSDT (Offset 24).
	mov rsi, rax			; RSI now points to the XSDT
	lodsd				; Grab the Signiture
	cmp eax, 'XSDT'			; Make sure the signiture is valid
	jne novalidacpi			; Not the same? Bail out
	sub rsi, 4
	mov [os_ACPITableAddress], rsi	; Save the XSDT Table Address
	add rsi, 4
	xor eax, eax
	lodsd				; Length
	add rsi, 28			; Skip to the start of the Entries (offset 36)
	sub eax, 36			; EAX holds the table size. Subtract the preamble
	shr eax, 3			; Divide by 8
	mov rdx, rax			; RDX is the entry count
	xor ecx, ecx
foundACPIv2_nextentry:
	lodsq
	push rax
	add ecx, 1
	cmp ecx, edx
	jne foundACPIv2_nextentry

findACPITables:
	mov al, '3'			; Search through the ACPI tables
	mov [0x000B809C], al
	mov al, '4'
	mov [0x000B809E], al
	xor ecx, ecx
nextACPITable:
	pop rsi
	lodsd
	add ecx, 1
	mov ebx, 'APIC'			; Signature for the Multiple APIC Description Table
	cmp eax, ebx
	je foundAPICTable
	mov ebx, 'HPET'			; Signiture for the HPET Description Table
	cmp eax, ebx
	je foundHPETTable
	cmp ecx, edx
	jne nextACPITable
	jmp init_smp_acpi_done		;noACPIAPIC

foundAPICTable:
	call parseAPICTable
	jmp nextACPITable

foundHPETTable:
	call parseHPETTable
	jmp nextACPITable

init_smp_acpi_done:
	ret

noACPI:
novalidacpi:
	mov al, 'X'
	mov [0x000B809A], al
	jmp $


; -----------------------------------------------------------------------------
parseAPICTable:
	push rcx
	push rdx

	lodsd				; Length of MADT in bytes
	mov ecx, eax			; Store the length in ECX
	xor ebx, ebx			; EBX is the counter
	lodsb				; Revision
	lodsb				; Checksum
	lodsd				; OEMID (First 4 bytes)
	lodsw				; OEMID (Last 2 bytes)
	lodsq				; OEM Table ID
	lodsd				; OEM Revision
	lodsd				; Creator ID
	lodsd				; Creator Revision
	xor eax, eax
	lodsd				; Local APIC Address
	mov [os_LocalAPICAddress], rax	; Save the Address of the Local APIC
	lodsd				; Flags
	add ebx, 44
	mov rdi, 0x0000000000005100	; Valid CPU IDs

readAPICstructures:
	cmp ebx, ecx
	jge parseAPICTable_done
;	call os_print_newline
	lodsb				; APIC Structure Type
;	call os_debug_dump_al
;	push rax
;	mov al, ' '
;	call os_print_char
;	pop rax
	cmp al, 0x00			; Processor Local APIC
	je APICapic
	cmp al, 0x01			; I/O APIC
	je APICioapic
	cmp al, 0x02			; Interrupt Source Override
	je APICinterruptsourceoverride
;	cmp al, 0x03			; Non-maskable Interrupt Source (NMI)
;	je APICnmi
;	cmp al, 0x04			; Local APIC NMI
;	je APIClocalapicnmi
;	cmp al, 0x05			; Local APIC Address Override
;	je APICaddressoverride
	cmp al, 0x09			; Processor Local x2APIC
	je APICx2apic
;	cmp al, 0x0A			; Local x2APIC NMI
;	je APICx2nmi

	jmp APICignore

APICapic:
	xor eax, eax
	xor edx, edx
	lodsb				; Length (will be set to 8)
	add ebx, eax
	lodsb				; ACPI Processor ID
	lodsb				; APIC ID
	xchg eax, edx			; Save the APIC ID to EDX
	lodsd				; Flags (Bit 0 set if enabled/usable)
	bt eax, 0			; Test to see if usable
	jnc readAPICstructures		; Read the next structure if CPU not usable
	inc word [cpu_detected]
	xchg eax, edx			; Restore the APIC ID back to EAX
	stosb
	jmp readAPICstructures		; Read the next structure

APICioapic:
	xor eax, eax
	lodsb				; Length (will be set to 12)
	add ebx, eax
	lodsb				; IO APIC ID
	lodsb				; Reserved
	xor eax, eax
	lodsd				; IO APIC Address
	push rdi
	push rcx
	mov rdi, os_IOAPICAddress
	xor ecx, ecx
	mov cl, [os_IOAPICCount]
	shl cx, 3			; Quick multiply by 8
	add rdi, rcx
	pop rcx
	stosd				; Store the IO APIC Address
	lodsd				; System Vector Base
	stosd				; Store the IO APIC Vector Base
	pop rdi
	inc byte [os_IOAPICCount]
	jmp readAPICstructures		; Read the next structure

APICinterruptsourceoverride:
	xor eax, eax
	lodsb				; Length (will be set to 10)
	add ebx, eax
	lodsb				; Bus
	lodsb				; Source
;	call os_print_newline
;	call os_debug_dump_al
;	mov al, ' '
;	call os_print_char
	lodsd				; Global System Interrupt
;	call os_debug_dump_eax
	lodsw				; Flags
	jmp readAPICstructures		; Read the next structure

APICx2apic:
	xor eax, eax
	xor edx, edx
	lodsb				; Length (will be set to 16)
	add ebx, eax
	lodsw				; Reserved; Must be Zero
	lodsd
	xchg eax, edx			; Save the x2APIC ID to EDX
	lodsd				; Flags (Bit 0 set if enabled/usable)
	bt eax, 0			; Test to see if usable
	jnc APICx2apicEnd		; Read the next structure if CPU not usable
	xchg eax, edx			; Restore the x2APIC ID back to EAX
	call os_debug_dump_eax
	call os_print_newline
	; Save the ID's somewhere
APICx2apicEnd:
	lodsd				; ACPI Processor UID
	jmp readAPICstructures		; Read the next structure

APICignore:
	xor eax, eax
	lodsb				; We have a type that we ignore, read the next byte
	add ebx, eax
	add rsi, rax
	sub rsi, 2			; For the two bytes just read
	jmp readAPICstructures		; Read the next structure

parseAPICTable_done:
	pop rdx
	pop rcx
	ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
parseHPETTable:
	lodsd				; Length of HPET in bytes
	lodsb				; Revision
	lodsb				; Checksum
	lodsd				; OEMID (First 4 bytes)
	lodsw				; OEMID (Last 2 bytes)
	lodsq				; OEM Table ID
	lodsd				; OEM Revision
	lodsd				; Creator ID
	lodsd				; Creator Revision
	lodsd				; Event Timer Block ID
	lodsd				; Base Address Settings
	lodsq				; Base Address Value
	mov [os_HPETAddress], rax	; Save the Address of the HPET
	lodsb				; HPET Number
	lodsw				; Main Counter Minimum
	lodsw				; Page Protection And OEM Attribute
	ret
; -----------------------------------------------------------------------------


; =============================================================================
; EOF
