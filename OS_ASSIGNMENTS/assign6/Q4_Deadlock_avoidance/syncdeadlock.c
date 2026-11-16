#include "kernel/types.h"
#include "user/user.h"

#define PRINTER 0
#define SCANNER 1
#define DISK 2
#define PRINT_MUTEX 6

#define NPROC 5
#define CYCLES 5

void
worker(int id)
{
  for(int i = 0; i < CYCLES; i++){

    // Request Printer
    sem_wait(PRINT_MUTEX);
    printf("Process %d: requesting Printer\n", id);
    sem_post(PRINT_MUTEX);

    sem_wait(PRINTER);

    // Printer granted
    sem_wait(PRINT_MUTEX);
    printf("Process %d: granted Printer\n", id);
    sem_post(PRINT_MUTEX);


    // Request Scanner
    sem_wait(PRINT_MUTEX);
    printf("Process %d: requesting Scanner\n", id);
    sem_post(PRINT_MUTEX);

    sem_wait(SCANNER);

    // Scanner granted
    sem_wait(PRINT_MUTEX);
    printf("Process %d: granted Scanner\n", id);
    sem_post(PRINT_MUTEX);


    // Request Disk
    sem_wait(PRINT_MUTEX);
    printf("Process %d: requesting Disk\n", id);
    sem_post(PRINT_MUTEX);

    sem_wait(DISK);

    // Disk granted
    sem_wait(PRINT_MUTEX);
    printf("Process %d: granted Disk\n", id);
    sem_post(PRINT_MUTEX);


    // Start work
    sem_wait(PRINT_MUTEX);
    printf("Process %d: starting work (cycle %d)\n",
           id, i + 1);
    sem_post(PRINT_MUTEX);

    pause(2);


    // Release Disk
    sem_wait(PRINT_MUTEX);
    printf("Process %d: releasing Disk\n", id);
    sem_post(PRINT_MUTEX);

    sem_post(DISK);


    // Release Scanner
    sem_wait(PRINT_MUTEX);
    printf("Process %d: releasing Scanner\n", id);
    sem_post(PRINT_MUTEX);

    sem_post(SCANNER);


    // Release Printer
    sem_wait(PRINT_MUTEX);
    printf("Process %d: releasing Printer\n", id);
    sem_post(PRINT_MUTEX);

    sem_post(PRINTER);


    // Cycle completed
    sem_wait(PRINT_MUTEX);
    printf("Process %d: completed cycle %d\n",
           id, i + 1);
    sem_post(PRINT_MUTEX);

    pause(1);
  }

  sem_wait(PRINT_MUTEX);
  printf("Process %d: completed all cycles\n", id);
  sem_post(PRINT_MUTEX);

  exit(0);
}


int
main(void)
{
  int pid;

  // Resource pools:
  // 2 printers, 1 scanner, 2 disks

  sem_init(PRINTER, 2);
  sem_init(SCANNER, 1);
  sem_init(DISK, 2);

  // Semaphore used only to keep console output readable
  sem_init(PRINT_MUTEX, 1);

  sem_wait(PRINT_MUTEX);
  printf("Starting multi-resource synchronization\n");
  printf("Resource ordering: Printer -> Scanner -> Disk\n");
  sem_post(PRINT_MUTEX);


  // Create 5 processes
  for(int i = 0; i < NPROC; i++){

    pid = fork();

    if(pid < 0){
      sem_wait(PRINT_MUTEX);
      printf("fork failed\n");
      sem_post(PRINT_MUTEX);

      exit(1);
    }

    if(pid == 0){
      worker(i);
    }
  }


  // Wait for all five processes
  for(int i = 0; i < NPROC; i++)
    wait(0);


  sem_wait(PRINT_MUTEX);
  printf("All 5 processes completed successfully\n");
  sem_post(PRINT_MUTEX);

  exit(0);
}