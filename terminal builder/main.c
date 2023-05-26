#include "main.h"
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define LENGTH 1000
#define ARR 100

void parse(char* input,char** userCmd, int* length){// function for exeargsS
         char cmd[ARR][LENGTH];
   char *found;
    int i=0;

    while( (found = strsep(&input," ")) != NULL ) {

        strcpy(cmd[i], found);
        userCmd[i] = cmd[i];
        i++;
    }
    length=&i;
    userCmd[*length]=NULL;


}

int main (int argc, char **argv){ 
while(1){
int length = 0;
int childPid; 
char ** cmdLine=(char**)malloc(ARR * sizeof(char*)) ; 
char info[LENGTH]; 
 

	printf(" %s:%s > ",getenv("USER"),getenv("PWD"));
	gets(info);

	if(strcmp(info,"exit") == 0){
                break;
        }
        else{
        	parse(info, cmdLine, &length);
        	childPid = fork();
		if (childPid == 0)
			{
                	if (execvp(cmdLine[0],cmdLine) == -1) {
                    		printf("Error! command is not executing ");
				printf("\n");}
				exit(0);			
		}

		else 
		{ 
  			wait(NULL);         
		}	 
 	} 
    
    
     }
     
     
     
}
