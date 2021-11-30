# MESH Shell

<em> by Mohammad Khan and Edward Wu </em>

This shell is for a project for Systems Level Programming by Mr. DW. (Period 4)

## Getting Started

#### Cloning:
<pre>git clone git@github.com:edward-wu-19/Project-01-shell.git
cd Project-01-shell</pre>
#### Compiling:
```make```
#### Executing:
```make run```<br>
This will place you in the MESH Shell. You will be in the Project-01-shell directory.

## Features
<ul>
  <li>Implements bash commands using fork and execvp</li>
  <li>Parses a line for multiple commands</li>
  <li>Offers multipiping and redirection functionality</li>
  <li>exit, cd, history, previous command line functions</li>
  <li>Input sanitation capability for same line whitespace (ie. extra spaces and tabs)</li>
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

## Attempted Features

All attempted features have been completed.

## Bugs
