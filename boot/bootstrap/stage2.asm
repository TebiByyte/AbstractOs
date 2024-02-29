;Most of the code here is either taken completely or adapted from the OSdev Wiki
%include 'common_macros.asm'
[org STAGE2_LOCATION]
bits 16
start:
stage2_info_header:
dw entry
size: dw (end - start) / 512
dw STAGE2_SIGNATURE
entry:
; To reference the size of this file, you have to do (end - start)
; Start off by loading stage3 at the appropriate location
; query the memory map and store it somewhere useful
; next check and enable A20 if needed
; TODO: enable VESA BIOS Extensions and get a frame buffer
; create the paging tables to identity map the first MB
; enter long mode

xor eax, eax
xor dx, dx
mov dl, [DRV_NUM_LOCATION]
mov si, GPT_ENTRY_LOCATION
mov ax, [si + 0x20] ;Move the starting LBA into eax
add ax, [size] ;Add the number of sectors this stage takes up to the starting LBA
mov bx, [si + 0x28] 
sub bx, ax ;Subtract the end from the start to get the number of sectors to read into memory
mov si, DAP_STRUCT_LOCATION
mov [si], byte 0x10
mov [si + 0x2], bx
mov [si + 0x4], dword STAGE3_LOCATION
mov [si + 0x8], dword eax 
mov [si + 0xC], dword 0x0
mov ah, 0x42
int 0x13
jc drive_error
;Loaded stage 3, now get memory map
call get_mem_map
;Next check A20 and enable
call check_a20
cmp ax, 1
je a20_enabled
; A20 is disabled
mov ax, a20_disabled_message
call print_string
; Try the BIOS interrupt
mov ax,2403h                ;--- A20-Gate Support ---
int 15h
jb bios_failed                  ;INT 15h is not supported
cmp ah,0
jnz bios_failed                  ;INT 15h is not supported
mov ax,2402h                ;--- A20-Gate Status ---
int 15h
jb bios_failed              ;couldn't get status
cmp ah,0
jnz bios_failed              ;couldn't get status
cmp al,1
jz a20_enabled           ;A20 is already activated
mov ax,2401h                ;--- A20-Gate Activate ---
int 15h
jb bios_failed              ;couldn't activate the gate
cmp ah,0
jnz bios_failed              ;couldn't activate the gate

bios_failed:
mov ax, a20_gate_not_supported_msg
call print_string
cli 

call    a20_wait_for_data_read
mov     al,0xAD ; Disable the keyboard
out     0x64,al

call    a20_wait_for_data_read
mov     al,0xD0 ; Read the controller output port
out     0x64,al

call    a20_wait_for_data_ready
in      al,0x60 
push    eax

call    a20_wait_for_data_read
mov     al,0xD1
out     0x64,al

call    a20_wait_for_data_read
pop     eax
or      al,2
out     0x60,al

call    a20_wait_for_data_read
mov     al,0xAE
out     0x64,al

call    a20_wait_for_data_read

call check_a20
cmp ax, 1
je a20_enabled
mov ax, keyboard_controller_failed
call print_string
; TODO add a more complete implementation for turning on A20. Need to find a way to test all methods
jmp $

a20_enabled:
mov ax, 0
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
cli
mov ax, a20_enabled_message
call print_string
;Now that A20 is enabled, identity map the first MiB of RAM with
;Clear the memory where the page tables will be
mov edi, 0x1000
mov cr3, edi
xor eax, eax
mov ecx, 4096
rep stosd 
mov edi, 0x1000

mov dword [edi], 0x2003
add edi, 0x1000
mov dword [edi], 0x3003
add edi, 0x1000
mov dword [edi], 0x4003
add edi, 0x1000
; TODO make the above code more configurable. Look into page tables and create a macro function for that
mov ebx, 0x00000003
mov ecx, 512
set_pt_entry:
mov dword [edi], ebx
add ebx, 0x1000
add edi, 8
loop set_pt_entry
mov eax, cr4
or eax, 1 << 5
mov cr4, eax

mov ecx, 0xC0000080
rdmsr
or eax, 1 << 8
wrmsr

mov eax, cr0
or eax, 1 << 31 | 1 << 0
mov cr0, eax

lgdt [GDT64.Pointer]
jmp GDT64.Code:long_mode

bits 64

long_mode:
cli
mov ax, GDT64.Data
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov rax, STAGE3_LOCATION
jmp rax

bits 16

; Access bits
PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0
 
; Flags bits
GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5
 
GDT64:
.Null: equ $ - GDT64
    dq 0
.Code: equ $ - GDT64
    dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
    db 0                                        ; Base (mid, bits 16-23)
    db PRESENT | NOT_SYS | EXEC | RW            ; Access
    db GRAN_4K | LONG_MODE | 0xF                ; Flags & Limit (high, bits 16-19)
    db 0                                        ; Base (high, bits 24-31)
.Data: equ $ - GDT64
    dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
    db 0                                        ; Base (mid, bits 16-23)
    db PRESENT | NOT_SYS | RW                   ; Access
    db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
    db 0                                        ; Base (high, bits 24-31)
.Pointer:
    dw $ - GDT64 - 1
    dq GDT64

%include 'print_utils.asm'

drive_error: 
    xor bx, bx
    mov bl, ah
    mov ax, drive_error_msg
    call print_string
    mov ax, bx ;The error code should be in bx
    call print_hex
    jmp $ ;Loop forever

; 0 - A20 is DISABLED
; 1 - A20 is ENABLED
check_a20:
    push es
    xor ax, ax
    not ax
    mov es, ax
    mov di, 0x7E0E
    mov si, 0x7DFE

    mov al, byte [es:di]
    mov byte [check_a20_hi_mem_buf], al
    mov al, byte [ds:si]
    mov byte [check_a20_low_mem_buf], al

    mov byte [ds:si], 0
    mov byte [es:di], 1

    mov al, byte [ds:si]
    mov bl, byte [es:di]
    cmp al, bl
    mov ah, 1
    jne check_a20_done
    mov ah, 0
check_a20_done:
    mov al, byte [check_a20_hi_mem_buf]
    mov byte [es:di], al
    mov al, byte [check_a20_low_mem_buf]
    mov byte [ds:si], al
    pop es
    shr ax, 8
    ret

check_a20_low_mem_buf: db 0
check_a20_hi_mem_buf:  db 0
 
a20_wait_for_data_read:
    in      al,0x64
    test    al,2 ;check to see if bit 1 of the status register is clear. If it is, we can write data to the controller 
    jnz     a20_wait_for_data_read
    ret
 
a20_wait_for_data_ready:
    in      al,0x64
    test    al,1 ;check to see if bit 0 of the status register is set. If it is, we can read from the data port
    jz      a20_wait_for_data_ready
    ret

get_mem_map:
    ;Do a first call to int 0x15
    mov di, MEM_MAP_LOCATION
    xor ebx, ebx
    push bp
    xor bp, bp
    mov edx, 0x0534D4150
    mov eax, 0xE820
    mov [es:di + 20], dword 1 ;Force a valid ACPI entry
    mov ecx, 24 ;Ask for 24 bytes
    int 0x15
    jc mem_map_error ;The carry flag will be set if there's a failure
    mov edx, 0x0534D4150 ;Restore register, sometimes the BIOS will trash it
    cmp eax, edx ;on success, eax will have been reset to the magic 0x0534D4150 number
    jne mem_map_error
    test ebx, ebx ;ebx = 0 implies the list is one entry long, which is useless
    je mem_map_error
    jmp mem_map_jmp_to
mem_map_loop:
    mov eax, 0xE820 ;Eax gets trashed every time
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc mem_map_exit ;carry set means end of the list
    mov edx, 0x0534D4150
mem_map_jmp_to:
    jcxz mem_map_skip_entry ;skip any zero entries
    cmp cl, 20 ;Got a 24 byte response? 
    jbe mem_map_not_extended
    test byte [es:di + 20], 1 ;If the "ignore this data" bit is set, ignore entry
    je mem_map_skip_entry
mem_map_not_extended:
    mov ecx, [es:di + 8] ;get lower uint32 of memory region length
    or ecx, [es:di + 12] ;or it with the upper uint32 to test for zero
    jz mem_map_skip_entry ;If it's 0, skip it
    inc bp ;Good entry, incrememnt the count and move to next storage spot
    add di, 24
mem_map_skip_entry:
    test ebx, ebx ;if ebx resets to 0, list is complete
    jne mem_map_loop
mem_map_exit:
    mov [MEM_MAP_LOCATION - 4], bp
    pop bp
    ret
mem_map_error:
    mov ax, mem_map_err_msg ;Print out an error message.
    call print_string
    jmp $


mem_map_err_msg: db "Something went wrong fetching memory map.", 0xD, 0xA, 0
a20_disabled_message: db "A20 disabled. Enabling.", 0xD, 0xA, 0
a20_enabled_message: db "A20 enabled", 0xD, 0xA, 0
a20_gate_not_supported_msg: db "A20 BIOS call failed", 0xD, 0xA, 0
keyboard_controller_failed: db "8042 controller configuration failed", 0xD, 0xA, 0
entering_long_mode_msg: db "Page tables created, entering long mode", 0xD, 0xA, 0
drive_error_msg: db "There was an error reading from the drive ", 0xD, 0xA, 0

times ((($ - $$) + 512) / 512) * 512 - ($ - $$) db 0 
end: