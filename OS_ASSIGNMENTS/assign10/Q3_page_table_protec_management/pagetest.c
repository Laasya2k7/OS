#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  char *p;
  int pid;

  printf("Page Protection Test\n");

  // Allocate one page.
  p = sbrk(4096);

  if (p == (char *)-1) {
    printf("Allocation failed\n");
    exit(1);
  }

  // Touch the page to trigger lazy allocation.
  p[0] = 'A';

  printf("\nBefore changing permissions:\n");
  pageinfo((uint64)p);

  printf("\nChanging page to read-only...\n");

  if (setreadonly((uint64)p) < 0) {
    printf("setreadonly failed\n");
    exit(1);
  }

  printf("\nAfter changing permissions:\n");
  pageinfo((uint64)p);

  // Use a child so the parent can survive the protection fault.
  pid = fork();

  if (pid < 0) {
    printf("Fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    printf("\nChild attempting to write to read-only page...\n");
    p[0] = 'B';

    printf("ERROR: Write unexpectedly succeeded\n");
    exit(1);
  }

  wait(0);

  printf("\nParent survived the child's protection fault.\n");

  exit(0);
}