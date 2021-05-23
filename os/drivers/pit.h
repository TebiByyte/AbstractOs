#ifndef __PITH__
#define __PITH__
#include <include/common/type.h>

volatile uint32 pitd_tick;
uint32 pitd_init(uint32 clock_frequency);
void pitd_wait(uint32 ticks);

#endif