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

#ifndef COMPILE_NO_FLOAT

static const double rounders[MAX_PRECISION + 1] = {
    0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};

void string_ftos(double n, uint32 p, string *buffer){
    bool negative = false;
    if (n < 0) {
        n = -n;
        negative = true;
    }
    if (p > MAX_PRECISION){
        p = MAX_PRECISION;
    }

    if (p < 0){
        if (n < 1.0) p = 6;
        else if (n < 10.0) p = 5;
        else if (n < 100.0) p = 4;
        else if (n < 1000.0) p = 3;
        else if (n < 10000.0) p = 2;
        else if (n < 100000.0) p = 1;
        else p = 0;
    }

    if (p){
        n += rounders[p];
    }


    int intPart = n;
    n -= intPart;

    intPart = negative ? -intPart : intPart;

    string_itos(intPart, 10, buffer);

    char *charBuffer = (char*)(buffer->data + buffer->length);

    if (p){
        *charBuffer++ = '.';

        while (p--){
            n *= 10.0;
            char c = n;
            *charBuffer++ = '0' + c;
            n -= c;
        }
    }

    *charBuffer = '\0';
    buffer->length = charBuffer - buffer->data;
}

#endif

void string_reverse(string *str){
    void swap(char *a, char *b){ char t = *a; *a = *b; *b = t; }
    int start = 0, end = str->length - 1;

    while (start < end){
        swap((char*)(str->data + start), (char*)(str->data + end));
        start++;
        end--;
    }
}