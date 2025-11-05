#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

/**
 * trim_spaces - removes leading and trailing spaces from a string
 * @str: string to trim
 * Return: pointer to trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	while (isspace((unsigned char)*str))
		str++;

	if (*str == 0)
		return str;

	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;

	end[1] = '\0';

	return str;
}

/**
 * main - simple shell
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

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		cmd = trim_spaces(line);
		if (cmd[0] == '\0')
			continue;

		if (strcmp(cmd, "exit") == 0)
			break;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		if (pid == 0)
		{
			/* Child process */
			char *argv_exec[2];

			argv_exec[0] = cmd;
			argv_exec[1] = NULL;

			execve(cmd, argv_exec, envp);

			/* execve failed */
			write(2, argv[0], strlen(argv[0]));
			write(2, ": 1: ", 5);
			write(2, cmd, strlen(cmd));
			write(2, ": not found\n", 12);
			exit(EXIT_FAILURE);
		}
		else
		{
			/* Parent process */
			wait(&status);
		}
	}

	free(line);
	return 0;
}
