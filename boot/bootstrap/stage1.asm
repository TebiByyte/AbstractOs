;Stage 1 goal: Load stage 2 from the first partition in the boot drive (must be present there, this will begin at LBA=35) and jump to it
[org 0x7C00]
bits 16
%include 'common_macros.asm'
;-----------------MAIN RUNTIME-----------------------------
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov [DRV_NUM_LOCATION], dl ;Save our drive number
mov ah, 0x41
mov bx, 0x55AA
int 0x13
jc extension_not_present
;If the extensions are present, read from LBA 2 for partition entries to find our boot code
mov ah, 0x42
mov si, DAP_STRUCT_LOCATION
mov [si], byte 0x10
mov [si + 0x2], word 0x1 ;Read a single entry of the GPT 
mov [si + 0x4], dword GPT_ENTRY_LOCATION
mov [si + 0x8], dword 0x2
mov [si + 0xC], dword 0x0
int 0x13
jc drive_error
;Look for the first partition entry and load that
mov ah, 0x42
mov si, GPT_ENTRY_LOCATION
mov ebx, dword [si + 0x20]
mov si, DAP_STRUCT_LOCATION
mov [si + 0x4], dword STAGE2_LOCATION
mov [si + 0x8], ebx
int 0x13
jc drive_error
;Loaded the first sector of the partition, check to make sure this is right by checking the header
mov si, STAGE2_LOCATION
mov cx, [si + 0x4]
cmp cx, STAGE2_SIGNATURE ;Check the header
jnz header_not_present
mov ecx, [si + 0x2] ;Get the number of blocks that need to be loaded
mov si, DAP_STRUCT_LOCATION
mov [si + 0x2], cx
mov ah, 0x42
int 0x13
jc drive_error
mov si, STAGE2_LOCATION
jmp [si] ;Jump into the entry point! 
;-----------------ERROR CONDITIONS--------------------------
extension_not_present:
    mov bx, 0x0000 ;Need to have an actual error code here
    jmp error

drive_error:
    xor bx, bx
    mov bl, ah
    jmp error

header_not_present:
    mov bx, 0x8000
    jmp error

error: 
    mov ax, ERROR_MSG
    call print_string
    mov ax, bx ;The error code should be in bx
    call print_hex
    jmp $ ;Loop forever

%include 'print_utils.asm'
ERROR_MSG: db "Error code ", 0
times 446 - ($ - $$) db 0
;Reserve some space for an MBR. This gets formated by our build process when formatting the disk for GPT
;Parition entry 1
times 16 db 0
;Parition entry 2
times 16 db 0
;Parition entry 3
times 16 db 0
;Parition entry 4
times 16 db 0
times 510 - ($ - $$) db 0
dw 0xaa55; boot signature