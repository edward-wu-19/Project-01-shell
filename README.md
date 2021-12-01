# MESH Shell
- by Mohammad Khan and Edward Wu
- This shell is for a project for Systems Level Programming by Mr. DW. (Period 4)


## Getting Started
- Clone the repository
```
git clone git@github.com:edward-wu-19/Project-01-shell.git
```
- Change directories
```
cd Project-01-shell
```
- Compile using `make`
- Run using `make run` or `./bin/shell`
- Exit using `exit` or `quit`


## Description
### Features
- Implements bash commands using fork and execvp
- Parses a line for multiple commands using `;`
- Offers redirection functionality using `<`, `>`, and `>>`
- Able to pipe and multipipe commands using `|`
- Implements the following shell specific functions: `exit`, `cd`, `history`, and `previous`
- Input sanitation capability for whitespace (ie. extra spaces and tabs)

### Attempted Features
- Complete signal handling, but sadly we could only manage to implement catching the `SIGINT` signal. All other signals that we tried were unable to be caught

### Bugs
- We have attempted to prevent infinite recursion loops with the `previous` command to the best of our capability, but some of them may still occur in obscure cases
- The arrow keys cause extraneous text to appear instead of actually moving your cursor in the shell prompt, just use backspace to rewrite text in these cases
- Tabbing and then spamming backspace on an empty input may causes the shell prompt to glitch visually for a bit, but it should not cause problems elsewhere
- Though we have tested MESH successfully on Mac and Linux OS, we are unsure about its Windows compatibility
- In some cases when redirecting a file of commands into MESH, MESH will infinite loop because it cannot find the end of the file. This can be fixed by putting and `exit` or `quit` command at the end of the file of commands, and then MESH behaves regularly

### Limitations
- The maximum argument number for line of input is `128`, but this can be easily raised if needed
- The maximum string buffer size for a line of input is `2048` characters, but this can be easily raised if needed
- The maximum history size is `32768`, but this can be easily raised if needed
</ul>

### Shell Specific Functions
- `exit` or `quit`: Takes in either zero (defaults to 0) or one numeric argument and exits the shell with that number as its exit value
- `cd` or `chdir`: Takes in either zero (defaults to home directory) or one string argument and attempts to change to the specificed directory
- `history`: Similar to the bash `history` command, accepts either zero (defaults to 1000) or one non negative numeric argument n and prints a list of size n of previous commands inputted to the shell
- `previous`: Similar to the `!number` command in bash, accepts either zero (defaults to -1) or one numeric argument n and runs the input either n events ago (if n is negative) or the event with index n (if n is positive)
  
## Function Headers

#### execute.c
```c
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
```

#### header.c
```c
void he_sighandler(int signo);
char *get_home_dir();
void clrscr();
char *get_input();
void print_header();
```

#### parse.c
```c
char *join(char **split, char *sep);
char **subarray(char **cmd, int l, int r);
char **parse_line(char *line, char *sep);
char ***parse_input(char *line);
```

#### shell.c
```c
int get_mesh_index();
void add_event(char *input);
char *get_event(int i);
void free_all();
void print_error(int err, char *msg);
```
