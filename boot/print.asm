bits 16

;Expects the pointer to the string to be inside of ax
_printString:
    push bx ;Push bx to the stack so it can be restored later
    mov bx, ax ;Move the contents of ax into bx
    mov ah, 0x0e ;Move the interrupt code into ah
_printStringLoop:
    mov al, [bx] ;Move current character at bx into al to print
    inc bx ;increment the counter
    cmp al, 0 ;Compare al to 0, as this string is null terminating
    je _printStringExit ;jump if al is not 0
    int 0x10 ;Print the character
    jmp _printStringLoop
_printStringExit:
    pop bx ;Restore bx
    ret

;Expects the number to be printed to be in ax
;Only prints positive integers
;This code is taken from https://www.youtube.com/watch?v=WNCW39LmC_A&t=1489s&ab_channel=QuesoFuego
_printHex:
    pusha
    mov cx, 0
_printHexLoop:
    cmp cx, 4
    je _printHexEnd

    mov ax, dx
    and ax, 0x000F
    add al, 0x30
    cmp al, 0x39
    jle _moveIntoBX
    add al, 0x7

_moveIntoBX:
    mov bx, hexString + 5
    sub bx, cx
    mov [bx], al
    ror dx, 4
    add cx, 1
    jmp _printHexLoop

_printHexEnd:
    mov ax, hexString
    call _printString
    popa
    ret

hexString: db '0x0000', 0