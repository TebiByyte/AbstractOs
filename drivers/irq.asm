global irq_asm
global load_idt
extern master_IRQ
irq_asm:
    call master_IRQ
    iretq
extern idtP
load_idt:
    lidt [idtP]
    sti
    ret