#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/**
 * trim_spaces - remove leading and trailing spaces from a string
 * @str: the string to trim
 *
 * Return: pointer to trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	if (str == NULL)
		return (NULL);

	/* Remove leading spaces */
	while (isspace((unsigned char)*str))
		str++;

	if (*str == '\0')
		return (str);

	/* Remove trailing spaces */
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * execute_command - fork and execute a command
 * @cmd: command to execute
 * @envp: environment variables
 *
 * Return: 0 on success, 1 on failure
 */
int execute_command(char *cmd, char **envp)
{
	pid_t pid;
	int status;
	char *args[2];

	if (cmd == NULL || *cmd == '\0')
		return (1);

	args[0] = cmd;
	args[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		execve(cmd, args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return (0);
}

/**
 * read_line - read a line from stdin
 *
 * Return: pointer to the line, caller must free it
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	/* Remove trailing newline */
	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}

/**
 * main - simple shell main loop
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 *
 * Return: 0 on success
 */
int main(int argc, char **argv, char **envp)
{
	char *line;
	char *cmd;

	(void)argc;
	(void)argv;

	while (1)
	{
		write(1, "$ ", 2);
		line = read_line();
		if (line == NULL)
			break;

		cmd = trim_spaces(line);
		if (*cmd != '\0')
			execute_command(cmd, envp);

		free(line);
	}

	write(1, "\n", 1);
	return (0);
}
