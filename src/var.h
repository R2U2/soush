#pragma once
#include "str.h"

#define VAR_NOT_FOUND -1
#define IS_ENV -2

typedef struct {
    string name;
    string value;
} var;
typedef struct {
    var variable;
    long int position;
} varGet_var;

varGet_var varGet(char *name);
long int varSet(string name, string value);