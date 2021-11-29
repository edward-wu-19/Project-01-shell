#include "shell.h"

// TODO
//  - Piping

// String To Store User Input
char *mesh_input;

// String To Store Parsed Commands
char ***mesh_cmds;

// Array To Store Past Commands
char *mesh_hist[MESH_HISTORY_SIZE];

// Indexing Variable
int mesh_index = 0;

int get_mesh_index() {
    // Returning Global Variable Mesh_Index
    return mesh_index;
}

// TODO FIX THIS FUNCTION (Error: ls -al    previous;) enter wrong history 
void add_event(char *input) {
    // Spliting Input Into Commands And Args
    char **split = parse_line(strdup(input), " \t");

    // Looping Through The Split Input To Find Previous Command
    for (int i = 0; i < MESH_ARG_COUNT && split[i] != NULL; i++) {
        // If Current Command Is Previous
        if (strcmp(split[i], "previous") == 0) {
            // If Next Command Is Valid Number
            if (split[i+1] != NULL && check_digits(split[i+1])) {
                // Variable Declaration
                int pos, num;

                // Convert The String Into An Integer
                num = (int)strtol(split[i+1], NULL, 10);

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
                    split[i+1] = str;
                }
            } else if (split[i+1] == NULL) {
                // Initializing String
                char str[10] = "\0";

                // Converting Integer Into String
                sprintf(str, "%d", mesh_index-1);

                // Concatenating This Number To Previous
                split[i] = strncat(split[i], " ", 1);
                split[i] = strncat(split[i], str, 10);

            }
        }
    }

    // Combining String Again
    char *joined = join(split, " ");

    // Adding Event To Mesh_Hist And Updating Mesh_Index
    mesh_hist[mesh_index++] = strdup(joined);

    // Freeing Memory
    free(joined);

    // Exiting Function
    return;
}

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

void free_all() {
    // Freeing Memory
    free(mesh_input);
    free(mesh_cmds);

    // Exiting Function
    return;
}

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
