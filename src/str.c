#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "str.h"
#define STRING_BUFFER_SIZE 32
#define getsize(n) (n)

string stringInit() {
    string arr;
    arr.bufsize = STRING_BUFFER_SIZE;
    arr.buffer = malloc(getsize(arr.bufsize));
    bzero(arr.buffer, getsize(arr.bufsize));
    arr.length = 0;
    return arr;
}
string stringInitString(char *value) {
    string str = stringInit();
    stringPushString(&str, value);
    return str;
}
void stringFree(string *arr) {
    free(arr->buffer);
    arr->length = 0;
    arr->bufsize = 0;
}
void stringClear(string *arr) {
    stringFree(arr);
    *arr = stringInit();
}

void stringPush(string *arr, char elem) {
    if (arr->length >= arr->bufsize)
    {
        arr->bufsize += STRING_BUFFER_SIZE;
        arr->buffer = realloc(arr->buffer, getsize(arr->bufsize));
        bzero(arr->buffer+arr->length, getsize(STRING_BUFFER_SIZE));
    }
    arr->buffer[arr->length++] = elem;
}
void stringPushString(string *arr, char *str) {
    for (; *str; str++)
        stringPush(arr, *str);
}
bool stringIncludes(string arr, char *str) {
    if (strstr(arr.buffer, str) != NULL)
        return true;
    return false;
}