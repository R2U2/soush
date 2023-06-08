#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    size_t bufsize;
    size_t length;
} string;

string stringInit();
string stringInitString(char *value);
void stringClear(string *arr);
void stringFree(string *arr);

void stringPush(string *arr, char elem);
void stringPushString(string *arr, char *str);
bool stringIncludes(string arr, char *str);