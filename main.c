#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
	{
		print_prompt();
		nread = getline(&line, &len, stdin);
		if (nread == -1) /* EOF (Ctrl+D) */
		{
			printf("\n");
			break;
		}

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0')
			continue;

		execute_command(line, envp);
	}

	free(line);
	return (0);
}
