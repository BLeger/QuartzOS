extern _ZN16InterruptManager15handleInterruptEhj

section .data
	interrupt_number dd 0
	IRQ_BASE db 0x20

section .text

global _ZN16InterruptManager22ignoreInterruptRequestEv


%macro HandleException 1
	global _ZN16InterruptManager19handleException%+%1%+Ev
	_ZN16InterruptManager19handleException%+%1%+Ev:
	mov dword [interrupt_number], %1
	jmp int_bottom
%endmacro

%macro HandleInterruptRequest 1
	global _ZN16InterruptManager26handleInterruptRequest%+%1%+Ev
	_ZN16InterruptManager26handleInterruptRequest%+%1%+Ev:
	mov dword [interrupt_number], %1 + IRQ_BASE ; if bug <= inspect why i needed a dword, should mov the value of %1 + IRQ_BASE, not a 32 bit address
	jmp int_bottom
%endmacro

HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:
	pusha
	push ds
	push es
	push fs
	push gs

	push esp
	push dword [interrupt_number]
	call _ZN16InterruptManager15handleInterruptEhj

	; Override stack pointer with the return from handler
	mov esp, eax

	pop gs
	pop fs
	pop es
	pop ds
	popa

_ZN16InterruptManager22ignoreInterruptRequestEv:
	ret

