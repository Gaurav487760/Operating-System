#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IO_WAIT 2
#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int cpu_burst1;
    int cpu_burst2;
    int start_cpu1;
    int finish_cpu1;
    int start_io;
    int finish_io;
    int start_cpu2;
    int finish_cpu2;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
} Process;

void swap(Process* a, Process* b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

// Function to find next process index using SJF (non-preemptive)
int find_next_process(Process proc[], int n, int current_time) {
    int idx = -1;
    int min_burst = 1 << 30; // large number

    for (int i = 0; i < n; i++) {
        if (!proc[i].is_completed && proc[i].arrival_time <= current_time) {
            if (proc[i].cpu_burst1 < min_burst) {
                min_burst = proc[i].cpu_burst1;
                idx = i;
            } else if (proc[i].cpu_burst1 == min_burst) {
                // If tie, pick lower arrival time
                if (proc[i].arrival_time < proc[idx].arrival_time)
                    idx = i;
            }
        }
    }
    return idx;
}

int main() {
    int n;
    Process proc[MAX_PROCESSES];
    srand(time(0)); // seed random

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        proc[i].is_completed = 0;
        printf("Enter arrival time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter first CPU burst time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].cpu_burst1);
    }

    int completed = 0;
    int current_time = 0;
    int total_turnaround = 0, total_waiting = 0;

    // For Gantt chart display, keep a record of events
    typedef struct {
        char type[10]; // "CPU1", "IO", "CPU2", or "Idle"
        int pid;
        int start;
        int end;
    } Event;

    Event gantt[3 * MAX_PROCESSES + 100]; // enough space
    int e_count = 0;

    while (completed < n) {
        int idx = find_next_process(proc, n, current_time);

        if (idx == -1) {
            // No process available to run, CPU idle
            int next_arrival = 1 << 30;
            for (int i = 0; i < n; i++) {
                if (!proc[i].is_completed && proc[i].arrival_time < next_arrival) {
                    next_arrival = proc[i].arrival_time;
                }
            }
            // CPU idle from current_time to next_arrival
            gantt[e_count].pid = -1;
            snprintf(gantt[e_count].type, sizeof(gantt[e_count].type), "Idle");
            gantt[e_count].start = current_time;
            gantt[e_count].end = next_arrival;
            e_count++;
            current_time = next_arrival;
            continue;
        }

        // Process starts first CPU burst
        proc[idx].start_cpu1 = current_time;
        proc[idx].finish_cpu1 = proc[idx].start_cpu1 + proc[idx].cpu_burst1;

        // Record CPU1 in Gantt
        gantt[e_count].pid = proc[idx].pid;
        snprintf(gantt[e_count].type, sizeof(gantt[e_count].type), "CPU1");
        gantt[e_count].start = proc[idx].start_cpu1;
        gantt[e_count].end = proc[idx].finish_cpu1;
        e_count++;

        current_time = proc[idx].finish_cpu1;

        // I/O wait (fixed)
        proc[idx].start_io = current_time;
        proc[idx].finish_io = proc[idx].start_io + IO_WAIT;

        // Record IO in Gantt
        gantt[e_count].pid = proc[idx].pid;
        snprintf(gantt[e_count].type, sizeof(gantt[e_count].type), "IO");
        gantt[e_count].start = proc[idx].start_io;
        gantt[e_count].end = proc[idx].finish_io;
        e_count++;

        current_time = proc[idx].finish_io;

        // Generate second CPU burst (random 1-10)
        proc[idx].cpu_burst2 = (rand() % 10) + 1;
        proc[idx].start_cpu2 = current_time;
        proc[idx].finish_cpu2 = proc[idx].start_cpu2 + proc[idx].cpu_burst2;

        // Record CPU2 in Gantt
        gantt[e_count].pid = proc[idx].pid;
        snprintf(gantt[e_count].type, sizeof(gantt[e_count].type), "CPU2");
        gantt[e_count].start = proc[idx].start_cpu2;
        gantt[e_count].end = proc[idx].finish_cpu2;
        e_count++;

        current_time = proc[idx].finish_cpu2;

        proc[idx].completion_time = proc[idx].finish_cpu2;
        proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
        proc[idx].waiting_time = proc[idx].turnaround_time - (proc[idx].cpu_burst1 + IO_WAIT + proc[idx].cpu_burst2);

        proc[idx].is_completed = 1;
        completed++;
    }

    // Print Gantt chart
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < e_count; i++) {
        if (gantt[i].pid == -1)
            printf(" Idle(%d-%d) |", gantt[i].start, gantt[i].end);
        else
            printf(" P%d_%s(%d-%d) |", gantt[i].pid, gantt[i].type, gantt[i].start, gantt[i].end);
    }
    printf("\n");

    // Print turnaround and waiting times
    printf("\nPID\tArrival\tCPU1\tCPU2\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
            proc[i].pid,
            proc[i].arrival_time,
            proc[i].cpu_burst1,
            proc[i].cpu_burst2,
            proc[i].completion_time,
            proc[i].turnaround_time,
            proc[i].waiting_time);

        total_turnaround += proc[i].turnaround_time;
        total_waiting += proc[i].waiting_time;
    }

    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting / n);

    return 0;
}

