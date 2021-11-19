#include "shell.h"

int main(int argc, char *argv[]) {
    // Forever While Loop Representing Shell
    while (1) {
        char input[1024];
        // Print Current Working Directory

        // Take In Commands
        if (argc == 1){
            fgets(input, 1024, stdin);
            *strchr(input, '\n') = '\0';
        }

        // Parse Commands
        char ***commands = parse_line(input);
        // printf("%s %s\n", args[0], args[1]);

        // Run Prgram From SRC

    }

    // Ending Statement
    return 0;
}
