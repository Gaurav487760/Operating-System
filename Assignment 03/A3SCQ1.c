#include <stdio.h>
#include <stdbool.h>

#define P 5  // Number of processes
#define R 4  // Number of resource types: A, B, C, D

int main() {
    int allocation[P][R] = {
        {0, 0, 1, 2},
        {1, 0, 0, 0},
        {1, 3, 5, 4},
        {0, 6, 3, 2},
        {0, 0, 1, 4}
    };

    int max[P][R] = {
        {0, 0, 7, 2},
        {1, 1, 5, 0},
        {2, 2, 3, 6},
        {0, 0, 6, 2},
        {0, 1, 5, 6}
    };

    int available[R] = {1, 5, 2, 0};
    int need[P][R];
    int i, j;

    // Step a) Calculate Need matrix = Max - Allocation
    for (i = 0; i < P; i++) {
        for (j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Display Need matrix
    printf("Need matrix:\n");
    printf("Process\tA B C D\n");
    for (i = 0; i < P; i++) {
        printf("P%d\t", i);
        for (j = 0; j < R; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Step b) Check if system is in safe state
    bool finished[P] = {false};
    int work[R];
    for (j = 0; j < R; j++) {
        work[j] = available[j];
    }

    int safeSeq[P];
    int count = 0;

    while (count < P) {
        bool found = false;
        for (i = 0; i < P; i++) {
            if (!finished[i]) {
                bool canAllocate = true;
                for (j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (j = 0; j < R; j++) {
                        work[j] += allocation[i][j];
                    }
                    safeSeq[count++] = i;
                    finished[i] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            break;  // No progress possible
        }
    }

    if (count == P) {
        printf("\nSystem is in a SAFE state.\nSafe sequence is: ");
        for (i = 0; i < P; i++) {
            printf("P%d ", safeSeq[i]);
        }
        printf("\n");
    } else {
        printf("\nSystem is NOT in a safe state!\n");
    }

    // Step c) Process P1 requests (0,4,2,0)
    int process_request = 1; // P1
    int request[R] = {0, 4, 2, 0};

    printf("\nRequest from process P%d: ", process_request);
    for (j = 0; j < R; j++) {
        printf("%d ", request[j]);
    }
    printf("\n");

    // Check if request <= need
    bool canRequest = true;
    for (j = 0; j < R; j++) {
        if (request[j] > need[process_request][j]) {
            canRequest = false;
            break;
        }
    }

    if (!canRequest) {
        printf("Error: Process has exceeded its maximum claim.\n");
        return 0;
    }

    // Check if request <= available
    for (j = 0; j < R; j++) {
        if (request[j] > available[j]) {
            canRequest = false;
            break;
        }
    }

    if (!canRequest) {
        printf("Request cannot be granted immediately because resources are not available.\n");
        return 0;
    }

    // Try to allocate requested resources temporarily and check safety
    for (j = 0; j < R; j++) {
        available[j] -= request[j];
        allocation[process_request][j] += request[j];
        need[process_request][j] -= request[j];
    }

    // Check system safety again with new allocation
    for (j = 0; j < R; j++) {
        work[j] = available[j];
    }

    for (i = 0; i < P; i++) {
        finished[i] = false;
    }
    count = 0;

    while (count < P) {
        bool found = false;
        for (i = 0; i < P; i++) {
            if (!finished[i]) {
                bool canAllocate = true;
                for (j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (j = 0; j < R; j++) {
                        work[j] += allocation[i][j];
                    }
                    finished[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) {
            break;
        }
    }

    if (count == P) {
        printf("Request can be granted while keeping the system in safe state.\n");
    } else {
        printf("Request cannot be granted as it leads to unsafe state.\n");

        // Rollback allocation since unsafe
        for (j = 0; j < R; j++) {
            available[j] += request[j];
            allocation[process_request][j] -= request[j];
            need[process_request][j] += request[j];
        }
    }

    return 0;
}

