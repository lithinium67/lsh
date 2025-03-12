/*
MIT License

Copyright (c) 2025 lithinium67

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

        signal(SIGINT, SIG_DFL); // allow CTRL+C

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

    signal(SIGINT, SIG_IGN); // ignore CTRL+C
    
    while (1){
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
