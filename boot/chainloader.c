#include <include/typeout.h>
#include <include/string.h>
#include <include/common/port.h>
#include <include/memorymapentry.h>
#include <stddef.h>
#include <drivers/pic.h>
#include <include/interrupts/interrupt.h>




void chainloader_entry(){
    screen_clear();

    int memoryEntryCount = *((uint32*)0x700); //Retrieve memory map structure from 0x8000 
    smap_entry_t* entryptr = (smap_entry_t*)0x704;

    screen_print_str("Hello, world!\n");

    /*for (int i = 0; i < 8; i++){
        void (*idt_ptr)(void) = *((uint64*)idt_stub_table + i);
        (idt_ptr)();
    }*/

    picd_init();
    int_init();




    while(1){}
    return;
}