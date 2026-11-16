#include "kernel/types.h"
#include "user/user.h"

#define N 5

int allocation[N][N];
int request[N][N];
int graph[N][N];

int visited[N];
int in_stack[N];
int path[N];
int path_len;


// Build wait-for graph
// P_i -> P_j if P_i is waiting for a resource
// currently held by P_j.
void
build_graph(void)
{
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      graph[i][j] = 0;
    }
  }

  // For every process Pi and resource Rj that Pi requests,
  // find the process holding Rj and create Pi -> holder.
  for(int i = 0; i < N; i++){
    for(int r = 0; r < N; r++){

      if(request[i][r] == 1){

        for(int holder = 0; holder < N; holder++){

          if(allocation[holder][r] == 1){
            graph[i][holder] = 1;
          }
        }
      }
    }
  }
}


// DFS cycle detection
int
dfs(int node)
{
  visited[node] = 1;
  in_stack[node] = 1;

  path[path_len++] = node;

  for(int next = 0; next < N; next++){

    if(graph[node][next] == 0)
      continue;

    // Found a cycle
    if(in_stack[next]){
      int start = 0;

      for(int i = 0; i < path_len; i++){
        if(path[i] == next){
          start = i;
          break;
        }
      }

      printf("Deadlock cycle: ");

      for(int i = start; i < path_len; i++)
        printf("P%d -> ", path[i]);

      printf("P%d\n", next);

      return 1;
    }

    if(!visited[next]){
      if(dfs(next))
        return 1;
    }
  }

  path_len--;
  in_stack[node] = 0;

  return 0;
}


// Detect cycle in the wait-for graph
int
detect_deadlock(void)
{
  for(int i = 0; i < N; i++){
    visited[i] = 0;
    in_stack[i] = 0;
  }

  path_len = 0;

  for(int i = 0; i < N; i++){
    if(!visited[i]){
      if(dfs(i))
        return 1;
    }
  }

  return 0;
}


// Print allocation and request matrices
void
print_matrices(void)
{
  printf("\nAllocation Matrix:\n");

  for(int i = 0; i < N; i++){
    printf("P%d: ", i);

    for(int j = 0; j < N; j++)
      printf("%d ", allocation[i][j]);

    printf("\n");
  }

  printf("\nRequest Matrix:\n");

  for(int i = 0; i < N; i++){
    printf("P%d: ", i);

    for(int j = 0; j < N; j++)
      printf("%d ", request[i][j]);

    printf("\n");
  }
}


// Construct and print wait-for graph
void
print_graph(void)
{
  printf("\nWait-for Graph:\n");

  int edges = 0;

  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){

      if(graph[i][j]){
        printf("P%d -> P%d\n", i, j);
        edges++;
      }
    }
  }

  if(edges == 0)
    printf("No edges\n");
}


// Scenario with NO deadlock
void
scenario_no_deadlock(void)
{
 
  printf("SCENARIO 1: NO DEADLOCK\n");
  

  // Clear matrices
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      allocation[i][j] = 0;
      request[i][j] = 0;
    }
  }

  // P0 holds R0 and waits for R1
  allocation[0][0] = 1;
  request[0][1] = 1;

  // P1 holds R1 and waits for R2
  allocation[1][1] = 1;
  request[1][2] = 1;

  // P2 holds R2 but does not wait
  allocation[2][2] = 1;

  print_matrices();

  build_graph();
  print_graph();

  if(detect_deadlock())
    printf("Deadlock detected.\n");
  else
    printf("No deadlock detected.\n");
}


// Scenario with deadlock among 3 processes
void
scenario_deadlock(void)
{
 
  printf("SCENARIO 2: DEADLOCK\n");


  // Clear matrices
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      allocation[i][j] = 0;
      request[i][j] = 0;
    }
  }

  // P0 holds R0 and waits for R1
  allocation[0][0] = 1;
  request[0][1] = 1;

  // P1 holds R1 and waits for R2
  allocation[1][1] = 1;
  request[1][2] = 1;

  // P2 holds R2 and waits for R0
  allocation[2][2] = 1;
  request[2][0] = 1;

  print_matrices();

  build_graph();
  print_graph();

  if(detect_deadlock())
    printf("Deadlock detected.\n");
  else
    printf("No deadlock detected.\n");
}


int
main(void)
{
  printf("Deadlock Detection using Wait-for Graph\n");

  scenario_no_deadlock();

  scenario_deadlock();

  printf("\nSimulation Complete\n");

  exit(0);
}