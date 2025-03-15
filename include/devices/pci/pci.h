#ifndef __PCI_COMMONH__
#define __PCI_COMMONH__
#include <common/type.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct pci_address {
    uint8 bus;
    uint8 device;
    uint8 function;
} pci_address;

typedef struct pci_device {
    pci_address address;

    uint8 device_class;
    uint8 device_subclass;
    uint8 header_type;
} pci_device;

typedef union pci_header_reg_0 {
    uint32 reg_value;
    struct __attribute__((__packed__)) {
        uint16 vendor_id;
        uint16 device_id;
    };
} pci_header_reg_0;

typedef union pci_header_reg_1 {
    uint32 reg_value;
    struct __attribute__((__packed__)) {
        uint16 command;
        uint16 status;
    };
} pci_header_reg_1;


typedef union pci_header_reg_2 {
    uint32 reg_value;
    struct __attribute__((__packed__)) {
        uint8 rev_id;
        uint8 prog_if;
        uint8 sub_class_code;
        uint8 class_code;
    };
} pci_header_reg_2;


typedef union pci_header_reg_3 {
    uint32 reg_value;
    struct __attribute__((__packed__)) {
        uint8 cache_size;
        uint8 lat_time;
        uint8 header_type;
        uint8 bist;
    };
} pci_header_reg_3;

uint32 read_pci_register(pci_address address, uint8 reg_num);
pci_device *find_all_pci_devices(uint32* device_count_buffer);
void find_pci_devices_on_bus(uint32* device_count_buffer, uint8 bus);


#endif
