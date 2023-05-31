#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

#define STRING_BUFFER_SIZE 8

string stringInit(char *data)
{
    string str;
    if (data)
    {
        str.size = strlen(data);
        str.pos = str.size;
    }
    else
    {
        str.size = STRING_BUFFER_SIZE;
        str.pos = 0;
    }
    if (str.size < STRING_BUFFER_SIZE)
        str.size = STRING_BUFFER_SIZE;
    str.buffer = malloc(str.size);
    bzero(str.buffer, str.size);
    if (data)
        strcpy(str.buffer, data);
    return str;
}
void stringPush(string *str, char ch)
{
    if (str->buffer == NULL)
    {
        fprintf(stderr, "str.c: stringFree(): String not initialized\n");
        exit(EXIT_FAILURE);
    }

    if (str->pos >= str->size)
    {
        str->size += STRING_BUFFER_SIZE;
        str->buffer = realloc(str->buffer, str->size);
        bzero(str->buffer+(str->size-STRING_BUFFER_SIZE), STRING_BUFFER_SIZE);
        if (!str->buffer)
            perror("str.c: realloc()");
    }
    str->buffer[str->pos++] = ch;
}
void stringPushString(string *str, char *str2)
{
    for (size_t pos = 0; pos < strlen(str2); pos++)
        stringPush(str, str2[pos]);
}
void stringFree(string *str)
{
    if (str->buffer == NULL)
    {
        fprintf(stderr, "str.c: stringFree(): String not initialized\n");
        exit(EXIT_FAILURE);
    }
    free(str->buffer);
}
