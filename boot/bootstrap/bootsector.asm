[org 0x7C00]
[bits 16]
;Steps of this boot sector are as follows:
;1.) Load additional code to find the main kernel within our file system
;2.) Enter long mode
;3.) Enter the kernel

;NOTE FOR NEXT TIME I WORK ON THIS!!!
;This is going to have to change drastically, it's moving to a multistage boot process to facilitate a GPT partition table
;Stage 1: this code is reduced to having a hard 512 byte limit. In this stage, we load in code from the very first partition. The first parition will ALWAYS contain boot code, it should be marked as so to not disturb it
;Stage 2: After being loaded into 0x7F00, this stage sets up SSE, Long Mode, etc etc. This also loads in C code needed to read the file system and load a kernel

;Zero all the segment registers early 
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, ax

mov ax, hellomsg
call _printString
call _LoadExtendedBoot
call _getMemoryMap
call _Enable_SSE
call _LoadChainLoader
call _EnterLongMode
jmp $ ;Safety hang

_LoadExtendedBoot:
    mov [BOOTDRIVE], dl
    mov al, 1 ;Load in 2 sectors, so that we can enter long mode from outside our bootsector
    mov ch, 0x0 ;Select cylinder 0
    mov dh, 0x0 ;Select head 0
    mov cl, 0x02 ;Start reading from second sector (i.e. after the boot sector)
    xor bx, bx
    mov es, bx
    mov bx, 0x7E00
    call diskload
    ret
;This call can possibly be optimized to one call to the drive, saving some space in the boot sector
;Update this to use a different bios interrupt mode. INT=13h AH=42h
;This should fix the issue with drive geometry being a factor
_LoadChainLoader:
    mov dl, [BOOTDRIVE]
    mov al, 60
    mov ch, 0x0
    mov dh, 0x0
    mov cl, 0x03
    xor bx, bx
    mov es, bx
    mov bx, CHAINLOADER_OFFSET
    call diskload
    ret

;Include essential asm files
%include 'bootstrap/print.asm' 
%include 'bootstrap/gdt.asm'
%include 'bootstrap/diskload.asm'
BOOTDRIVE: db 0x80
hellomsg: db "Loaded successfully, loading addtional code from disk", 0xD, 0xA, 0
debugmsg: db "Loaded additional code and mapped memory", 0xD, 0xA, 0
CHAINLOADER_OFFSET equ 0x8000
times 446 - ($ - $$) db 0
;Begin the partition table
;Parition entry 1
times 16 db 0; We don't know where we want the partition to start, or any other data about it yet, so leave this blank for now.
;The rest of these entries should just be zeros (invalid entries)
;Parition entry 2
times 16 db 0
;Parition entry 3
times 16 db 0
;Parition entry 4
times 16 db 0
times 510 - ($ - $$) db 0
dw 0xaa55; boot signature

bits 16

_Enable_SSE:
    mov eax, cr0
    and ax, 0xFFFB
    or ax, 0x2
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9
    mov cr4, eax
    ret

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
    or eax, 1 << 31 | 1 << 0 ; 32 bit paging being used here, I may eventually want to change this to PAE mode
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
    mov rax, CHAINLOADER_OFFSET
    jmp rax
    jmp $


bits 16
;This is currently really weird that I have to include this file here, but defining any bytes above this causes strange issues. 
;I think it's because it can't jump into the chain loader properly if the code gets offset by a certain amount? 
;TODO: investigate this 
%include 'bootstrap/mem.asm'

times 0x400 - ($ - $$) db 0 ;Pad this out to fill up the sector on disk
