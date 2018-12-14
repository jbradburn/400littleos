global gdt_flush     ; Allows the C code to call gdt_flush().
global idt_flush     ; Allows the C code to call idt_flush().
global memset

gdt_flush:
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer

   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: far jump
.flush:
   ret

idt_flush:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
   lidt [eax]        ; Load the IDT pointer.
   ret

memset:
    push ebp
    mov ebp, esp
    add ebp, 4 
    push ebx
    push ecx

    mov eax, DWORD [esp + 12]     ; size
    mov ecx, DWORD [esp + 8]      ; val
    mov ebx, DWORD [esp + 4]      ; destination

  .memset:
    or eax, eax ; Fast compare to 0
    jz .memset_end
    mov [ebx], BYTE cl
    dec eax
    inc ebx
    jmp .memset
  .memset_end:
    mov eax, DWORD [ebp + 4]          
    pop ecx 
    pop ebx
    pop ebp
    ret
    