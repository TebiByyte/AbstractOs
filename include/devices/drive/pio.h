#ifndef PIO_DRIVER_H
#define PIO_DRIVER_H
// TODO add support for multiple buses and secondary drives
#include <common/type.h>
#include <devices/pci/pci.h>

#define MASTER_DATA    0x1F0
#define MASTER_ERROR   0x1F1
#define MASTER_FEAT    0x1F1
#define MASTER_COUNT   0x1F2
#define MASTER_LBALO   0x1F3
#define MASTER_LBAMID  0x1F4
#define MASTER_LBAHI   0x1F5
#define DRIVE_SELECT   0x1F6
#define MASTER_STATUS  0x1F7
#define MASTER_COMMAND 0x1F7

pci_device *find_ide_drive(pci_device *devices, uint32 pci_count);
bool initialize_pio();
void wait_bsy();
void wait_drq();
bool read_drive(uint16 *buffer, uint32 sector_count, uint32 lba_start);

#endif
