#include <stdio.h>

int main() {
    int n, m;  // n = number of processes, m = number of resources
    int max[10][10], allocation[10][10], need[10][10];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the number of resources: ");
    scanf("%d", &m);

    printf("Enter the Maximum matrix (max demand for each process-resource):\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the Allocation matrix (allocated resources for each process-resource):\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate Need matrix = Max - Allocation
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Display Need matrix
    printf("\nNeed Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < m; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    return 0;
}

