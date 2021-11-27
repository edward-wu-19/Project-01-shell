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

void mesh_exit(char **cmd) {
    // Counting Arguments
    int i = 0;
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 1) print_error(-1, "mesh_exit: Unable To Use More Than 0 Arguments");

    // Goodbye Message
    printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);

    // Freeing Memory
    free_all();

    // Exiting Function
    exit(0);
}

void mesh_cd(char **cmd) {
    // Counting Arguments
    int i = 0;
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) print_error(-1, "mesh_cd: Unable To Use More Than 1 Arguments");

    // If NULL OR ~, Convert To Home Directory
    char *path;
    if (cmd[1] == NULL || strcmp(cmd[1], "~") == 0) path = get_home_dir();
    else path = strdup(cmd[1]);

    // Checking If Directory Exists
    DIR *dir = opendir(path);
    if (dir == NULL) print_error(-1,"mesh_cd: Unable To Find Directory");
    else {
        // Changing Directory
        print_error(chdir(path), "mesh_cd: Unable To Change Directory");

        // Closing Variable
        closedir(dir);
    }

    // Exiting Function
    return;
}

void mesh_history(char **cmd) {
    // Counting Arguments
    int i = 0;
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) print_error(-1, "mesh_history: Unable To Use More Than 1 Arguments");

    // Converting Argument Into Int
    int num;
    if (i >= 2) num = (int)strtol(cmd[1], NULL, 10);
    else num = -16;
    int mx = get_mesh_index();
    num = (num < 0 ? (mx+num < 0 ? 0 : mx+num) : num);

    // Checking If Out Of Range
    if (num >= mx) {
        print_error(-1, "mesh_history: Unable To Access Nonexistent Event");
        return;
    }

    // Concatenating All Events
    for (i = num; i < mx; i++) {
        printf(" %-5d  %s\n", i, get_event(i));
    }

    // Exiting Function
    return;
}

void mesh_prev(char **cmd) {
    // Counting Arguments
    int i = 0;
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) print_error(-1, "mesh_prev: Unable To Use More Than 1 Arguments");

    // Converting Argument Into Int
    int num;
    if (i >= 2) num = (int)strtol(cmd[1], NULL, 10);
    else num = 1;
    int mx = get_mesh_index();
    num = (num < 0 ? (mx+num < 0 ? 0 : mx+num) : num);

    // Checking If Out Of Range
    if (num >= mx) {
        print_error(-1, "Unable To Access Nonexistent Event");
        return;
    }

    // Running Previous Command
    char ***cmds = parse_input(get_event(num));
    execute_cmds(cmds);

    // Exiting Function
    free(cmds);
    return;
}

void piping(char **cmd) {
    printf("This will do piping eventually");
}

void redirect(char **cmd) {
    // Backups For Stdin And Stdout
    int stdout_cpy = dup(STDOUT_FILENO), stdin_cpy = dup(STDIN_FILENO);
    if (stdout_cpy == -1 || stdin_cpy == -1) {
        print_error(-1, "redirect: Unable To Create Redirection Backups");
        return;
    }

    // Creating Combined Input File
    int input = open(MESH_DATA_DUMP, O_CREAT | O_RDWR | O_APPEND, 0644), i, end = 0;
    if (input == -1) {
        print_error(-1, "redirect: Unable To Create Combined Input File");
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
        print_error(-1, "redirect: Unable To Run Nonexistent Command");
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
                print_error(-1, "redirect: Unable To Open Nonexistent Input File");
                continue;
            }

            // Opening File
            int fd = open(name, O_RDONLY);
            if (fd == -1) {
                print_error(-1, "redirect: Unable To Open Input File");
                continue;
            }

            // Getting File Size
            struct stat info;
            int err1 = stat(name, &info);
            if (err1 == -1) {
                print_error(-1, "redirect: Unable To Get File Size");
                continue;
            }
            int sz = info.st_size;

            // Reading File
            char arr[sz];
            int err3 = read(fd, arr, sz);
            if (err3 == -1) {
                print_error(-1, "redirect: Unable To Read Input File");
                continue;
            }

            // Appending To Combined Input File
            int err4 = write(input, arr, sz);
            if (err4 == -1) {
                print_error(-1, "redirect: Unable To Append To Combined Input File");
                continue;
            }

            // Closing Input File;
            int err5 = close(fd);
            if (err5 == -1) {
                print_error(-1, "redirect: Unable To Close Input File");
                continue;
            }
        }
    }

    // Redirecting Input
    int err1 = dup2(input, STDIN_FILENO);
    if (err1 == -1) {
        print_error(-1, "redirect: Unable To Redirect Input");
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
                print_error(-1, "redirect: Unable To Open When No Output File Given");
                continue;
            }

            // Creating File
            int fd = open(name, O_CREAT, 0644);
            if (fd == -1) {
                print_error(-1, "redirect: Unable To Open Output File");
                continue;
            }

            // Closing File
            int err = close(fd);
            if (err == -1) {
                print_error(-1, "redirect: Unable To Close Output File");
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
                    print_error(-1, "redirect: Unable To Open When No Output File Given");
                    continue;
                }

                // Opening File
                int fd = open(name, permissions, 0644);
                if (fd == -1) {
                    print_error(-1, "redirect: Unable To Open Output File");
                    continue;
                }

                // Redirecting Output
                int err = dup2(fd, STDOUT_FILENO);
                if (err == -1) {
                    print_error(-1, "redirect: Unable To Redirect Output");
                    continue;
                }

                // Reseting Input File Descriptor
                int err1 = lseek(input, 0, SEEK_SET);
                if (err1 == -1) {
                    print_error(-1, "redirect: Unable To Reset Combined Input File");
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
        print_error(-1, "redirect: Unable To Reset STDOUT");
        exit(0);
    }

    // Resetting STDIN
    int err3 = dup2(stdin_cpy, STDIN_FILENO);
    if (err3 == -1) {
        print_error(-1, "redirect: Unable To Reset STDIN");
        exit(0);
    }

    // Deleting Combined Input File
    print_error(remove(MESH_DATA_DUMP), "redirect: Unable To Remove Combined Input File");
}

// Running Regular Commands
void execute(char **cmd) {
    // Forking Process
    frk = fork();

    // Parent
    if (frk) {
        // Checking Forking
        print_error(frk, "execute: Unable To Fork Process");

        // Checking Waiting
        print_error(waitpid(frk, &status, 0), "execute: Unable To Wait For Child Process To End");
    } else { // Child
        // Check Running Command
        print_error(execvp(cmd[0], cmd), "execute: Unable To Run Command");

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

    // Not Functional
    if (found_redirection && found_piping) print_error(-1, "execute_cmd: Unable To Redirect And Pipe In The Same Command");

    // Redirect Function
    else if (found_redirection) redirect(cmd);

    // Piping Function
    else if (found_piping) piping(cmd);

    // Regular Command
    else execute(cmd);

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
        if (strcmp(cmd[0],"exit") == 0 || strcmp(cmd[0],"quit") == 0) mesh_exit(cmd);

        // Cd/Chdir Command
        else if (strcmp(cmd[0],"cd") == 0 || strcmp(cmd[0],"chdir") == 0) mesh_cd(cmd);

        // History Command
        else if (strcmp(cmd[0],"history") == 0) mesh_history(cmd);

        // Prev Command
        else if (strcmp(cmd[0],"prev") == 0) mesh_prev(cmd);

        // All Other Commands
        else execute_cmd(cmd);
    }

    // Exiting Function
    return;
}
