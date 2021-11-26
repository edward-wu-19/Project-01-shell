#ifndef MESH

// Preventing Duplicates
#define MESH

// Library Imports
#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

// Color Constants (Not Sure About Window Compatability)
#define MESH_BLACK "\x1B[30m"
#define MESH_RED "\x1B[31m"
#define MESH_GREEN "\x1B[32m"
#define MESH_YELLOW "\x1B[33m"
#define MESH_BLUE "\x1B[34m"
#define MESH_MAGENTA "\x1B[35m"
#define MESH_CYAN "\x1B[36m"
#define MESH_WHITE "\x1B[37m"
#define MESH_RESET "\x1B[0m"

// Number Constants
#define MESH_BUFFER_SIZE 1024
#define MESH_ARG_COUNT 64

// Execute.c
void ex_sighandler(int signo);
void shell_exit();
void shell_cd(char *path);
void execute(char **cmd);
void execute_cmds();

// Header.c
void he_sighandler(int signo);
char *get_home_dir();
void clrscr();
char *get_input();
void print_header();

// Parse.c
char **parse_line(char *line, char *sep);
char ***parse_input(char *line);

// Shell.c
void free_all();
void print_error();

#endif
