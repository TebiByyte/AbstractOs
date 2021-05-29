#include <include/interrupts/interrupt.h>
#include <include/typeout.h>
#include <include/common/port.h>

void idt_set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr){
    idt[entry].offset_low = address & 0xffff;
    idt[entry].selector = selector;
    idt[entry].zero = 0;
    idt[entry].type_attr = type_attr;
    idt[entry].offset_mid = (address & 0xffff0000) >> 16;
    idt[entry].offset_high = (address & 0xffffffff00000000) >> 32;
    idt[entry].reserved = 0;
}

//Note, it's important to initiate the PIC first before calling this
void int_init(){
    for (int i = 0; i < 256; i++)
    {
        idt_set_entry(i, *((uint64*)idt_stub_table + i), 0x08, 0x8E);
    }

    idtPtr.limit = sizeof(struct idt_entry) * 256 - 1;
    idtPtr.offset = (uint64)&idt;

    load_idt(idtPtr);
}

void idt_common(uint32 vector){
    //Do something
    screen_printf("si\n", "Interrupt from vector: ", vector);//Just print for now
    p_write8(0x20, 0x20);
    p_write8(0xA0, 0x20);//EOI
}