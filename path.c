#include "shell.h"

/**
 * find_path - searches for a command in the PATH
 * @command: command name
 *
 * Return: malloc'ed full path if found, NULL otherwise
 */
char *find_path(char *command)
{
	char *path_env = NULL;
	char *path_copy;
	char *dir;
	char *full;
	size_t i;
	int cmd_len, dir_len;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path_env = environ[i] + 5;
			break;
		}
	}
	if (path_env == NULL || *path_env == '\0')
		return (NULL);

	path_copy = strdup(path_env);
	if (path_copy == NULL)
		return (NULL);

	cmd_len = _strlen(command);
	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		dir_len = _strlen(dir);
		full = malloc(dir_len + 1 + cmd_len + 1);
		if (full == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		memcpy(full, dir, dir_len);
		full[dir_len] = '/';
		memcpy(full + dir_len + 1, command, cmd_len);
		full[dir_len + 1 + cmd_len] = '\0';

		if (access(full, X_OK) == 0)
		{
			free(path_copy);
			return (full);
		}

		free(full);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
.
