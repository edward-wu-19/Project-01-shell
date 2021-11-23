#include "shell.h"

void redirectTo(char* name, char* s){
    int fd = open(name, O_WRONLY);
    int w = write(fd, s, sizeof(s));
}

void redirectFrom(char* name){
    int fd = open(name, O_RDONLY);
    struct stat *fileinput = malloc(sizeof(struct stat));
    stat(name, fileinput);
    int size = fileinput->st_size;

    char * buffer[size];
    int r = read(fd, buffer, size);
}