// Header File
#include "shell.h"

char *join(char **split, char *sep) {
    // Variable Declarations
    int i;

    // Allocating Enough Memory To Ret String
    char *ret = calloc(MESH_BUFFER_SIZE, sizeof(char));

    // Looping Through String Array
    for (i = 0; i < MESH_ARG_COUNT && split[i] != NULL; i++) {
        // Concatenating String
        ret = strncat(ret, split[i], MESH_BUFFER_SIZE);

        // Concatenating Separator
        ret = strncat(ret, sep, MESH_BUFFER_SIZE);
    }

    // Exiting Function
    return ret;
}

char **subarray(char **cmd, int l, int r) {
    // Variable Declaration
    int i, j = 0;

    // Allocating Memory
    char **ret = calloc(r-l+2, sizeof(char *));

    // Looping Through Arguments
    for (i = l; i <= r; i++, j++) {
        // Copying Arguments Over
        ret[j] = cmd[i];
    }

    // Setting Last Value Null
    ret[r-l+1] = NULL;

    // Exiting Function
    return ret;
}

char **parse_line(char *str, char *sep) {
    // Variable Declarations
    char *copy = str, *token;
    int i = 0;

    // Allocating Argument Array
    char **A = calloc(MESH_ARG_COUNT, sizeof(char *));

    // Adding To Argument Array
    for (token = strtok(copy, sep); i < MESH_ARG_COUNT-1 && token; token = strtok(NULL, sep)) {
        // Only Adding None NULL Token
        if (*token != '\0') A[i++] = token;
    }

    // Looping Through Rest Of Array
    while (i < MESH_ARG_COUNT) {
        // Handling Empty Spaces
        A[i++] = NULL;
    }

    // Exiting Function
    return A;
}

char ***parse_input(char *input) {
    // Variable Declaration
    int i;

    // Splitting Lines
    char **lines = parse_line(input, ";");

    // Allocating Memory
    char ***cmds = calloc(MESH_ARG_COUNT, sizeof(lines));

    // Looping Through Lines
    for (i = 0; i < MESH_ARG_COUNT; i++) {
        // If Line Is NULL Make Command NULL As Well
        if (lines[i] == NULL) cmds[i] = NULL;

        // Parse Line For Pipes
        else cmds[i] = parse_line(lines[i], "|");
    }

    // Freeing Memory
    free(lines);

    // Exiting Function
    return cmds;
}
