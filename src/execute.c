#include "shell.h"

void execute(char **cmd) {
    if (strcmp(cmd[0],"exit") == 0 || strcmp(cmd[0],"quit") == 0) {
        printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);
        free_all(); exit(0);
    } else if (strcmp(cmd[0],"cd") == 0) {
        if (cmd[1] == NULL) printf("No Directory Provided\n");
        else {
            DIR *dir = opendir(cmd[1]);
            if (dir == NULL && errno == ENOENT) printf("Directory Doesn't Exist\n");
            else {
                print_error(chdir(cmd[1]), "Unable To Change Directory");
                closedir(dir);
            }
        }
    } else {
        int f = fork(), status;
        // Parent
        if (f) {
            print_error(f, "Unable To Fork Process");
            print_error(waitpid(f, &status, 0), "Unable To Wait For Child Process To End");
        } else { // Child
            print_error(execvp(cmd[0], cmd), "Unable To Run Command");
            free_all(); exit(0);
        }
    }

}

void execute_cmds(char ***cmds) {
    // Looping Through Commands
    int i = 0;
    for (; i < MESH_ARG_COUNT; i++) {
        if (cmds[i] == NULL) break;
        execute(cmds[i]);
    }

    // Exiting Function
    return;
}
