#include <mem_mgt.h>

mem_alloc_t allocator_info;

void mem_initialize(void* start, uint32 limit){
    allocator_info.mem_start = start;
    allocator_info.mem_limit = limit;
    allocator_info.mem_used = 0;
}

void* mem_alloc(uint32 size){
    if ((allocator_info.mem_used + size) >= allocator_info.mem_limit) return 0; //Error case, if the memory used will be greater than the memory limit

    void* mem_start = allocator_info.mem_start + allocator_info.mem_used;
    allocator_info.mem_used += size;

    return mem_start;
}

//Memory utilities
void mem_set(void* mem_location, uint8 value, uint32 range){
    for (uint32 offset = 0; offset < range; offset++){
        *(uint8*)(mem_location + offset) = value;
    }
}

void mem_cpy(void* copy_from, void* copy_to, uint32 range){
    for (uint32 offset = 0; offset < range; offset++){
        *(uint8*)(copy_to + offset) = *(uint8*)(copy_from + offset);
    }
}
