#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 10
int arr_size=0;

//enum of algorithms
//////////////////////////////////////////////////
enum Algorithm
{
Fcfs=1,
Sjf=2,
Hrrn=3,
Roundrobin=4,
priorty_with_Shortest_Job_First=5
};



//the tasks struct
//////////////////////////////////////////////////
typedef struct 
{
int priority;
int taskid;
int bursttime;
int arrivaltime;
} task;




//functions
//////////////////////////////////////////////////
void count_of_tasks(const char * file);
task* build(const char * file);
task* Table(const char* file);
void Display(const char * file);
void firstComeFirstServer(const char *file);
void shortJobFirst(const char* file);
void HighestResponseRatioNext(const char*file);
void RoundRobin(const char* file, int );
void Multilevel_Scheduling_with_SJF(const char* file, int );
void Schedule(const char* file,int );
