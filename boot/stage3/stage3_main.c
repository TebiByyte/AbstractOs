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

void test_allocator(){
    uint32 *test_array = (uint32 *)mem_alloc(8 * sizeof(uint32));

    for (uint32 i = 0; i < 8; i++){
        test_array[i] = i;
    }

    uint32 *test_array2 = (uint32 *)mem_alloc(8 * sizeof(uint32));

    for (uint32 i = 0; i < 8; i++){
        test_array2[i] = i + 8;
    }

    for (uint32 i = 0; i < 16; i++){
        if (test_array[i] != i){
            screen_print_str("ALLOCATION FAILED!\n");

            for (uint32 j = 0; j < 16; j++){
                screen_printf("ish\n", j, ": ", test_array[j]);
            }

            return;
        }
    }

    screen_print_str("ALLOCATION TEST PASSED!");
}

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
    //screen_printf("sh\n", "PCI devices found: ", *pci_count);


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

    /*pci_address isa_bridge_addr = (pci_address){.bus=0, .device=1, .function=0};
    
    pci_header_reg_3 reg_3_bridge = (pci_header_reg_3)read_pci_register(isa_bridge_addr, 0x3);

    screen_print_int(reg_3_bridge.header_type, 2);
    screen_print_char('\n');*/


 
    uint32* ide_controller_count = ide_find_controllers(device_list, *pci_count);
    ide_controller *controller_array = (ide_controller *)((void*)ide_controller_count + sizeof(uint32));

    //screen_printf("sh\n", "Number of ide controllers found: ", *ide_controller_count);

    write_pci_register((pci_address){.bus=0,.device=1,.function=4}, 0x4, 0x1F0);

    for (int drive_index = 0; drive_index < *ide_controller_count; drive_index++){
        ide_controller current = controller_array[drive_index];

        if (current.primary_channel.mode == IDE_COMPATIBILITY){
            screen_print_str("Compatibility mode IDE controller found\n");
        } else {
            screen_print_str("PCI native mode IDE controller found\n");
        }

        screen_printf("s hs hs h\n", "PCI address: ", 
                      current.pci_device_controller->address.bus, ", ", 
                      current.pci_device_controller->address.device, ", ", 
                      current.pci_device_controller->address.function);


        uint8 progif = ((pci_header_reg_2)read_pci_register(current.pci_device_controller->address, 0x2)).prog_if;

        screen_print_int(progif, 2);
        screen_print_char('\n');

        screen_printf("sh\n", "ID: ", read_pci_register(current.pci_device_controller->address, 0));
        screen_printf("sh\n", "Bar0: ", read_pci_register(current.pci_device_controller->address, 0x4));
        screen_printf("sh\n", "Bar1: ", read_pci_register(current.pci_device_controller->address, 0x5));
        screen_printf("sh\n", "Bar2: ", read_pci_register(current.pci_device_controller->address, 0x6));
        screen_printf("sh\n", "Bar4: ", read_pci_register(current.pci_device_controller->address, 0x7));
    }

    /*uint8 base = (uint8)0x1F0;

    screen_printf("h\n", p_read8(0x1F7));

    p_write8(0x1F6, 0xA0); 
    p_write8(0x1F2, 0);
    p_write8(0x1F3, 0);
    p_write8(0x1F4, 0);
    p_write8(0x1F5, 0);
    p_write8(0x1F7, 0xEC);

    if (p_read8(0x1F4) != 0 || p_read8(0x1F5) != 0){
        screen_print_str("Drive is not ATA\n");
    } else {
        screen_print_str("Drive is ATA\n");
    }

    while (true){
        if ((p_read8(0x1F7) & 0x80) == 0){
            break;
        }
    }

    uint8 status = p_read8(0x1F7);

    screen_print_int(status, 2);

    if (status == 0){
        screen_print_str("No PIO drive detected\n");
    } else {
        screen_print_str("PIO mode drive detected\n");
    }

    while (true){
        if ((p_read8(0x1F7) & 0x08) != 0){
            break;
        }
    }

    for (int j = 0; j<256; j++){
        buffer[j] = p_read16(0x1F0);
    }

    screen_print_int(p_read8(0x1F7), 2);
    screen_print_str("\n");

    p_write8(0x1F6, 0xE0); 
    p_write8(0x1F2, 1);
    p_write8(0x1F7, 0x20);

    status = p_read8(0x1F7);
    screen_print_int(status, 2);
    screen_print_str("\n");

    if (status & 0x1){
        screen_printf("sh\n", "Error reported: ", p_read8(0x1F1));
    }

    while (true){
        if ((p_read8(0x1F7) & 0x80) == 0){
            break;
        }
    }

    for (int j = 0; j<256; j++){
        buffer[j] = p_read16(0x1F0);
    }

    screen_printf("h\n", buffer[255]);*/

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
