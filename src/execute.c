#include "shell.h"

bool EX_FOUND_SIGINT = false;
int frk, status;

void ex_sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) {
        if (frk == 0) { // Child
            free_all();
            exit(signo);
        } else { // Parent
            EX_FOUND_SIGINT = true;
        }
    }
}

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
        strncpy(path, get_home_dir(), MESH_BUFFER_SIZE);
    }

    // Checking If Directory Exists
    DIR *dir = opendir(path);
    if (dir == NULL) print_error(-1,"Unable To Find Directory");
    else {
        // Changing Directory
        print_error(chdir(path), "Unable To Change Directory");

        // Closing Variable
        closedir(dir);
    }

    // Exiting Function
    return;
}

void piping(char **cmd) {
    printf("This will do piping eventually");
}

void redirect(char **cmd) {
    // Backups For Stdin And Stdout
    int stdout_cpy = dup(STDOUT_FILENO), stdin_cpy = dup(STDIN_FILENO);
    if (stdout_cpy == -1 || stdin_cpy == -1) {
        print_error(-1, "Unable To Create Redirection Backups");
        return;
    }

    // Creating Combined Input File
    int input = open(MESH_DATA_DUMP, O_CREAT | O_RDWR | O_APPEND, 0644), i, end = 0;
    if (input == -1) {
        print_error(-1, "Unable To Create Combined Input File");
        return;
    }

    // Finding Command And Arguments
    for (i = 0; i < MESH_ARG_COUNT && cmd[i] != NULL; i++) {
        if (strcmp(cmd[i], "<") == 0) {end = i; break;}
        if (strcmp(cmd[i], ">") == 0) {end = i; break;}
        if (strcmp(cmd[i], ">>") == 0) {end = i; break;}
    }

    // Checking For Valid Command
    if (end == 0) {
        print_error(-1, "Unable To Run Nonexistent Command");
        return;
    }

    // Getting Command
    char **scmd = subarray(cmd, 0, end-1);

    // Combining All Inputs
    for (i = 0; i < MESH_ARG_COUNT && cmd[i] != NULL; i++) {
        // Checking For Redirection Input Operator
        if (strcmp(cmd[i], "<") == 0) {
            // Checking If File Name Is Given
            char *name = cmd[i+1];
            if (cmd[i+1] == NULL) {
                print_error(-1, "Unable To Open Nonexistent Input File");
                continue;
            }

            // Opening File
            int fd = open(name, O_RDONLY);
            if (fd == -1) {
                print_error(-1, "Unable To Open Input File");
                continue;
            }

            // Getting File Size
            struct stat info;
            int err1 = stat(name, &info);
            if (err1 == -1) {
                print_error(-1, "Unable To Get File Size");
                continue;
            }
            int sz = info.st_size;

            // Reading File
            char arr[sz];
            int err3 = read(fd, arr, sz);
            if (err3 == -1) {
                print_error(-1, "Unable To Read Input File");
                continue;
            }

            // Appending To Combined Input File
            int err4 = write(input, arr, sz);
            if (err4 == -1) {
                print_error(-1, "Unable To Append To Combined Input File");
                continue;
            }

            // Closing Input File;
            int err5 = close(fd);
            if (err5 == -1) {
                print_error(-1, "Unable To Close Input File");
                continue;
            }
        }
    }

    // Redirecting Input
    int err1 = dup2(input, STDIN_FILENO);
    if (err1 == -1) {
        print_error(-1, "Unable To Redirect Input");
        return;
    }

    // Bool To Check For Output Files
    bool found_output = false;

    // Looping Through To Create Output Files
    for (i = 0; i < MESH_ARG_COUNT && cmd[i] != NULL; i++) {
        // Checking For Redirection Output Operator
        if (strcmp(cmd[i], ">") == 0 || strcmp(cmd[i],">>") == 0) {
            // Checking If File Name Is Given
            char *name = cmd[i+1];
            if (cmd[i+1] == NULL) {
                print_error(-1, "Unable To Open When No Output File Given");
                continue;
            }

            // Creating File
            int fd = open(name, O_CREAT, 0644);
            if (fd == -1) {
                print_error(-1, "Unable To Open Output File");
                continue;
            }

            // Closing File
            int err = close(fd);
            if (err == -1) {
                print_error(-1, "Unable To Close Output File");
                continue;
            }

            found_output = true;
        }
    }

    if (found_output) {
        // Looping Through To Add To Output Files
        for (i = 0; i < MESH_ARG_COUNT && cmd[i] != NULL; i++) {
            // Checking For Redirection Output Operator
            if (strcmp(cmd[i], ">") == 0 || strcmp(cmd[i], ">>") == 0) {
                // Setting Permissions
                int permissions = O_WRONLY | O_CREAT;
                if (strcmp(cmd[i], ">>") == 0) permissions |= O_APPEND;
                else permissions |= O_TRUNC;

                // Checking If File Name Is Given
                char *name = cmd[i+1];
                if (cmd[i+1] == NULL) {
                    print_error(-1, "Unable To Open When No Output File Given");
                    continue;
                }

                // Opening File
                int fd = open(name, permissions, 0644);
                if (fd == -1) {
                    print_error(-1, "Unable To Open Output File");
                    continue;
                }

                // Redirecting Output
                int err = dup2(fd, STDOUT_FILENO);
                if (err == -1) {
                    print_error(-1, "Unable To Redirect Output");
                    continue;
                }

                // Reseting Input File Descriptor
                int err1 = lseek(input, 0, SEEK_SET);
                if (err1 == -1) {
                    print_error(-1, "Unable To Reset Combined Input File");
                    return;
                }

                // Running Command
                execute(scmd);
            }
        }
    } else execute(scmd);

    // Resetting STDOUT
    int err2 = dup2(stdout_cpy, STDOUT_FILENO);
    if (err2 == -1) {
        print_error(-1, "Unable To Reset STDOUT");
        exit(0);
    }

    // Resetting STDIN
    int err3 = dup2(stdin_cpy, STDIN_FILENO);
    if (err3 == -1) {
        print_error(-1, "Unable To Reset STDIN");
        exit(0);
    }

    // Deleting Combined Input File
    print_error(remove(MESH_DATA_DUMP), "Unable To Remove Combined Input File");
}

// Running Regular Commands
void execute(char **cmd) {
    // Forking Process
    frk = fork();

    // Parent
    if (frk) {
        // Checking Forking
        print_error(frk, "Unable To Fork Process");

        // Checking Waiting
        print_error(waitpid(frk, &status, 0), "Unable To Wait For Child Process To End");
    } else { // Child
        // Check Running Command
        print_error(execvp(cmd[0], cmd), "Unable To Run Command");

        // Ending Child Process
        free_all(); exit(0);
    }

    // Exiting Function
    return;
}

void execute_cmd(char **cmd) {
    // Catching Signals
    signal(SIGINT, ex_sighandler);

    // Booleans Used Later
    bool found_redirection = false, found_piping = false;

    // Looping Through To Find Redirects And Pipes
    int i;
    for (i = 0; i < MESH_ARG_COUNT && cmd[i] != NULL; i++) {
        if (strcmp(cmd[i], ">") == 0) found_redirection = true;
        if (strcmp(cmd[i], "<") == 0) found_redirection = true;
        if (strcmp(cmd[i], ">>") == 0) found_redirection = true;
        if (strcmp(cmd[i], "|") == 0) found_piping = true;
    }

    // Checking If Piping And Redirection Are Occuring On Same Line
    if (found_redirection && found_piping)
        // Not Functional
        print_error(-1, "Unable To Redirect And Pipe In The Same Command");
    else if (found_redirection)
        // Redirect Function
        redirect(cmd);
    else if (found_piping)
        // Piping Function
        piping(cmd);
    else
        // Regular Command
        execute(cmd);

    // Exiting Function
    return;
}

void execute_cmds(char ***cmds) {
    // Setting FOUND_SIGINT To False
    EX_FOUND_SIGINT = false;

    // Looping Through Commands
    int i = 0;
    for (; i < MESH_ARG_COUNT; i++) {
        // Checking If SIGINT Has Been Found
        if (EX_FOUND_SIGINT) break;

        // Checking If Commands Exist
        if (cmds[i] == NULL) break;

        // Running Commands
        char **cmd = cmds[i];

        // Quit/Exit Command
        if (strcmp(cmd[0],"exit") == 0 || strcmp(cmd[0],"quit") == 0) shell_exit();

        // Cd/Chdir Command
        else if (strcmp(cmd[0],"cd") == 0 || strcmp(cmd[0],"chdir") == 0) shell_cd(cmd[1]);

        // All Other Commands
        else execute_cmd(cmd);
    }

    // Exiting Function
    return;
}
