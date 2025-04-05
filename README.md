# Unix Shell 

A lightweight Unix-style shell implemented in C, supporting both interactive and batch processing modes. This project demonstrates key operating system concepts such as process creation, command parsing, and execution management.


## Project Overview

This custom shell, named `msh`, replicates fundamental behavior of standard Unix shells like `bash`.

It operates in two modes:
- **Interactive Mode** – Displays a prompt (`msh> `) and accepts user input in real-time.
- **Batch Mode** – Reads and executes a list of commands from a file.

The shell supports external commands (e.g., `ls -la /tmp`) via child processes and handles key **built-in commands** like `cd` and `exit`.


## Objectives

- Strengthen knowledge of the Linux C programming environment.
- Learn how processes are created, managed, and terminated in Unix systems.


## Build & Run Instructions

### Compile the Shell
From the root project directory:
```
make
```

### Run in Interactive Mode
Launch the shell and type commands directly:
```
./msh
```
Example:
```
msh> ls -la
msh> cd /tmp
msh> exit
```

### Run in Batch Mode
Provide a file containing shell commands:
```
./msh test.txt
### Run in Interactive Mode
Launch the shell and type commands directly:
```
./msh
```
Example:
```
msh> ls -la
msh> cd /tmp
msh> exit
```

### Run in Batch Mode
Provide a file containing shell commands:
```
./msh test.txt
```


## Example `test.txt` for Batch Mode
```
pwd
cd ..
ls -l
exit
```

The shell will execute the commands in sequence, with no prompt displayed.
---
## Supported Features

- Execution of external commands via `fork()` and `execvp()`
- Built-in commands:
  - `exit` – Terminates the shell (no arguments allowed)
  - `cd <directory>` – Changes the working directory (exactly one argument required)
- Argument parsing and basic error handling
- Dual operation modes: interactive and batch
