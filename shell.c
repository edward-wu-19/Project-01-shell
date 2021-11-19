#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void bin_compilation();
char **parse_args(char *line);

void bin_compilation() {
    DIR *cwd = opendir("bin");

}

char **parse_args(char *line) {
    // Figuring Out Argument Count by counting number of spaces
    char *copy = strdup(line), *token;
    int cnt = 1;
    while (token = strsep(&line, " ")) {
        cnt++;
    }

    // Creating Argument Array
    char **A = calloc(cnt, sizeof(char *));

    // Adding the pointers to the array
    int i = 0;
    while (token = strsep(&copy, " ")) {
        A[i] = token;
        i++;
    }

    // Returning Argument Array
    return A;
}

int main(int argc, char *argv[]) {
    char input[1024];
    // Making Sure All Binary Executables Are Created
    bin_compilation();

    // Forever While Loop Representing Shell
    while (1) {
        // Print Current Working Directory

        // Take In Commands
        if (argc == 1){
            fgets(input, 1024, stdin);
            *strchr(input, '\n') = '\0';
        }

        // Parse Commands
        char **args = parse_args(input);
        // printf("%s %s\n", args[0], args[1]);

        // Run Prgram From SRC

    }

    // Ending Statement
    return 0;
}
