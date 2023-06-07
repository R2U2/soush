#pragma once
#include "str.h"

typedef struct {
    string name;
    string value;
} var;
typedef struct {
    var variable;
    long int position;
} envGet_var;

envGet_var envGet(string name);
long int envSet(string name, string value);