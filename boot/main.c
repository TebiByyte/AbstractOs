#include "../lib/typeout.h"
#include "../lib/string.h"

void main(){
    screen_clear();

    screen_printf("sf\0", "Pi is approximately \0", 3.14159);

    while(1){}
    return;
}