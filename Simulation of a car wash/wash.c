#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS
#define S_PER_M 0600
#define TRUE 1
#define FALSE 0

///////////////////////////////////////////////////////////function decleration
float Next_Time(float Rate);
int pop_Time(int s_id);
int push_Time(int s_id);
int pop_Queue(int id, int s_num);
int push_Queue(int s_id, int s_num);
int Enter_Wash_Machines(key_t s_key, int s_num);
int Enter_Time(key_t s_key);
void Wash_Stat(key_t skey, int s_num);
void Handler();
volatile int MchnCount;
volatile int State = 0;
///////////////////////////////////////////////////////////wash struct
struct wash
{
  int number_Machine;
  int AverageOfArriveTime;
  int AverageOfWashTime;
  float time;
  int cars;

} typedef wash;
///////////////////////////////////////////////////////////
typedef union _Signal
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
} Signal;
///////////////////////////////////////////////////////////next time functiom
float Next_Time(float Rate){
  return (-(logf(1.0f - (float)rand() / (RAND_MAX + 1.0f)))) / Rate;
}
///////////////////////////////////////////////////////////pop queue function
int pop_Queue(int s_id, int s_num) {
  struct sembuf p_buf;
  p_buf.sem_num = s_num;
  p_buf.sem_op = -1;
  p_buf.sem_flg = SEM_UNDO;

  if (semop(s_id, &p_buf, 1) == -1) {
    exit(1);
  }
  return 0;
}
///////////////////////////////////////////////////////////push queue function
int push_Queue(int s_id, int s_num) {
  struct sembuf v_buf;
  v_buf.sem_num = s_num;
  v_buf.sem_op = 1;
  v_buf.sem_flg = SEM_UNDO;

  if (semop(s_id, &v_buf, 1) == -1) {
    exit(1);
  }
  return 0;
}
///////////////////////////////////////////////////////////pop Time function
int pop_Time(int s_id){
  struct sembuf p_buf;
  p_buf.sem_num = 0;
  p_buf.sem_op = -1;
  p_buf.sem_flg = SEM_UNDO;

  if (semop(s_id, &p_buf, 1) == -1)
  {
    exit(1);
  }
  return 0;
}
///////////////////////////////////////////////////////////push Time function
int push_Time(int s_id){
  struct sembuf v_buf;
  v_buf.sem_num = 0;
  v_buf.sem_op = 1;
  v_buf.sem_flg = SEM_UNDO;

  if (semop(s_id, &v_buf, 1) == -1)
  {
    exit(1);
  }
  return 0;
}
///////////////////////////////////////////////////////////Enter Time function
int Enter_Time(key_t s_key){
  s_key = s_key+1;
  int status = 0, s_id;
  if( ( s_id = semget(s_key, 1,S_PER_M|IPC_CREAT|IPC_EXCL)) == -1 )
  {
    if( errno == EEXIST )
    {
      s_id = semget( s_key, 1, 0 );
    }
  }
  else
  {
    status = semctl(s_id, 0, SETVAL, 1);
    if( s_id == -1 || status == -1 )
    {
      exit(-1);
    }
  }
  return (s_id);
}
///////////////////////////////////////////////////////////Handler function
void Handler(){
  State = 1;
  printf("\n--------Washing station is close--------n");
}
///////////////////////////////////////////////////////////Enter Wash Machines function
int Enter_Wash_Machines(key_t s_key, int nsems){
  int status = 0, s_id;
  if ((s_id = semget(s_key, nsems, S_PER_M | IPC_CREAT | IPC_EXCL)) == -1) {
    if (errno == EEXIST) {
      s_id = semget(s_key, nsems, 0);
    }
  } else {
    struct sembuf sops[nsems];
    int i;
    for (i = 0; i < nsems; i++) {
      sops[i].sem_num = i;
      sops[i].sem_op = 1;
      sops[i].sem_flg = 0;
    }
    status = semop(s_id, sops, nsems);
  }
  //check for errors
  if (s_id == -1 || status == -1) {
    exit(-1);
  }

  return s_id;
}

///////////////////////////////////////////////////////////Wash stat function
void Wash_Stat(key_t s_key, int s_num){
  int s_id;
  int time_semi_id;
  pid_t pid = getpid();
  if ((s_id = Enter_Wash_Machines(s_key, MchnCount)) < 0)  { return; exit(1); }
  if ((time_semi_id = Enter_Time(s_key)) < 0)  { return; exit(1); }
  int shmid;
  shmid = shmget(s_key, sizeof(wash), 0666 | IPC_STAT);
  wash *SegmentPointer;
  SegmentPointer = shmat(shmid, (void *)0, 0);
  float ArrivingTime = SegmentPointer->time;
  float WashingTime = Next_Time(SegmentPointer->AverageOfWashTime);
  float ExitTime;
  printf("Car : %d, Arrive at time  %lf\n", getpid(), ArrivingTime);
  int i = rand() % s_num;
  pop_Queue(s_id, i);
  if(State == 1){
    push_Queue(s_id, i);
    kill(getpid(), SIGQUIT);
  }
  printf("Car : %d, in the washing machine ! , Time %lf \n", getpid(),  SegmentPointer->time);
  sleep(WashingTime);
  pop_Time(time_semi_id);
  SegmentPointer->time = SegmentPointer->time + WashingTime;
  ExitTime = SegmentPointer->time;
  SegmentPointer->cars += 1;
  push_Time(time_semi_id);
  printf("Car : %d, leave the washing machine !, %lf\n", getpid(), ExitTime);
  push_Queue(s_id, i);
  kill(getpid(), SIGQUIT);
}
///////////////////////////////////////////////////////////main
int main(int argc, char *argv[]){
  signal(SIGINT, Handler);
  srand(time(0));
  int status = 0;
  pid_t wpid;
  key_t s_key = ftok(".", 357);
  int shmid;
  if ((shmid = shmget(s_key, sizeof(wash), 
  IPC_CREAT | IPC_EXCL | 0666)) == -1)  
    {
     printf("Shared memory Error \n"); 
     }
  wash *SegmentPointer;
  if ((SegmentPointer = shmat(shmid, 0, 0)) == NULL)  { 
    exit(1); 
    }
  SegmentPointer->number_Machine = atoi(argv[1]);
  MchnCount = SegmentPointer->number_Machine;
  SegmentPointer->AverageOfWashTime = atoi(argv[2]);
  SegmentPointer->AverageOfArriveTime = atoi(argv[3]);
  int run_time = atoi(argv[4]);
  SegmentPointer->time = 0.0;
  SegmentPointer->cars = 0;
  do
  {
    float arrive = Next_Time(SegmentPointer->AverageOfArriveTime);
    SegmentPointer->time += arrive;
    if (SegmentPointer->time > run_time) { break; }
    if (fork() == 0)
    {
      Wash_Stat(s_key, MchnCount);
    }
    else
    {
      sleep(1);
    }
  }while (SegmentPointer->time <= run_time);
  while(wait(&status)>0);
  printf("car: %d finished ,Time:%lf seconds, \n ", SegmentPointer->cars, SegmentPointer->time);
  shmctl(shmid, IPC_RMID, 0);
  semctl(s_key, 0, IPC_RMID, NULL);

  return 0;
}
