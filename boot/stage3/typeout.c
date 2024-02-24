#include <typeout.h>
#include <common/port.h>
#include <lib/string.h>
#include <stdarg.h>

void screen_clear(){
    cursor.x = 0;
    cursor.y = 0;
    screen_update_cursor();

    uint32 mem_size = MAXH * MAXV;
    for (int i = 0; i < mem_size; i++){
        (VMEM)[i] = 0x0F00;
    }
}

void screen_print_str(char *str){
    uint16 col = 0x0F00;
    int i = 0;
    while (str[i] != '\0'){
        uint16 fullChar = col | str[i];
        screen_print_char(str[i]);
        i++;
    }
}

void screen_print_char(char c){
    uint16 color = 0x0F00;
    uint32 index = INDEX(cursor.x, cursor.y);
    if (c != '\n'){
        (VMEM)[index] = c | color;
        cursor.x++;
    } else {
        cursor.x = 0;
        cursor.y++;
    }

    if (cursor.y >= MAXV){
        cursor.x = 0;
        cursor.y = 0;
        screen_clear();
        screen_update_cursor();
    }

    screen_update_cursor();
}

void screen_print_int(long n, uint32 base){
    char intBuffer[32];
    string intStr = string_create(intBuffer);
    string_itos(n, base, &intStr);
    screen_print_str(intStr.data);
}
// Format:
// i  -> integer(base 10)
// h  -> integer(base 16)
// s  -> string
// \n -> newline 
void screen_printf(char *format, ...){
    va_list args;
    va_start(args, format);
    while (*format != '\0'){
        if (*format == 'i'){
            long i = va_arg(args, long);
            screen_print_int(i, 10);
        }
        else if (*format == 'h'){
            long i = va_arg(args, long);
            screen_print_int(i, 16);
        } else if (*format == 's'){
            char *str = va_arg(args, char*);
            screen_print_str(str);
        } else if (*format == '\n'){
            screen_print_char('\n');
        }
        format++;
    }
    va_end(args);
}

void screen_update_cursor(){
    uint16 pos = INDEX(cursor.x, cursor.y);
    p_write8(0x3D4, 0x0F);
    p_write8(0x3D5, (uint8)(pos & 0xFF)); // write the low bits of the pos
    p_write8(0x3D4, 0x0E);
    p_write8(0x3D5, (uint8)((pos >> 8) & 0xFF)); // write the high bits of the pos
}

void screen_enable_cursor(uint8 cursor_start, uint8 cursor_end){
    p_write8(0x3D4, 0x0A);
    p_write8(0x3D5, (p_read8(0x3D5) & 0xC0) | cursor_start);

    p_write8(0x3D4, 0x0B);
    p_write8(0x3D5, (p_read8(0x3D5) & 0xE0) | cursor_end);
}