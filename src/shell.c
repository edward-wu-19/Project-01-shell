#include "shell.h"

int main(int argc, char *argv[]) {
    char input[1024];

    // Print Current Working Directory
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s MESH> ", cwd);

    int w, status;

    // Forever While Loop Representing Shell
    while (1) {

        if (argc == 1){
            // Take In Commands
            fgets(input, 1024, stdin);
            *strchr(input, '\n') = '\0';

            // Parse Commands
            char ***commands = parse_line(input);
            if (errno){
                printf("Error: %s\n\n", strerror(errno));
            }
            // printf("%s\n", *commands[0]);

            int f = fork();
            if (f){
                w = wait(&status);
            }
            else{
                execvp(*commands[0], *commands);
            }

            // Print Current Working Directory
            getcwd(cwd, sizeof(cwd));
            printf("%s MESH> ", cwd);
        }

        // Run Prgram From SRC

    }

    // Ending Statement
    return 0;
}
