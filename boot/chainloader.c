#include <include/typeout.h>
#include <include/string.h>
#include <drivers/pic.h>
#include <drivers/pci.h>
#include <drivers/pit.h>
#include <drivers/usb.h>
#include <include/common/port.h>
#include <stddef.h>

void chainloader_entry(){

    picd_init();
    screen_clear();
    pcid_enumerate();
    //pitd_init(1000);

    

    /*for (int i = 0; i < pci_device_count; i++){
        struct pci_device device = pci_devices[i];
        struct pci_header_common header = device.header.header_common;
        //if (header.class_code == 0xC){
            //pitd_wait(1000);
            screen_printf("sisi\n\0", "vendor: \0", header.vendor_id, " class code: \0", header.class_code);
        //}
    }*/

    screen_print_int(pci_device_count, 10);


    //pitd_wait(5000);

    //screen_print_str("\nTimed message!\0");


    while(1){}
    return;
}