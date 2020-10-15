#include "typeout.h"
#include "common/port.h"

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
        screen_update_cursor();
    }

    screen_update_cursor();
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