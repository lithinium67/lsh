/* Copyright (C) 2025 Lithinium <lithinium67@gmail.com> */

#include <shell.h>

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

        size_t home_len = strlen(pw->pw_dir);

        if (strncmp(dir, pw->pw_dir, home_len) == 0) {
            if (dir[home_len] == '/' || dir[home_len] == '\0') {
                char buffer[MAX_BUFF];

                if (dir[home_len] == '\0') {
                    strcpy(buffer, "~");
                } else {
                    strcpy(buffer, "~");
                    strcat(buffer, dir + home_len);
                }

                strcpy(dir, buffer);
            }
        }

        // Making the prompt
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

        /* char* command = readline(prompt);

        add_history(command);
        */

        if (strcmp(command, "exit") == 0) {
            exit(0);
        }

        execCommand(command);

        // free(command);
    }

    return 2;  // whatever that break the loop it's a important error
}
