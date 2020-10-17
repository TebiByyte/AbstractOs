#include "irq.h"
#include "../lib/common/port.h"
#include "../lib/typeout.h"

void set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr){
    IDT[entry].offset_low = address & 0xffff;
    IDT[entry].selector = selector;
    IDT[entry].zero = 0;
    IDT[entry].type_attr = type_attr;
    IDT[entry].offset_mid = (address & 0xffff0000) >> 16;
    IDT[entry].offset_high = (address & 0xffffffff00000000) >> 32;
    IDT[entry].reserved = 0;
}

void idt_init(){
    extern int load_idt();
    extern int irq0();
    extern int irq1();
    extern int irq2();
    extern int irq3();
    extern int irq4();
    extern int irq5();
    extern int irq6();
    extern int irq7();
    extern int irq8();
    extern int irq9();
    extern int irq10();
    extern int irq11();
    extern int irq12();
    extern int irq13();
    extern int irq14();
    extern int irq15();

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

    set_entry(32, (uint64)irq0, 0x08, 0x8E);
    set_entry(33, (uint64)irq1, 0x08, 0x8E);
    set_entry(34, (uint64)irq2, 0x08, 0x8E);
    set_entry(35, (uint64)irq3, 0x08, 0x8E);
    set_entry(36, (uint64)irq4, 0x08, 0x8E);
    set_entry(37, (uint64)irq5, 0x08, 0x8E);
    set_entry(38, (uint64)irq6, 0x08, 0x8E);
    set_entry(39, (uint64)irq7, 0x08, 0x8E);
    set_entry(40, (uint64)irq8, 0x08, 0x8E);
    set_entry(41, (uint64)irq9, 0x08, 0x8E);
    set_entry(42, (uint64)irq10, 0x08, 0x8E);
    set_entry(43, (uint64)irq11, 0x08, 0x8E);
    set_entry(44, (uint64)irq12, 0x08, 0x8E);
    set_entry(45, (uint64)irq13, 0x08, 0x8E);
    set_entry(46, (uint64)irq14, 0x08, 0x8E);
    set_entry(47, (uint64)irq15, 0x08, 0x8E);

    idtP.limit = sizeof(struct IDT_entry) * 256 - 1;
    idtP.offset = (uint64)&IDT;

    load_idt();
}

void irq0_handler(){
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

