// Header File
#include "shell.h"

void he_sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) return;

    // Exiting Function
    return;
}

void clrscr() {
    // ANSI Escape Codes (Not Sure About Window Compatability)
    // \x1B[H Moves Cursor To Home Position
    // \x1B[2J Clears The Screen
    // \x1B[3J Clears Scroll Back
    printf("\x1B[H\x1B[2J\x1B[3J");

    // Exiting Function
    return;
}

char *get_home_dir() {
    // Checking If HOME Environment Exists
    char *home = getenv("HOME");

    // Checking If Home Is Still Empty
    if (home == NULL) {
        // Getting Username Info
        struct passwd *pswd = getpwuid(getuid());

        // Error Checking
        if (pswd == NULL) print_error(-1, "Unable To Get Home Directory");

        // Setting Home Value
        else home = pswd->pw_dir;
    }

    // Exiting Function
    return home;
}

void print_header() {
    // Getting Username
    char username[MESH_BUFFER_SIZE];
    print_error(getlogin_r(username, MESH_BUFFER_SIZE), "Unable To Get Username");

    // Getting Hostname
    char hostname[MESH_BUFFER_SIZE];
    print_error(gethostname(hostname, MESH_BUFFER_SIZE), "Unable To Get Hostname");

    // Getting CWD
    char bcwd[MESH_BUFFER_SIZE];
    strncpy(bcwd, getcwd(bcwd, MESH_BUFFER_SIZE), MESH_BUFFER_SIZE);

    // Error Checking
    if (bcwd == NULL) print_error(-1, "Unable To Get CWD");

    // Getting Home Directory
    char *home = get_home_dir();

    // Searching For Home Directory In CWD Path
    char *pos = strstr(bcwd, home);

    // Variable Declaration
    char cwd[MESH_BUFFER_SIZE];

    // If Home Directory Isn't Found Just Copy Directly
    if (pos == NULL) strncpy(cwd, bcwd, MESH_BUFFER_SIZE);

    // Home Directory Is Found
    else {
        // Adding ~ Symbol To Path
        cwd[0] = '~';

        // Finding Size Of Home Directory Path
        int sz = strlen(home);

        // Copying Everything But Home Directory
        strncpy(&cwd[1], &bcwd[sz], MESH_BUFFER_SIZE-sz);
    }

    // Getting Current Time
    time_t t = time(NULL);

    // Converting Time To String
    char *tm = ctime(&t);

    // Error Checking
    if (tm == NULL) print_error(-1, "Unable To Get Time");

    // Getting Rid Of Extraneous Information
    tm = tm+11;

    // Ending To Get Rid Of Extraneous Information
    *strchr(tm, ' ') = '\0';

    // Going To Newline
    printf("\n");

    // Printing Header With Colors
    printf("ǁ%s%s@%s%s》➤➤➤ %s%s\n", MESH_GREEN, username, hostname, MESH_RESET, MESH_BLUE, cwd);
    printf("%sǁ%s%s@MESH%s》➤➤➤ ", MESH_RESET, MESH_CYAN, tm, MESH_RESET);

    // Flushing STDOUT To Make Sure Lines Are Printed In Correct Order
    print_error(fflush(stdout), "Unable To Flush STDOUT");

    // Exiting Function
    return;
}

char *get_input() {
    // Signal Catching
    signal(SIGINT, he_sighandler);

    // Allocating Memory
    char *input = calloc(MESH_BUFFER_SIZE, sizeof(char));

    // Getting User Input
    input = fgets(input, MESH_BUFFER_SIZE, stdin);

    // End String Properly If Needed
    if (input != NULL) *strchr(input, '\n') = '\0';

    // New Line For Viewing Purposes
    printf("\n");

    // Exiting Function
    return input;
}
