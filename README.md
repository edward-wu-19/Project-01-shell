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
  <li>Offers piping and redirection functionality</li>
  </ul>

## Function Headers

#### execute.c
<pre>void ex_sighandler(int signo);
void shell_exit();
void shell_cd(char *path);
void piping(char **cmd);
void redirect(char **cmd);
void execute(char **cmd);
void execute_cmd(char **cmd);
void execute_cmds(char ***cmds);</pre>

#### header.c
<pre>void he_sighandler(int signo);
char *get_home_dir();
void clrscr();
char *get_input();
void print_header();</pre>

#### parse.c
<pre>char **subarray(char **cmd, int l, int r);
char **parse_line(char *line, char *sep);
char ***parse_input(char *line);</pre>

#### shell.c
<pre>void free_all();
void print_error(int err, char *msg);</pre>

## Attempted Features

## Bugs
