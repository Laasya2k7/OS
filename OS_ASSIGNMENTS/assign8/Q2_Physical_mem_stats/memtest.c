
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



void
printmem(char *label)
{
  struct meminfo mi;

  if (getmeminfo(&mi) < 0) {
    printf("getmeminfo failed\n");
    return;
  }

  printf("\n%s\n", label);
  printf("Total physical pages: %d\n", (int)mi.total_pages);
  printf("Free physical pages: %d\n", (int)mi.free_pages);
  printf("Process address-space pages: %d\n", (int)mi.process_pages);
  printf("Page-table pages: %d\n", (int)mi.page_table_pages);
}

int
main(void)
{
  printmem("Initial memory statistics");

  char *mem = sbrk(10 * 4096);
  if (mem == (char *)-1) {
    printf("sbrk allocation failed\n");
    exit(1);
  }

  // Touch the allocated pages so physical frames are actually used.
  for (int i = 0; i < 10; i++)
    mem[i * 4096] = 'A';

  printmem("After allocating 10 pages");

  int pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    printmem("Inside child process");
    exit(0);
  }

  wait(0);
  printmem("After child exits and is reaped");

  sbrk(-10 * 4096);
  printmem("After freeing allocated memory");

  exit(0);
}
