#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
display_stats(char *message)
{
  struct meminfo mi;

  printf("\n%s\n", message);

  if (getmeminfo(&mi) < 0) {
    printf("Unable to retrieve memory statistics\n");
    return;
  }

  printf("Total allocated pages: %ld\n", mi.total_allocated);
  printf("Total freed pages: %ld\n", mi.total_freed);
  printf("Currently allocated pages: %ld\n", mi.currently_allocated);
  printf("Available physical pages: %ld\n", mi.available_pages);
}

int
main(void)
{
  char *p;

  display_stats("Initial memory statistics:");

  printf("\nAllocating 5 pages...\n");

  p = sbrk(5 * 4096);

  if (p == (char *)-1) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  // Touch each page to trigger lazy allocation.
  for (int i = 0; i < 5; i++)
    p[i * 4096] = 'A';

  display_stats("After allocation:");

  printf("\nReleasing 5 pages...\n");

  sbrk(-5 * 4096);

  display_stats("After deallocation:");

  exit(0);
}