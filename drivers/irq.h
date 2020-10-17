#ifndef __IRQH__
#define __IRQH__
#include "../lib/common/type.h"

struct __attribute__((__packed__)) IDT_entry{
    uint16 offset_low;
    uint16 selector;
    uint8  zero;
    uint8  type_attr;
    uint16 offset_mid;
    uint32 offset_high;
    uint32 reserved;
};

struct __attribute__((__packed__)) idtptr{
    uint16 limit;
    uint64 offset;
};

struct idtptr idtP;

static struct IDT_entry IDT[256];

void set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr);

void idt_init();
void irq0_handler();
void irq1_handler();
void irq2_handler();
void irq3_handler();
void irq4_handler();
void irq5_handler();
void irq6_handler();
void irq7_handler();
void irq8_handler();
void irq9_handler();
void irq10_handler();
void irq11_handler();
void irq12_handler();
void irq13_handler();
void irq14_handler();
void irq15_handler();

#endif