# Simple Shell

A simple UNIX command interpreter written in C, similar to the original `/bin/sh`.

---

## Description

This project is a simple implementation of a UNIX shell. It reads commands from the user, parses them, and executes them using system calls. The shell can work in both interactive and non-interactive modes.

It is part of the Holberton School low-level programming curriculum.

---

## Features

- Displays a prompt and waits for user input (interactive mode)
- Executes commands with their full path or found in the PATH environment
- Handles command arguments
- Handles the built-in command `exit`
- Works in non-interactive mode (e.g., when piped from a file or command)
- Produces output identical to `/bin/sh` for all tested commands

---

## Compilation

Compile all `.c` files with the following command

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
