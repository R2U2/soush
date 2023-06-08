#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "env.h"

struct {
    var *buffer;
    size_t size;
} vars;

void envInit() {
    vars.buffer = malloc(0);
    vars.size = 0;
}
envGet_var envGet(char *name) {
    for (size_t pos = 0; pos < vars.size; pos++) {
        if (!strcmp(vars.buffer[pos].name.buffer, name)) {
            envGet_var out;
            out.variable = vars.buffer[pos];
            out.position = pos;
            return out;
        }
    }
    envGet_var out;
    out.variable.name = stringInit();
    out.variable.value = out.variable.name;
    out.position = -1;
    return out;
}
long int envSet(string name, string value) {
    envGet_var existVar = envGet(name.buffer);
    if (existVar.position == -1) {
        var newVar;
        newVar.name = name;
        newVar.value = value;
        vars.buffer = realloc(vars.buffer, (++vars.size) * sizeof(var));
        vars.buffer[vars.size-1] = newVar;
    } else {
        vars.buffer[existVar.position].value = value;
    }
    return existVar.position;
}