#include "pic.h"
#include "../lib/common/port.h"
#include "../lib/typeout.h"
#include <stddef.h>

void picd_set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr){
    IDT[entry].offset_low = address & 0xffff;
    IDT[entry].selector = selector;
    IDT[entry].zero = 0;
    IDT[entry].type_attr = type_attr;
    IDT[entry].offset_mid = (address & 0xffff0000) >> 16;
    IDT[entry].offset_high = (address & 0xffffffff00000000) >> 32;
    IDT[entry].reserved = 0;
}

void picd_register_interrupt_handler(uint32 interruptline, void (*handler) (void)){
    handlers[interruptline] = handler;
}

void master_IRQ(){
    //Read ISR to tell which interrupt was fired
    p_write8(0x20, 0x0b);
    p_write8(0xA0, 0x0b);
    uint16 isrValue = (p_read8(0xA0) << 8) | p_read8(0x20);
    if (handlers[isrValue - 1] != NULL){
        handlers[isrValue - 1]();//Call the correct handler for this 
    }
    //EOI
    p_write8(0x20, 0x20);
    p_write8(0xA0, 0x20);
}

void picd_init(){
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
        picd_set_entry(i, (uint64)irq_asm, 0x08, 0x8E);
    }

    idtP.limit = sizeof(struct IDT_entry) * 256 - 1;
    idtP.offset = (uint64)&IDT;

    load_idt();
}



