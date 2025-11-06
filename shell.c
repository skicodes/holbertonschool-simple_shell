#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define PROMPT "#cisfun$ "
#define MAX_ARGS 64

/* Trim leading and trailing whitespace */
char *trim_whitespace(char *str)
{
    char *end;

    /* Trim leading space */
    while (*str && (*str == ' ' || *str == '\t'))
        str++;

    if (*str == 0) /* All spaces */
        return str;

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        end--;

    *(end + 1) = '\0';
    return str;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("%s", PROMPT);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Trim spaces */
        line = trim_whitespace(line);
        if (line[0] == '\0') /* Skip empty lines */
            continue;

        /* Split command into argv */
        char *argv[MAX_ARGS];
        int argc = 0;
        char *token = strtok(line, " \t");
        while (token != NULL && argc < MAX_ARGS - 1)
        {
            argv[argc++] = token;
            token = strtok(NULL, " \t");
        }
        argv[argc] = NULL;

        pid = fork();
        if (pid == -1)
        {
            perror("Error:");
            continue;
        }

        if (pid == 0)
        {
            /* Child process */
            if (execve(argv[0], argv, environ) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Parent process */
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
