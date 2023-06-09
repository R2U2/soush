#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "var.h"

struct {
    var *buffer;
    size_t size;
} vars;

void varInit() {
    vars.buffer = malloc(0);
    vars.size = 0;
}
varGet_var varGet(char *name) {
    varGet_var out;
    for (size_t pos = 0; pos < vars.size; pos++) {
        if (!strcmp(vars.buffer[pos].name.buffer, name)) {
            out.variable = vars.buffer[pos];
            out.position = pos;
            return out;
        }
    }
    char *envVar = getenv(name);
    if (envVar) {
        out.variable.name = stringInitString(name);
        out.variable.value = stringInitString(envVar);
        out.position = IS_ENV;
    } else {
        out.variable.name = stringInit();
        out.variable.value = out.variable.name;
        out.position = VAR_NOT_FOUND;
    }
    return out;
}
long int varSet(string name, string value) {
    varGet_var existVar = varGet(name.buffer);
    if (existVar.position == VAR_NOT_FOUND) {
        var newVar;
        newVar.name = name;
        newVar.value = value;
        vars.buffer = realloc(vars.buffer, (++vars.size) * sizeof(var));
        vars.buffer[vars.size-1] = newVar;
    } else
        vars.buffer[existVar.position].value = value;
    return existVar.position;
}