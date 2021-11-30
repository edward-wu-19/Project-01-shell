#ifndef MESH

// Preventing Duplicates
#define MESH

// Library Imports
#include <pwd.h>
#include <time.h>
#include <ctype.h>
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
#define MESH_HISTORY_SIZE 32768
#define MESH_BUFFER_SIZE 2048
#define MESH_ARG_COUNT 128

// Execute.c
void ex_sighandler(int signo);
void mesh_exit(char **cmd);
void mesh_cd(char **cmd);
bool check_digits(char *str);
void mesh_history(char **cmd);
void mesh_prev(char **cmd);
void execute_pipes(char **cmd, int n);
void execute(char **cmd);
bool need_redirect(char **split);
int start_redirect(char **split);
void reset_redirect(int stdin_cpy, int stdout_cpy);
void execute_cmd(char *str);
void execute_cmds(char ***cmds);

// Header.c
void he_sighandler(int signo);
char *get_home_dir();
void clrscr();
char *get_input();
void print_header();

// Parse.c
char *join(char **split, char *sep);
char **subarray(char **cmd, int l, int r);
char **parse_line(char *line, char *sep);
char ***parse_input(char *line);

// Shell.c
int get_mesh_index();
void add_event(char *input);
char *get_event(int i);
void free_all();
void print_error(int err, char *msg);

#endif
