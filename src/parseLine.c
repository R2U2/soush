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
    for (size_t index = 0; index < line.length+1; index++) {
        if (line.buffer[index] == '$')
            checkingVar = true;
        else if(checkingVar) {
            if (ispunct(line.buffer[index]) || line.buffer[index] == ' '
                || line.buffer[index] == '\0') {
                checkingVar = false;
                stringPushString(&outLine, envGet(varName).variable.value.buffer);
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