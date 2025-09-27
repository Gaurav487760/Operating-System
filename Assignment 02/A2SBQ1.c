#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PROCESSES 100
#define IO_WAIT 2

typedef struct {
    int pid;
    int arrival_time;
    int cpu_burst1;
    int cpu_burst1_remaining;
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
    bool is_cpu1_done;
    bool is_io_done;
    bool is_completed;
} Process;

typedef struct {
    char type[10];  // CPU1, IO, CPU2, Idle
    int pid;
    int start;
    int end;
} Event;

int all_processes_completed(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        if (!proc[i].is_completed)
            return 0;
    }
    return 1;
}

int main() {
    int n;
    Process proc[MAX_PROCESSES];
    Event gantt[1000];
    int event_count = 0;

    srand(time(NULL));

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter arrival time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter first CPU burst for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].cpu_burst1);
        proc[i].cpu_burst1_remaining = proc[i].cpu_burst1;
        proc[i].cpu_burst2 = 0;
        proc[i].is_cpu1_done = false;
        proc[i].is_io_done = false;
        proc[i].is_completed = false;
        proc[i].start_cpu1 = -1;
        proc[i].start_io = -1;
        proc[i].start_cpu2 = -1;
    }

    int time = 0;
    int last_pid = -1;  // for Gantt chart event tracking

    // Ready queue for second CPU bursts (processes that finished I/O)
    Process *ready_for_cpu2[MAX_PROCESSES];
    int cpu2_count = 0, cpu2_index = 0;

    while (!all_processes_completed(proc, n)) {
        // 1. Check which processes have arrived and not done first CPU burst
        int idx = -1;
        int min_remaining = 1 << 30;

        for (int i = 0; i < n; i++) {
            if (!proc[i].is_cpu1_done && proc[i].arrival_time <= time && proc[i].cpu_burst1_remaining > 0) {
                if (proc[i].cpu_burst1_remaining < min_remaining) {
                    min_remaining = proc[i].cpu_burst1_remaining;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // Process idx runs for 1 unit time
            if (proc[idx].start_cpu1 == -1) {
                proc[idx].start_cpu1 = time;
            }

            // For Gantt chart: record event changes
            if (last_pid != proc[idx].pid || (event_count > 0 && strcmp(gantt[event_count-1].type, "CPU1") != 0)) {
                // Start a new CPU1 event
                gantt[event_count].pid = proc[idx].pid;
                snprintf(gantt[event_count].type, sizeof(gantt[event_count].type), "CPU1");
                gantt[event_count].start = time;
                event_count++;
            }

            proc[idx].cpu_burst1_remaining--;

            // If CPU1 finished for this process
            if (proc[idx].cpu_burst1_remaining == 0) {
                proc[idx].finish_cpu1 = time + 1;
                proc[idx].is_cpu1_done = true;

                // Close the current CPU1 event in Gantt chart
                gantt[event_count-1].end = time + 1;

                // Schedule I/O immediately (fixed 2 units)
                proc[idx].start_io = time + 1;
                proc[idx].finish_io = proc[idx].start_io + IO_WAIT;

                // Add IO event to Gantt chart
                gantt[event_count].pid = proc[idx].pid;
                snprintf(gantt[event_count].type, sizeof(gantt[event_count].type), "IO");
                gantt[event_count].start = proc[idx].start_io;
                gantt[event_count].end = proc[idx].finish_io;
                event_count++;

            } else {
                // CPU1 continues - do not close event yet
            }

            last_pid = proc[idx].pid;

            time++;  // increment time after running 1 unit

        } else {
            // No process available for CPU1 - CPU idle
            if (event_count > 0 && strcmp(gantt[event_count-1].type, "Idle") == 0) {
                gantt[event_count-1].end++;
            } else {
                gantt[event_count].pid = -1;
                snprintf(gantt[event_count].type, sizeof(gantt[event_count].type), "Idle");
                gantt[event_count].start = time;
                gantt[event_count].end = time + 1;
                event_count++;
            }
            last_pid = -1;
            time++;
        }

        // After updating time, check for processes that finished IO and are ready for CPU2
        for (int i = 0; i < n; i++) {
            if (proc[i].is_cpu1_done && !proc[i].is_io_done && proc[i].finish_io <= time) {
                proc[i].is_io_done = true;
                // Generate random CPU2 burst 1-10
                proc[i].cpu_burst2 = (rand() % 10) + 1;
                // Add process to ready queue for CPU2
                ready_for_cpu2[cpu2_count++] = &proc[i];
            }
        }
    }

    // Now process CPU2 bursts in FCFS order (non-preemptive)
    for (cpu2_index = 0; cpu2_index < cpu2_count; cpu2_index++) {
        Process *p = ready_for_cpu2[cpu2_index];

        if (time < p->finish_io) {
            // Wait if CPU is idle until IO finishes
            if (event_count > 0 && strcmp(gantt[event_count-1].type, "Idle") == 0) {
                gantt[event_count-1].end = p->finish_io;
            } else {
                gantt[event_count].pid = -1;
                snprintf(gantt[event_count].type, sizeof(gantt[event_count].type), "Idle");
                gantt[event_count].start = time;
                gantt[event_count].end = p->finish_io;
                event_count++;
            }
            time = p->finish_io;
        }

        // Start CPU2 burst
        p->start_cpu2 = time;
        p->finish_cpu2 = p->start_cpu2 + p->cpu_burst2;

        // Add CPU2 event
        gantt[event_count].pid = p->pid;
        snprintf(gantt[event_count].type, sizeof(gantt[event_count].type), "CPU2");
        gantt[event_count].start = p->start_cpu2;
        gantt[event_count].end = p->finish_cpu2;
        event_count++;

        time = p->finish_cpu2;

        // Mark process complete and calculate times
        p->completion_time = p->finish_cpu2;
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - (p->cpu_burst1 + IO_WAIT + p->cpu_burst2);
        p->is_completed = true;
    }

    // Print Gantt Chart
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < event_count; i++) {
        if (gantt[i].pid == -1)
            printf(" Idle(%d-%d) |", gantt[i].start, gantt[i].end);
        else
            printf(" P%d_%s(%d-%d) |", gantt[i].pid, gantt[i].type, gantt[i].start, gantt[i].end);
    }
    printf("\n");

    // Print turnaround and waiting times
    printf("\nPID\tArrival\tCPU1\tCPU2\tCompletion\tTurnaround\tWaiting\n");

    float total_turnaround = 0, total_waiting = 0;

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

    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", total_waiting / n);

    return 0;
}

