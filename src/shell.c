#include "shell.h"

int main(int argc, char *argv[]) {
    // Forever While Loop Representing Shell
    while (1) {
        char input[1024];
        // Print Current Working Directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("Current working dir: %s\n", cwd);

        if (argc == 1){
            // Take In Commands
            fgets(input, 1024, stdin);
            *strchr(input, '\n') = '\0';

            // Parse Commands
            char ***commands = parse_line(input);
            if (errno){
                printf("Error: %s\n\n", strerror(errno));
            }
            // printf("%s %s\n", args[0], args[1]);
        }

        // Run Prgram From SRC

    }

    // Ending Statement
    return 0;
}
