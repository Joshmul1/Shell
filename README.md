# Shell
A Shell written in C.

## Built in commands
The list of built in commands are given below, but all external commands that your command line usually provides are available to be used.

* ``` cd PATH ``` - Allows you to change directory to PATH.
* ``` setpath ``` - Sets path of current working directory.
* ``` getpath ``` - Returns path of current working directory.
* ``` history ``` - Displays history of commands.
* ``` !! ``` - Runs previously executed command.
* ``` !x ``` - Runs the x'th command in history as shown by executing 'history'.
* ``` !-x ``` - Runs the x'th last command. E.g. ``` !-1 ``` will run the previous command.
* ``` exit ``` - Exits the shell setting the working directory to its original path before running shell.


## How to run

Ensure [GGC](https://gcc.gnu.org/) is installed.
Type ``` gcc shell.c ``` into command line to compile
Type ``` ./a.out ``` to run shell.
