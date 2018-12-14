global outb ;   expose outb to other files
global inb  ;    

; outb - send a byte to an I/O port
; stack:    [esp + 8] data byte 
;           [esp + 4] I/O port
;           [esp    ] return address
outb:
    mov al, [esp + 8]   ; move data to be sent into al
    mov dx, [esp + 4]   ; move I/O port address into dx
    out dx, al          ; send data to I/O port
    ret                 ; return to calling fct

; inb - returns a byte from the given I/O port
; stack: [esp + 4] The address of the I/O port
;        [esp    ] The return address
inb:
    mov dx, [esp + 4]       ; move the address of the I/O port to the dx register
    in  al, dx              ; read a byte from the I/O port and store it in the al register
    ret                     ; return the read byte
    
