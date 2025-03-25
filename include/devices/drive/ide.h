#ifndef __IDE_DRIVERH__
#define __IDE_DRIVERH__
// TODO add support for multiple buses and secondary drives
#include <common/type.h>
#include <devices/pci/pci.h>

//BASE ADDRESS PORTS
#define IDE_PRIMARY    0x1F0
#define IDE_SECONDARY  0x170

//PORT OFFSETS
#define IDE_DATA    0x0
#define IDE_ERROR   0x1
#define IDE_FEAT    0x1
#define IDE_COUNT   0x2
#define IDE_LBALO   0x3
#define IDE_LBAMID  0x4
#define IDE_LBAHI   0x5
#define IDE_SELECT  0x6
#define IDE_STATUS  0x7
#define IDE_COMMAND 0x7

//ALT CONTROL BASE


//PROG IF BIT MASKS
#define IDE_OPM_PRIMARY   0b0001
#define IDE_PGI_PRIMARY   0b0010
#define IDE_OPM_SECONDARY 0b0100
#define IDE_PGI_SECONDARY 0b1000

enum ide_channel_mode { IDE_PCI_NATIVE = 0, IDE_COMPATIBILITY = 1 };
enum ide_id_dev_result { IDE_DRIVE_NOT_PRESENT = 0, IDE_DRIVE_IS_ATAPI = 1, IDE_ER = 2, IDE_SUCCESS = 3 };

typedef struct ide_channel {
    bool active;
    enum ide_channel_mode mode;
    uint32 base_addr;
    uint8 num_drives; // Maybe 0, 1, or 2
} ide_channel;

typedef struct ide_controller {
    pci_device *pci_device_controller;
    ide_channel primary_channel;
    ide_channel secondary_channel;
} ide_controller;

uint32 *ide_find_controllers(pci_device *devices, uint32 pci_count);
void wait_for_drive_ready(uint16 base_addr);
void wait_for_data_ready(uint16 base_addr);
void select_drive(uint16 base_addr, uint8 drive, uint8 lba_enable);
enum ide_id_dev_result identify_drive(uint16 base_addr, uint8 drive, void *buffer);

#endif
