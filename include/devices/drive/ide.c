#include <devices/drive/ide.h>
#include <common/type.h>
#include <common/port.h>
#include <mem_mgt.h>

//NOTES: This shit is ANNOYING. If the ide device doesn't support pci_native mode, all the interfacing has to be done through
//Legacy interface. This is a pain in the ass for setting up DMA on ATAPI drives, because to use DMA on ATAPI, I need to set the features register via legacy.
//There is no good way of pairing up pci entries to ide controller ports. 
//
//The solution might be to simply not support ATAPI devices. Who uses optical drives anyways? 
ide_controller ide_create_device_entry(pci_device *device_ptr){
    ide_controller controller;
    controller.pci_device_controller = device_ptr;

    //Get the prog_if byte from the pci config space
    uint8 progif = (pci_header_reg_2){.reg_value = read_pci_register(device_ptr->address, 0x2)}.prog_if;
    //screen_print_int(progif, 2);
    //screen_print_char('\n');
    bool primary_pci = (progif & IDE_OPM_PRIMARY) != 0;
    bool secondary_pci = (progif & IDE_OPM_SECONDARY) != 0;

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

//Blocks until BSY bit of status is 0
void wait_for_drive_ready(uint16 base_addr){
    while ((p_read8(base_addr + IDE_STATUS) & 0x80) != 0){}
}

//Blocks until DRQ bit of status is 1 or there is an error
void wait_for_data_ready(uint16 base_addr){
    uint8 status = p_read8(base_addr + IDE_STATUS);
    while ((status & 0x08) == 0 && (status & 0x1) == 0){
        status = p_read8(base_addr + IDE_STATUS);
    }
}

void select_drive(uint16 base_addr, uint8 drive, uint8 lba_enable){
    p_write8(base_addr + IDE_SELECT, 0b10100000 | (lba_enable << 6) | (drive << 4));
}

//Returns false if there was an error
enum ide_id_dev_result identify_drive(uint16 base_addr, uint8 drive, void *buffer){
    select_drive(base_addr, drive, 0);
    p_write8(base_addr + IDE_LBALO, 0);
    p_write8(base_addr + IDE_LBAMID, 0);
    p_write8(base_addr + IDE_LBAHI, 0);
    p_write8(base_addr + IDE_COMMAND, 0xEC);

    uint8 status = p_read8(base_addr + IDE_STATUS);

    if (status == 0){
        return IDE_DRIVE_NOT_PRESENT;
    }

    wait_for_drive_ready(base_addr);

    if (p_read8(base_addr + IDE_LBAMID) != 0 && p_read8(base_addr + IDE_LBAHI) != 0){
        return IDE_DRIVE_IS_ATAPI;
    }

    wait_for_data_ready(base_addr);

    if ((p_read8(base_addr + IDE_STATUS) & 0x1) != 0){
        return IDE_ER;
    }

    uint16 *read_ptr = (uint16*)buffer;

    for (uint32 i = 0; i < 256; i++){
        read_ptr[i] = p_read16(base_addr);
    }

    return IDE_SUCCESS;
}


//Does not support 48 bit lba yet
bool ide_read_sectors(uint16 base_addr, uint8 drive, uint16 sector_num, uint64 lba_start, void *buffer){
    uint32 lbalo = lba_start & 0xFF;
    uint32 lbamid = (lba_start & 0xFF00) >> 8;
    uint32 lbahi = (lba_start & 0xFF0000) >> 16;

    select_drive(base_addr, drive, 1);
    p_write8(base_addr + IDE_COUNT, sector_num);
    p_write8(base_addr + IDE_LBALO, lbalo);
    p_write8(base_addr + IDE_LBAMID, lbamid);
    p_write8(base_addr + IDE_LBAHI, lbahi);

    p_write8(base_addr + IDE_COMMAND, 0x20);

    wait_for_data_ready(base_addr);

    if ((p_read8(base_addr + IDE_STATUS) & 0x1) != 0){
        return false;
    }

    // Note that I need to wait for data after every sector

    for (int sec = 0; sec < sector_num; sec++){
        for (int j = 0; j < 256; j++){
            ((uint16*)buffer)[j] = p_read16(base_addr + IDE_DATA);
        }   

        wait_for_data_ready(base_addr);

        if ((p_read8(base_addr + IDE_STATUS) & 0x1) != 0){
            return false;
        }

    }

    return true;
}
