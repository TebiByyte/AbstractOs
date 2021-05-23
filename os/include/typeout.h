#ifndef __TELETYPEOUT__
#define __TELETYPEOUT__
#include "common/type.h"

#define MAXH 80
#define MAXV 25
#define VMEM (uint16*)0xB8000
#define INDEX(x, y) MAXH * y + x

static struct {uint32 x, y;} cursor = {0, 0};

void screen_clear();
void screen_print_str(char *str);
void screen_print_int(long n, uint32 base);
void screen_print_float(double n, uint32 precision);
void screen_print_char(char c);
void screen_printf(char *format, ...);
void screen_update_cursor();
void screen_enable_cursor(uint8 cursor_start, uint8 cursor_end);

#endif