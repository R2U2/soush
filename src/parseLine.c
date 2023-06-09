#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "str.h"
#include "var.h"

char *puncts = ".,/\\)()[]{}*&^%$#@!?~`'\":;|-+\0 ";

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
            if (strchr(puncts, line.buffer[index])) {
                if (line.buffer[index] == '$') {
                    stringPush(&outLine, '$');
                    checkingVar = false;
                    continue;
                }
                if (varName.length == 0) {
                    if (line.buffer[index] == '?') {
                        stringPush(&varName, '?');
                    } else {
                        fprintf(stderr, "soush: Expected a variable name after $\n");
                        return stringInit();
                    }
                } else 
                    stringPush(&outLine, line.buffer[index]);
                checkingVar = false;
                stringPushString(&outLine, varGet(varName.buffer).variable.value.buffer);
                stringClear(&varName);
            } else
                stringPush(&varName, line.buffer[index]);
        } else if (line.buffer[index])
            stringPush(&outLine, line.buffer[index]);
    }
    stringFree(&varName);
    return outLine;
}