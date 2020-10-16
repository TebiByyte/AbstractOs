#include "string.h"
#include "common/type.h"
#include <stddef.h>

string string_create(char *source){
    string result = {source, 0};
    for (; source[result.length] != '\0'; result.length++);
    return result;
}

void string_itos(long n, uint32 b, string *buffer){
    bool negative = n < 0 && b == 10; //If it's not base 10, we assume it's unsigned
    buffer->length = 0;//Zero this, just in case
    n = negative ? -n : n;
    
    if (n == 0){
        buffer->data[buffer->length++] = '0';
        buffer->data[buffer->length] = '\0';
    } else {
        while (n != 0){
            long rem = n % b;
            buffer->data[buffer->length++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
            n /= b;
        }

        if (negative){
            buffer->data[buffer->length++] = '-';
        }
        buffer->data[buffer->length] = '\0';
        string_reverse(buffer);
    }
}

void string_reverse(string *str){
    void swap(char *a, char *b){ char t = *a; *a = *b; *b = t; }
    int start = 0, end = str->length - 1;

    while (start < end){
        swap((char*)(str->data + start), (char*)(str->data + end));
        start++;
        end--;
    }
}