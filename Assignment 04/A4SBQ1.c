#include <stdio.h>
#include <stdbool.h>

#define REF_LENGTH 16

int main() {
    int frames[100], n, frame_count = 0, page_faults = 0;
    int ref_string[REF_LENGTH] = {12,15,12,18,6,8,11,12,19,12,6,8,12,15,19,8};

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

            // If there is an empty frame, fill it
            if (frame_count < n) {
                frames[frame_count++] = page;
            } else {
                // OPT replacement: find the page in frames that won't be used for longest time

                int farthest = i + 1;
                int replace_index = -1;

                for (int j = 0; j < n; j++) {
                    int k;
                    // Search future reference for frames[j]
                    for (k = i + 1; k < REF_LENGTH; k++) {
                        if (frames[j] == ref_string[k]) {
                            if (k > farthest) {
                                farthest = k;
                                replace_index = j;
                            }
                            break;
                        }
                    }
                    // If the page is never used again, replace it immediately
                    if (k == REF_LENGTH) {
                        replace_index = j;
                        break;
                    }
                }

                // If all pages are used in future, but we picked no page to replace,
                // replace the first frame by default
                if (replace_index == -1)
                    replace_index = 0;

                frames[replace_index] = page;
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

