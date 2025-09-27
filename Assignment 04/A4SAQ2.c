#include <stdio.h>
#include <stdbool.h>

#define REF_LENGTH 15

int main() {
    int frames[100], n, page_faults = 0;
    int front = 0;  // points to the oldest page
    int ref_string[REF_LENGTH] = {3, 4, 5, 6, 3, 4, 7, 3, 4, 5, 6, 7, 2, 4, 6};

    printf("Enter number of frames: ");
    scanf("%d", &n);

    // Initialize frames to -1 (empty)
    for (int i = 0; i < n; i++)
        frames[i] = -1;

    printf("\nPage Reference String: ");
    for (int i = 0; i < REF_LENGTH; i++)
        printf("%d ", ref_string[i]);
    printf("\n\n");

    printf("Step\tPage\tFrames\t\tPage Fault\n");
    printf("----\t----\t-------------------------\t----------\n");

    for (int i = 0; i < REF_LENGTH; i++) {
        int page = ref_string[i];
        bool hit = false;

        // Check if page is already in frame (page hit)
        for (int j = 0; j < n; j++) {
            if (frames[j] == page) {
                hit = true;
                break;
            }
        }

        if (!hit) {
            page_faults++;
            // If there is space, insert at next empty spot
            bool placed = false;
            for (int j = 0; j < n; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    placed = true;
                    break;
                }
            }
            // If full, replace page at front pointer (FIFO)
            if (!placed) {
                frames[front] = page;
                front = (front + 1) % n;
            }
        }

        // Print step info
        printf("%2d\t%4d\t", i + 1, page);
        for (int j = 0; j < n; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\t\t%s\n", hit ? "No" : "Yes");
    }

    printf("\nTotal Page Faults = %d\n", page_faults);

    return 0;
}

