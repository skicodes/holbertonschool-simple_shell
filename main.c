#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/**
 * trim_spaces - remove leading and trailing spaces
 * @str: string to trim
 * Return: pointer to trimmed string
 */
char *trim_spaces(char *str)
{
    char *end;

    if (str == NULL)
        return (NULL);

    while (isspace((unsigned char)*str))  /* remove leading spaces */
        str++;

    if (*str == 0)  /* all spaces */
        return (str);

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = '\0';
    return (str);
}

/**
 * main - simple shell 0.1
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 * Return: 0 on success
 */
int main(int argc, char **argv, char **envp)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status;
    char *cmd;

    (void)argc;
    (void)argv;

    while (1)
    {
        write(1, "($) ", 4);
        nread = getline(&line, &len, stdin);
        if (nread == -1)  /* handle EOF */
        {
            write(1, "\n", 1);
            break;
        }

        line[nread - 1] = '\0';  /* remove newline */
        cmd = trim_spaces(line);

        if (cmd[0] == '\0')  /* empty line */
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        if (pid == 0)  /* child process */
        {
            execve(cmd, NULL, envp);
            /* if execve fails */
            write(2, argv[0], strlen(argv[0]));
            write(2, ": 1: ", 5);
            write(2, cmd, strlen(cmd));
            write(2, ": not found\n", 12);
            exit(EXIT_FAILURE);
        }
        else  /* parent process */
            waitpid(pid, &status, 0);
    }

    free(line);
    return (0);
}
