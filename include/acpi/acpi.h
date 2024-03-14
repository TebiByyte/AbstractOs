#ifndef __ACPIH__
#define __ACPIH__
#include <common/type.h>

typedef struct __attribute__((__packed__)) {
    char signature[8];
    uint8 checksum;
    char OEMID[6];
    uint8 revision;
    uint32 rsdt_addr;      // deprecated since version 2.0
    
    uint32 length;
    uint64 xsdt_addr;
    uint8 ext_checksum;
    uint8 reserved[3];
} acpi_rsdp;

acpi_rsdp* acpi_find_rsdp();

#endif