global loader

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER

KERNEL_STACK_SIZE equ 4096  ; stack size in bytes

extern sum_of_three

section .text:
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .bss:
align 4                     ; align at 4 bytes
kernel_stack:               ; label points at beginning of memory
    resb KERNEL_STACK_SIZE  ; reserve stack for kernel

loader:
    mov eax, 0xCAFEBABE

    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to head of stack memory

    push dword 3        ; arg3
    push dword 2        ; arg2
    push dword 1        ; arg1
    call sum_of_three   ; call external c function


.loop:
    jmp .loop
