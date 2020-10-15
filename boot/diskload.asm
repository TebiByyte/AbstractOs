;Load AL sectors to ES:BX from drive DL
diskload:
    push ax ;Push the number of sectors to read onto the stack to be compared later
            ;In case of an error
    mov ah, 0x02
    int 0x13 ;BIOS interrupt

    jc diskError ;Jump if error (carry flag is set)

    pop dx ;Restore ax into the dx register
    cmp dl, al ;if al (sectors read) != dh (sectors expected)
    jne diskError
    ret

diskError:
    mov dl, dh
    mov dh, 0
    mov cl, ah
    mov ax, DISKERRORMSG
    call _printString
    call _printHex
    mov ax, DISKERRORMSG2
    call _printString
    mov ax, DISKERRORMSG3
    call _printString
    mov dx, cx
    call _printHex
    jmp $

DISKERRORMSG db "Disk read error! Loaded ", 0
DISKERRORMSG2 db " Sectors.", 0
DISKERRORMSG3 db " Error code: ", 0