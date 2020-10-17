global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
global load_idt
extern irq0_handler
extern irq1_handler
extern irq2_handler
extern irq3_handler
extern irq4_handler
extern irq5_handler
extern irq6_handler
extern irq7_handler
extern irq8_handler
extern irq9_handler
extern irq10_handler
extern irq11_handler
extern irq12_handler
extern irq13_handler
extern irq14_handler
extern irq15_handler
irq0:
    call irq0_handler
    iretq
irq1:
    call irq1_handler
    iretq
irq2:
    call irq2_handler
    iretq
irq3:
    call irq3_handler
    iretq
irq4:
    call irq4_handler
    iretq
irq5:
    call irq5_handler
    iretq
irq6:
    call irq6_handler
    iretq
irq7:
    call irq7_handler
    iretq
irq8:
    call irq8_handler
    iretq
irq9:
    call irq9_handler
    iretq
irq10:
    call irq10_handler
    iretq
irq11:
    call irq11_handler
    iretq
irq12:
    call irq12_handler
    iretq
irq13:
    call irq13_handler
    iretq
irq14:
    call irq14_handler
    iretq
irq15:
    call irq15_handler
    iretq
extern idtP
load_idt:
    lidt [idtP]
    sti
    ret