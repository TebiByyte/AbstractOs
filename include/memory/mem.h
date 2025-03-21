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
// 512 entries per table, 9 bit offset into each, every table must be 4Ki aligned. 
// This does NOT support pml5, but that may be a future goal

//Address translation mapping (PML4) ->
// (9 bits: index into pgd) (9 bits: index into PUD) (9 bits: index into PMD) (9 bits: index into pt) (12 bits: page offset)
// Virtual Address => |51 - 42 pgd |41 - 32 pud|31 - 22 pmd|21 - 12 pt|11 - 0 offset| 

//NOTE: this might be wrong. I may want to swap every unit here with uint64 to ensure they're all in the same QWORD.
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
    uint8 ignore1: 1;
    uint8 pg_size: 1;
    uint8 ignored2: 4;
    uint64 addr: 39;
    uint8 res: 1;
    uint16 ignored3: 11;
    uint8 nx: 1;
} pud_entry;

typedef struct {
    uint8 present : 1;
    uint8 rw : 1;
    uint8 us : 1;
    uint8 pwt : 1;
    uint8 cd : 1;
    uint8 accessed: 1;
    uint8 ignore1: 1;
    uint8 pg_size: 1;
    uint8 ignored2: 4;
    uint64 addr: 39;
    uint8 res: 1;
    uint16 ignored3: 11;
    uint8 nx: 1;
} pmd_entry;

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

//TBD what errors can be thrown
enum PAGE_MAP_ERROR {
    NO_ERROR = 0,
    ENTRY_INVALID = 1
};

enum PAGE_MAP_ERROR map_page_frame(uint64 phys_addr, uint64 v_addr);
//NOT IMPLEMENTED YET (look at Intel sdm, section 5.2
void set_page_tbl(pgd_entry *tbl, uint64 flags);


#endif
