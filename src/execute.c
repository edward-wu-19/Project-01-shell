#include "shell.h"

void execute(char **cmd) {
    // Quit/Exit Command
    if (strcmp(cmd[0],"exit") == 0 || strcmp(cmd[0],"quit") == 0) {
        // Goodbye Message
        printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);

        // Freeing Memory And Exiting
        free_all(); exit(0);
    } else if (strcmp(cmd[0],"cd") == 0) { // CD Command
        // Checking For Second Argument
        if (cmd[1] == NULL) printf("No Directory Provided\n");
        else {
            // Checking If Directory Exists
            DIR *dir = opendir(cmd[1]);
            if (dir == NULL && errno == ENOENT) printf("Directory Doesn't Exist\n");
            else {
                // Changing Directory
                print_error(chdir(cmd[1]), "Unable To Change Directory");

                // Closing Variable
                closedir(dir);
            }
        }
    } else { // All Other Commands
        int f = fork(), status;
        // Parent
        if (f) {
            // Checking Forking
            print_error(f, "Unable To Fork Process");

            // Checking Waiting
            print_error(waitpid(f, &status, 0), "Unable To Wait For Child Process To End");
        } else { // Child
            // Checking Running Command
            print_error(execvp(cmd[0], cmd), "Unable To Run Command");

            // Ending Child Process
            free_all(); exit(0);
        }
    }

}

void execute_cmds(char ***cmds) {
    // Looping Through Commands
    int i = 0;
    for (; i < MESH_ARG_COUNT; i++) {
        // Checking If Commands Exist
        if (cmds[i] == NULL) break;
        
        // Running Commands
        execute(cmds[i]);
    }

    // Exiting Function
    return;
}
