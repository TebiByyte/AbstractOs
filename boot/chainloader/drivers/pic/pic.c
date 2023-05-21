#include <drivers/pic/pic.h>
#include <common/port.h>

void picd_init(){
    p_write8(PICM_COMMAND, ICW1_INIT | ICW1_ICW4);
    p_write8(PICS_COMMAND, ICW1_INIT | ICW1_ICW4);
    p_write8(PICM_DATA, 0x20);//Offset of 32
    p_write8(PICS_DATA, 0x28);//Offset of 40
    p_write8(PICM_DATA, 4);
    p_write8(PICS_DATA, 2);
    p_write8(PICM_DATA, ICW4_8086);
    p_write8(PICS_DATA, ICW4_8086);
}

void picd_eoi(uint8 vector){
    if (vector >= 8){
        p_write8(PICS_COMMAND, PIC_EOI);
    }

    p_write8(PICM_COMMAND, PIC_EOI);
}

void picd_set_mask(uint8 il){
    uint16 port;

    if (il < 8){
        //Master PIC
        port = PICM_DATA;
    } else {
        //Slave pic
        port = PICS_DATA;
        il -= 8;
    }

    uint8 value = p_read8(port) | (1 << il); //Read the current mask value and set the bit of the interrupt line
    p_write8(port, value); //Write the new mask bitmap
}

void picd_clear_mask(uint8 il){
    uint16 port;

    if (il < 8){
        //Master PIC
        port = PICM_DATA;
    } else {
        //Slave pic
        port = PICS_DATA;
        il -= 8;
    }

    uint8 value = p_read8(port) & ~(1 << il); //Read the current mask value and clear the bit of the interrupt line
    p_write8(port, value); //Write the new mask bitmap
}

//Returns the combined value of the cascaded pics isr register. [15::8] slave pic, [7::0] master pic
uint16 picd_read_isr(){
    p_write8(PICM_COMMAND, READ_ISR);
    p_write8(PICS_COMMAND, READ_ISR);

    return (p_read8(PICS_COMMAND) << 8) | p_read8(PICM_COMMAND);
}

//Returns the combined value of the cascaded pics irr register. [15::8] slave pic, [7::0] master pic
uint16 picd_read_irr(){
    p_write8(PICM_COMMAND, READ_IRR);
    p_write8(PICS_COMMAND, READ_IRR);

    return (p_read8(PICS_COMMAND) << 8) | p_read8(PICM_COMMAND);
}
