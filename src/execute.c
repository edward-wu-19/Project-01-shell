// Header File
#include "shell.h"

// Signal Found Or Not
bool EX_FOUND_SIGINT = false;

// Fork Results
int frk;

// Status Results
int status;

void ex_sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) {
        // Child
        if (frk == 0) {
            // Freeing Memory
            free_all();

            // Exiting Child Program
            exit(signo);
        } else { // Parent
            // Informing Other Functions To Stop
            EX_FOUND_SIGINT = true;
        }
    }

    // Exiting Function
    return;
}

void mesh_exit(char **cmd) {
    // Variable Declarations
    int i = 0, num;

    // Counting Arguments
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) {
        print_error(-1, "mesh_exit: Unable To Use More Than 1 Arguments (A Number)");
        return;
    }

    // Sending Warning If First Argument Is Not A Number
    if (cmd[1] != NULL && !check_digits(cmd[1])) {
        print_error(-1, "mesh_exit: Unable To Use Non-Numeric First Argument");
        return;
    }

    // Converting First Argument Into Integer If It Exists And Is Numeric
    if (i >= 2) num = (int)strtol(cmd[1], NULL, 10);

    // If No Argument Given Do Default Value
    else num = 0;

    // Goodbye Message
    printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);

    // Freeing Memory
    free_all();

    // Exiting Program
    exit(num);
}

void mesh_cd(char **cmd) {
    // Variable Declarations
    int i = 0, j = 0;

    // Allocating Memory
    char *path = calloc(MESH_BUFFER_SIZE, sizeof(char));

    // Counting Arguments
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) {
        print_error(-1, "mesh_cd: Unable To Use More Than 1 Argument (A Directory Path)");
        return;
    }

    // If NULL Convert To Home Directory
    if (cmd[1] == NULL) strncpy(path, get_home_dir(), MESH_BUFFER_SIZE);

    // Else Make A Copy Of The Directory Path
    else {
        // Looping Through First Argument
        while (cmd[1][j] != '\0') {
            // Replacing ~ With Home Directory
            if (cmd[1][j] == '~') path = strncat(path, get_home_dir(), MESH_BUFFER_SIZE);

            // Adding All Other Characters Normally
            else {
                // Temporary String So Strncat Works
                char str[2] = "\0";

                // Replacing First Character Of String
                str[0] = cmd[1][j];

                // Adding Character Normally
                path = strncat(path, str, MESH_BUFFER_SIZE);
            }

            // Incrementing Index
            j++;
        }
    }

    // Attempting To Open Directory
    DIR *dir = opendir(path);

    // If Directory Does Not Exist
    if (dir == NULL) print_error(-1,"mesh_cd: Unable To Find Directory");

    // If Directory Does Exist
    else {
        // Changing Directory
        print_error(chdir(path), "mesh_cd: Unable To Change Directory");

        // Closing Variable
        closedir(dir);
    }

    // Freeing Memory
    free(path);

    // Exiting Function
    return;
}

bool check_digits(char *str) {
    // Variable Declarations
    int i;

    // Looping Through String
    for (i = 0; str[i] != '\0'; i++) {
        // Checking For Negative Sign
        if (i == 0 && str[i] == '-') continue;

        // Checking If Every Char Is A Digit
        if (!isdigit(str[i])) return false;
    }

    // Exiting Function
    return true;
}

void mesh_history(char **cmd) {
    // Variable Declarations
    int i = 0, num;

    // Counting Number Of Arguments
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) {
        print_error(-1, "mesh_history: Unable To Use More Than 1 Arguments (A Number)");
        return;
    }

    // Sending Warning If First Argument Is Not A Number
    if (cmd[1] != NULL && !check_digits(cmd[1])) {
        print_error(-1, "mesh_history: Unable To Use Non-Numeric First Argument");
        return;
    }

    // Converting Argument Into Int
    if (i >= 2) num = (int)strtol(cmd[1], NULL, 10);

    // If No Argument Given Do Default Value
    else num = 1000;

    // Sending Warning If Number Is Negative
    if (num < 0) {
        print_error(-1, "mesh_history: Unable To Use A Negative Number");
        return;
    }

    // Finding Max Number Possible
    int mx = get_mesh_index();

    // Getting Starting Position
    int start = (mx-num < 0 ? 0 : mx-num);

    // Printing All Events From Starting Position Onwards
    for (i = start; i < mx; i++) {
        printf(" %-5d  %s\n", i, get_event(i));
    }

    // Exiting Function
    return;
}

void mesh_previous(char **cmd) {
    // Variable Declarations
    int i = 0, num;

    // Counting Number Of Arguments
    while (cmd[i] != NULL) i++;

    // Sending Warning If Too Many Arguments
    if (i > 2) {
        print_error(-1, "mesh_previous: Unable To Use More Than 1 Arguments (A Number)");
        return;
    }

    // Sending Warning If First Argument Is Not A Number
    if (cmd[1] != NULL && !check_digits(cmd[1])) {
        print_error(-1, "mesh_history: Unable To Use Non-Numeric First Argument");
        return;
    }

    // Converting Argument Into Int
    if (i >= 2) num = (int)strtol(cmd[1], NULL, 10);

    // If No Argument Given Do Default Value
    else num = 0;

    // Finding Max Number Possible
    int mx = get_mesh_index();

    // Finding Start Position
    int pos;

    // If Num Is Negative Have A Negative Offset
    if (num < 0) pos = mx+num-1;

    // Else Just Make It The Normal Number
    else pos = num;

    // Checking If Pos Is Still Out Of Range
    if (pos < 0 || pos >= mx) {
        print_error(-1, "mesh_previous: Unable To Access Nonexistent Event");
        return;
    }

    // Getting Previous Event
    char ***cmds = parse_input(get_event(pos));

    // Running Previous Events
    execute_cmds(cmds);

    // Freeing Memory
    free(cmds);

    // Exiting Function
    return;
}

void execute_pipes(char **cmd, int n) {
    // Variable Declarations
    int i, q;

    // Creating An Array For Pipe File Descriptors
    int pipes[n-1][2];

    // Creating An Array For PIDs
    int pids[n];

    // Looping Through Pipe Array
    for (i = 0; i < n-1; i++) {
        // Creating Pipes
        int err = pipe(pipes[i]);

        // Error Checking
        if (err == -1) {
            // Parent Process Shouldn't End From This Error
            print_error(-1, "execute_pipes: Unable To Create Pipes In Parent Process");
            return;
        }
    }

    // Looping Through Commands
    for (i = 0; i < n; i++) {
        // Forking Process
        frk = fork();

        // Child
        if (frk == 0) {
            // First Command Doesn't Need Input Redirection
            if (i != 0) {
                // Redirecting Input From Previous Pipe Output (0)
                int err = dup2(pipes[i-1][0], STDIN_FILENO);

                // Error Checking
                if (err == -1) {
                    // Child Process Should End Because Input Messed Up
                    print_error(-1, "execute_pipes: Unable To Redirect Input In Child Process");
                    exit(-1);
                }
            }

            // Last Command Doesn't Need Output Redirection
            if (i != n-1) {
                // Redirecting Output To Next Pipe Input (1)
                int err = dup2(pipes[i][1], STDOUT_FILENO);

                // Error Checking
                if (err == -1) {
                    // Child Process Should End Because Output Messed Up
                    print_error(-1, "execute_pipes: Unable To Redirect Output In Child Process");
                    exit(-1);
                }
            }

            // Looping Through Child's Pipes
            for (q = 0; q < n-1; q++) {
                // Closing Pipes Otherwise Too Many Pipes Open In Duplicate Processes Might Occur: O(N^2) Pipes
                int err1 = close(pipes[q][0]);
                int err2 = close(pipes[q][1]);

                // Error Checking
                if (err1 == -1 || err2 == -1) {
                    // Child Process Should Exit Because Pipes May Be Messed Up
                    print_error(-1, "execute_pipes: Unable To Close Pipes In Child Process");
                    exit(-1);
                }
            }

            // Executing Command
            execute_cmd(cmd[i]);

            // Exiting Process Normally
            exit(0);
        } else if (frk < 0) { // Error
            // Parent Process Should Return Because Pipes May Be Messed Up
            print_error(-1, "execute_pipes: Unable To Fork Child Process");
            return;
        } else { // Parent
            // Saving Child PID For Later
            pids[i] = frk;

            // Parent Process Doesn't Do Anything Else Yet
            continue;
        }
    }

    // Closing Parent Process's Pipes
    for (i = 0; i < n-1; i++) {
        // Closing Pipes
        int err1 = close(pipes[i][0]);
        int err2 = close(pipes[i][1]);

        // Error Checking
        if (err1 == -1 || err2 == -1) {
            print_error(-1, "execute_pipes: Unable To Close Pipes In Parent Process");
            return;
        }
    }

    // Looping Through Commands
    for (i = 0; i < n; i++) {
        // Waiting For Child Process To End
        int err = waitpid(pids[i], &status, 0);

        // Error Checking
        if (err == -1) {
            print_error(-1, "execute_pipes: Unable To Wait For Child Process");
            return;
        }

        // Exit Status Checking
        if (WEXITSTATUS(status) == -1) {
            print_error(-1, "execute_pipes: Unable To Run At Least One Child Process Correctly");
            return;
        }
    }

    // Exiting Function
    return;
}

void execute(char **cmd) {
    // Catching Signals
    signal(SIGINT, ex_sighandler);
    //
    // Checking If Command Is Exit/Quit
    if (strcmp(cmd[0],"exit") == 0 || strcmp(cmd[0],"quit") == 0) mesh_exit(cmd);

    // Checking If Command Is Cd/Chdir
    else if (strcmp(cmd[0], "cd") == 0 || strcmp(cmd[0],"chdir") == 0) mesh_cd(cmd);

    // Checking If Command Is History
    else if (strcmp(cmd[0], "history") == 0) mesh_history(cmd);

    // Checking If Command Is Previous
    else if (strcmp(cmd[0], "previous") == 0) mesh_previous(cmd);

    // Running All Other Commands
    else {
        // Forking
        frk = fork();

        // Parent
        if (frk) {
            // Checking Forking
            print_error(frk, "execute: Unable To Fork Process");

            // Checking Waiting
            print_error(waitpid(frk, &status, 0), "execute: Unable To Wait For Child Process To End");
        } else { // Child
            // Checking And Running Command
            print_error(execvp(cmd[0], cmd), "execute: Unable To Run Command");

            // Ending Child Process
            free_all(); exit(0);
        }
    }

    // Exiting Function
    return;
}

bool need_redirect(char **split) {
    // Variable Declaration
    int i;

    // Looping Through Arguments
    for (i = 0; i < MESH_ARG_COUNT && split[i] != NULL; i++) {
        // Checking For Redirect In Operator
        if (strcmp(split[i], "<") == 0) return true;

        // Checking For Redirect Out Operator
        if (strcmp(split[i], ">") == 0) return true;

        // Checking For Redirect Concatenate Operator
        if (strcmp(split[i], ">>") == 0) return true;
    }

    // Exiting Function With Default Value
    return false;
}

int start_redirect(char **split) {
    // Variable Declarations
    int end = 0, i, in_pos = -1, out_pos = -1;

    // Looping Through Arguments To Find Redirection Operators
    for (i = 0; i < MESH_ARG_COUNT && split[i] != NULL; i++) {
        // Redirecting STDIN
        if (strcmp(split[i], "<") == 0) {
            // Setting End To Position Of First Redirect Operator
            if (end == 0) end = i;

            // Finding Latest Redirection Operator
            in_pos = i;
        }

        // Redirecting STDOUT
        if (strcmp(split[i], ">") == 0 || strcmp(split[i], ">>") == 0) {
            // Setting End To Position Of First Redirect Operator
            if (end == 0) end = i;

            // Finding Latest Redirection Operator
            out_pos = i;
        }
    }

    // Redirecting Input If Found Appropiate Redirection Operator
    if (in_pos != -1) {
        // Checking If File Name Exists
        char *name = split[in_pos+1];

        // Checking Error
        if (name == NULL) {
            print_error(-1, "start_redirect: Unable To Redirect From Nonexistent Input File");
            return -1;
        }

        // Opening File
        int fd = open(name, O_RDONLY, 0644);

        // Checking Error
        if (fd == -1) {
            print_error(-1, "start_redirect: Unable To Open Input File");
            return -1;
        }

        // Redirecting Input
        int err = dup2(fd, STDIN_FILENO);

        // Checking Error
        if (err == -1) {
            print_error(-1, "start_redirect: Unable To Redirect From Input File");
            return -1;
        }
    }

    // Redirecting Output If Found Appropiate Redirection Operator
    if (out_pos != -1) {
        // Checking If File Name Exists
        char *name = split[out_pos+1];

        // Checking Error
        if (name == NULL) {
            print_error(-1, "start_redirect: Unable To Redirect To Nonexistent Output File");
            return -1;
        }

        // Creating Basic Permissions
        int permissions = O_CREAT | O_WRONLY;

        // Setting Other Appropiate Permissions
        if (strcmp(split[out_pos], ">") == 0) permissions |= O_TRUNC;
        else permissions |= O_APPEND;

        // Opening File
        int fd = open(name, permissions, 0644);

        // Checking Error
        if (fd == -1) {
            print_error(-1, "start_redirect: Unable To Open Output File");
            return -1;
        }

        // Redirecting Input
        int err = dup2(fd, STDOUT_FILENO);

        // Checking Error
        if (err == -1) {
            print_error(-1, "start_redirect: Unable To Redirect To Output File");
            return -1;
        }
    }

    // Exiting Function
    return end;
}

void reset_redirect(int stdin_cpy, int stdout_cpy) {
    // Resetting STDIN
    int err1 = dup2(stdin_cpy, STDIN_FILENO);

    // Checking Error
    if (err1 == -1) {
        // Printing Error
        print_error(-1, "reset_redirect: Unable To Reset STDIN");

        // Goodbye Message
        printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);

        // Exiting Program
        exit(-1);
    }

    // Resetting STDOUT
    int err2 = dup2(stdout_cpy, STDOUT_FILENO);

    // Checking Error
    if (err2 == -1) {
        // Printing Error
        print_error(-1, "reset_redirect: Unable To Reset STDIN");

        // Goodbye Message
        printf("\n%sThank You For Using MESH.%s\n\n", MESH_CYAN, MESH_RESET);

        // Exiting Program
        exit(-1);
    }

    // Exiting Function
    return;
}

void execute_cmd(char *str) {
    // STDIN Copy
    int stdin_cpy = dup(STDIN_FILENO);

    // STDOUT Copy
    int stdout_cpy = dup(STDOUT_FILENO);

    // Splitting Into Arguments
    char **split = parse_line(str, " \t");

    // Checking For Redirect
    if (need_redirect(split)) {
        // Redirect STDIN And STDOUT
        int end = start_redirect(split);

        // Was Start_Redirect Successful
        if (end == -1) return;

        // Checking For Valid Command
        if (end == 0) {
            print_error(-1, "execute_cmd: Unable To Redirect Nonexistent Command");
            return;
        }

        // Getting Command
        char **cmd = subarray(split, 0, end-1);

        // Running Command
        execute(cmd);

        // Resetting Redirects
        reset_redirect(stdin_cpy, stdout_cpy);

        // Freeing Memory
        free(cmd);
    } else {
        // Checking For Valid Command
        if (split[0] == NULL) {
            print_error(-1, "execute_cmd: Unable To Run Nonexistent Command");
            return;
        }

        // Running Command
        execute(split);
    }

    // Freeing Memory
    free(split);

    // Exiting Function
    return;
}

void execute_cmds(char ***cmds) {
    // Setting FOUND_SIGINT To False
    EX_FOUND_SIGINT = false;

    // Variable Declarations
    int i, j, n;

    // Looping Through Commands
    for (i = 0; i < MESH_ARG_COUNT && cmds[i] != NULL; i++) {
        // Checking If SIGINT Has Been Found
        if (EX_FOUND_SIGINT) break;

        // Counting How Many Pipes The Command Has
        while (cmds[i][n] != NULL && n < MESH_ARG_COUNT) n++;

        // Running Pipes
        if (n > 1) execute_pipes(cmds[i], n);

        // Running Regular Commands
        else execute_cmd(cmds[i][0]);
    }

    // Exiting Function
    return;
}
