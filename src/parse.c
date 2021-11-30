// Header File
#include "shell.h"

// Function: Concatenates multiple strings together using a certain string as a seperator
// Arguments: A string array that is to be concatenated and a separator that is going to be placed between each string in the string array given
// Return Values: A string representing the concatenated strings in the string array
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

// Function: Returns a new memory allocated subarray of a larger array
// Arguments: An array of strings representing a command and its arguments, along with two integers representing a left bound and a right bound
// Return Values: An array of strings that is a new memory allocated subarray of the original array parameter
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

// Function: Parses and splits a line of input depending on a cetain string of separators
// Arguments: The string to be parses, and a separator string that represents all separators to be used (usually ;, space, or \t)
// Return Values: A string array repesenting the original string after it has been split
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

// Function: Takes in user input and then parses it for semi-colons and pipe symbols and returns the results after parsing
// Arguments: A string representing user input
// Return Values: A 2d array of strings, first representing commands split by ;, then arguments split by |
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
