#ifndef __PICH__
#define __PICH__
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
static void (*handlers[256]) (void);

void picd_set_entry(uint32 entry, uint64 address, uint8 selector, uint8 type_attr);
void picd_register_interrupt_handler(uint32 interruptline, void (*handler) (void));
void master_IRQ();
void picd_init();

#endif