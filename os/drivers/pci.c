#include "pci.h"
#include <include/common/port.h>
#include <include/typeout.h>


uint32 pcid_config_read_register(uint8 bus, uint8 slot, uint8 func, uint8 offset){
    uint32 dbus = (uint32)bus;
    uint32 dslot = (uint32)slot;
    uint32 dfunc = (uint32)func;

    uint32 address = (uint32)((dbus << 16) | (dslot << 11 ) | (dfunc << 8) | (offset & 0xCFC) | ((uint32)0x80000000));
    p_write32(CONFIG_ADDRESS, address);

    return p_read32(CONFIG_DATA);
}

void pcid_confid_write_register(uint8 bus, uint8 slot, uint8 func, uint8 offset, uint32 data){
    uint32 dbus = (uint32)bus;
    uint32 dslot = (uint32)slot;
    uint32 dfunc = (uint32)func;

    uint32 address = (uint32)((dbus << 16) | (dslot << 11 ) | (dfunc << 8) | (offset & 0xCFC) | ((uint32)0x80000000));
    p_write32(CONFIG_ADDRESS, address);  
    p_write32(CONFIG_DATA, data);
}

void pcid_fill_header(uint8 bus, uint8 slot, uint8 func, pci_header *header){
    for (int i = 0; i < 0x11; i++){
        uint32 reg_dword = pcid_config_read_register(bus, slot, func, i * 4);
        header->header_raw.registers[i].dword = reg_dword;
    }
}

uint16 get_vendor(uint8 bus, uint8 slot, uint8 func){
    return (uint16)(pcid_config_read_register(bus, slot, func, 0) & 0xFFFF);
}

uint8 get_header(uint8 bus, uint8 slot, uint8 func){
    return (uint8)((pcid_config_read_register(bus, slot, func, 0x0C) & 0x00FF0000) >> 16);
}


void checkfunction(uint8 bus, uint8 device, uint8 func){
    uint16 vendor = get_vendor(bus, device, func);
    if (vendor != 0xFFFF)
    {
        screen_print_char('d');
        //pci_device_count++;

        /*pci_devices[pci_device_count].bus = bus;
        pci_devices[pci_device_count].slot = device;
        pci_devices[pci_device_count].func = func;
        pcid_fill_header(bus, device, func, &(pci_devices[pci_device_count].header));*/
    }
}

void checkdevice(uint8 bus, uint8 device){
    uint16 vendor = get_vendor(bus, device, 0);
    if (vendor == 0xFFFF)return;                    //Not a device
    checkfunction(bus, device, 0); // Check the function
    uint8 header = get_header(bus, device, 0);
    if (header & 0x80 != 0)
    {
        //Multi function device, check the rest of the functions
        for (uint32 function = 1; function < 8; function++)
        {
            checkfunction(bus, device, function);
        }
    }
}

void pcid_enumerate(){
    pci_device_count = 0;
    for (uint32 bus = 0; bus < 256; bus++){
        for (uint32 device = 0; device < 32; device++){
            checkdevice(bus, device);   
        }
    }
}

#undef get_vendor
#undef get_header
#undef checkfunction
#undef checkdevice