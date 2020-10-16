#include "../lib/typeout.h"
#include "../lib/string.h"

void main(){
    screen_clear();

    screen_printf("si\ns", "Hello!", 0xFF, "Goodbye!");

    while(1){}
    return;
}