#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define PROMPT "#cisfun$ "

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status;

    while (1)
    {
        /* Display prompt only if input is from terminal */
        if (isatty(STDIN_FILENO))
            printf("%s", PROMPT);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            /* Handle Ctrl+D (EOF) */
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Remove trailing newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Ignore empty input */
        if (line[0] == '\0')
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("Error:");
            continue;
        }

        if (pid == 0)
        {
            char *argv[2];
	    argv[0] = line;
	    argv[1] = NULL;
	    
	    if (execve(line, argv, environ) == -1)
	    {
		    perror("./shell");
		    exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Parent waits for child */
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
