#include <acpi/acpi.h>

acpi_rsdp* acpi_find_rsdp(){
    char* signature = "RSD PTR ";
    uint8* current_addr = (uint8*)0x80000;

    while ((uint64)current_addr < 0x0009FFFF){
        for (int i = 0; i < 8; i++){
            char character_at_addr = *(current_addr + i);
            if (character_at_addr != *(signature + i)) break;
            if (i == 7) {
                return (acpi_rsdp*)current_addr;
            }
        }

        current_addr += 0x10;
    }

    current_addr = (uint8*)0x000E0000;
    while ((uint64)current_addr < 0x000FFFFF){
        for (int i = 0; i < 8; i++){
            char character_at_addr = *(current_addr + i);
            
            if (character_at_addr != *(signature + i)) break;
            if (i == 7) {
                return (acpi_rsdp*)current_addr;
            }
        }

        current_addr += 0x10;
    }

    return 0;
}