#include <mem_mgt.h>

void mem_initialize(mem_alloc_t* mem_struct, void* start, uint32 limit){
    mem_struct->mem_start = start;
    mem_struct->mem_limit = limit;
    mem_struct->mem_used = 0;
}

void* mem_alloc(mem_alloc_t* mem_struct, uint32 size){
    if ((mem_struct->mem_used + size) >= mem_struct->mem_limit) return 0; //Error case, if the memory used will be greater than the memory limit

    void* mem_start = mem_struct->mem_start + mem_struct->mem_used;
    mem_struct->mem_used += size;

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
        *(uint64*)(copy_to + offset) = *(uint64*)(copy_from + offset);
    }
}