
#include "kernel/types.h"
#include "user/user.h"

#define N 20
#define MAX_FRAMES 5
#define MAX_PAGE 8

int pages[N] = {
    7, 0, 1, 2, 0, 3, 0, 4, 2, 3,
    0, 3, 2, 1, 2, 0, 1, 7, 0, 1
};

int next_use(int page, int pos) {
    for (int i = pos + 1; i < N; i++) {
        if (pages[i] == page)
            return i;
    }
    return 999; // Page is not used again
}

void print_frames(int frames[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) {
        if (frames[i] == -1)
            printf("- ");
        else
            printf("%d ", frames[i]);
    }
    printf("]");
}

void simulate(int algorithm, int n) {
    int frames[MAX_FRAMES];
    int last_used[MAX_PAGE];

    int faults = 0, hits = 0;
    int fifo_ptr = 0;

    for (int i = 0; i < n; i++)
        frames[i] = -1;

    for (int i = 0; i < MAX_PAGE; i++)
        last_used[i] = -1;

    if (algorithm == 0)
        printf("\nFIFO");
    else if (algorithm == 1)
        printf("\nLRU");
    else
        printf("\nOPTIMAL");

    printf(" with %d frames\n", n);
    printf("Page\tFrames\t\tStatus\n");

    for (int i = 0; i < N; i++) {
        int page = pages[i];
        int found = 0;
        int empty = -1;

        // Check whether page is already present
        for (int j = 0; j < n; j++) {
            if (frames[j] == page)
                found = 1;

            if (frames[j] == -1 && empty == -1)
                empty = j;
        }

        if (found) {
            hits++;
            printf("%d\t", page);
            last_used[page] = i;
            print_frames(frames, n);
            printf("\tHIT\n");
            continue;
        }

        faults++;

        // If an empty frame is available
        if (empty != -1) {
            frames[empty] = page;
        }
        else {
            int victim = 0;

            if (algorithm == 0) {
                // FIFO
                victim = fifo_ptr;
                fifo_ptr = (fifo_ptr + 1) % n;
            }
            else if (algorithm == 1) {
                // LRU
                for (int j = 1; j < n; j++) {
                    if (last_used[frames[j]] <
                        last_used[frames[victim]]) {
                        victim = j;
                    }
                }
            }
            else {
                // Optimal
                int farthest = -1;

                for (int j = 0; j < n; j++) {
                    int next = next_use(frames[j], i);

                    if (next > farthest) {
                        farthest = next;
                        victim = j;
                    }
                }
            }

            frames[victim] = page;
        }

        last_used[page] = i;

        printf("%d\t", page);
        print_frames(frames, n);
        printf("\tFAULT\n");
    }

    printf("Total Page Faults = %d\n", faults);
    printf("Total Page Hits   = %d\n", hits);
    printf("Hit Ratio         = %d%%\n", hits * 100 / N);
    printf("Fault Ratio       = %d%%\n", faults * 100 / N);
}

int main(void) {
    int frame_sizes[] = {3, 4, 5};

    printf("PAGE REPLACEMENT ALGORITHMS\n");

    for (int i = 0; i < 3; i++) {
        int n = frame_sizes[i];

        simulate(0, n); // FIFO
        simulate(1, n); // LRU
        simulate(2, n); // Optimal
    }

    exit(0);
}
