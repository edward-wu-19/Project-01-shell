#include "shell.h"

char **parse(char *str, char *sep) {
    // Declarations
    char *copy = str, *token;
    int i = 0;

    // Allocating Argument Array
    char **A = calloc(MESH_ARG_COUNT, sizeof(char *));

    // Adding To Array
    while (token = strsep(&copy, sep)) 
        if (*token != '\0') A[i++] = token;
    while (i < MESH_ARG_COUNT)
        A[i++] = NULL;

    // Exiting Function
    return A;
}

char ***parse_input(char *input) {
    // Splitting Lines
    char **lines = parse(input, ";");

    // Splitting Args
    char ***cmds = calloc(MESH_ARG_COUNT, sizeof(lines));
    int i = 0, j = 0;
    for (; i < MESH_ARG_COUNT; i++) {
        if (lines[i] == NULL) cmds[i] = NULL;
        else cmds[i] = parse(lines[i], " ");
    }

    // Freeing Lines
    free(lines);

    // Exiting Function
    return cmds;
}
