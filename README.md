# project-1-shell

MESH Shell by Mohammad Khan and Edward Wu
----------------------------------------------
Compilation:
make
----------------------------------------------
Execution:
make run
----------------------------------------------
Features:
----------------------------------------------
Function Headers:

void ex_sighandler(int signo);
void shell_exit();
void shell_cd(char *path);
void execute(char **cmd);
void execute_cmds();

void he_sighandler(int signo);
char *get_home_dir();
void clrscr();
char *get_input();
void print_header();

char **parse_line(char *line, char *sep);
char ***parse_input(char *line);

void free_all();
void print_error();
----------------------------------------------
Attempted Features:
----------------------------------------------
Bugs:
