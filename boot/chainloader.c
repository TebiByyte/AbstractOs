#include <typeout.h>
#include <memorymapentry.h>
#include <stddef.h>
#include <drivers/pic/pic.h>
#include <interrupts/interrupt.h>

void chainloader_entry(){
    screen_clear();

    int memoryEntryCount = *((uint32*)0x700); //Retrieve memory map structure from 0x8000 
    smap_entry_t* entryptr = (smap_entry_t*)0x704;

    screen_print_str("Hello, world!\n");

    picd_init();
    int_init();

    while(1){}
    return;
}