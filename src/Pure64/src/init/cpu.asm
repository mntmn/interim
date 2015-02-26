; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; INIT CPU
; =============================================================================


init_cpu:

; Disable Cache
	mov rax, cr0
	btr rax, 29			; Clear No Write Thru (Bit 29)
	bts rax, 30			; Set Cache Disable (Bit 30)
	mov cr0, rax

; Flush Cache
	wbinvd

; Diable Paging Global Extensions
	mov rax, cr4
	btr rax, 7			; Clear Paging Global Extensions (Bit 7)
	mov cr4, rax
	mov rax, cr3
	mov cr3, rax

; Disable MTRRs and Configure default memory type to UC
	mov ecx, 0x000002FF
	rdmsr
	and eax, 0xFFFFF300		; Clear MTRR Enable (Bit 11), Fixed Range MTRR Enable (Bit 10), and Default Memory Type (Bits 7:0) to UC (0x00)
	wrmsr

; Setup variable-size address ranges
; Cache 0-64 MiB as type 6 (WB) cache
; See example in Intel Volume 3A. Example Base and Mask Calculations
;	mov ecx, 0x00000200		; MTRR_Phys_Base_MSR(0)
;	mov edx, 0x00000000		; Base is EDX:EAX, 0x0000000000000006
;	mov eax, 0x00000006		; Type 6 (write-back cache)
;	wrmsr
;	mov ecx, 0x00000201		; MTRR_Phys_Mask_MSR(0)
;;	mov edx, 0x00000000		; Mask is EDX:EAX, 0x0000000001000800 (Because bochs sucks)
;;	mov eax, 0x01000800		; Bit 11 set for Valid
;	mov edx, 0x0000000F		; Mask is EDX:EAX, 0x0000000F80000800 (2 GiB)
;	mov eax, 0x80000800		; Bit 11 set for Valid
;	wrmsr


; vmem at 0xfd000000

;  mov ecx, 0x00000200           
;  mov edx, 0x00000000
;  mov eax, 0xfd000000
;  mov eax, 0x00000001 ; write-combine, http://users.sosdg.org/~qiyong/mxr/source/sys/arch/x86/include/mtrr.h#L62
;  wrmsr

;  mov ecx, 0x00000201		; MTRR_Phys_Mask_MSR(0)
; 	mov edx, 0x00000000		; Mask is EDX:EAX, 0x000000002D000800  --> up to 0xfd2ffffff
; 	mov eax, 0xd3000800		; Bit 11 set for Valid
;  wrmsr


; vmem at 0xe0000000

;  mov ecx, 0x00000200
;  mov edx, 0x00000000
;  mov eax, 0xe0000000
;  mov eax, 0x00000001 ; write-combine, http://users.sosdg.org/~qiyong/mxr/source/sys/arch/x86/include/mtrr.h#L62
;  wrmsr

;  mov ecx, 0x00000201		; MTRR_Phys_Mask_MSR(0)
; 	mov edx, 0x0000000f		; Mask is EDX:EAX, 0x000000002D000800  --> (up to 0xfd2ffffff)  --> 0xf1fd00000
; 	mov eax, 0x1fd00800		; Bit 11 set for Valid
;  wrmsr
  

; MTRR notes:
; Base 0x0000000000000000 = 0 MiB
; Base 0x0000000080000000 = 2048 MiB, 2048 is 0x800
; Base 0x0000000100000000 = 4096 MiB, 4096 is 0x1000
; Mask 0x0000000F80000000 = 2048 MiB, 0xFFFFFFFFF - F80000000 = 7FFFFFFF = 2147483647 (~2 GiB)
; Mask 0x0000000FC0000000 = 1024 MiB, 0xFFFFFFFFF - FC0000000 = 3FFFFFFF = 1073741823 (~1 GiB)
; Mask 0x0000000FFC000000 = 64 MiB,   0xFFFFFFFFF - FFC000000 =  3FFFFFF =   67108863 (~64 MiB)

; Enable MTRRs
	mov ecx, 0x000002FF
	rdmsr
	bts eax, 11			; Set MTRR Enable (Bit 11), Only enables Variable Range MTRR's
	wrmsr

; Flush Cache
	wbinvd

; Enable Cache
	mov rax, cr0
	btr rax, 29			; Clear No Write Thru (Bit 29)
	btr rax, 30			; Clear CD (Bit 30)
	mov cr0, rax

; Enable Paging Global Extensions
;	mov rax, cr4
;	bts rax, 7			; Set Paging Global Extensions (Bit 7)
;	mov cr4, rax

; Enable Floating Point
	mov rax, cr0
	bts rax, 1			; Set Monitor co-processor (Bit 1)
	btr rax, 2			; Clear Emulation (Bit 2)
	mov cr0, rax

; Enable SSE
	mov rax, cr4
	bts rax, 9			; Set Operating System Support for FXSAVE and FXSTOR instructions (Bit 9)
	bts rax, 10			; Set Operating System Support for Unmasked SIMD Floating-Point Exceptions (Bit 10)
	mov cr4, rax

; Enable Math Co-processor
	finit

; Enable and Configure Local APIC
	mov rsi, [os_LocalAPICAddress]
	cmp rsi, 0x00000000
	je noMP				; Skip MP init if we didn't get a valid LAPIC address

	xor eax, eax			; Clear Task Priority (bits 7:4) and Priority Sub-Class (bits 3:0)
	mov dword [rsi+0x80], eax	; Task Priority Register (TPR)

	mov eax, 0x01000000		; Set bits 31-24 for all cores to be in Group 1
	mov dword [rsi+0xD0], eax	; Logical Destination Register

	xor eax, eax
	sub eax, 1			; Set EAX to 0xFFFFFFFF; Bits 31-28 set for Flat Mode
	mov dword [rsi+0xE0], eax	; Destination Format Register

	mov eax, dword [rsi+0xF0]	; Spurious Interrupt Vector Register
	mov al, 0xF8
	bts eax, 8			; Enable APIC (Set bit 8)
	mov dword [rsi+0xF0], eax

	mov eax, dword [rsi+0x320]	; LVT Timer Register
	bts eax, 16			; Set bit 16 for mask interrupts
	mov dword [rsi+0x320], eax

;	mov eax, dword [rsi+0x350]	; LVT LINT0 Register
;	mov al, 0			;Set interrupt vector (bits 7:0)
;	bts eax, 8			;Delivery Mode (111b==ExtlNT] (bits 10:8)
;	bts eax, 9
;	bts eax, 10
;	bts eax, 15			;bit15:Set trigger mode to Level (0== Edge, 1== Level)
;	btr eax, 16			;bit16:unmask interrupts (0==Unmasked, 1== Masked)
;	mov dword [rsi+0x350], eax

;	mov eax, dword [rsi+0x360]	; LVT LINT1 Register
;	mov al, 0			;Set interrupt vector (bits 7:0)
;	bts eax, 8			;Delivery Mode (111b==ExtlNT] (bits 10:8)
;	bts eax, 9
;	bts eax, 10
;	bts eax, 15			;bit15:Set trigger mode to Edge (0== Edge, 1== Level)
;	btr eax, 16			;bit16:unmask interrupts (0==Unmasked, 1== Masked)
;	mov dword [rsi+0x360], eax

;	mov eax, dword [rsi+0x370]	; LVT Error Register
;	mov al, 0			;Set interrupt vector (bits 7:0)
;	bts eax, 16			;bit16:Mask interrupts (0==Unmasked, 1== Masked)
;	mov dword [rsi+0x370], eax


ret


; =============================================================================
; EOF
