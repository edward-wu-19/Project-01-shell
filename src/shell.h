#ifndef SHELL

#define SHELL

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *cmd[] = {"ls"};

void print_error(char *msg, int err) {
    if (err == -1) {
        printf("%s: %s\n", msg, strerror(errno));
    }
}

#endif
