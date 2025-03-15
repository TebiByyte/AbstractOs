#ifndef __IDE_DRIVERH__
#define __IDE_DRIVERH__
// TODO add support for multiple buses and secondary drives
#include <common/type.h>
#include <devices/pci/pci.h>

//PIO PORTS

//PRIMARY CHANNEL PORTS
#define PPIO_DATA    0x1F0
#define PPIO_ERROR   0x1F1
#define PPIO_FEAT    0x1F1
#define PPIO_COUNT   0x1F2
#define PPIO_LBALO   0x1F3
#define PPIO_LBAMID  0x1F4
#define PPIO_LBAHI   0x1F5
#define PPIO_SELECT  0x1F6
#define PPIO_STATUS  0x1F7
#define PPIO_COMMAND 0x1F7

//SECONDARY CHANNEL PORTS
#define SPIO_DATA    0x170
#define SPIO_ERROR   0x171
#define SPIO_FEAT    0x171
#define SPIO_COUNT   0x172
#define SPIO_LBALO   0x173
#define SPIO_LBAMID  0x174
#define SPIO_LBAHI   0x175
#define SPIO_SELECT  0x176
#define SPIO_STATUS  0x177
#define SPIO_COMMAND 0x177

//PROG IF BIT MASKS
#define OPM_PRIMARY   0b0001
#define PGI_PRIMARY   0b0010
#define OPM_SECONDARY 0b0100
#define PGI_SECONDARY 0b1000

enum ide_channel_mode { IDE_PCI_NATIVE = 0, IDE_COMPATIBILITY = 1 };

typedef struct ide_channel {
    bool active;
    enum ide_channel_mode mode;
    uint32 base_addr;

} ide_channel;

typedef struct ide_controller {
    pci_device *pci_device_controller;
    ide_channel primary_channel;
    ide_channel secondary_channel;
} ide_controller;

uint32 *ide_find_controllers(pci_device *devices, uint32 pci_count);



#endif
