#ifndef __PCIDH__
#define __PCIDH__
#include <include/common/type.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

typedef union pci_register{
    uint32 dword;
    uint16 word[2];
    uint8 byte[4];
} pci_reg;

struct __attribute((__packed__)) pci_header_raw {
    pci_reg registers[0x11];
};

struct __attribute((__packed__)) pci_header_common {
    uint16 vendor_id, device_id;//Register 0
    uint16 command, status;//Register 1
    uint8 reivision_id, progif, subclass_code, class_code;//Register 2
    uint8 cache_line_size, latency_timer, header_type, bist;//Register 3
    pci_reg register4;
    pci_reg register5;
    pci_reg register6;
    pci_reg register7;
    pci_reg register8;
    pci_reg register9;
    pci_reg registerA;
    pci_reg registerB;
    pci_reg registerC;
    pci_reg registerD;
    pci_reg registerE;
    pci_reg registerF;
    pci_reg register10;
    pci_reg regstier11;
};

struct __attribute((__packed__)) pci_header_0{
    uint8 common_header_data[16];//This shouldn't be accessed from here, it should be accessed through the pci_header_common struct, so just store it as byte array
    uint32 BAR0;//register 4
    uint32 BAR1;//register 5
    uint32 BAR2;//register 6
    uint32 BAR3;//register 7
    uint32 BAR4;//register 8
    uint32 BAR5;//register 9
    uint32 cardbus_cis_ptr;//register A
    uint16 subsystem_id, subsystem_vendor;//register B
    uint32 expansion_rom_ba;//register C
    uint8 capability_ptr; uint16 reserved_reg_d;//register D
    uint32 reserved;//register E
    uint8 interrupt_line, interrupt_pin, min_grant, max_latency;//register F
    pci_reg register10;
    pci_reg register11;
};

struct __attribute((__packed__)) pci_header_1{
    uint8 common_header_data[16];//This shouldn't be accessed from here, it should be accessed through the pci_header_common struct, so just store it as byte array
    uint32 BAR0;//register 4
    uint32 BAR1;//register 5
    uint8 primary_bus_num, secondary_bus_num, subord_bus_num, seconday_latency_timer;//register 6
    uint8 io_base, io_limit; uint16 secondary_status;//register 7
    uint16 memory_base, memory_limit;//register 8
    uint16 prefetch_mem_base, prefetch_mem_limit;//register 9
    uint32 prefetch_base_upper32;//register A
    uint32 prefetch_limit_upper32;//register B
    uint16 io_base_upper16, io_limit_upper16;//register C
    uint8 capability_ptr, reserved_regd_8; uint16 reserved_regd_16;//register D
    uint32 expansion_rom_addr;//register E
    uint8 interrupt_line, interrupt_pin; uint16 bridge_control;//register F
    pci_reg register10; 
    pci_reg register11;
};

struct __attribute((__packed__)) pci_header_2{
    uint8 common_header_data[16];//This shouldn't be accessed from here, it should be accessed through the pci_header_common struct, so just store it as byte array
    uint32 cardbus_socket;//register 4
    uint8 capability_list_offset, reserved; uint16 secondary_status;//register 5
    uint8 pci_bus_number, cardbus_bus_number, subord_bus_number, cardbus_latency_timer;//register 6
    uint32 memory_base_address0;//register 7
    uint32 memory_limit0;//register 8
    uint32 memory_base_address1;//register 9
    uint32 memory_limit1;//register A
    uint32 io_base_address0;//register B
    uint32 io_limit0;//register C
    uint32 io_base_address1;//register D
    uint32 io_limit1;//register E
    uint8 interrupt_line, interrupt_pin; uint16 bridge_control;//register F
    uint16 subsystem_device_id, subsystem_vendor_id;//register 10
    uint32 legacy_mode_base_address;//register 11
};

typedef union pci_head{
    struct pci_header_raw    header_raw;
    struct pci_header_common header_common;
    struct pci_header_0      header_type0;
    struct pci_header_1      header_type1;
    struct pci_header_2      header_type2;
} pci_header;

struct pci_device {
    uint8 bus;
    uint8 slot;
    uint8 func;
    pci_header header;
};

struct pci_device pci_devices[64];//64 might be overkill, but this should be enough space
volatile uint32 pci_device_count;


void pcid_fill_header(uint8 bus, uint8 slot, uint8 func, pci_header *header);
uint32 pcid_config_read_register(uint8 bus, uint8 slot, uint8 func, uint8 offset);
void pcid_confid_write_register(uint8 bus, uint8 slot, uint8 func, uint8 offset, uint32 data);
void pcid_enumerate();

#endif