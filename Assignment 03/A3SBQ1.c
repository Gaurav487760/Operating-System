#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n, m;  // number of processes and resources
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

// Function to calculate Need matrix
void calculate_need() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n--- Banker's Algorithm Menu ---\n");
        printf("1. Accept Allocation and Max matrices\n");
        printf("2. Accept Available resources\n");
        printf("3. Display Allocation and Max matrices\n");
        printf("4. Calculate and Display Need matrix\n");
        printf("5. Display Available resources\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter number of processes: ");
                scanf("%d", &n);
                printf("Enter number of resources: ");
                scanf("%d", &m);

                printf("Enter Allocation matrix:\n");
                for (int i = 0; i < n; i++) {
                    printf("Process %d:\n", i);
                    for (int j = 0; j < m; j++) {
                        scanf("%d", &allocation[i][j]);
                    }
                }

                printf("Enter Max matrix:\n");
                for (int i = 0; i < n; i++) {
                    printf("Process %d:\n", i);
                    for (int j = 0; j < m; j++) {
                        scanf("%d", &max[i][j]);
                    }
                }
                break;

            case 2:
                printf("Enter Available resources:\n");
                for (int j = 0; j < m; j++) {
                    scanf("%d", &available[j]);
                }
                break;

            case 3:
                printf("Allocation matrix:\n");
                for (int i = 0; i < n; i++) {
                    printf("Process %d: ", i);
                    for (int j = 0; j < m; j++) {
                        printf("%d ", allocation[i][j]);
                    }
                    printf("\n");
                }

                printf("Max matrix:\n");
                for (int i = 0; i < n; i++) {
                    printf("Process %d: ", i);
                    for (int j = 0; j < m; j++) {
                        printf("%d ", max[i][j]);
                    }
                    printf("\n");
                }
                break;

            case 4:
                calculate_need();
                printf("Need matrix:\n");
                for (int i = 0; i < n; i++) {
                    printf("Process %d: ", i);
                    for (int j = 0; j < m; j++) {
                        printf("%d ", need[i][j]);
                    }
                    printf("\n");
                }
                break;

            case 5:
                printf("Available resources: ");
                for (int j = 0; j < m; j++) {
                    printf("%d ", available[j]);
                }
                printf("\n");
                break;

            case 6:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

