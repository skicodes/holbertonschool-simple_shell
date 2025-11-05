#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

char *trim_spaces(char *str)
{
    char *end;

    if (!str)
        return NULL;

    while (isspace((unsigned char)*str))  // remove leading spaces
        str++;

    if (*str == 0)  // if string is all spaces
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))  // remove trailing spaces
        end--;

    *(end + 1) = '\0';
    return str;
}

/**
 * print_prompt - prints the shell prompt
 */
void print_prompt(void)
{
	printf("#cisfun$ ");
	fflush(stdout);
}

/**
 * execute_command - forks and executes a single-word command
 * @line: command line string
 * @envp: environment variables
 */
void execute_command(char *line, char **envp)
{
	pid_t pid;
	int status;
	char *args[2]; /* argv for execve: command + NULL */

	args[0] = line;
	args[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("shell");
		return;
	}
	if (pid == 0)
	{
		execve(line, args, envp); /* pass args instead of NULL */
		perror("shell");
		_exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

/**
 * main - entry point for simple_shell 0.1
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 *
 * Return: 0 on success
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	(void)argc;
	(void)argv;

	while (1)
	int main(int argc, char **argv, char **envp)
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        printf("$ ");
        fflush(stdout);

        ssize_t read = getline(&line, &len, stdin);
        if (read == -1)  // Ctrl+D
        {
            printf("\n");
            break;
        }

        line[read - 1] = '\0';  // remove newline
        line = trim_spaces(line);  // remove leading/trailing spaces

        if (line[0] == '\0')  // empty input, just spaces
            continue;

        execute_command(line, envp);  // call your execve wrapper
    }

    free(line);
    return 0;
}
