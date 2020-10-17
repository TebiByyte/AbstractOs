#ifndef __PCIDH__
#define __PCIDH__
#include "../lib/common/type.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

struct pci_device {
    uint8 bus;
    uint8 slot;
    uint8 head_type;
    uint16 vendor;
    uint8 func;
    uint8 class_code;
    uint8 sub_code;
};

struct pci_device devices[64];//64 might be overkill, but this should be enough space
uint32 pci_device_count;

uint32 pcid_config_read_register(uint8 bus, uint8 slot, uint8 func, uint8 offset);
uint16 pcid_get_vendor(uint8 bus, uint8 slot, uint8 func);
uint8 pcid_get_class(uint8 bus, uint8 slot, uint8 func);
uint8 pcid_get_sub(uint8 bus, uint8 slot, uint8 func);
uint8 pcid_get_header(uint8 bus, uint8 slot, uint8 func);
void pcid_enumerate();

#endif