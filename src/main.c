#include <shell.h>

void changeDir(char* path){
    if (chdir(path) != 0){
        perror("cd");
    }
}

void execCommand(char* command){

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
    if (argc > 0 && strcmp(args[0], "cd") == 0){
        changeDir(args[1]);
        return;
    }

    // new Fork

    int id = fork();

    if (id == 0){

        int result = execvp(args[0], args);
            
        if(result == -1){
            fprintf(stderr, "\'%s\' command not found\n", args[0]);
            exit(1); // close fork
        }

    } else if (id > 0){
        waitpid(id, NULL, 0);
        return;
    } else {
        fprintf(stdout, "FORK ERROR");
        exit(-1);
    }
}

int main(void){

    while (1){ // MAIN LOOP
        char command[MAX_BUFF];

        write(1, "# ", 2); // PS1

        int readBytes = read(0, command, MAX_BUFF); // STDIN
        
        command[readBytes - 1] = 0;

        if (strcmp(command, "exit") == 0) {
            exit(0);
        }

        execCommand(command);
    }

}
