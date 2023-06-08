#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "str.h"
#include "env.h"

string parseLine(string line) {
    string outLine = stringInit();
    string varName = stringInit();
    bool checkingVar = false;
    size_t index = 0;
    for (; line.buffer[index] && line.buffer[index] == ' '; index++);
    for (; index < line.length+1; index++) {
        if (!checkingVar && line.buffer[index] == '$')
            checkingVar = true;
        else if(checkingVar) {
            if (ispunct(line.buffer[index]) || line.buffer[index] == ' '
                || line.buffer[index] == '\0' || line.buffer[index] == '$') {
                if (line.buffer[index] == '$') {
                    stringPush(&outLine, '$');
                    checkingVar = false;
                    continue;
                }
                if (varName.length == 0) {
                    fprintf(stderr, "soush: Expected a variable name after $\n");
                    return stringInit();
                }
                checkingVar = false;
                stringPushString(&outLine, envGet(varName.buffer).variable.value.buffer);
                stringClear(&varName);
                stringPush(&outLine, line.buffer[index]);
            } else
                stringPush(&varName, line.buffer[index]);
        } else if (line.buffer[index])
            stringPush(&outLine, line.buffer[index]);
    }
    stringFree(&varName);
    return outLine;
}