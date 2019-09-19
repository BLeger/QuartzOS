global loader
extern kmain
extern callConstructors

; Kernel signature to be recognised by GRUB
MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                ; (magic number + checksum + flags should equal 0)
KERNEL_STACK equ 4096

section .bss
align 4
kernel_stack:
    mov esp, kernel_stack + KERNEL_STACK  ; Move the stack pointer

section .text:                  ; start of the text (code) section
align 4                         ; the code must be 4 byte aligned
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd FLAGS                    ; the flags,
    dd CHECKSUM                 ; and the checksum

loader:
    call callConstructors
    
    push eax
    push ebx
    call kmain

    ; Should not ever return from kmain
stop:
    cli
    hlt
    jmp stop

