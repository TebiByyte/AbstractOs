#include <typeout.h>
#include <memory/mem.h>
#include <stddef.h>
#include <devices/pic/pic.h>
#include <interrupts/interrupt.h>
#include <devices/pci/pci.h>
#include <acpi/acpi.h>
#include <mem_mgt.h>

extern uint32 endkernel;

void printMemoryMapOutput(int count, smap_entry_t* smap_entries);

__attribute((interrupt))
void page_exception_handler(int_frame* int_frame, uint64 error_code){
    screen_clear();
    screen_print_str("Page fault! Dumping interrupt frame and error code: \n");
    screen_printf("sh\n", "RIP: ", int_frame->rip);
    screen_printf("sh\n", "CS: ", int_frame->cs);
    screen_printf("sh\n", "RFLAGS: ", int_frame->rflags);
    screen_printf("sh\n", "RSP: ", int_frame->rsp);
    screen_printf("sh\n", "SS: ", int_frame->ss);
    screen_printf("sh\n", "Error code: ", error_code);
    while(1){}
}

//TODO create a pageframe allocator
void chainloader_entry(){
    screen_clear();

    int memoryEntryCount = *((uint32*)0x6FC);
    smap_entry_t* entryptr = (smap_entry_t*)0x700;
    //printMemoryMapOutput(memoryEntryCount, entryptr);
    mem_alloc_t* mem_struct = (mem_alloc_t*)(endkernel + 0x10000); //Memory after the stack
    mem_initialize(mem_struct, mem_struct + sizeof(mem_alloc_t), 0x10000);

    uint32* pci_count = (uint32*)mem_alloc(mem_struct, sizeof(uint32));
    *pci_count = 0; // Initialize this to 0
    pci_device_t* device_list = (pci_device_t*)(pci_count + sizeof(uint32));

    find_all_pci_devices(pci_count, device_list);
    mem_alloc(mem_struct, (*pci_count) * sizeof(pci_device_t)); //This memory manager lets me get away with sneaky allocations like this after I've already written to the memory
    uint8* test_allocation = (uint8*)mem_alloc(mem_struct, 0x1000);

    if (test_allocation == 0){
        screen_print_str("Heap full!\n");
    }

    screen_printf("si\n", "Number of PCI devices found: ", *pci_count);


    for (int i = 0; i < *pci_count; i++){
        screen_printf("shsh\n", "PCI found. Class code: 0x", device_list[i].device_class, ", Subclass code: 0x", device_list[i].device_subclass);
    }

    picd_init();
    int_init();

    idt_set_entry(0xE, (uint64)(&page_exception_handler), 0x08, 0x8E);

    acpi_rsdp* rsdp = acpi_find_rsdp();

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
