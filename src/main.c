#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include "var.h"
#include "str.h"
#include "parseLine.h"
#include "soush.h"
#define SOUSH_DEFAULT_PS "\\w $ "

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **argv) {
    string soushrc = stringInit();
    stringPushString(&soushrc, getpwuid(getuid())->pw_dir);
    stringPushString(&soushrc, "/.soushrc");
    if (!access(soushrc.buffer, F_OK))
        soush_runFile(soushrc.buffer);
    if (varGet("PS").position == -1)
        varSet(stringInitString("PS"), stringInitString(SOUSH_DEFAULT_PS));
    varSet(stringInitString("?"), stringInitString("0"));

    if (argc == 2) {
        soush_runFile(argv[1]);
    } else {
        signal(SIGINT, SIG_IGN);
        using_history();
        
        string prompt = stringInit();
        while (1) {
            soush_updatePrompt(&prompt, varGet("PS").variable.value.buffer);
            char *input = readline(prompt.buffer);
            if (!strcmp(input, "exit"))
                break;
            
            string vecInput = stringInit();
            stringPushString(&vecInput, input);
            int exitCode = soush_runCommand(parseLine(vecInput));
            stringFree(&vecInput);
            char exitCodeStr[sizeof(int) * 3];
            sprintf(exitCodeStr, "%d", exitCode);
            if (exitCode != 0)
                add_history(vecInput.buffer);
            varSet(stringInitString("?"), stringInitString(exitCodeStr));
        }
    }
    return 0;
}