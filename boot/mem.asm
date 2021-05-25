; Code adapted from https://wiki.osdev.org/Detecting_Memory_(x86)
error_msg: db "Something went wrong fetching memory map.", 0
mmap_ent equ 0x700

_getMemoryMap:
    ;Do a first call to int 0x15
    mov di, 0x704
    xor ebx, ebx
    xor bp, bp
    mov edx, 0x0534D4150
    mov eax, 0xE820
    mov [es:di + 20], dword 1 ;Force a valid ACPI entry
    mov ecx, 24 ;Ask for 24 bytes
    int 0x15
    jc .failed ;The carry flag will be set if there's a failure
    mov edx, 0x0534D4150 ;Restore register, sometimes the BIOS will trash it
    cmp eax, edx ;on success, eax will have been reset to the magic 0x0534D4150 number
    jne .failed
    test ebx, ebx ;ebx = 0 implies the list is one entry long, which is useless
    je .failed
    jmp .jmpin
.e820lp:
    mov eax, 0xE820 ;Eax gets trashed every time
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc .e820f ;carry set means end of the list
    mov edx, 0x0534D4150
.jmpin:
    jcxz .skipent ;skip any zero entries
    cmp cl, 20 ;Got a 24 byte response? 
    jbe .notext
    test byte [es:di + 20], 1 ;If the "ignore this data" bit is set, ignore entry
    je .skipent
.notext:
    mov ecx, [es:di + 8] ;get lower uint32 of memory region length
    or ecx, [es:di + 12] ;or it with the upper uint32 to test for zero
    jz .skipent ;If it's 0, skip it
    inc bp ;Good entry, incrememnt the count and move to next storage spot
    add di, 24
.skipent:
    test ebx, ebx ;if ebx resets to 0, list is complete
    jne .e820lp
.e820f:
    mov [mmap_ent], bp
    ret
.failed:
    mov ax, error_msg ;Print out an error message.
    call _printString
    jmp $