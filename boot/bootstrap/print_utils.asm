;Expects the pointer to the string to be inside of ax
[bits 16]
print_string:
    push bx ;Push bx to the stack so it can be restored later
    mov bx, ax ;Move the contents of ax into bx
    mov ah, 0x0e ;Move the interrupt code into ah
_print_string_loop:
    mov al, [bx] ;Move current character at bx into al to print
    inc bx ;increment the counter
    cmp al, 0 ;Compare al to 0, as this string is null terminating
    je _print_string_exit ;jump if al is not 0
    int 0x10 ;Print the character
    jmp _print_string_loop
_print_string_exit:
    pop bx ;Restore bx
    ret
;Expects the number to be in ax
print_hex:
    pusha
    mov dx, ax ;Move the number into the rotator
    mov cx, 4 ;Set our counter to 4 (4 nibbles to a word)
    mov ah, 0xe ;Interrupt number
    mov bx, HEX_TABLE ;Character table
_print_hex_loop:
    rol dx, 4 ;Rotate left by 4 bits to move the last character into dl
    mov al, dl 
    and al, 0x0F ;Isolate the lowest nibble 
    xlat ;Index into our table stored in bx and move it into al
    int 0x10
    loop _print_hex_loop 
    popa
    ret
    
HEX_TABLE: db "0123456789ABCDEF"