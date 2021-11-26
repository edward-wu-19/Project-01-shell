#include "shell.h"

void shell_exit() {
    // Goodbye Message
    printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);

    // Freeing Memory
    free_all();

    // Exiting Function
    exit(0);
}

void shell_cd(char *path) {
    // If NULL OR ~, Convert To Home Directory
    if (path == NULL || strcmp(path, "~") == 0) {
        path = realloc(path, sizeof(char *));
        strncpy(path, get_home_dir(), MESH_BUFFER_SIZE);
    }

    // Checking If Directory Exists
    DIR *dir = opendir(path);
    if (dir == NULL && errno == ENOENT) print_error(-1, "Directory Doesn't Exist");
    else {
        // Changing Directory
        print_error(chdir(path), "Unable To Change Directory");

        // Closing Variable
        closedir(dir);
    }

    // Exiting Function
    return;
}

void execute(char **cmd) {
    // Forking Process
    int f = fork(), status;

    // Parent
    if (f) {
        // Checking Forking
        print_error(f, "Unable To Fork Process");

        // Checking Waiting
        print_error(waitpid(f, &status, 0), "Unable To Wait For Child Process To End");
    } else { // Child
        // Check Running Command
        print_error(execvp(cmd[0], cmd), "Unable To Run Command");

        // Ending Child Process
        free_all(); exit(0);
    }

    // Exiting Function
    return;
}

void execute_cmds(char ***cmds) {
    // Looping Through Commands
    int i = 0;
    for (; i < MESH_ARG_COUNT; i++) {
        // Checking If Commands Exist
        if (cmds[i] == NULL) break;

        // Running Commands
        char **cmd = cmds[i];

        // Quit/Exit Command
        if (strcmp(cmd[0],"exit") == 0 || strcmp(cmd[0],"quit") == 0) shell_exit();

        // Cd/Chdir Command
        else if (strcmp(cmd[0],"cd") == 0 || strcmp(cmd[0],"chdir") == 0) shell_cd(cmd[1]);

        // All Other Commands
        else execute(cmd);
    }

    // Exiting Function
    return;
}
