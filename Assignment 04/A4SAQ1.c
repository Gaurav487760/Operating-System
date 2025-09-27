#include <stdio.h>
#include <stdbool.h>

#define MAX_REF 12

int main() {
    int referenceString[MAX_REF] = {0, 2, 1, 6, 4, 0, 1, 0, 3, 1, 2, 1};
    int frames[10]; // max 10 frames for simplicity
    int n, i, j, k, pageFaults = 0;
    bool found;
    int nextToReplace = 0;

    printf("Enter number of frames: ");
    scanf("%d", &n);

    // Initialize frames with -1 (empty)
    for (i = 0; i < n; i++) {
        frames[i] = -1;
    }

    printf("\nPage reference string: ");
    for (i = 0; i < MAX_REF; i++) {
        printf("%d ", referenceString[i]);
    }
    printf("\n\n");

    printf("Step-by-step page frames status:\n");

    for (i = 0; i < MAX_REF; i++) {
        found = false;

        // Check if page is already in frames (hit)
        for (j = 0; j < n; j++) {
            if (frames[j] == referenceString[i]) {
                found = true;
                break;
            }
        }

        // Page fault if not found in frames
        if (!found) {
            frames[nextToReplace] = referenceString[i];
            nextToReplace = (nextToReplace + 1) % n;  // FIFO circular index
            pageFaults++;
        }

        // Print current frame status
        printf("After accessing page %d: ", referenceString[i]);
        for (k = 0; k < n; k++) {
            if (frames[k] == -1)
                printf("- ");
            else
                printf("%d ", frames[k]);
        }
        if (found)
            printf("(Hit)\n");
        else
            printf("(Page Fault)\n");
    }

    printf("\nTotal number of page faults = %d\n", pageFaults);

    return 0;
}

