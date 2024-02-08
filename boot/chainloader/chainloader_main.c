#include <typeout.h>
#include <memorymapentry.h>
#include <stddef.h>
#include <drivers/pic/pic.h>
#include <interrupts/interrupt.h>

__attribute((interrupt))
void keyboard_int(int_frame* int_frame){
    screen_print_str("Interrupt triggered");
    picd_eoi(1);
}

void chainloader_entry(){
    screen_clear();

    int memoryEntryCount = *((uint32*)0x700); //Retrieve memory map structure from 0x8000 
    smap_entry_t* entryptr = (smap_entry_t*)0x704;

    picd_init();
    int_init();

    idt_set_entry(33, (uint64)(&keyboard_int), 0x08, 0x8E); // Keyboard input test 

    screen_print_str("Loaded chainloader successfully!\n");
    while(1){}
    return;
}