#include "pit.h"
#include "pic.h"
#include <include/common/port.h>
#include <include/typeout.h>

void timer_interrupt(){

    pitd_tick++;
}

uint32 pitd_init(uint32 clock_frequency){

    uint32 divisor = 1193182 / clock_frequency;
    p_write8(0x43, 0x36);
    pitd_tick = 0;

    uint8 l = (uint8)(divisor & 0xFF);
    uint8 h = (uint8)(divisor >> 8);
    p_write8(0x40, l);
    p_write8(0x40, h);
    picd_register_interrupt_handler(0, timer_interrupt);
}

void pitd_wait(uint32 ticks){
    uint32 ticks_now = pitd_tick;
    uint32 time_passed = 0;
    while (time_passed <= ticks){
        time_passed = pitd_tick - ticks_now;
    }
    return;
}