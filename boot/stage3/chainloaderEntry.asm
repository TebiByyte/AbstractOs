start:
global start
[extern endkernel]
[extern chainloader_entry]

;Set up the stack in a safe place
mov ebp, endkernel
add ebp, 0x10000
mov esp, ebp

call chainloader_entry ;Invoke chainloader_entry

jmp $
