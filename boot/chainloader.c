#include <include/typeout.h>
#include <include/string.h>
#include <drivers/pic.h>
#include <drivers/pci.h>
#include <drivers/pit.h>
#include <drivers/usb.h>
#include <include/common/port.h>
#include <include/memorymapentry.h>
#include <stddef.h>

void chainloader_entry(){

    picd_init();
    screen_clear();
    //pcid_enumerate();
    //pitd_init(1000);

    int memoryEntryCount = *((uint32*)0x700); //Retrieve memory map structure from 0x8000 
    smap_entry_t* entryptr = (smap_entry_t*)0x704;


    for (int i = 0; i < memoryEntryCount; i++){
        smap_entry_t entry = *entryptr;

        uint64 base = (uint64)entry.base_high << 32 | entry.base_low;
        uint64 length = (uint64)entry.length_high << 32 | entry.length_low;

        screen_printf("sis\n", "entry no. ", i, ":");
        screen_printf("sh\n", "   base address: ", base);
        screen_printf("sh\n", "   length address: ", length);
        screen_printf("sh\n", "   type: ", entry.type);
        entryptr++;
    }






    while(1){}
    return;
}