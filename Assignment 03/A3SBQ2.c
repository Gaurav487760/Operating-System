#include <stdio.h>
#include <stdbool.h>

#define P 5  // Number of processes
#define R 3  // Number of resource types

int main() {
    int total_resources[R] = {7, 2, 6};
    int allocation[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 3},
        {2, 1, 1},
        {0, 0, 2}
    };
    int request[P][R] = {
        {0, 0, 0},
        {2, 0, 2},
        {0, 0, 1},
        {1, 0, 0},
        {0, 0, 2}
    };

    int available[R] = {0, 0, 0};
    int sum_allocation[R] = {0, 0, 0};
    bool finished[P] = {false};
    int i, j;

    // Calculate available = total_resources - sum of allocation
    for (j = 0; j < R; j++) {
        int sum = 0;
        for (i = 0; i < P; i++) {
            sum += allocation[i][j];
        }
        sum_allocation[j] = sum;
        available[j] = total_resources[j] - sum;
    }

    // Display Available array
    printf("Available resources:\n");
    for (j = 0; j < R; j++) {
        printf("%c: %d ", 'A' + j, available[j]);
    }
    printf("\n");

    // Deadlock detection using safety algorithm

    int count = 0;  // number of processes finished
    bool progress;

    do {
        progress = false;
        for (i = 0; i < P; i++) {
            if (!finished[i]) {
                bool can_finish = true;
                for (j = 0; j < R; j++) {
                    if (request[i][j] > available[j]) {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {
                    for (j = 0; j < R; j++) {
                        available[j] += allocation[i][j];
                    }
                    finished[i] = true;
                    progress = true;
                    count++;
                }
            }
        }
    } while (progress);

    if (count == P) {
        printf("No deadlock detected. All processes can finish.\n");
    } else {
        printf("Deadlock detected! The following processes are deadlocked:\n");
        for (i = 0; i < P; i++) {
            if (!finished[i]) {
                printf("Process P%d\n", i);
            }
        }
    }

    return 0;
}

