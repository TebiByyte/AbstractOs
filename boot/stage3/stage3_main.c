#include "common/port.h"
#include "common/type.h"
#include "devices/drive/ide.h"
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

//Memory initialization outline steps: 
//Consult memory map and only allocate memory that is type 1 (free) to virtual address space. 
//Note: I should add a check to make sure the end of stage 3 in memory is in a free area
//Map all those areas to be a continguous block of memory 

//TODO create a pageframe allocator
//TODO test the memory allocator to make sure it's working
//TODO fix PCI enumeration to
void chainloader_entry(){
    screen_clear();

    int memoryEntryCount = *((uint32*)0x6FC);
    smap_entry_t* entryptr = (smap_entry_t*)0x700;
    //printMemoryMapOutput(memoryEntryCount, entryptr);
    mem_initialize((void*)(endkernel + 0x100000l), 0x10000);

    /*test_allocator();

    for (;;){}*/

    uint32* pci_count = (uint32*)mem_alloc(sizeof(uint32));
    *pci_count = 0; // Initialize this to 0
    //pci_device* device_list = (pci_device*)(pci_count + sizeof(uint32));

    pci_device *device_list = find_all_pci_devices(pci_count);
    screen_printf("sh\n", "PCI devices found: ", *pci_count);



    //Hard drive read test: PASSED
    //TODO: abstract these things into functions
    /*for (int i = 0; i < *pci_count; i++){
        pci_device current_device = device_list[i];
        screen_printf("sisi\n", "PCI found. Class code: ", current_device.device_class, ", Subclass code: ", current_device.device_subclass);
        pci_address device_address = device_list[i].address;
        screen_printf("shshsh\n", "PCI address: ", 
                      device_address.bus, ", ", 
                      device_address.device, ", ", 
                      device_address.function);

		//pci_header_reg_2 reg_2 = {.reg_value = read_pci_register(device_address, 0x02)};
        //uint32 progIF = (read_pci_config(device_list[i].bus, device_list[i].device, device_list[i].function, 0x02) >> 8) & 0xFF;
        //screen_printf("sh\n", "  Prog IF: ", reg_2.prog_if);
    }*/

 
    uint32* ide_controller_count = ide_find_controllers(device_list, *pci_count);
    ide_controller *controller_array = (ide_controller *)((void*)ide_controller_count + sizeof(uint32));

    ide_controller first = controller_array[0];
    pci_header_reg_1 reg = {.reg_value = read_pci_register(first.pci_device_controller->address, 0x1)};

    reg.command = 0;

    write_pci_register(first.pci_device_controller->address, 0x1, 0);

    screen_printf("h\n", read_pci_register(first.pci_device_controller->address, 0x1));

    uint16 id_buff[256];

    enum ide_id_dev_result drv_id_err = identify_drive(IDE_PRIMARY, 0, (void*)id_buff);

    if (drv_id_err == IDE_SUCCESS){
        screen_print_str("Drive 0 located at port\n");
    } else {
        screen_print_str("No drive detected\n");
    }

    //screen_printf("sh\n", "Number of ide controllers found: ", *ide_controller_count);
    
    uint8 buffer[512];

    bool read_success = ide_read_sectors(IDE_PRIMARY, 0, 1, 0, (void*)(&buffer));

    if (read_success){
        screen_printf("h\n", buffer[511]);
    } else {
        screen_print_str("There was an error\n");
        screen_print_int(p_read8(IDE_PRIMARY + IDE_ERROR), 2);
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
        screen_printf("sh\n", "Type: ", type);
    }

    screen_printf("sis\n", "Total memory: ", totalMemory / 1024, " KB");
}
