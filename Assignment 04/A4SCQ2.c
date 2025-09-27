#include <stdio.h>
#include <stdbool.h>

#define REF_LENGTH 16

// Function to find the most recently used frame index
int findMRU(int time[], int n) {
    int max = time[0], max_index = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] > max) {
            max = time[i];
            max_index = i;
        }
    }
    return max_index;
}

int main() {
    int frames[100], n, page_faults = 0, time_counter = 0;
    int time[100];  // to track last used time for each frame
    int ref_string[REF_LENGTH] = {2, 5, 2, 8, 5, 4, 1, 2, 3, 2, 6, 1, 2, 5, 9, 8};

    printf("Enter number of frames: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    printf("\nPage Reference String: ");
    for (int i = 0; i < REF_LENGTH; i++)
        printf("%d ", ref_string[i]);
    printf("\n\n");

    printf("Step\tPage\tFrames\t\tPage Fault\n");
    printf("----\t----\t-------------------------\t----------\n");

    for (int i = 0; i < REF_LENGTH; i++) {
        int page = ref_string[i];
        bool hit = false;

        // Check if page is already in frames (page hit)
        for (int j = 0; j < n; j++) {
            if (frames[j] == page) {
                hit = true;
                time[j] = ++time_counter;  // update last used time
                break;
            }
        }

        if (!hit) {
            page_faults++;
            // Check for empty frame
            bool placed = false;
            for (int j = 0; j < n; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    time[j] = ++time_counter;
                    placed = true;
                    break;
                }
            }

            // If no empty frame, replace MRU page
            if (!placed) {
                int mru_index = findMRU(time, n);
                frames[mru_index] = page;
                time[mru_index] = ++time_counter;
            }
        }

        // Print current frame status
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

