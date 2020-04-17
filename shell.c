#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
//declarations

int BUFFER_SIZE = 512;
int TOKEN_SIZE = 50;
int front=0;
int back=1;
int historyCount=0;
char* history[20];
char* original_path=NULL;
char* home_path=NULL;

bool historyCall=false;
char* buffer_save=NULL;
const char * delimitors = " |><&;\t\n";

//functions

void cycle();
void prompt();
void getInput();
void end();
void parse();

//commands
void inputProcess();
void external_process(char* argv[]);
void getpath(char* tokenBank[]);
void setpath(char* tokenBank[]);
void cd(char* tokenBank[]);
void historyInv(char* tokenBank[]);
void printHistory(char* tokenBank[]);
void addHistory();

//error functions
void historyInvExample();

//void printAllHistory();
//void save_history();
//void read_history();

/*
main():
Saves original paths and changes the current working directory to the users
home directory then starts the program
*/
void main() {
    original_path=getenv("PATH");
    chdir(home_path=getenv("HOME"));
    cycle();

}

/*
cycle():
Prints prompt and then asks user for input
Constantly loops program until the user wishes to exit
*/
void cycle() {
    while (1) {
        prompt();
        getInput();

    }

}

/*
prints a shell prompt
*/
void prompt() {

    printf(">");

}

/*
getInput():
gets user input and determines wether it is an exit (Ctrl + D) or it is an
input. passes buffer variable to parse if there is an input.
*/
void getInput() {
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        end();
        exit(0);

    }
    else {
        buffer_save= malloc(100);
        parse(buffer);


    }
}

/*
parse(char buffer[]):
Param: (char buffer[]) - this is the user input
Parses through the user input to split it into tokens based on the delimitors
defintd at start of program. Saves these tokens to array tokenBank and passes
it to inputProcess
*/
void parse(char buffer[]) {
    strcpy(buffer_save,buffer);
    char * tokenBank[50];
    int token_count = 0;

    tokenBank[token_count] = strtok(buffer, delimitors);
    while (tokenBank[token_count] != NULL) {
        token_count++;
 	if (token_count>=50){
	break;
	}
        tokenBank[token_count] = strtok(NULL, delimitors);

    }

    if (tokenBank[0] == NULL) {

        return;

    }

    inputProcess(tokenBank);




}

/*
inputProcess(char * tokenBank[TOKEN_SIZE]):
 Param (char * tokenBank[TOKEN_SIZE])- parsed / tokened input
Tries to match user input to a command. First checks to see if it is a built in
command (exit,cd,getpath,setpath,history or history invocation) and
 calls the relevant function. Otherwise it will try run it as an external
 process
*/
void inputProcess(char * tokenBank[TOKEN_SIZE]) {

    if (strcmp(tokenBank[0], "exit") == 0) {
        if (tokenBank[1]!=NULL) {
            printf("Invalid Parameter for exit\n");

        }
        else {
            end();
            exit(0);
        }


    } else if(strcmp(tokenBank[0],"cd")==0) {
        cd(tokenBank);
    }
    else if (strcmp(tokenBank[0],"getpath")==0) {

        getpath(tokenBank);


    }
    else if(strcmp(tokenBank[0],"setpath")==0) {

        setpath(tokenBank);


    }
    else if(strcmp(tokenBank[0],"history")==0) {
        if (!historyCall) {
            addHistory();

        }

        printHistory(tokenBank);
        return;
    }
    else if (strcspn(tokenBank[0],"!")==0) {
        historyCall=true;
        historyInv(tokenBank);
        return;
    }



    else {
        external_process(tokenBank);
    }


    if (!historyCall) {

        addHistory();


    }



}

/*
historyInv(char* tokenBank[]):
Param (char * tokenBank[TOKEN_SIZE])- parsed / tokened user input
Determines what type of history invocation it is '!x' || '!!' || '!-x' where x
is an int. Then calculates where the users choice is to run the correct
command in history. Does checks to make sure it is a valid request
*/
void historyInv(char* tokenBank[]) {

//if its just "!" entered then wrong input
    if(strcmp(tokenBank[0],"!")==0) {
        printf("No parameter was entered. One is expected ");
        historyInvExample();
        historyCall=false;
        return;
    }

// if theres a second parameter then wrong input
    if (tokenBank[1]!=NULL) {
        printf("History invocation only takes one parameter.");
        historyInvExample();
        historyCall=false;
        return;
    }


    int token;
    char* tokenToken=strtok(tokenBank[0],"!");

//checks if it is null first to avoid crashing. if it is then it must have been a '!!' call
    if (tokenToken=='\0') {
        if (historyCount>=20) {
            token=20;

        }
        else {
            token=historyCount;
        }

    }
//otherwise check if it has a '-' in input
    else if(strcspn(tokenToken,"-")==0) {
        tokenToken=strtok(tokenToken,"-");
        if (tokenToken==NULL) {
            printf("No parameter was given. One was expected.");
            historyInvExample();
            historyCall=false;
            return;
        }
        token=atoi(tokenToken);

        if (token=='\0') {
            printf("A non-number parameter was given but is expected.");
            historyInvExample();
            historyCall=false;
            return;
        }
	if (token > 20) {
		printf("Invalid command number. Must be between -20 and 20, except from 0. \n");
	historyCall = false;
	return;
	}

        token=historyCount-token+1;
    }

//else it is just a standard '!x' input
    else {

        token=atoi(tokenToken);

    }


// checks to see if it was a number that was given as parameter
    if (token=='\0') {
        printf("A non-number parameter was given but is expected.");
        historyInvExample();
        historyCall=false;
        return;

    }
// checks to see if the number its trying to use is actually valid.
    if (token<=-20 ||token>20) {

        printf("Invalid command number. Must be between -20 and 20, except from 0. \n");
        historyCall=false;
        return;
    }




    token=back-token;
    if (token<0) {
        token=token+20;


    }
    if (historyCount>=20) {
        token++;

    }

    if (token>19) {

        token=0;
    }


    if (history[token]==NULL) {

        printf("Invalid history number entered. \nE.g. must be a number shown next to a command when 'history' is entered \n");

    }
    else {
        char* parse_command = (char*)malloc(100*sizeof(char));
        strcpy(parse_command, history[token]);
        parse(parse_command);
    }






    historyCall=false;
}

/*
printHistory(char * tokenBank[TOKEN_SIZE]):
Param (char * tokenBank[TOKEN_SIZE])- parsed / tokened input
Prints the last 20 commands executed which were not history invocations
This is all stored in an circular array called 'history'
*/
void printHistory(char* tokenBank[]) {
    int backL;

    if (tokenBank[1]!=NULL) {
        printf("history does not take a parameter. Correct usage 'history' \n");
        return;
    }


    if (history[back]==NULL) {

        backL=back-1;
    }
    else {
        backL=back;

    }

    for (int i=0; i<20; i++) {
        if(backL<0) {
            backL=19;

        }

        if (history[backL]==NULL) {

            return;
        }




        printf("%i : %s",i+1,history[backL]);
        backL--;
    }



}

/*
external_process(char* argv[]):
Param-(char* argv[]) this is the tokened user input
Attempts to run a external process based on user input. Only ran if no built in
command is matched to user input
*/
void external_process(char* argv[]) {
    pid_t pid = fork();

    if(pid < 0) {
        perror("");

    }

    else if (pid == 0) {
        if (execvp(argv[0],argv)== -1) {
            perror(argv[0]);


            exit(1);

        }
        exit(0);
    }
    else {
        wait(NULL);


    }

}

/*
getpath(char* tokenBank[]):
Param (char * tokenBank[TOKEN_SIZE])- parsed / tokened input from parse
function
Gets and prints the current enviroment path the user is using
*/
void getpath(char* tokenBank[]) {
    if (tokenBank[1]!=NULL) {
        printf("getpath does not take a parameter. Correct example: 'getpath' \n");
    }
    else {
        printf("Current path:\n %s \n",getenv("PATH"));

    }
}
/*setpath(char * tokenBank[TOKEN_SIZE]):
Param (char * tokenBank[TOKEN_SIZE]) - parsed / tokened input
Sets the current enviroment path from user input
*/
void setpath(char* tokenBank[]) {

    if (tokenBank[1]==NULL) {

        printf("No Paramaters given. Correct example: 'setpath PATH_NAME'  \n");


    }
    else if(tokenBank[2]!=NULL) {
        printf("More than one parameter given. Only one allowed. Example: 'setpath PATH_NAME' \n");



    }

    else {

        setenv("PATH",tokenBank[1],1) ;

        printf("Enviroment Path changed to \n %s \n",getenv("PATH"));
    }

}

/*
cd(char * tokenBank[TOKEN_SIZE]):
Param (char * tokenBank[TOKEN_SIZE]) - parsed / tokened input
Changes users current working directory. If input is 'cd' will change it to
home directory  otherwise will attempt to change it to user input
*/
void cd(char * tokenBank[]) {
    if (tokenBank[1] == NULL) {

        chdir(home_path);
    }

    else if (tokenBank[2]!=NULL) {

        printf("cd has too many parameters. Only one parameter allowed. Example 'cd DIRECTORY' \n");
    }
    else {
        if (chdir(tokenBank[1]) == -1) {

            perror(tokenBank[1]);
        }

    }

}

/*
addHistory():
Adds the previously executed command to history and maintains the circular
array
*/
void addHistory() {


    if (front==back) {

        back--;

        if (back<0) {

            back=19;

        }


    }

    history[front]=buffer_save;
    if (historyCount<20) {
        historyCount++;
    }
    front--;
    if (front<0) {


        front=19;
    }


}
/* end():
When exiting the shell it sets the users enviroment path back to the original
path upon opening the shell
*/
void end() {


    if (setenv("PATH",original_path,1)!=0) {
        printf("Could not restore original path");
    }

    printf("Path is restored to : \n %s \n ",getenv("PATH"));




}

/* historyInvExample():
Prints the correct usage of history invocation if the user was to incorrectly
use the command
*/
void historyInvExample() {
    printf("\n Example: '!x' for x'th command in history\n OR \n '!!' for the last entered command \n OR  \n '!-x' for the x'th last command \n");


}
