//auther :mohammed abbas id:208172569
#include <stdio.h>
#include <sys/wait.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAXLENGTH 1000
#define MAXARR 10


void execArgs(char* input,char** userCmd, int* length);
void printCurrentPath();