# Unix-Shell

This project creates a simple Unix shell. The shell is the heart of
the command-line interface, and thus is central to the Unix/C programming
environment. Mastering use of the shell is necessary to become proficient in
this world; knowing how the shell itself is built is the focus of this
project.

There are three specific objectives to this project:

* To further familiarize yourself with the Linux programming environment.
* To learn how processes are created, destroyed, and managed.
* To gain exposure to the necessary functionality in shells.

## Overview

Implement a *command line interpreter (CLI)* or,as it is more commonly known,
a *shell*. The shell should operate in this basic way: when you type in a command
(in response to its prompt), the shell creates a child process that executes the
command you entered and then prompts for more user input when it has finished.

### Source Code Directories
msh - contains the file msh.c which you will modify for this assignment. No
      other file may be modified

### Building the Shell
You can build the code by typing:
```
make
```

### Basic Shell: `msh`

The basic shell, called `msh` is basically an interactive loop: it repeatedly 
prints a prompt `msh> ` (note the space after the greater-than sign), parses the 
input, executes the command specified on that line of input, and waits for 
the command to finish. This is repeated until the user types `exit`.

The shell can be invoked with either no arguments or a single argument;
anything else is an error. Here is the no-argument way:

```
prompt> ./msh
msh> 
```

At this point, `msh` is running, and ready to accept commands. Type away!

The mode above is called *interactive* mode, and allows the user to type
commands directly. The shell also supports a *batch mode*, which instead reads
input from a file and executes commands from therein. Here is how you
run the shell with a batch file named `batch.txt`:

```
prompt> ./msh batch.txt
```

One difference between batch and interactive modes: in interactive mode, a
prompt is printed (`msh> `). In batch mode, no prompt should be printed.

The shell is structured such that it creates a process for each new
command (the exception are *built-in commands*, discussed below).  The basic
shell parses a command and runs the program corresponding to the command.
For example, if the user types `ls -la /tmp`, the shell will
run the program `/bin/ls` with the given arguments `-la` and `/tmp` 


### Paths

**Important:** Note that the shell itself does not *implement* `ls` or other
commands (except built-ins). 

### Built-in Commands

Whenever your shell accepts a command, it should check whether the command is
a **built-in command** or not. If it is, it should not be executed like other
programs. Instead, your shell will invoke your implementation of the built-in
command. For example, to implement the `exit` built-in command, you simply
call `exit(0);` in your msh source code, which then will exit the shell.

In this project, `exit`, and `cd`  are implemented as built-in
commands.

* `exit`: When the user types `exit`, the shell simply calls the `exit`
  system call with 0 as a parameter. It is an error to pass any arguments to
  `exit`. 

* `cd`: `cd` always take one argument (0 or >1 args should be signaled as an
error).

