#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

int main() {
    int n;
    int max_demand[MAX_PROCESSES], allocated[MAX_PROCESSES];
    int total_allocated = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter maximum demand of each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &max_demand[i]);
    }

    printf("Enter allocated resources for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &allocated[i]);
        total_allocated += allocated[i];
    }

    int min_resources = -1;

    // Try increasing total resources from total_allocated to find safe state
    for (int total_resources = total_allocated; total_resources < 1000; total_resources++) {
        int available = total_resources - total_allocated;
        bool finished[MAX_PROCESSES] = {false};
        int need[MAX_PROCESSES];
        for (int i = 0; i < n; i++) {
            need[i] = max_demand[i] - allocated[i];
        }

        int work = available;
        int count = 0;
        bool found;

        while (count < n) {
            found = false;
            for (int i = 0; i < n; i++) {
                if (!finished[i] && need[i] <= work) {
                    work += allocated[i];
                    finished[i] = true;
                    found = true;
                    count++;
                }
            }
            if (!found) {
                break; // Deadlock situation
            }
        }

        if (count == n) {
            min_resources = total_resources;
            break;
        }
    }

    if (min_resources != -1) {
        printf("Minimum number of resources needed to avoid deadlock: %d\n", min_resources);
    } else {
        printf("Could not determine minimum resources (try smaller values).\n");
    }

    return 0;
}

