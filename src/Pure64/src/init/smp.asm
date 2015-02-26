; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; INIT SMP
; =============================================================================


init_smp:
	mov al, '5'			; Start of MP init
	mov [0x000B809C], al
	mov al, '0'
	mov [0x000B809E], al

; Check if we want the AP's to be enabled.. if not then skip to end
;	cmp byte [cfg_smpinit], 1	; Check if SMP should be enabled
;	jne noMP			; If not then skip SMP init

; Start the AP's one by one
	xor eax, eax
	xor edx, edx
	mov rsi, [os_LocalAPICAddress]
	add rsi, 0x20			; Add the offset for the APIC ID location
	lodsd				; APIC ID is stored in bits 31:24
	shr rax, 24			; AL now holds the BSP CPU's APIC ID
	mov dl, al			; Store BSP APIC ID in DL

	mov al, '8'			; Start the AP's
	mov [0x000B809E], al

	mov rsi, 0x0000000000005100
	xor eax, eax
	xor ecx, ecx
	mov cx, [cpu_detected]
smp_send_INIT:
	cmp cx, 0
	je smp_send_INIT_done
	lodsb

	cmp al, dl			; Is it the BSP?
	je smp_send_INIT_skipcore

	; Broadcast 'INIT' IPI to APIC ID in AL
	mov rdi, [os_LocalAPICAddress]
	shl eax, 24
	mov dword [rdi+0x310], eax	; Interrupt Command Register (ICR); bits 63-32
	mov eax, 0x00004500
	mov dword [rdi+0x300], eax	; Interrupt Command Register (ICR); bits 31-0
smp_send_INIT_verify:
	mov eax, [rdi+0x300]		; Interrupt Command Register (ICR); bits 31-0
	bt eax, 12			; Verify that the command completed
	jc smp_send_INIT_verify

smp_send_INIT_skipcore:
	dec cl
	jmp smp_send_INIT

smp_send_INIT_done:

	mov rax, [os_Counter_RTC]
	add rax, 10
wait1:
	mov rbx, [os_Counter_RTC]
	cmp rax, rbx
	jg wait1

	mov rsi, 0x0000000000005100
	xor ecx, ecx
	mov cx, [cpu_detected]
smp_send_SIPI:
	cmp cx, 0
	je smp_send_SIPI_done
	lodsb

	cmp al, dl			; Is it the BSP?
	je smp_send_SIPI_skipcore

	; Broadcast 'Startup' IPI to destination using vector 0x08 to specify entry-point is at the memory-address 0x00008000
	mov rdi, [os_LocalAPICAddress]
	shl eax, 24
	mov dword [rdi+0x310], eax	; Interrupt Command Register (ICR); bits 63-32
	mov eax, 0x00004608		; Vector 0x08
	mov dword [rdi+0x300], eax	; Interrupt Command Register (ICR); bits 31-0
smp_send_SIPI_verify:
	mov eax, [rdi+0x300]		; Interrupt Command Register (ICR); bits 31-0
	bt eax, 12			; Verify that the command completed
	jc smp_send_SIPI_verify

smp_send_SIPI_skipcore:
	dec cl
	jmp smp_send_SIPI

smp_send_SIPI_done:

	mov al, 'A'
	mov [0x000B809E], al

; Let things settle (Give the AP's some time to finish)
	mov rax, [os_Counter_RTC]
	add rax, 20
wait3:
	mov rbx, [os_Counter_RTC]
	cmp rax, rbx
	jg wait3

; Finish up
noMP:
	lock inc word [cpu_activated]	; BSP adds one here

	xor eax, eax
	mov rsi, [os_LocalAPICAddress]
	add rsi, 0x20			; Add the offset for the APIC ID location
	lodsd				; APIC ID is stored in bits 31:24
	shr rax, 24			; AL now holds the CPU's APIC ID (0 - 255)
	mov [os_BSP], eax		; Store the BSP APIC ID

	mov al, 'C'
	mov [0x000B809E], al

; Calculate speed of CPU (At this point the RTC is firing at 1024Hz)
	cpuid
	xor edx, edx
	xor eax, eax
	mov rcx, [os_Counter_RTC]
	add rcx, 10
	rdtsc
	push rax
speedtest:
	mov rbx, [os_Counter_RTC]
	cmp rbx, rcx
	jl speedtest
	rdtsc
	pop rdx
	sub rax, rdx
	xor edx, edx
	mov rcx, 10240
	div rcx
	mov [cpu_speed], ax

; Clear the periodic flag in the RTC
	mov al, 0x0B			; Status Register B
	out 0x70, al			; Select the address
	in al, 0x71			; Read the current settings
	push rax
	mov al, 0x0B			; Status Register B
	out 0x70, al			; Select the address
	pop rax
	btc ax, 6			; Set Periodic(6)
	out 0x71, al			; Write the new settings

	mov al, 'E'
	mov [0x000B809E], al

	cli				; Disable Interrupts

	ret


; =============================================================================
; EOF
