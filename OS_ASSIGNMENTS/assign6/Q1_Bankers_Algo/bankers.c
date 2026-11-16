#include "kernel/types.h"
#include "user/user.h"

#define N 5
#define M 3

// Resources currently allocated to each process
int Allocation[N][M] = {
  {0, 1, 0},
  {2, 0, 0},
  {3, 0, 2},
  {2, 1, 1},
  {0, 0, 2}
};

// Maximum resources that each process may need
int Max[N][M] = {
  {7, 5, 3},
  {3, 2, 2},
  {9, 0, 2},
  {2, 2, 2},
  {4, 3, 3}
};

// Currently available resources
int Available[M] = {3, 3, 2};

// Resources still needed by each process
int Need[N][M];


// Calculate Need = Max - Allocation
void
calculate_need(void)
{
  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      Need[i][j] = Max[i][j] - Allocation[i][j];
    }
  }
}


// Check whether process i can finish with the currently
// available Work resources
int
can_finish(int i, int Work[])
{
  for(int j = 0; j < M; j++){
    if(Need[i][j] > Work[j])
      return 0;
  }

  return 1;
}


// Banker's Safety Algorithm
int
safety_algorithm(void)
{
  int Work[M];
  int Finish[N];
  int safe_sequence[N];
  int count = 0;

  // Work starts with Available
  for(int j = 0; j < M; j++)
    Work[j] = Available[j];

  // Initially no process has finished
  for(int i = 0; i < N; i++)
    Finish[i] = 0;

  while(count < N){

    int found = 0;

    for(int i = 0; i < N; i++){

      if(Finish[i] == 0 && can_finish(i, Work)){

        // Pretend process i finishes and releases
        // its currently allocated resources
        for(int j = 0; j < M; j++)
          Work[j] += Allocation[i][j];

        Finish[i] = 1;
        safe_sequence[count] = i;
        count++;
        found = 1;
      }
    }

    // No process could finish
    if(found == 0)
      break;
  }

  if(count != N){
    printf("System is NOT in a safe state.\n");
    return 0;
  }

  printf("System is in a SAFE state.\n");
  printf("Safe sequence: ");

  for(int i = 0; i < N; i++){
    printf("P%d", safe_sequence[i]);

    if(i != N - 1)
      printf(" -> ");
  }

  printf("\n");

  return 1;
}


// Banker's Resource Request Algorithm
int
request_resources(int pid, int Request[])
{
  printf("\nP%d requests: (%d, %d, %d)\n",
         pid, Request[0], Request[1], Request[2]);

  // Step 1: Check Request <= Need
  for(int j = 0; j < M; j++){
    if(Request[j] > Need[pid][j]){
      printf("Request denied - exceeds process need.\n");
      return 0;
    }
  }

  // Step 2: Check Request <= Available
  for(int j = 0; j < M; j++){
    if(Request[j] > Available[j]){
      printf("Request denied - resources not currently available.\n");
      return 0;
    }
  }

  // Step 3: Temporarily grant the request
  for(int j = 0; j < M; j++){
    Available[j] -= Request[j];
    Allocation[pid][j] += Request[j];
    Need[pid][j] -= Request[j];
  }

  printf("Testing safety after temporary allocation...\n");

  // Step 4: Run Safety Algorithm
  if(safety_algorithm()){
    printf("Request GRANTED safely.\n");
    return 1;
  }

  // Step 5: Roll back because state is unsafe
  for(int j = 0; j < M; j++){
    Available[j] += Request[j];
    Allocation[pid][j] -= Request[j];
    Need[pid][j] += Request[j];
  }

  printf("Request denied - would lead to unsafe state.\n");

  return 0;
}


// Print Allocation, Max, Need and Available
void
print_data(void)
{
  printf("\nAllocation Matrix:\n");

  for(int i = 0; i < N; i++){
    printf("P%d: ", i);

    for(int j = 0; j < M; j++)
      printf("%d ", Allocation[i][j]);

    printf("\n");
  }

  printf("\nMax Matrix:\n");

  for(int i = 0; i < N; i++){
    printf("P%d: ", i);

    for(int j = 0; j < M; j++)
      printf("%d ", Max[i][j]);

    printf("\n");
  }

  printf("\nNeed Matrix:\n");

  for(int i = 0; i < N; i++){
    printf("P%d: ", i);

    for(int j = 0; j < M; j++)
      printf("%d ", Need[i][j]);

    printf("\n");
  }

  printf("\nAvailable: ");

  for(int j = 0; j < M; j++)
    printf("%d ", Available[j]);

  printf("\n");
}


// Restore the original textbook example
void
reset_data(void)
{
  int allocation[N][M] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
  };

  int available[M] = {3, 3, 2};

  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      Allocation[i][j] = allocation[i][j];
    }
  }

  for(int j = 0; j < M; j++)
    Available[j] = available[j];

  calculate_need();
}


int
main(void)
{
  printf("Banker's Algorithm\n");

  // Calculate Need matrix
  calculate_need();

  // Display initial data
  print_data();

  // Check initial safety
  printf("\nINITIAL SAFETY CHECK\n");
  safety_algorithm();


  // -----------------------------------------
  // SAFE REQUEST SCENARIO
  // -----------------------------------------

  printf("\nSAFE REQUEST SCENARIO\n");

  // Restore original state
  reset_data();

  // P1 requests (1, 0, 2)
  int Request1[M] = {1, 0, 2};

  request_resources(1, Request1);


  // -----------------------------------------
  // UNSAFE REQUEST SCENARIO
  // -----------------------------------------

  printf("\nUNSAFE REQUEST SCENARIO\n");

  // Restore original state
  reset_data();

  // P4 requests (3, 3, 0)
  int Request2[M] = {3, 3, 0};

  request_resources(4, Request2);


  printf("\nSimulation Complete\n");

  exit(0);
}