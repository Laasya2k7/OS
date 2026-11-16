#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096
#define LAZY_SIZE (32 * 1024 * 1024)

void
printmem(char *label)
{
  struct meminfo mi;

  if (getmeminfo(&mi) == 0) {
    printf("%s: total=%d free=%d process=%d page_tables=%d\n",
           label,
           (int)mi.total_pages,
           (int)mi.free_pages,
           (int)mi.process_pages,
           (int)mi.page_table_pages);
  } else {
    printf("getmeminfo failed\n");
  }
}

int
main(void)
{
  char *p;
  int pid;

  printmem("Before sbrk");

  p = sbrklazy(LAZY_SIZE);
  if (p == (char *)-1) {
    printf("Large sbrk failed\n");
    exit(1);
  }

  printmem("After lazy sbrk");

  // Touch only three widely separated pages.
  p[0] = 'A';
  p[10 * PGSIZE] = 'B';
  p[LAZY_SIZE - PGSIZE] = 'C';

  printf("Touched 3 pages: %c %c %c\n",
         p[0], p[10 * PGSIZE], p[LAZY_SIZE - PGSIZE]);

  printmem("After touching 3 pages");

  pid = fork();
  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    printf("Child reads: %c %c %c\n",
           p[0], p[10 * PGSIZE], p[LAZY_SIZE - PGSIZE]);
    exit(0);
  }

  wait(0);

  // Test a system call using an untouched lazy page as a buffer.
  char *buf = p + 20 * PGSIZE;
  if (read(0, buf, 1) < 0)
    printf("read from console failed\n");
  else
    printf("Read into untouched lazy buffer succeeded\n");

  printmem("After fork and buffer test");

  // Release the large region.
  if (sbrk(-LAZY_SIZE) == (char *)-1)
    printf("Heap shrink failed\n");
  else
    printf("Heap shrink succeeded\n");

  printmem("After shrink");

  exit(0);
}