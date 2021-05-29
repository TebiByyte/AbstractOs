#ifndef __INTERRUPTSH__
#define __INTERRUPTSH__
#include <include/common/type.h>

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

static struct idt_entry idt[256];
struct idt_ptr idtPtr;
extern void idt_stub_table();
extern void load_idt(struct idt_ptr ptr);
void int_init();
void idt_common(uint32 vector);
#endif