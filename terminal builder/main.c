//auther :mohammed abbas id:208172569
#include "main.h"


int main (int argc, char **argv)
 {

    printf("------------- starting custom shell --------------------- \n");
    printf("------------- -------------------- --------------------- \n");
	int childPid; 
      while (1) {
        int length = 0;
        printCurrentPath(); // print the crruent path
        char user[MAXLENGTH];
        char **cmd =(char**)malloc(MAXARR * sizeof(char*)) ;
        gets(user); //the command that we write
        if(strcmp(user,"exit") == 0){
                break;
        }else{
        execArgs(user, cmd, &length);
	    childPid = fork();
        if ( childPid == 0) {
                if (execvp(cmd[0],cmd) == -1) {
                    printf("command is not execute .. ");
			printf("\n");
                }
                exit(0);
            }
        else {
            wait(NULL);
        }
        }



 }
 }

