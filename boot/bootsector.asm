org 0x7C00
bits 16
;Steps of this boot sector are as follows:
;1.) Load additional code to find the main kernel within our file system
;2.) Enter long mode
;3.) Enter the kernel
;Note that I won't be setting up paging until later on

;Zero all the segment registers early 
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, ax

mov ax, hellomsg
call _printString
call _LoadExtendedBoot
call _LoadChainLoader
call _EnterLongMode
jmp $ ;Safety hang

_LoadExtendedBoot:
    mov dl, [BOOTDRIVE]
    mov al, 2 ;Load in 2 sectors, so that we can enter long mode from outside our bootsector
    mov ch, 0x0 ;Select cylinder 0
    mov dh, 0x0 ;Select head 0
    mov cl, 0x02 ;Start reading from second sector (i.e. after the boot sector)
    xor bx, bx
    mov es, bx
    mov bx, 0x7E00
    call diskload
    ret

_LoadChainLoader:
    mov dl, [BOOTDRIVE]
    mov al, 32
    mov ch, 0x0
    mov dh, 0x0
    mov cl, 0x03
    xor bx, bx
    mov es, bx
    mov bx, KERNEL_OFFSET
    call diskload
    ret

;Include essential asm files
%include 'boot/print.asm' 
%include 'boot/gdt.asm'
%include 'boot/diskload.asm'

BOOTDRIVE: db 0x80
hellomsg: db "Loaded boot sector, loading additional code", 0xD, 0xA, 0
testS: db "Read succesfully!", 0
testF: db "Read failed!", 0
KERNEL_OFFSET equ 0x7F00
times 0x1BE - ($ - $$) db 0; Pad out to 446 bytes so we can include a partition table
;Begin the partition table
;Parition entry 1
db 0x80; bootable
times 15 db 0; We don't know where we want the partition to start, or any other data about it yet, so leave this blank for now.
;The rest of these entries should just be zeros (invalid entries)
;Parition entry 2
times 16 db 0
;Parition entry 3
times 16 db 0
;Parition entry 4
times 16 db 0
dw 0xaa55; boot signature

bits 16

_EnterLongMode:
    ;Set up the paging (This won't be used for now)
    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, 0x1000

    ;PML4T -> 0x1000
    ;PDPT  -> 0x2000
    ;PDT   -> 0x3000
    ;PT    -> 0x4000

    mov DWORD [edi], 0x2003
    add edi, 0x1000
    mov DWORD [edi], 0x3003
    add edi, 0x1000
    mov DWORD [edi], 0x4003
    add edi, 0x1000
    mov ebx, 0x00000003 
    mov ecx, 512

.SetEntry:
    mov DWORD [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .SetEntry
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ;Now switch to compatibility mode
    
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    mov eax, cr0
    or eax, 1 << 31 | 1 << 0
    mov cr0, eax 
    ;Now switch into the sub mode

    lgdt [GDT64.Pointer]
    jmp GDT64.Code:Realm64

bits 64

Realm64:
    cli
    mov ax, GDT64.Data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov edi, 0xB8000              ; Set the destination index to 0xB8000.
    mov rax, 0x1F201F201F201F20   ; Set the A-register to 0x1F201F201F201F20.
    mov ecx, 500                  ; Set the C-register to 500.
    rep stosq  
    mov ebp, 0x90000
    mov esp, ebp
    mov rax, KERNEL_OFFSET
    jmp rax
    jmp $

times 0x400 - ($ - $$) db 0 ;Pad this out to fill up the sector on disk