/* Copyright (C) 2025 Lithinium <lithinium67@gmail.com> */

#include <command.h>

void execCommand(char* command) {
    // Divide command by args

    char* args[MAX_BUFF];
    int argc = 0;

    char* token = strtok(command, " ");

    while (token != NULL && argc < MAX_BUFF - 1) {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }

    args[argc] = NULL;  // last arg always has to be NULL

    // cd
    if (argc > 0 && strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) != 0) perror("cd");
        return;
    }

    // new Fork

    int id = fork();

    if (id == 0) {
        signal(SIGINT, SIG_DFL);  // allow CTRL+C

        int result = execvp(args[0], args);

        if (result == -1) {
            fprintf(stderr, "\'%s\' command not found\n", args[0]);
            exit(1);  // close fork
        }

    } else if (id > 0) {
        waitpid(id, NULL, 0);
        return;
    } else {
        fprintf(stdout, "FORK ERROR");
        exit(-1);
    }
}
