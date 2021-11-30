# MESH Shell
<em> by Mohammad Khan and Edward Wu </em> <br>
This shell is for a project for Systems Level Programming by Mr. DW. (Period 4)


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
- Run using make or directly running the executable
`make run` or `./bin/shell`
- Exit using exit or quit
`exit` or `quit`


## Description
### Features
- Implements bash commands using fork and execvp
- Parses a line for multiple commands using ;
- Offers multipiping and redirection functionality using 
    `<`, `>`, and `>>`
- 'Exit', 'cd', 'history', and 'previous': shell specific functions
- Input sanitation capability for whitespace (ie. extra spaces and tabs)

### Attempted Features
<ul> 
  <li>Complete signal handling, but sadly we could only manage to implement catching the SIGINT signal. All other signals that we tried were unable to be caught</li>
</ul>

### Bugs
<ul>
  <li>We have attempted to prevent infinite recursion loops with the 'previous' command to the best of our capability, but some of them may still occur in obscure cases</li>
  <li>The arrow keys cause extraneous text to appear instead of actually moving your cursor in the shell prompt</li>
  <li>Tabbing and then spamming backspace on an empty input may causes the shell prompt to glitch visually for a bit, but it should not cause problems elsewhere</li>
  <li>Though we have tested MESH successfully on Mac and Linux OS, we are unsure about its Windows compatibility</li>
  <li>When redirecting a file of commands into MESH, there must be an exit or quit at the end of the commands or else MESH will error because it cannot find the end of the file</li>
</ul> 

### Limitations
<ul> 
  <li>The maximum argument number for line of input is 128, but this can be easily raised if needed</li>
  <li>The maximum string buffer size for a line of input is 2048 characters, but this can be easily raised if needed</li>
  <li>The maximum history size is 32768, but this can be easily raised if needed</li>
</ul>

### Shell Specific Functions
<ul>
  <li> 'exit' or 'quit': Takes in either zero (defaults to 0) or one numeric argument and exits the shell with that number as its exit value</li>
  <li> 'cd' or 'chdir': Takes in either zero (defaults to home directory) or one string argument and attempts to change to the specificed directory</li> 
  <li> 'history': Similar to the bash history command, accepts either zero (defaults to 1000) or one non negative numeric argument n and prints a list of size n of previous commands inputted to the shell</li>
  <li> 'previous': Similar to the '!number' command in bash, accepts either zero (defualts to -1) or one numeric argument n and runs the input either n events ago (if n is negative) or the event with index n (if n is positive)</li>
</ul>
  
## Function Headers

#### execute.c
<pre>void ex_sighandler(int signo);
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
void execute_cmds(char ***cmds);</pre>

#### header.c
<pre>void he_sighandler(int signo);
char *get_home_dir();
void clrscr();
char *get_input();
void print_header();</pre>

#### parse.c
<pre>char *join(char **split, char *sep);
char **subarray(char **cmd, int l, int r);
char **parse_line(char *line, char *sep);
char ***parse_input(char *line);</pre>

#### shell.c
<pre>int get_mesh_index();
void add_event(char *input);
char *get_event(int i);
void free_all();
void print_error(int err, char *msg);</pre>
