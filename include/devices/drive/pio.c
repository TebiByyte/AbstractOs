#include <devices/pci/pci.h>
#include <devices/drive/pio.h>
#include <common/type.h>

pci_device *find_ide_drive(pci_device* devices, uint32 pci_count){

    return NULL;
}

bool initialize_pio(){
    return false;
}

void wait_bsy(){

}

void wait_drq(){

}

bool read_drive(uint16 *buffer, uint32 sector_count, uint32 lba_start){
    return false;
}
