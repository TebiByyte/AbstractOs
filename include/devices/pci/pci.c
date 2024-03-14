#include <devices/pci/pci.h>
#include <common/port.h>

uint32 read_pci_config(uint8 bus, uint8 device, uint8 function, uint8 reg){
    uint32 address = 0x80000000 | (bus << 16) | (device << 11) | (function << 6) | (reg << 2);
    p_write32(PCI_CONFIG_ADDRESS, address);

    return p_read32(PCI_CONFIG_DATA);
}

void find_all_pci_devices(uint32* device_count_buffer, pci_device_t* buffer){
    uint8 header_type = get_header_type(0, 0, 0);
    if ((header_type & 0x80) == 0){
        //Single PCI host controller
        find_pci_devices_on_bus(device_count_buffer, buffer, 0);
    } else {
        for (uint8 bus = 0; bus < 8; bus++){
            if (get_vendor_id(0, 0, bus) != 0xFFFF) break;
            find_pci_devices_on_bus(device_count_buffer, buffer, bus);
        }
    }
}

//This could be made more safe by calculating the number of devices first, but this is fine for now
//TODO handle multiple PCI host controllers (simply check if the device at 0:0:0 is multifunction, if it is, run this code on each function as the bus number)
void find_pci_devices_on_bus(uint32* device_count_buffer, pci_device_t* buffer, uint8 bus){
    for (uint8 device = 0; device < 32; device++){//Start by scanning all the devices on the selected bus
        if (get_vendor_id(bus, device, 0) == 0xFFFF) continue; // device doesn't exist

        if (get_header_type(bus, device, 0) & 0x80 != 0){
            //multi function device
            for (uint8 func = 1; func < 8; func++){
                if (get_class_code(bus, device, func) == 0x6 && get_subclass_code(bus, device, 0) == 0x4){
                    //PCI-to-PCI bridge, this needs to be checked as well
                    find_pci_devices_on_bus(device_count_buffer, buffer, (read_pci_config(bus, device, func, 0x6) >> 8) & 0xFF);
                } else {
                    buffer[*device_count_buffer] = get_device_info_struct(bus, device, func);
                    (*device_count_buffer)++;
                }
            }
        } else {
            //Single device, add to the list
            buffer[*device_count_buffer] = get_device_info_struct(bus, device, 0);
            *device_count_buffer = *device_count_buffer + 1;
        }
    }
}

uint16 get_vendor_id(uint8 bus, uint8 device, uint8 function){
    return read_pci_config(bus, device, function, 0) & 0xFFFF;
}

uint8 get_class_code(uint8 bus, uint8 device, uint8 function){
    return read_pci_config(bus, device, function, 2) >> 24;
}

uint8 get_subclass_code(uint8 bus, uint8 device, uint8 function){
    return read_pci_config(bus, device, function, 2) >> 16;
}

uint8 get_header_type(uint8 bus, uint8 device, uint8 function){
    return (read_pci_config(bus, device, function, 3) >> 16) & 0xFF;
}

//TODO I could probably add more information to this
pci_device_t get_device_info_struct(uint8 bus, uint8 device, uint8 function){
    pci_device_t pci_device_info;
    pci_device_info.bus = bus;
    pci_device_info.device = device;
    pci_device_info.function = function;
    pci_device_info.device_class = get_class_code(bus, device, function);
    pci_device_info.device_subclass = get_subclass_code(bus, device, function);
    pci_device_info.header_type = get_header_type(bus, device, function);

    return pci_device_info;
}