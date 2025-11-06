#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

/* extern environ for execve */
extern char **environ;

/**
 * trim - removes leading and trailing whitespace from a string
 * @str: input string
 * Return: pointer to trimmed string
 */
char *trim(char *str)
{
    char *end;

    /* Trim leading space */
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) /* All spaces */
        return str;

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    /* Null terminate */
    *(end + 1) = '\0';

    return str;
}

/**
 * main - Simple shell
 * Return: 0 on success
 */
int main(void)
{
    char *line;
    size_t len;
    ssize_t nread;
    pid_t pid;
    int status;

    line = NULL;
    len = 0;

    while (1)
    {
        /* Display prompt if input is from terminal */
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");

        nread = getline(&line, &len, stdin);
        if (nread == -1) /* EOF (Ctrl+D) */
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Remove trailing newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Trim spaces */
        line = trim(line);

        /* Skip empty lines */
        if (line[0] == '\0')
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        else if (pid == 0)
        {
            char *argv[2];
            argv[0] = line;
            argv[1] = NULL;

            if (execve(line, argv, environ) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
