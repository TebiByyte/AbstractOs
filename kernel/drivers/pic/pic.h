#ifndef __PICDRIVERH__
#define __PICDRIVERH__
#include <common/type.h>
//Driver for the 8259 PIC

#define PICM            0x20
#define PICS            0xA0
#define PICM_COMMAND    0x20
#define PICM_DATA       0x21
#define PICS_COMMAND    0xA0
#define PICS_DATA       0xA1
//------IO PORTS---------//
#define PIC_EOI         0x20
//------COMMANDS---------//
#define ICW1_ICW4       0x01 //ICW4 needed
#define ICW1_SINGLE     0x02 //Single cascade mode
#define ICW1_INT4       0x04 //Call address interal 4
#define ICW1_LEVEL      0x08 //Level Triggered (edge) mode
#define ICW1_INIT       0x10 //Initialization

#define ICW4_8086       0x01 //8086/88 mode
#define ICW4_AUTO       0x02 //auto (normal) eoi
#define ICW4_BUF_SLAVE  0x08 //buffered mode/slave
#define ICW4_BUF_MASTER 0x0C //buffered mode/master
#define ICW4_SFNM       0x10 //special fully nested 

#define READ_IRR        0x0a 
#define READ_ISR        0x0b

void picd_init();
void picd_eoi(uint8 vector);
void picd_set_mask(uint8 il);
void picd_clear_mask(uint8 il);
uint16 picd_read_isr();
uint16 picd_read_irr();

#endif