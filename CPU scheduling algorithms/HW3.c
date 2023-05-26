#include "HW3.h"

///////////////////////////////////////////////////////////////////////////
//to return the count the task of test.txt file
void count_of_tasks(const char * file){
    FILE * test;
    char c;
    test = fopen(file,"r");
    for (c = getc(test); c != EOF; c = getc(test)){
        if (c == '\n') 
            arr_size = arr_size + 1;
    }
    fclose(test);
}
///////////////////////////////////////////////////////////////////////////
//store each line into a struct called task that has the same fields
task* build(const char * file)
    {
    FILE * test; 
    int i=0;
    task* temp;
    test = fopen(file,"r");
    temp = (task*)malloc(arr_size * sizeof(task));
    test = fopen(file,"r");
         while(fscanf(test,"%d, %d, %d,%d",&temp[i].taskid,&temp[i].priority,&temp[i].arrivaltime,&temp[i].bursttime)!=EOF)
            {
            i++;
            }
        fclose(test);
        return temp;
    }

///////////////////////////////////////////////////////////////////////////
//returns an array of tasks
task* Table(const char* file)
{
      task* arr = (task*)malloc(arr_size* sizeof(task));
      arr = build(file);
      return arr;
}
///////////////////////////////////////////////////////////////////////////
//to view the table of tasks data
void Display(const char * file)
{
    task* arr = (task*) malloc(arr_size* sizeof(task));
    arr = Table(file);
    printf("-----------------------PROCESS TABLE-------------------\n");
    printf("ID      |priority   |Arrivial Time  |Burst Time \n");
    printf("_______________________________________________________\n");
    for (int i = 0; i < arr_size; i++)
    {
            printf("%d       |",arr[i].taskid);
            printf("%d          |",arr[i].priority);
            printf("%d              |",arr[i].arrivaltime);
            printf("%d  \n",arr[i].bursttime);
            printf("_______________________________________________________\n");

    }
    printf("\n");
}

///////////////////////////////////////////////////////////////////////////
//First Come First Serve
void firstComeFirstServer(const char *file) 
{
    task* arr = (task*) malloc(arr_size* sizeof(task));
    int i;
    printf("Schedule arr - First Come First Serve ALgorithm: \n");
    arr = build(file);
    for(i=0; i<arr_size; i++)
        printf("<P%d,%d> ", arr[i].taskid, arr[i].bursttime);
    printf("\n\n");
}

///////////////////////////////////////////////////////////////////////////
//Shortest Job First
void shortJobFirst(const char* file)
{
   task* arr = (task*) malloc(arr_size * sizeof(task));
   int i,k, temp;
   task help;
   printf("Scheduling Tasks - Shortest Job First Algorithm:\n");
   arr = build(file);
   for(i=1; i<arr_size;i++)
      for(k=i+1; k<arr_size;k++)
          if(arr[i].bursttime>arr[k].bursttime)
          {
          help = arr[i];
          arr[i]=arr[k];
          arr[k]=help;
          }
    for(i=0;i<arr_size;i++)
        printf("<P%d,%d> ", arr[i].taskid, arr[i].bursttime);
    printf("\n\n");
 }
 
///////////////////////////////////////////////////////////////////////////
//Highest Response Ratio Next  
void HighestResponseRatioNext(const char*file)
{

    task* arr = (task*) malloc(arr_size*sizeof(task));
    int i,k,temp;
    task help;
    printf("Scheduling Tasks - HRRN Algorithm:\n");
    arr = build(file);
    for(i=1;i<arr_size;i++){
       for(k=i+1;k<arr_size;k++)
           if(arr[i].arrivaltime>arr[k].arrivaltime)
           {
           help=arr[i];
           arr[i]=arr[k];
           arr[k]=help;
           }
    }
    printf("<P%d,%d> ", arr[0].taskid, arr[0].bursttime);
    printf("<P%d,%d> ", arr[1].taskid, arr[1].bursttime);
    for(i=2;i<arr_size;i++){
       for(k=i+1;k<arr_size;k++)
           if(arr[i].bursttime>arr[k].bursttime)
           {
           help=arr[i];
           arr[i]=arr[k];
           arr[k]=help;
           }
    }
    for(i=2;i<arr_size;i++)
        printf("<P%d,%d> ", arr[i].taskid, arr[i].bursttime);
    printf("\n\n");
      
}


///////////////////////////////////////////////////////////////////////////
//Round Robin
void RoundRobin(const char* file, int t) 
{

    task* arr = (task*)malloc(arr_size*sizeof(task));
    int i, k, max, j;
    printf("Scheduling Tasks - Round Robin ALgorithm:\n");
    arr = build(file);
    int ct[10], temp=0, tat[10], wa[10];
    for(i=0;i<arr_size;i++)
           ct[i]=arr[i].bursttime;
    max=arr[0].bursttime;
    for(i=1;i<arr_size;i++)
        if(max<=arr[i].bursttime)
                max=arr[i].bursttime;
    for(j=0;j<(max/t)+1;j++)
          for(i=0;i<arr_size;i++)
               if(arr[i].bursttime!=0)
                    if(arr[i].bursttime<=t)
                    {
                    tat[i]=temp+arr[i].bursttime;
                    temp=temp+arr[i].bursttime;
                    printf("<P%d,%d> ", arr[i].taskid, arr[i].bursttime);
                    arr[i].bursttime=0;
                    }
                    else
                    {
                    arr[i].bursttime=arr[i].bursttime-t;
                    temp=temp+t;
                    printf("<P%d,%d> ", arr[i].taskid, t);
                    }
                    
        printf("\n\n");
 }
 
 ///////////////////////////////////////////////////////////////////////////
 //Multilevel Scheduling with Shortest Job First
 void Multilevel_Scheduling_with_SJF(const char* file, int t)
{
     int k,i;
     task help;
     task* arr = (task*) malloc(arr_size*sizeof(task));
     printf("Scheduling Tasks - Multilevel Scheduling with Shortest Job First ALgorithm:\n");
     arr = build(file);
     //sortin by priority
     for(i=1;i<arr_size;i++)
         for(k=i+1;k<arr_size;k++)
             if(arr[i].priority<arr[k].priority)
             {
             help=arr[i];
             arr[i]=arr[k];
             arr[k]=help;
             }
     for(i=0;i<arr_size;i++){
     
        printf("<P%d,%d> ", arr[i].taskid, arr[i].bursttime);
     }
           
        printf("\n\n");
}
 
 
///////////////////////////////////////////////////////////////////////////
 void Schedule(const char* file,int num)
 {
      int QuantumTime = 2;
      
      switch(num) {
      case Fcfs:
            firstComeFirstServer(file);
            break;
      case Sjf:
            shortJobFirst(file);
            break;
      case Hrrn: 
            HighestResponseRatioNext(file);
            break;
      case Roundrobin: 
            RoundRobin(file, QuantumTime);
            break;
      case priorty_with_Shortest_Job_First:
            Multilevel_Scheduling_with_SJF(file, QuantumTime);
            break;
      default: printf("THe ENd!\n");
        
    }  
    
}  

///////////////////////////////////////////////////////////////////////////
int main(int argc, char const* argv[])
{
    count_of_tasks(argv[1]);
    Display(argv[1]);
    Schedule(argv[1],Fcfs);
    Schedule(argv[1],Sjf);
    Schedule(argv[1],Hrrn);
    Schedule(argv[1],Roundrobin);
    Schedule(argv[1],priorty_with_Shortest_Job_First);
    
    
    return 0;
}
