#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define PROMPT "#cisfun$ "
#define MAX_LINE 1024  /* optional if you switch to fgets */

/* Function prototypes */
ssize_t get_input(char **line, size_t *len);
void execute_command(char *line);

#endif /* SHELL_H */
