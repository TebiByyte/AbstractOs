#ifndef __INTERRUPTSH__
#define __INTERRUPTSH__
#include <common/type.h>
//Note that this will need to be aware of what hardware is being used for interrupts to send a proper EOI 

struct __attribute__((__packed__)) idt_entry{
    uint16 offset_low;
    uint16 selector;
    uint8  zero;
    uint8  type_attr;
    uint16 offset_mid;
    uint32 offset_high;
    uint32 reserved;
};

struct __attribute__((__packed__)) idt_ptr{
    uint16 limit;
    uint64 offset;
};

typedef struct __attribute__((__packed__)) {
    uint64 rip;
    uint64 cs;
    uint64 rflags;
    uint64 rsp;
    uint8 ss;
} int_frame;

void lidt(struct idt_ptr IDTR);
void idt_set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr); //This needs a better abstraction
void int_init();
__attribute__((interrupt)) void int_blank(int_frame* frame);
#endif
