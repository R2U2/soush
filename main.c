#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

#include "runner.h"

#define PROMPT_SIZE 5120

int main()
{
    signal(SIGINT, SIG_IGN);
    char prompt[PROMPT_SIZE];
    while (1)
    {
        bzero(prompt, sizeof(prompt));
        getcwd(prompt, sizeof(prompt));
        strcat(prompt, " $ ");

        char *input = readline(prompt);
        add_history(input);
        if (strcmp(input, ""))
            runner_runLine(input);
    }
    return 0;
}
