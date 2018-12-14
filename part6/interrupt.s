global isr0
extern interrupt_handler

isr0:
  cli                 ; disable interrupts
  push byte 0         ; push a dummy error code
  push byte 0         ; push the interrupt number (0)
  jmp isr_common_handler ; go to our common handler

%macro isr_no_error_code 1      ; define macro
  global isr%1                  ; %1 -> first param.
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_handler
%endmacro

%macro isr_error_code 1
  global isr%1
  isr%1:
    cli
    push byte %1
    jmp isr_common_handler
%endmacro

extern interrupt_handler
isr_common_handler:
    pusha

    mov ax, ds      ;push ds to low bits of ax
    push eax

    mov ax, 0x10    ; data segment descriptor
    mov ds, ax      
    mov es, ax
    mov fs, ax 
    mov gs, ax

    call interrupt_handler

    pop eax

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret

isr_no_error_code 1
isr_no_error_code 2

    