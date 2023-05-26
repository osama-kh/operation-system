//auther :mohammed abbas id:208172569
#include "main.h"
void execArgs(char* input,char** userCmd, int* length){// function for exeargsS
         char cmd[MAXARR][MAXLENGTH];
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

void printCurrentPath(){//function that return crrunt path
    char * currentPath = getenv("PWD");
    char * userName = getenv("USER");
    printf(" %s:%s >> ",userName,currentPath);

}