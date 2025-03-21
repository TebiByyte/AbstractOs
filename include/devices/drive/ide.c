#include <devices/drive/ide.h>
#include <common/type.h>
#include <mem_mgt.h>

ide_controller ide_create_device_entry(pci_device *device_ptr){
    ide_controller controller;
    controller.pci_device_controller = device_ptr;

    //Get the prog_if byte from the pci config space
    uint8 progif = (pci_header_reg_2){.reg_value = read_pci_register(device_ptr->address, 0x2)}.prog_if;
    //screen_print_int(progif, 2);
    //screen_print_char('\n');
    bool primary_pci = (progif & OPM_PRIMARY) != 0;
    bool secondary_pci = (progif & OPM_SECONDARY) != 0;

    controller.primary_channel.mode = (primary_pci ? IDE_PCI_NATIVE : IDE_COMPATIBILITY);
    controller.secondary_channel.mode = (secondary_pci ? IDE_PCI_NATIVE : IDE_COMPATIBILITY);

    if (primary_pci) {
        //Read BAR0

    }

    if (secondary_pci) {
        //Read BAR2
    }

    //Check DMA 

    return controller;

}

uint32 *ide_find_controllers(pci_device *devices, uint32 pci_count){
    uint32 *count_ptr = mem_alloc(sizeof(uint32));
    *count_ptr = 0;

    for (int i = 0; i < pci_count; i++){
        //screen_printf("shsh\n" "Checking device. Class: ", devices[i].device_class, " Subclass: ", devices[i].device_subclass);
        if (devices[i].device_class == 0x01 && devices[i].device_subclass == 0x01){
            //screen_print_str("Found device\n");
            *count_ptr = *count_ptr + 1;
            void *ide_ptr = mem_alloc(sizeof(ide_controller));

            *((ide_controller*)ide_ptr) = ide_create_device_entry(&devices[i]);
        }
    }

    return count_ptr;
}

