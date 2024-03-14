#ifndef __STAGE3MEMMGTH__
#define __STAGE3MEMMGTH__
#include <common/type.h>

//Very simple memory allocator, uses stack allocation, does not free memory for the lifetime of stage 3
typedef struct memory_allocator {
    void* mem_start;
    uint32 mem_used;
    uint32 mem_limit;
} mem_alloc_t;

void mem_initialize(mem_alloc_t* mem_struct, void* start, uint32 limit);
void* mem_alloc(mem_alloc_t* mem_struct, uint32 size);

//Memory utilities
void mem_set(void* mem_location, uint8 value, uint32 range);
void mem_cpy(void* copy_from, void* copy_to, uint32 range);

#endif