#include "../lib/typeout.h"
#include "../lib/string.h"
#include "../drivers/pic.h"
#include "../drivers/pci.h"
#include "../drivers/usb.h"
#include <stddef.h>

void main(){
    picd_init();
    screen_clear();
    pcid_enumerate();

    for (int i = 0; i < pci_device_count; i++){
        struct pci_device device = pci_devices[i];
        struct pci_header_common header = device.header.header_common;
        screen_printf("sisi\n\0", "vendor: \0", header.vendor_id, " class code: \0", header.class_code);
    }

    pci_header header;
    //header.header_raw.registers[0].dword = 0xFFFF0123;
    pcid_fill_header(pci_devices[1].bus, pci_devices[1].slot, pci_devices[1].func, &header);

    while(1){}
    return;
}