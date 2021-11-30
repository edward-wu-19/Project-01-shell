#include "shell.h"

// String To Store User Input
char *mesh_input;

// String To Store Parsed Commands
char ***mesh_cmds;

// Array To Store Past Commands
char *mesh_hist[MESH_HISTORY_SIZE];

// Indexing Variable
int mesh_index = 0;

// Function: Obtains the current index of the mesh_hist array, created to sother areas in the executable can be guaranteed access to the index
// Arguments: None
// Return Values: An integer representing the current index of the history array
int get_mesh_index() {
    // Returning Global Variable Mesh_Index
    return mesh_index;
}

// Function: Parses user input and replaces commands in such a way as to prevent infinite loops and other errors from occurring when previous accesses previous commands
// Arguments: A string representing user input
// Return Values: Void
void add_event(char *input) {
    // Variable Declaration
    int i, q;

    // Spliting Input Into Lines
    char **split = parse_line(strdup(input), ";");

    // Looping Through Commands
    for (i = 0; i < MESH_ARG_COUNT && split[i] != NULL; i++) {
        // Parsing Command Into Separate Arguments
        char **cmd = parse_line(strdup(split[i]), " \t");

        // Looping Through Args To Find Previous
        for (q = 0; q < MESH_ARG_COUNT && cmd[q] != NULL; q++) {
            // If Current Command Is Previous
            if (strcmp(cmd[q], "previous") == 0) {
                // If Next Command Is Valid Number
                if (cmd[q+1] != NULL && check_digits(cmd[q+1])) {
                    // Variable Declarations
                    int pos, num;

                    // Convert The String Into An Integer
                    num = (int)strtol(cmd[q+1], NULL, 10);

                    // If Num Is Negative Make It Positive To Prevent Infinite Loops
                    // (Ie Previous -1 Two Times In A Row)
                    if (num < 0) {
                        // If Number Is In Range
                        if (num > -mesh_index) {
                            // Making Positive
                            pos = mesh_index+num;
                        } else {
                            // Making It Outrageously Negative So It Will Never Run Even Later On
                            // Preventing More Infinite Negative Loops Later On
                            pos = -999999;
                        }

                        // Initializing String
                        char str[10] = "\0";

                        // Converting Integer Into String
                        sprintf(str, "%d", pos);

                        // Replacing Command String
                        cmd[q+1] = str;
                    }
                } else if (cmd[q+1] == NULL) {
                    // Initializing String
                    char str[10] = "\0";

                    // Converting Integer Into String
                    sprintf(str, "%d", mesh_index-1);

                    // Concatenating This Number To Previous
                    cmd[q] = strncat(cmd[q], " ", 2);
                    cmd[i] = strncat(cmd[q], str, 10);

                }
            }
        }

        char *joined_cmd = join(cmd, " ");

        split[i] = joined_cmd;
    }

    // Combining String Again
    char *joined_lines = join(split, ";");

    // Adding Event To Mesh_Hist And Updating Mesh_Index
    mesh_hist[mesh_index++] = strdup(joined_lines);

    // Freeing Memory
    free(joined_lines);

    // Exiting Function
    return;
}

// Function: Obtains a previous command from the history array
// Arguments: An integer representing the index of the command to be obtained
// Return Values: A string representing the previous comand that is obtained
char *get_event(int i) {
    // Checking If Event Is In Bounds
    if (i < 0 || i >= mesh_index) {
        // If Out Of Bounds Print Error
        print_error(-1, "Unable To Access Event");

        // Exiting Function
        return NULL;
    } else {
        // If Not Out Of Bounds Return The Correct String
        return strdup(mesh_hist[i]);
    }
}

// Function: Frees most of the relevant data of this section
// Arguments: None
// Return Values: None
void free_all() {
    // Freeing Memory
    free(mesh_input);
    free(mesh_cmds);

    // Exiting Function
    return;
}

// Function: Prints a customized error message along with errno if an error occurs
// Arguments: An integer representing an err value and a string representing the customized error message
// Return Values: None
void print_error(int err, char *msg) {
    // Checking If There Is An Error
    if (err == -1) {
        // Printing Program Error Message
        printf("Error: %s\n", msg);

        // Printing Errno
        printf("%s\n", strerror(errno));
    }

    // Exiting Function
    return;
}

// Function: Simulates the entire shell process using a while loop
// Arguments: Default argument count and array even though they aren't used
// Return Values: An integer that is the exit value of the program
int main(int argc, char *argv[]) {
    // Clearing Terminal Screen
    clrscr();

    // Printing Welcome Message
    printf("%sWelcome To MESH By Mohammad Khan And Edward Wu\n%s", MESH_CYAN, MESH_RESET);

    // Setting The 0th Command
    mesh_hist[mesh_index++] = strdup("history");

    // While There Are Inputs Run Shell
    while (!feof(stdin)) {
        // Printing Header
        print_header();

        // Getting User Input
        mesh_input = get_input();

        // If Input Is Null Dont Try To Parse Or Run
        if (mesh_input == NULL) continue;

        // Add Input To History
        add_event(strdup(mesh_input));

        // Parse Input Into Commands
        mesh_cmds = parse_input(mesh_input);

        // Fork And Run Commands
        execute_cmds(mesh_cmds);

        // Freeing Memory
        free_all();
    }

    // Exiting Function
    return 0;
}
