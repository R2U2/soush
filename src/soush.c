#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include "str.h"
#include "var.h"
#include "exec.h"
#include "parseLine.h"

int soush_runCommand(string line) {
    int exitCode = EXIT_SUCCESS;
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
        return EXIT_FAILURE;
    
    char *equal = strstr(argv.buffer[0], "=");
    if (equal != NULL && equal != argv.buffer[0]) {
        char *pos = strstr(line.buffer, "=");
        string name = stringInit();
        string value = stringInit();
        long int index = 0;
        for (; index < pos-line.buffer; index++)
            stringPush(&name, line.buffer[index]);
        for (index++; line.buffer[index]; index++)
            stringPush(&value, line.buffer[index]);
        varSet(name, value);
    } else if(!strcmp(argv.buffer[0], "cd")) {
        if (argv.buffer[1]) {
            if (chdir(argv.buffer[1])) perror("cd");
        } else
            chdir(getpwuid(getuid())->pw_dir);
    } else
        exitCode = forkExec(argv.buffer);

    free(argv.buffer);
    stringFree(&word);
    stringFree(&line);
    return exitCode;
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
void soush_runFile(char *filepath) {
    FILE *file = fopen(filepath, "r");
    string content = stringInit();
    char ch;
    while ((ch = fgetc(file)) != EOF)
        stringPush(&content, ch);
    struct {
        char **buffer;
        size_t size;
    } lines;
    lines.size = 0;
    lines.buffer = malloc(0);
    char *line = strtok(content.buffer, "\n\r");
    while (line != NULL) {
        lines.buffer = realloc(lines.buffer, (++lines.size) * sizeof(char*));
        lines.buffer[lines.size-1] = line;
        line = strtok(NULL, "\n\r");
    }
    for (size_t index = 0; index < lines.size; index++) {
        string strLine = stringInit();
        stringPushString(&strLine, lines.buffer[index]);
        soush_runCommand(parseLine(strLine));
        stringFree(&strLine);
    }
    free(lines.buffer);
    stringFree(&content);
    fclose(file);
}