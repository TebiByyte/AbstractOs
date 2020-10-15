#include "../lib/typeout.h"

void main(){
    char testStr[15] = "Hello, World!\n\0";
    screen_clear();
    screen_print_str(testStr);

    while(1){}
    return;
}