%include 'common_macros.asm'
[org STAGE2_LOCATION]
[bits 16]
start:
stage2_info_header:
dw entry
dw (end - start) / 512
dw STAGE2_SIGNATURE
entry:
; To reference the size of this file, you have to do (end - start)
; Start off by loading stage3 at the appropriate location
; next check and enable A20 if needed
; query the memory map and store it somewhere useful
; create the paging tables to identity map the first MB
; enter long mode



jmp $


%include 'print_utils.asm'
times ((($ - $$) + 512) / 512) * 512 - ($ - $$) db 0 
end: