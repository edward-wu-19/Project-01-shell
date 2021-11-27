<h1>MESH Shell</h1>

<em> by Mohammad Khan and Edward Wu </em>

<b>Compilation:</b> make

<b>Execution:</b> make run

<b>Features:</b>
-
Implements bash commands, cd, and exit<br>
Parses a line for multiple commands<br>

<b>Function Headers:</b>
-
<em>execute.c</em><br>
void ex_sighandler(int signo);<br>
void shell_exit();<br>
void shell_cd(char *path);<br>
void execute(char **cmd);<br>
void execute_cmds();<br>

<em>header.c</em><br>
void he_sighandler(int signo);<br>
char *get_home_dir();<br>
void clrscr();<br>
char *get_input();<br>
void print_header();<br>

<em>parse.c</em><br>
char **parse_line(char *line, char *sep);<br>
char ***parse_input(char *line);<br>

<em>shell.c</em><br>
void free_all();<br>
void print_error();<br>

<b>Attempted Features:</b>
-
<b>Bugs:</b>
-
