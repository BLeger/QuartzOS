extern _ZN16InterruptManager15handleInterruptEhj

section .data
	interrupt_number db 0

section .text

global _ZN16InterruptManager22ignoreInterruptRequestEv

%define INT_OFFSET 0x20

%macro HandleException 1
	global _ZN16InterruptManager19handleException%+%1%+Ev
	_ZN16InterruptManager19handleException%+%1%+Ev:
	mov byte [interrupt_number], %1
	jmp int_bottom
%endmacro

%macro HandleInterruptRequest 1
	global _ZN16InterruptManager26handleInterruptRequest%+%1%+Ev
	_ZN16InterruptManager26handleInterruptRequest%+%1%+Ev:
	mov byte [interrupt_number], %1 + INT_OFFSET
	jmp int_bottom
%endmacro


HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:	
	;cli
	;pusha
	push ebp
	push edi
	push esi

	push edx
	push ecx
	push ebx
	push eax

	;push ds
	;push es
	;push fs
	;push gs

	push esp
	push dword [interrupt_number]
	call _ZN16InterruptManager15handleInterruptEhj

	; Override stack pointer with the return from handler
	mov esp, eax

	pop eax
	pop ebx
	pop ecx
	pop edx
	
	pop esi
	pop edi
	pop ebp

	;pop gs
	;pop fs
	;pop es
	;pop ds
	;popa
	sti

_ZN16InterruptManager22ignoreInterruptRequestEv:
	ret

