#include "irq.h"
#include "../lib/common/port.h"
#include "../lib/typeout.h"
#include <stddef.h>

void set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr){
    IDT[entry].offset_low = address & 0xffff;
    IDT[entry].selector = selector;
    IDT[entry].zero = 0;
    IDT[entry].type_attr = type_attr;
    IDT[entry].offset_mid = (address & 0xffff0000) >> 16;
    IDT[entry].offset_high = (address & 0xffffffff00000000) >> 32;
    IDT[entry].reserved = 0;
}

void master_IRQ(){
    //Read ISR to tell which interrupt was fired
    p_write8(0x20, 0x0b);
    p_write8(0xA0, 0x0b);
    uint16 isrValue = (p_read8(0xA0) << 8) | p_read8(0x20);
    if (handlers[isrValue - 1] != NULL){
        handlers[isrValue - 1]();//Call the correct handler for this 
    } else {
        //Since there's no handler, we'll need to end the interrupt ourselves
        p_write8(0x20, 0x20);
    }
}

void idt_init(){
    extern int load_idt();
    extern int irq_asm();

    p_write8(0x20, 0x11);
    p_write8(0xA0, 0x11);
    p_write8(0x21, 0x20);
    p_write8(0xA1, 40);
    p_write8(0x21, 0x04);
    p_write8(0xA1, 0x02);
    p_write8(0x21, 0x01);
    p_write8(0xA1, 0x01);
    p_write8(0x21, 0x0);
    p_write8(0xA1, 0x0);

    for (int i = 32; i < 48; i++){
        set_entry(i, (uint64)irq_asm, 0x08, 0x8E);
    }

    handlers[0] = irq0_handler;
    handlers[1] = irq1_handler;
    handlers[2] = irq2_handler;
    handlers[3] = irq3_handler;
    handlers[4] = irq4_handler;
    handlers[5] = irq5_handler;
    handlers[6] = irq6_handler;
    handlers[7] = irq7_handler;
    handlers[8] = irq8_handler;
    handlers[9] = irq9_handler;
    handlers[10] = irq10_handler;
    handlers[11] = irq11_handler;
    handlers[12] = irq12_handler;
    handlers[13] = irq13_handler;
    handlers[14] = irq14_handler;
    handlers[15] = irq15_handler;

    idtP.limit = sizeof(struct IDT_entry) * 256 - 1;
    idtP.offset = (uint64)&IDT;

    load_idt();
}

void irq0_handler(){
    screen_print_str("\nHandled the interrupt correctly!\0");
    p_write8(0x20, 0x20);
}

void irq1_handler(){
    p_write8(0x20, 0x20);
}

void irq2_handler(){
    p_write8(0x20, 0x20);
}

void irq3_handler(){
    p_write8(0x20, 0x20);
}

void irq4_handler(){
    p_write8(0x20, 0x20);
}

void irq5_handler(){
    p_write8(0x20, 0x20);
}

void irq6_handler(){
    p_write8(0x20, 0x20);
}

void irq7_handler(){
    p_write8(0x20, 0x20);
}

void irq8_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq9_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq10_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq11_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq12_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq13_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq14_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

void irq15_handler(){
    p_write8(0xA0, 0x20);
    p_write8(0x20, 0x20);
}

