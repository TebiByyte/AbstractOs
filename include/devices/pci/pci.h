#ifndef __PCI_COMMONH__
#define __PCI_COMMONH__
#include <common/type.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

//TODO I need to refactor this code so it provides a better interface to PCI devices.
//Requirements: 
//  - The programmer should not have to worry about register offsets.
//  - I think the idea is to encapsulate this address space as a triplet of values (A, B, C) maps to the register at offset A at (B determines uint8, uint16, uint32) at bit offset C
//  - And then add macros to refer to common names such as PROGIF to represent the "address" of the property
//  Example: class code would reside at address (0x2, 0x1, 0x0)

typedef struct pci_address{
    uint8 bus;
    uint8 device;
    uint8 function;
} pci_address_t;

typedef struct pci_device { // I think this should only include immutable properties of the device that relate to the device type and where it is
    uint8 bus;
    uint8 device;
    uint8 function;
    uint8 device_class;
    uint8 device_subclass;
    uint8 header_type;
} pci_device_t;

uint32 read_pci_config(uint8 bus, uint8 device, uint8 function, uint8 register);
void find_all_pci_devices(uint32* device_count_buffer, pci_device_t* buffer);
void find_pci_devices_on_bus(uint32* device_count_buffer, pci_device_t* buffer, uint8 bus);
uint16 get_vendor_id(uint8 bus, uint8 device, uint8 function);
uint8 get_class_code(uint8 bus, uint8 device, uint8 function);
uint8 get_subclass_code(uint8 bus, uint8 device, uint8 function);
uint8 get_header_type(uint8 bus, uint8 device, uint8 function);
pci_device_t get_device_info_struct(uint8 bus, uint8 device, uint8 function);

#endif
