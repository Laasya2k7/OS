#include "types.h"
#include "stat.h"
#include "user.h"

// Dummy function to simulate CPU bound work
void cpu_bound_task(int loops) {
    int x = 0;
    (void)x;

    for (int i = 0; i < loops * 10000000; i++) {
        x += i; 
    }
}

int main(int argc, char *argv[]) {
    printf(1, "Starting HRRN Scheduler Test...\n");

    // Array of mock burst times. Note how jobs 1 & 2 are long, 3 is short.
    int bursts[] = {100, 80, 10, 50}; 
    int num_jobs = 4;

    for (int i = 0; i < num_jobs; i++) {
        int pid = fork();
        
        if (pid == 0) {
            // Child process
            setburst(getpid(), bursts[i]);
            printf(1, "Job %d (Burst: %d) started.\n", i, bursts[i]);
            
            // Simulate work proportional to burst time
            cpu_bound_task(bursts[i]);
            
            printf(1, "--> Job %d (Burst: %d) COMPLETED.\n", i, bursts[i]);
            exit();
        } 
    }

    // Parent waits for all children
    for (int i = 0; i < num_jobs; i++) {
        wait();
    }

    printf(1, "All jobs finished.\n");
    exit();
}