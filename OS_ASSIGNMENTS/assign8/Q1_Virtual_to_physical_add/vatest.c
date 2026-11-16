#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int global_var = 100;

int
main(void)
{
  int local_var = 200;
  int *heap_var = malloc(sizeof(int));

  if (heap_var == 0) {
    printf("malloc failed\n");
    exit(1);
  }

  *heap_var = 300;

  printf("\n--- VA to PA Test ---\n");

  printf("Global: VA=%p PA=%p\n",
         &global_var, (void *)va2pa((uint64)&global_var));

  printf("Local:  VA=%p PA=%p\n",
         &local_var, (void *)va2pa((uint64)&local_var));

  printf("Heap:   VA=%p PA=%p\n",
         heap_var, (void *)va2pa((uint64)heap_var));

  printf("Invalid VA 0x7fffffff: PA=%ld\n",
         (long)va2pa(0x7fffffff));

  printf("\n--- Fork Test ---\n");

  int pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    free(heap_var);
    exit(1);
  }

  if (pid == 0) {
    printf("Child:  global VA=%p PA=%p\n",
           &global_var, (void *)va2pa((uint64)&global_var));
    exit(0);
  }

  wait(0);

  printf("Parent: global VA=%p PA=%p\n",
         &global_var, (void *)va2pa((uint64)&global_var));

  free(heap_var);
  exit(0);
}