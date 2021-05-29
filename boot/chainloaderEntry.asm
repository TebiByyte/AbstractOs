[extern chainloader_entry]
[extern idt_common]

call chainloader_entry ;Invoke chainloader_entry

jmp $

%define IRQLEN 256 

%macro idt_stub 1
    global idt_stub_%1
    idt_stub_%1:
        push rax ;Don't worry about trap gates for now. But I may need to handle those differently. 
        push rcx
        push rdi
        push rsi
        push r8
        push r9
        push r10
        push r11
        mov rdi, %1;Note, GCC uses sysv calling convention so the argument order goes: rdi, rsi, rdx, rcx, r8, r9, stack
        cld
        call idt_common
        pop r11
        pop r10
        pop r9
        pop r8
        pop rsi
        pop rdi
        pop rcx
        pop rax
        iretq

%endmacro

%assign i 0
%rep IRQLEN
    idt_stub i
    %assign i i + 1
%endrep


%macro D_STUB_ADDR 1
    dq idt_stub_%1
%endmacro

global load_idt
load_idt:
    lidt [rsp + 8]
    sti
    ret

global idt_stub_table
idt_stub_table:
    %assign i 0
    %rep IRQLEN
        D_STUB_ADDR i
    %assign i i + 1
    %endrep