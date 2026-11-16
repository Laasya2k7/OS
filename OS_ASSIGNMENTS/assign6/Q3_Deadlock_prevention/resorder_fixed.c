#include "kernel/types.h"
#include "user/user.h"

#define LOCK1 0
#define LOCK2 1

void
process_a(void)
{
  printf("Process A: trying to acquire Lock1\n");
  sem_wait(LOCK1);

  printf("Process A: acquired Lock1\n");

  pause(2);

  printf("Process A: trying to acquire Lock2\n");
  sem_wait(LOCK2);

  printf("Process A: acquired Lock2\n");
  printf("Process A: acquired both locks\n");

  sem_post(LOCK2);
  sem_post(LOCK1);

  printf("Process A: released Lock2 and Lock1\n");

  exit(0);
}

void
process_b(void)
{
  // IMPORTANT:
  // Process B also acquires Lock1 before Lock2.
  printf("Process B: trying to acquire Lock1\n");
  sem_wait(LOCK1);

  printf("Process B: acquired Lock1\n");

  pause(2);

  printf("Process B: trying to acquire Lock2\n");
  sem_wait(LOCK2);

  printf("Process B: acquired Lock2\n");
  printf("Process B: acquired both locks\n");

  sem_post(LOCK2);
  sem_post(LOCK1);

  printf("Process B: released Lock2 and Lock1\n");

  exit(0);
}

int
main(void)
{
  int pid;

  sem_init(LOCK1, 1);
  sem_init(LOCK2, 1);

  printf("Starting deadlock-free demonstration\n");

  pid = fork();

  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0)
    process_a();

  pid = fork();

  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0)
    process_b();

  wait(0);
  wait(0);

  printf("Both processes completed successfully\n");

  exit(0);
}