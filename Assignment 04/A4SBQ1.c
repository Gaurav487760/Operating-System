#include <stdio.h>
#include <stdbool.h>

#define MAX_REF 15  // length of the reference string

int main() {
    int referenceString[MAX_REF] = {3, 4, 5, 6, 3, 4, 7, 3, 4, 5, 6, 7, 2, 4, 6};
    int frames[10];  // Max 10 frames, you can increase if needed
    int n;           // Number of frames input by user
    int i, j, k;
    int pageFaults = 0;
    bool found;
    int nextToReplace = 0;

    printf("Enter number of frames: ");
    scanf("%d", &n);

    // Initialize frames with -1 indicating empty frames
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

        // Check if page is already in frames (Page Hit)
        for (j = 0; j < n; j++) {
            if (frames[j] == referenceString[i]) {
                found = true;
                break;
            }
        }

        // If not found, Page Fault occurs - replace the oldest page (FIFO)
        if (!found) {
            frames[nextToReplace] = referenceString[i];
            nextToReplace = (nextToReplace + 1) % n;  // Circular index for FIFO
            pageFaults++;
        }

        // Display current frames after accessing this page
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

