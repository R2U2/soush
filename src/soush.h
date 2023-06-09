#pragma once
#include "str.h"

int soush_runCommand(string line);
void soush_updatePrompt(string *prompt, const char *PS1);
void soush_runFile(char *filepath);