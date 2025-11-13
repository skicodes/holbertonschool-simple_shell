#include "shell.h"

/**
 * _strlen - returns the length of a string
 * @s: string
 *
 * Return: length of s
 */
int _strlen(const char *s)
{
	int len = 0;

	if (s == NULL)
		return (0);

	while (s[len] != '\0')
		len++;

	return (len);
}

/**
 * free_tokens - frees an array of tokens
 * @tokens: array of strings
 */
void free_tokens(char **tokens)
{
	size_t i;

	if (tokens == NULL)
		return;

	for (i = 0; tokens[i] != NULL; i++)
		free(tokens[i]);

	free(tokens);
}

/**
 * parse_line - splits a line into tokens
 * @line: input line
 *
 * Return: NULL-terminated array of strings, or NULL on failure
 */
char **parse_line(char *line)
{
	size_t bufsize = 16;
	size_t i = 0;
	char **tokens;
	char *token;

	tokens = malloc(sizeof(char *) * bufsize);
	if (tokens == NULL)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[i] = strdup(token);
		if (tokens[i] == NULL)
		{
			free_tokens(tokens);
			return (NULL);
		}
		i++;

		if (i >= bufsize)
		{
			char **tmp;

			bufsize *= 2;
			tmp = realloc(tokens, sizeof(char *) * bufsize);
			if (tmp == NULL)
			{
				free_tokens(tokens);
				return (NULL);
			}
			tokens = tmp;
		}

		token = strtok(NULL, " \t\r\n");
	}

	tokens[i] = NULL;
	return (tokens);
}
.
