Assignment 6 – Deadlock Handling and Resource Management
Overview

This assignment focuses on deadlock avoidance, deadlock detection, deadlock prevention, and synchronization of multiple resources in the xv6-riscv operating system.

The assignment consists of four questions:

Banker's Algorithm – Deadlock Avoidance
Wait-for Graph – Deadlock Detection
Resource Ordering – Deadlock Prevention
Multi-Resource Synchronization – Semaphore-based synchronization

The implementations were added as xv6 user programs and tested using QEMU.

Question 1 – Banker's Algorithm
Introduction

Banker's Algorithm is a deadlock avoidance technique. Before granting a resource request, the system checks whether granting it would leave the system in a safe state.

What We Implemented

We created:

user/bankers.c

The program uses:

Allocation matrix
Maximum resource matrix
Available resource vector
Need matrix
Safety algorithm
Resource-request algorithm

The Need matrix is calculated as:

Need = Max - Allocation

The program checks:

Whether the initial system is safe
The safe sequence
A resource request that can be safely granted
A resource request that must be denied because it results in an unsafe state
Implementation Steps
Define the Allocation, Max, and Available matrices.
Calculate the Need matrix.
Initialize Work with the Available resources.
Find a process whose remaining Need can be satisfied.
Simulate that process finishing and release its allocated resources.
Continue until all processes finish or no process can proceed.
If all processes can finish, the state is safe.
For a resource request, temporarily allocate the requested resources.
Run the safety algorithm again.
Grant the request if the resulting state is safe; otherwise roll back the allocation.
Deliverable
user/bankers.c

The output demonstrates the safe sequence and both granted and denied request scenarios.

Question 2 – Deadlock Detection using Wait-for Graph
Introduction

A Wait-for Graph represents which processes are waiting for resources held by other processes. A cycle in the graph indicates a deadlock.

What We Implemented

We created:

user/deadlockdetect.c

The program constructs a Wait-for Graph from the resource allocation and request information.

For every process requesting a resource:

Requesting Process → Process Holding Resource

A DFS-based cycle detection algorithm is then used to determine whether a deadlock exists.

Implementation Steps
Define the resource allocation and request information.
Construct the Wait-for Graph.
Add an edge from each requesting process to the process holding the requested resource.
Use DFS to traverse the graph.
Maintain visited and recursion-stack information.
Detect a cycle during traversal.
Print the cycle when a deadlock is found.
Test both a deadlock and a no-deadlock scenario.
Tested Cases

No deadlock:

P0 → P1 → P2

No cycle exists.

Deadlock:

P0 → P1 → P2 → P0

The cycle represents the deadlock.

Deliverable
user/deadlockdetect.c

The output shows graph construction, the detection result, and the deadlock cycle when present.

Question 3 – Resource Ordering / Deadlock Prevention
Introduction

Deadlock can be prevented by imposing a global ordering on resources. Every process must acquire resources according to the same order.

What We Implemented

Two versions were created:

user/resorder_bad.c
user/resorder_fixed.c
Bad Version

The bad implementation intentionally uses different resource acquisition orders:

Process A: Lock1 → Lock2
Process B: Lock2 → Lock1

This creates a circular wait:

A waits for B
B waits for A

and results in deadlock.

Fixed Version

The fixed implementation makes both processes follow:

Lock1 → Lock2

Therefore, circular wait cannot occur.

Implementation Steps
Create two resource locks using semaphores.
Create two processes.
In the bad version, make the processes acquire the locks in opposite orders.
Observe the resulting deadlock/hang.
Modify the implementation so all processes use the same resource order.
Run the fixed version.
Verify that both processes acquire, use, and release the resources successfully.
Why It Prevents Deadlock

When every process follows the same global resource order, a process cannot hold a later resource while waiting for an earlier resource. Therefore, the circular-wait condition is eliminated.

Deliverables
user/resorder_bad.c
user/resorder_fixed.c

The output logs demonstrate both the deadlocking and fixed executions.

Question 4 – Multi-Resource Synchronization
Introduction

This question deals with multiple processes competing for several limited resources. Semaphores are used to control access to the resources while resource ordering is used to prevent deadlock.

What We Implemented

We created:

user/syncdeadlock.c

The program contains:

5 processes
2 Printers
1 Scanner
2 Disks

The resources are represented using semaphores.

All processes acquire resources in the same order:

Printer → Scanner → Disk

Each process performs multiple work cycles and releases its resources after completing each cycle.

A separate semaphore is also used to protect console output so that messages from concurrent processes remain readable.

Implementation Steps
Define semaphores for Printer, Scanner, and Disk.
Initialize them according to the available resource instances.
Create five processes using fork().

Make each process request resources in the order:

Printer → Scanner → Disk
Allow the process to perform its work after acquiring all required resources.
Release the resources after the work is completed.
Repeat the process for five cycles.
Use a print mutex to keep concurrent output readable.
Wait for all five processes to finish.
Deadlock Prevention

All processes follow the same resource acquisition order:

Printer → Scanner → Disk

This eliminates circular wait and prevents deadlock.

Result

All five processes successfully completed all five work cycles without hanging.

Deliverable
user/syncdeadlock.c

The output log demonstrates all five processes completing their work across multiple cycles.

Project Structure
xv6-riscv/
│
├── Makefile
│
└── user/
    ├── bankers.c
    ├── deadlockdetect.c
    ├── resorder_bad.c
    ├── resorder_fixed.c
    └── syncdeadlock.c

The semaphore implementation from the previous process-synchronization assignment is reused by Questions 3 and 4.

How to Build and Run

From the xv6-riscv directory:

make clean
make qemu

Then run the individual programs inside xv6:

bankers
deadlockdetect
resorder_bad
resorder_fixed
syncdeadlock

For example:

$ bankers

and similarly for the other programs.

Conclusion

This assignment demonstrates different approaches to handling deadlocks:

Banker's Algorithm → Deadlock avoidance
Wait-for Graph → Deadlock detection
Resource Ordering → Deadlock prevention
Semaphores + Resource Ordering → Safe multi-resource synchronization

Together, the four implementations demonstrate how operating systems can detect, avoid, and prevent deadlocks while allowing multiple processes to safely share limited resources.
