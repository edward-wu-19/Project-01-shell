#ifndef SHELL

#define SHELL

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// parse.c
char **parse_command(char *line);
char ***parse_line(char *line);
// redirect.c
void redirectTo(char* name, char* s);
void redirectFrom(char* name);

#endif
