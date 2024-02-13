#ifndef __PCI_COMMONH__
#define __PCI_COMMONH__
#include <common/type.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct pci_device {
    uint8 bus;
    uint8 device;
    uint8 function;
    uint8 device_class;
    uint8 device_subclass;
    uint8 header_type;
} pci_device_t;

uint32 read_pci_config(uint8 bus, uint8 device, uint8 function, uint8 register);
void find_pci_devices(uint32* device_count_buffer, pci_device_t* buffer, uint8 bus);
uint16 get_vendor_id(uint8 bus, uint8 device, uint8 function);
uint8 get_class_code(uint8 bus, uint8 device, uint8 function);
uint8 get_subclass_code(uint8 bus, uint8 device, uint8 function);
uint8 get_header_type(uint8 bus, uint8 device, uint8 function);
pci_device_t get_device_info_struct(uint8 bus, uint8 device, uint8 function);

#endif