#include "shell.h"

// Global Variables
char *input, ***cmds;

void free_all() {
    // Freeing Memory
    free(input);
    free(cmds);

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
    // Clearing Screen And Printing Message
    clrscr();
    printf("%sWelcome To MESH By Mohammad Khan And Edward Wu\n%s", MESH_CYAN, MESH_RESET);

    // Forever While Loop Representing Shell
    while (1) {
        // Printing Header
        print_header();

        // Getting Input
        input = get_input();

        // Parse Input Into Commands
        cmds = parse_input(input);

        // Fork And Run Commands
        execute_cmds(cmds);

        // Freeing Memory
        free_all();
    }

    // Exiting Function
    return 0;
}
