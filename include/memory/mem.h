#ifndef __MEMH__
#define __MEMH__
#include <common/type.h>

typedef struct __attribute((__packed__)) smap_entry {
    uint32 base_low;
    uint32 base_high;
    uint32 length_low;
    uint32 length_high;
    uint32 type;
    uint32 acpi;
} smap_entry_t;
// PAGE TABLE STRUCTS

typedef struct {
    uint8 present : 1;
    uint8 rw : 1;
    uint8 us : 1;
    uint8 pwt : 1;
    uint8 cd : 1;
    uint8 accessed: 1;
    uint8 ignore1: 1;
    uint8 res1: 1;
    uint8 ignored2: 4;
    uint64 addr: 39;
    uint8 res: 1;
    uint16 ignored3: 11;
    uint8 nx: 1;
} pgd_entry;

typedef struct {
    uint8 present : 1;
    uint8 rw : 1;
    uint8 us : 1;
    uint8 pwt : 1;
    uint8 cd : 1;
    uint8 accessed: 1;
    uint8 dirty: 1;
    uint8 pat: 1;
    uint8 pg_size: 1;
    uint8 ignored2: 3;
    uint64 addr: 39;
    uint8 res: 1;
    uint8 ignored3: 7;
    uint8 mpk: 4;
    uint8 nx: 1;

} pagetbl_entry;

#endif
