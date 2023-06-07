#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include "exec.h"
#include "env.h"
#include "str.h"
#include "parseLine.h"
#define SOUSH_PS1 "\\w $ "

#include <readline/readline.h>
#include <readline/history.h>

void soush_runCommand(string line, string origLine);
void soush_updatePrompt(string *prompt, const char *PS1);

int main() {
    signal(SIGINT, SIG_IGN);
    string prompt = stringInit();
    soush_updatePrompt(&prompt, SOUSH_PS1);
    while (1) {
        char *input = readline(prompt.buffer);
        if (!strcmp(input, "exit"))
            exit(EXIT_SUCCESS);
        string vecInput = stringInit();
        stringPushString(&vecInput, input);
        soush_runCommand(parseLine(vecInput), vecInput);
        stringFree(&vecInput);
    }
    return 0;
}

void soush_runCommand(string line, string origLine) {
    struct {
        char **buffer;
        size_t size;
    } argv;
    argv.buffer = malloc(0);
    argv.size = 0;
    string word = stringInit();
    char stringSymbol = 0;

    for (size_t pos = 0; pos < line.length; pos++) {
        if (line.buffer[pos] == '\'' || line.buffer[pos] == '"') {
            if (!stringSymbol)
                stringSymbol = line.buffer[pos];
            else if (stringSymbol == line.buffer[pos])
                stringSymbol = 0;
            continue;
        } 
        if (!stringSymbol) {
            if (line.buffer[pos] == ' ') {
                argv.buffer = realloc(argv.buffer, (++argv.size) * sizeof(char*));
                argv.buffer[argv.size-1] = word.buffer;
                word = stringInit();
                continue;
            }
        }
        stringPush(&word, line.buffer[pos]);
    }
    if (word.length) {
        argv.buffer = realloc(argv.buffer, (++argv.size) * sizeof(char*));
        argv.buffer[argv.size-1] = word.buffer;
    }
    argv.buffer = realloc(argv.buffer, (++argv.size) * sizeof(char*));
    argv.buffer[argv.size-1] = NULL;

    if (argv.size == 1)
        return;
    else
        add_history(origLine.buffer);
    
    if (strstr(argv.buffer[0], "=") != NULL) {
        char *pos = strstr(line.buffer, "=");
        string name = stringInit();
        string value = stringInit();
        long int index = 0;
        for (; index < pos-line.buffer; index++)
            stringPush(&name, line.buffer[index]);
        for (index++; line.buffer[index]; index++)
            stringPush(&value, line.buffer[index]);
        envSet(name, value);
    } else
        forkExec(argv.buffer);

    free(argv.buffer);
    stringFree(&word);
}
void soush_updatePrompt(string *prompt, const char *PS1) {
    char cwd[PATH_MAX];
    stringClear(prompt);
    for (size_t pos = 0; pos < strlen(PS1); pos++) {
        if (PS1[pos] == '\\') {
            switch(PS1[++pos]) {
                case 'w':
                    getcwd(cwd, sizeof(cwd));
                    stringPushString(prompt, cwd);
                    break;
            }
        } else 
            stringPush(prompt, PS1[pos]);
    }
}