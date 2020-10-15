#include "port.h"
//8 bit
uint8 p_read8 (uint16 port){
    uint8 result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void p_write8(uint16 port, uint8 data){
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}
//16 bit
uint16 p_read16 (uint16 port){
    uint16 result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void p_write16(uint16 port, uint16 data){
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port));
}
//32 bit
uint32 p_read32 (uint16 port){
    uint32 result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void p_write32(uint16 port, uint32 data){
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port));
}