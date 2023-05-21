#ifndef __MEMORYMAPENTRYH__
#define __MEMORYMAPENTRYH__
#include <common/type.h>

typedef struct __attribute((__packed__)) smap_entry {
    uint32 base_low;
    uint32 base_high;
    uint32 length_low;
    uint32 length_high;
    uint32 type;
    uint32 acpi;
} smap_entry_t;

#endif