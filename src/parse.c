#include "shell.h"

char **parse_command(char *line);
char ***parse_line(char *line);

char **parse_command(char *line) {
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

char ***parse_line(char *line){
    // Figuring Out number of lines by counting number of semicolons
    char *copy = strdup(line), *token;
    int cnt = 1;
    while (token = strsep(&line, ";")) {
        cnt++;
    }

    // Creating command array
    char ***A = calloc(cnt, sizeof(char **));

    // Adding pointers to array
    int i = 0;
    while (token = strsep(&copy, ";")) {
        A[i] = parse_command(token);
        i++;
    }

    // Returning Array
    return A;
}