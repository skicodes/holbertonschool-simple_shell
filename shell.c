#include "shell.h"

/**
 * prompt - يطبع العلامة ويقرأ الأمر من المستخدم
 */
void prompt(void)
{
    char *line = NULL, *command = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    int status;
    char *cmd_path;

    while (1)
    {
        printf(":) ");
        read = getline(&line, &len, stdin);

        if (read == -1)
        {
            printf("\n");
            break;
        }

        /* إزالة السطر الجديد */
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        cmd_path = find_command_in_path(line);
        if (cmd_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", line);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            execl(cmd_path, line, (char *)NULL);
            perror("exec error");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork failed");
        }

        free(cmd_path);
    }

    free(line);
}

/**
 * main - دالة البداية
 */
int main(void)
{
    prompt();
    return 0;
}
