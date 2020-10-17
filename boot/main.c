#include "../lib/typeout.h"
#include "../lib/string.h"
#include "../drivers/pic.h"
#include "../drivers/pci.h"

void main(){
    picd_init();
    screen_clear();
    pcid_enumerate();

    for (int i = 0; i < pci_device_count; i++){
        struct pci_device device = devices[i];
        screen_printf("sisi\n\0", "vendor: \0", device.vendor, " class code: \0", device.class_code);
    }

    while(1){}
    return;
}