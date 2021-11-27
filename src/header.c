#include "shell.h"

// Prevents These Signals From Ending Shell, But Doesn't Do Anything Else Yet
void he_sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) return;
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

    // Failsafing Through User ID And Password ID Structs
    if (home == NULL) {
        struct passwd *pswd = getpwuid(getuid());
        if (pswd == NULL) print_error(-1, "Unable To Get Home Directory");
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
    if (bcwd == NULL) print_error(-1, "Unable To Get CWD");

    // Handling Home Directory Configuration
    char *home = get_home_dir();
    char *pos = strstr(bcwd, home);
    char cwd[MESH_BUFFER_SIZE];
    if (pos == NULL) strncpy(cwd, bcwd, MESH_BUFFER_SIZE);
    else {
        cwd[0] = '~';
        int sz = strlen(home);
        strncpy(&cwd[1], &bcwd[sz], MESH_BUFFER_SIZE-sz);
    }

    // Getting Time
    time_t t = time(NULL);
    char *tm = ctime(&t);
    if (tm == NULL) print_error(-1, "Unable To Get Time");
    tm = tm+11;
    *strchr(tm, ' ') = '\0';

    // Printing Header
    printf("\n");
    printf("%sǁ%s%s@%s%s》➤➤➤ %s%s\n", MESH_BLACK, MESH_GREEN, username, hostname, MESH_BLACK, MESH_BLUE, cwd);
    printf("%sǁ%s%s@MESH%s》➤➤➤ %s", MESH_BLACK, MESH_CYAN, tm, MESH_BLACK, MESH_RESET);

    // Exiting Function
    return;
}

char *get_input() {
    // Signal Catching
    signal(SIGINT, he_sighandler);

    // Obtaining User Input
    char *input = calloc(MESH_BUFFER_SIZE, sizeof(char));
    input = fgets(input, MESH_BUFFER_SIZE, stdin);
    if (input == NULL) print_error(-1, "Unable To Get User Input");
    else *strchr(input, '\n') = '\0';

    // Exiting Function
    return input;
}
