#include "pci.h"
#include "../lib/common/port.h"
#include "../lib/typeout.h"

uint32 pcid_config_read_register(uint8 bus, uint8 slot, uint8 func, uint8 offset){
    uint32 dbus = (uint32)bus;
    uint32 dslot = (uint32)slot;
    uint32 dfunc = (uint32)func;

    uint32 address = (uint32)((dbus << 16) | (dslot << 11 ) | (dfunc << 8) | (offset & 0xCFC) | ((uint32)0x80000000));
    p_write32(CONFIG_ADDRESS, address);

    return p_read32(CONFIG_DATA);
}

uint16 pcid_get_vendor(uint8 bus, uint8 slot, uint8 func){
    return (uint16)(pcid_config_read_register(bus, slot, func, 0) & 0xFFFF);
}

uint8 pcid_get_class(uint8 bus, uint8 slot, uint8 func){
    return (uint8)(pcid_config_read_register(bus, slot, func, 0x08) >> 24);
}

uint8 pcid_get_sub(uint8 bus, uint8 slot, uint8 func){
    return (uint8)((pcid_config_read_register(bus, slot, func, 0x08) & 0x00FF0000) >> 16);
}

uint8 pcid_get_header(uint8 bus, uint8 slot, uint8 func){
    return (uint8)((pcid_config_read_register(bus, slot, func, 0x0C) & 0x00FF0000) >> 16);
}

void checkfunction(uint8 bus, uint8 device, uint8 func){
    uint16 vendor = pcid_get_vendor(bus, device, func);
    if (vendor != 0xFFFF && pci_device_count < 64)
    {
        pci_device_count++;

        devices[pci_device_count].bus = bus;
        devices[pci_device_count].slot = device;
        devices[pci_device_count].head_type = pcid_get_header(bus, device, func);
        devices[pci_device_count].vendor = vendor;
        devices[pci_device_count].func = func;
        devices[pci_device_count].class_code = pcid_get_class(bus, device, func);
        devices[pci_device_count].sub_code = pcid_get_sub(bus, device, func);
    }
}

void checkdevice(uint8 bus, uint8 device){
    uint16 vendor = pcid_get_vendor(bus, device, 0);
    if (vendor == 0xFFFF)return;                    //Not a device
    checkfunction(bus, device, 0); // Check the function
    uint8 header = pcid_get_header(bus, device, 0);
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
    for (uint32 bus = 0; bus < 256; bus++){
        for (uint32 device = 0; device < 32; device++){
            checkdevice(bus, device);   
        }
    }
}
