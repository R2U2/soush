#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <stddef.h>

typedef struct
{
    char *buffer;
    size_t size;
    size_t pos;
} string;

string stringInit(char *data);
void stringPush(string *str, char ch);
void stringPushString(string *str, char *str2);
void stringFree(string *str);

#endif // STR_H_INCLUDED
