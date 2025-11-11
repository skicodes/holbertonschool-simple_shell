#include "shell.h"

static int run_command(char *prog_name, char **tokens);
static void shell_loop(char *prog_name);

/**
 * run_command - handles PATH lookup, fork and exec for a command
 * @prog_name: name of the shell program (argv[0])
 * @tokens: NULL-terminated array of arguments
 *
 * Return: 0 on success, -1 on error
 */
static int run_command(char *prog_name, char **tokens)
{
	char *cmd_path;
	int need_free;
	pid_t pid;
	int status;

	if (strchr(tokens[0], '/') != NULL)
	{
		cmd_path = tokens[0];
		need_free = 0;
	}
	else
	{
		cmd_path = find_path(tokens[0]);
		need_free = cmd_path != NULL;
	}
	if (cmd_path == NULL)
	{
		perror(prog_name);
		free_tokens(tokens);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (need_free)
			free(cmd_path);
		free_tokens(tokens);
		return (-1);
	}
	if (pid == 0)
	{
		execve(cmd_path, tokens, environ);
		perror(prog_name);
		if (need_free)
			free(cmd_path);
		free_tokens(tokens);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (need_free)
		free(cmd_path);
	free_tokens(tokens);
	return (0);
}

/**
 * shell_loop - main read/execute loop of the shell
 * @prog_name: name of the shell program (argv[0])
 */
static void shell_loop(char *prog_name)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **tokens;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;
		if (nread > 1 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		tokens = parse_line(line);
		if (tokens == NULL || tokens[0] == NULL)
		{
			free_tokens(tokens);
			continue;
		}
		run_command(prog_name, tokens);
	}
	free(line);
}

/**
 * main - entry point of the shell
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	(void)ac;
	shell_loop(av[0]);
	return (0);
}

