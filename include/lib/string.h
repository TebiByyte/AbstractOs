#ifndef __STRINGH__
#define __STRINGH__
#include "common/type.h"

typedef struct {
    char* data;
    uint32 length;
} string;

string string_create(char *source);
void string_itos(long n, uint32 b, string *buffer);
#define MAX_PRECISION 10
#ifndef COMPILE_NO_FLOAT
void string_ftos(double n, uint32 p, string *buffer);
#endif
void string_reverse(string* str);
#endif