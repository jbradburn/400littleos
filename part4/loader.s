%include "io.s"
global loader

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER

msg db 'Pikachu', 0xa
length equ $ - msg   

KERNEL_STACK_SIZE equ 4096  ; stack size in bytes

extern fb_write_cell    ; define framebuffer write cell .c function
extern fb_write            ; external write function

section .text:
align 4
    dd MAGIC_NUMBER ; define MAGIC_NUMBER constant
    dd FLAGS        ; multiboot flags
    dd CHECKSUM     ; calculate checksum 
                    ; magic number + flags + checksum should be 0
    

section .bss:
align 4                     ; align at 4 bytes
kernel_stack:               ; label points at beginning of memory
    resb KERNEL_STACK_SIZE  ; reserve stack for kernel

loader:
    mov eax, 0xCAFEBABE

    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to head of stack memory

    push length
    push msg
    call fb_write

.loop:
    jmp .loop
