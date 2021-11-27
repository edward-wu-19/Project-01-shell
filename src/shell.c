#include "shell.h"

// TODO
//  - Piping

// Global Variables
char *mesh_input, ***mesh_cmds;
char *mesh_hist[MESH_HISTORY_SIZE];
int mesh_index = 0;

int get_mesh_index() {
    // Returning Global Variable Mesh_Index
    return mesh_index;
}

void add_event(char *input) {
    // Adding Event To Mesh_Hist And Updating Mesh_Index
    mesh_hist[mesh_index++] = strdup(input);
}

char *get_event(int i) {
    // Checking If Event Is In Bounds
    if (i < 0 || i >= mesh_index) {
        // If Out Of Bounds Return NULL
        print_error(-1, "Unable To Access Event");
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
    // Printing Error Messages
    if (err == -1) {
        printf("Error: %s\n", msg);
        printf("%s\n", strerror(errno));
    }

    // Exiting Function
    return;
}

int main(int argc, char *argv[]) {
    // Clearing Screen
    clrscr();

    // Printing And Saving Welcome Message
    printf("%sWelcome To MESH By Mohammad Khan And Edward Wu\n%s", MESH_CYAN, MESH_RESET);
    mesh_hist[mesh_index++] = strdup("Welcome To MESH By Mohammad Khan And Edward Wu");

    // Forever While Loop Representing Shell
    while (1) {
        // Printing Header
        print_header();

        // Getting Input
        mesh_input = get_input();

        // Add Mesh_Input To History
        add_event(mesh_input);

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
