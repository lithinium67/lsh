/* Copyright (C) 2025 Lithinium <lithinium67@gmail.com> */

#include <shell.h>

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
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
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

int main(void) {
    signal(SIGINT, SIG_IGN);  // ignore CTRL+C

    FILE* fhostname = fopen("/etc/hostname", "r");

    char hostname[MAX_BUFF];

    fgets(hostname, sizeof(hostname), fhostname);

    hostname[strlen(hostname) - 1] = 0;  // quit \n

    fclose(fhostname);

    // Main Loop
    while (1) {
        // Get user data
        struct passwd* pw = getpwuid(getuid());  // It is not necessary to do a free,
        if (pw == NULL) perror("getpwuid");

        char* user = pw->pw_name;

        char dir[MAX_BUFF];
        if (getcwd(dir, sizeof(dir)) == NULL) perror("getcwd");

        // Making the Prompt
        char prompt[strlen(user) + strlen(dir) + 3];  // 3 is for "@" & "# "

        strcpy(prompt, user);
        strcat(prompt, "@");
        strcat(prompt, hostname);
        strcat(prompt, ":");
        strcat(prompt, dir);
        strcat(prompt, "# ");

        // Real Shell
        char command[MAX_BUFF];

        write(1, prompt, strlen(prompt));

        int readBytes = read(0, command, MAX_BUFF);  // STDIN

        command[readBytes - 1] = 0;

        if (strcmp(command, "exit") == 0) {
            exit(0);
        }

        execCommand(command);
    }

    return 2;  // whatever that break the loop it's a important error
}
