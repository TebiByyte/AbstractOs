#include <typeout.h>
#include <memory/memmgt.h>
#include <stddef.h>
#include <devices/pic/pic.h>
#include <interrupts/interrupt.h>
#include <devices/pci/pci.h>

extern uint32 endkernel;

void printMemoryMapOutput(int count, smap_entry_t* smap_entries);

__attribute((interrupt))
void keyboard_int(int_frame* int_frame){
    screen_print_str("Interrupt triggered");
    picd_eoi(1);
}

//TODO create a pageframe allocator
void chainloader_entry(){
    screen_clear();

    int memoryEntryCount = *((uint32*)0x700); //Retrieve memory map structure from 0x8000 
    smap_entry_t* entryptr = (smap_entry_t*)0x704;
    //printMemoryMapOutput(memoryEntryCount, entryptr);
    uint32* pci_count = (uint32*)0x10004;
    pci_device_t* device_list = (pci_device_t*)0x1000F;

    find_pci_devices(pci_count, device_list, 0);
    screen_printf("si\n", "Number of PCI devices found: ", *pci_count);

    for (int i = 0; i < *pci_count; i++){
        screen_printf("shsh\n", "PCI found. Class code: 0x", device_list[i].device_class, ", Subclass code: 0x", device_list[i].device_subclass);
    }

    picd_init();
    int_init();

    idt_set_entry(33, (uint64)(&keyboard_int), 0x08, 0x8E); // Keyboard input test 

    screen_print_str("Loaded chainloader successfully!\n");
    screen_printf("sh\n", "Kernel ends at address: ", &endkernel);
    while(1){}
    return;
}

void printMemoryMapOutput(int count, smap_entry_t* smap_entries){
    uint64 totalMemory = 0;

    for (int i = 0; i < count; i++){
        uint64 base = ((uint64)smap_entries[i].base_high << 32) | smap_entries[i].base_low;
        uint64 size = ((uint64)smap_entries[i].length_high << 32) | smap_entries[i].length_low;
        totalMemory += size;
        uint32 type = smap_entries[i].type;
        uint32 acpi = smap_entries[i].acpi;

        screen_printf("shsis\n", "Memory Location Start: ", base, " | Memory Size: ", size / 1024, " KB");

        if (type == 1){
            screen_print_str("Free?: yes\n");
        } else {
            screen_print_str("Free?: no\n");
        }

    }

    screen_printf("sis\n", "Total memory: ", totalMemory / 1024, " KB");
}
