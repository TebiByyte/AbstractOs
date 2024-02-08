#ifndef __INTERRUPTSH__
#define __INTERRUPTSH__
#include <common/type.h>
//Note that this will need to be aware of what hardware is being used for interrupts to send a proper EOI 
//TODO: Create a proper handler for interrupts that can be registered at run-time, have the idt common method take in the CPU and stack state as arguments.
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
    uint8 ip;
    uint8 cs;
    uint8 flags;
    uint8 sp;
    uint8 ss;
} int_frame;

void idt_set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr);
void int_init();
__attribute__((interrupt)) void int_blank(int_frame* frame);
#endif
