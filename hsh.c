#include "shell.h"

static int run_command(char *prog_name, char **tokens, int cmd_no);
static int shell_loop(char *prog_name);

/**
 * main - entry point of the shell
 * @ac: argument count
 * @av: argument vector
 *
 * Return: exit status of last command
 */
int main(int ac, char **av)
{
	int status;

	(void)ac;
	status = shell_loop(av[0]);
	return (status);
}

/**
 * shell_loop - main read/execute loop of the shell
 * @prog_name: name of the shell program (argv[0])
 *
 * Return: exit status of last executed command
 */
static int shell_loop(char *prog_name)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **tokens;
	int cmd_no = 0;
	int last_status = 0;

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

		cmd_no++;
		last_status = run_command(prog_name, tokens, cmd_no);
	}

	free(line);
	return (last_status);
}

/**
 * run_command - resolves command (PATH/full path) and runs it
 * @prog_name: name of the shell program (argv[0])
 * @tokens: NULL-terminated array of arguments
 * @cmd_no: command number (for error messages)
 *
 * Return: exit status code (127 when not found)
 */
static int run_command(char *prog_name, char **tokens, int cmd_no)
{
	char *cmd_path = NULL;
	int need_free = 0;
	pid_t pid;
	int status;

	if (strchr(tokens[0], '/') != NULL)
	{
		if (access(tokens[0], X_OK) == 0)
			cmd_path = tokens[0];
	}
	else
	{
		cmd_path = find_path(tokens[0]);
		if (cmd_path != NULL)
			need_free = 1;
	}

	if (cmd_path == NULL)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, cmd_no, tokens[0]);
		free_tokens(tokens);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (need_free)
			free(cmd_path);
		free_tokens(tokens);
		return (1);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, tokens, environ) == -1)
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

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (1);
}

