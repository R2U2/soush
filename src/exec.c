#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

int forkExec(char **argv) {
    int exitCode = 0;
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        if (execvp(argv[0], argv))
            perror("soush: execvp()");
        exit(EXIT_SUCCESS);
    } else if (pid < 0)
        perror("fork()");
    else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        exitCode = WEXITSTATUS(status);
    }
    return exitCode;
}