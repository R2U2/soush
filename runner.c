#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include "str/str.h"
#define SOUSH_LINE_SPLIT_DELIMS "\t\r\n\a;"
#define SOUSH_WORD_SIZE 5120

char **runner_splitLine(char *line, const char *delims);
void runner_runProg(char **argv);

void runner_runLine(char *line)
{
    char **lines = runner_splitLine(line, SOUSH_LINE_SPLIT_DELIMS);
    for (size_t linesPos = 0; lines[linesPos] != NULL; linesPos++)
    {
        char **words = malloc(0);
        size_t wordsSize = 0;
        string word = stringInit(NULL);
        char isString = 0;
        char stringChar = 0;
        for (size_t charPos = 0; charPos < strlen(lines[linesPos]); charPos++)
        {
            char ch = lines[linesPos][charPos];
            if (ch == '"' || ch == '\'')
            {
                if (isString && ch == stringChar)
                    isString = 0;
                else if(!isString)
                {
                    isString = 1;
                    stringChar = ch;
                }
                continue;
            }
            else if(ch == ' ' && !isString)
            {
                words = realloc(words, (++wordsSize) * sizeof(char*));
                words[wordsSize-1] = word.buffer;
                word = stringInit(NULL);
                continue;
            }
            stringPush(&word, ch);
        }
        if (word.pos > 0)
        {
            words = realloc(words, (++wordsSize) * sizeof(char*));
            words[wordsSize-1] = word.buffer;
        }
        words = realloc(words, (++wordsSize) * sizeof(char*));
        words[wordsSize-1] = NULL;

        if (!strcmp(words[0], "exit"))
            exit(0);
        else if(!strcmp(words[0], "cd"))
            chdir(words[1]);
        else
            runner_runProg(words);
        stringFree(&word);
        free(words);
    }
    free(lines);
}

char **runner_splitLine(char *line, const char *delims)
{
    char **words = malloc(0);
    if (!words) perror("soush");
    size_t wordsSize = 0;
    char *word = strtok(line, delims);
    while (word != NULL)
    {
        words = realloc(words, (++wordsSize) * sizeof(char*));
        if (!words) perror("soush");
        words[wordsSize-1] = word;
        word = strtok(NULL, delims);
    }
    words = realloc(words, (++wordsSize) * sizeof(char*));
    if (!words) perror("soush");
    words[wordsSize-1] = NULL;
    return words;
}
void runner_runProg(char **argv)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            perror("soush");
        }
        exit(0);
    }
    else if (pid < 0)
    {
        perror("soush");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}
