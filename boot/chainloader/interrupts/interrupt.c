#include <interrupts/interrupt.h>
#include <typeout.h>
#include <common/port.h>
#include <drivers/pic/pic.h>

//TODO it might be a good idea to combine the pic code here with the interrupt code

struct idt_ptr idtPtr;
static struct idt_entry idt[256];

void idt_set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr){
    idt[entry].offset_low = address & 0xffff;
    idt[entry].selector = selector;
    idt[entry].zero = 0;
    idt[entry].type_attr = type_attr;
    idt[entry].offset_mid = (address & 0xffff0000) >> 16;
    idt[entry].offset_high = (address & 0xffffffff00000000) >> 32;
    idt[entry].reserved = 0;
}

//I think I could abstract this out a little better
//Note, it's important to initiate the PIC first before calling this
void int_init(){
    for (int i = 0; i < 256; i++)
    {
        idt_set_entry(i, (uint64)(&int_blank), 0x08, 0x8E);
    }

    idtPtr.limit = sizeof(struct idt_entry) * 256 - 1;
    idtPtr.offset = (uint64)&idt;

    lidt(idtPtr);
}

__attribute__((interrupt))
void int_blank(int_frame* frame){
    //Do nothing here
}

void lidt(struct idt_ptr IDTR)
{
    asm ( "lidt %0\n sti" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}