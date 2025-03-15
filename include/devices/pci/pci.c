#include "common/type.h"
#include "typeout.h"
#include <devices/pci/pci.h>
#include <common/port.h>
#include <mem_mgt.h>
//I may need to conditionally compile pci enumeration from this with a macro. It won't be needed or availabe for the kernel. 

pci_device *find_all_pci_devices(uint32* device_count_buffer){
    pci_header_reg_3 reg_3_value = {.reg_value = read_pci_register((pci_address){.bus=0, .device=0, .function=0}, 0x03)};
    pci_device *array_start = mem_alloc(0);

    if ((reg_3_value.header_type & 0x80) == 0){
        //Single PCI host controller
        find_pci_devices_on_bus(device_count_buffer, 0);
    } else {
        for (uint8 bus = 0; bus < 8; bus++){
            pci_header_reg_0 reg_0_value = {.reg_value = read_pci_register((pci_address){.bus=bus, .device=0, .function=0}, 0)};
            if (reg_0_value.vendor_id != 0xFFFF) break;
            find_pci_devices_on_bus(device_count_buffer, bus);
        }
    }

    return array_start;
}

//This could be made more safe by calculating the number of devices first, but this is fine for now
//TODO handle multiple PCI host controllers (simply check if the device at 0:0:0 is multifunction, if it is, run this code on each function as the bus number)
void find_pci_devices_on_bus(uint32* device_count_buffer, uint8 bus){
    for (uint8 device = 0; device < 32; device++){//Start by scanning all the devices on the selected bus
        pci_address address_f0 = (pci_address){.bus=bus, .device=device, .function=0};
        pci_header_reg_3 reg_3_f0 = {.reg_value = read_pci_register(address_f0, 0x03)};
        pci_header_reg_0 reg_0_f0 = {.reg_value = read_pci_register(address_f0, 0x00)};

        if (reg_0_f0.vendor_id == 0xFFFF) continue; // device doesn't exist

        if ((reg_3_f0.header_type & 0x80) != 0){
            //multi function device
            for (uint8 func = 1; func < 8; func++){
                pci_address device_address = (pci_address){.bus=bus, .device=device, .function=func};
                pci_header_reg_2 reg_2 = {.reg_value = read_pci_register(device_address, 0x02)};

                if (reg_2.class_code == 0x6 && reg_2.sub_class_code == 0x4){
                    //PCI-to-PCI bridge, this needs to be checked as well
                    find_pci_devices_on_bus(device_count_buffer, (read_pci_register(device_address, 0x6) >> 8) & 0xFF);
                } else {
                    pci_device *new_device = (pci_device*)mem_alloc(sizeof(pci_device));
                    new_device->address = device_address;
                    new_device->device_class = reg_2.class_code;
                    new_device->device_subclass = reg_2.sub_class_code;
                    //screen_printf("shsh\n", "class: ", reg_2.class_code, " subclass: ", reg_2.sub_class_code);
                    /*buffer[*device_count_buffer] = (pci_device){
                        .address = device_address, 
                        .device_class = reg_2.class_code, 
                        .device_subclass = reg_2.sub_class_code
                    };*/


                    (*device_count_buffer)++;
                }
            }
        } else {
            //Single device, add to the list
            pci_header_reg_2 reg_2 = {.reg_value = read_pci_register(address_f0, 0x02)};

            pci_device *new_device = (pci_device*)mem_alloc(sizeof(pci_device));
            new_device->address = address_f0;
            new_device->device_class = reg_2.class_code;
            new_device->device_subclass = reg_2.sub_class_code;
            //screen_printf("shsh\n", "class: ", reg_2.class_code, " subclass: ", reg_2.sub_class_code);

            /*buffer[*device_count_buffer] = (pci_device){
                .address = address_f0,
                .device_class = reg_2.class_code,
                .device_subclass = reg_2.sub_class_code
            };*/
            (*device_count_buffer)++;
        }
    }
}

uint32 read_pci_register(pci_address address, uint8 reg_num){
    uint32 config_value = 0x80000000 | (address.bus << 16) | (address.device << 11) | (address.function << 6) | (reg_num << 2);
    p_write32(PCI_CONFIG_ADDRESS, config_value);

    return p_read32(PCI_CONFIG_DATA);
}
