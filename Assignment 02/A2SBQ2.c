#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IO_WAIT 2
#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int priority;
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
    int is_completed_cpu1;
    int is_completed;
} Process;

typedef struct {
    char type[10];  // CPU1, IO, CPU2, Idle
    int pid;
    int start;
    int end;
} Event;

int all_completed(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        if (!p[i].is_completed) return 0;
    }
    return 1;
}

int main() {
    int n;
    Process proc[MAX_PROCESSES];
    Event gantt[1000];
    int e_count = 0;

    srand(time(NULL));

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter arrival time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter first CPU burst for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].cpu_burst1);
        printf("Enter priority for process %d (lower number = higher priority): ", proc[i].pid);
        scanf("%d", &proc[i].priority);
        proc[i].cpu_burst2 = 0;
        proc[i].is_completed_cpu1 = 0;
        proc[i].is_completed = 0;
        proc[i].start_cpu1 = -1;
        proc[i].start_io = -1;
        proc[i].start_cpu2 = -1;
    }

    int time = 0;
    int last_pid = -1;

    // Queue for CPU2 bursts (processes done with IO)
    Process* ready_for_cpu2[MAX_PROCESSES];
    int cpu2_count = 0, cpu2_index = 0;

    while (!all_completed(proc, n)) {
        // Find next process for CPU1 (non-preemptive priority scheduling)
        int idx = -1;
        int highest_priority = 1 << 30;

        for (int i = 0; i < n; i++) {
            if (!proc[i].is_completed_cpu1 && proc[i].arrival_time <= time) {
                if (proc[i].priority < highest_priority) {
                    highest_priority = proc[i].priority;
                    idx = i;
                } else if (proc[i].priority == highest_priority) {
                    // Tie-breaker: earlier arrival
                    if (proc[i].arrival_time < proc[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx == -1) {
            // CPU idle
            if (e_count > 0 && strcmp(gantt[e_count - 1].type, "Idle") == 0) {
                gantt[e_count - 1].end++;
            } else {
                gantt[e_count].pid = -1;
                strcpy(gantt[e_count].type, "Idle");
                gantt[e_count].start = time;
                gantt[e_count].end = time + 1;
                e_count++;
            }
            time++;
        } else {
            // Run CPU1 non-preemptively
            proc[idx].start_cpu1 = time;
            proc[idx].finish_cpu1 = proc[idx].start_cpu1 + proc[idx].cpu_burst1;

            // Record CPU1 event
            gantt[e_count].pid = proc[idx].pid;
            strcpy(gantt[e_count].type, "CPU1");
            gantt[e_count].start = proc[idx].start_cpu1;
            gantt[e_count].end = proc[idx].finish_cpu1;
            e_count++;

            time = proc[idx].finish_cpu1;
            proc[idx].is_completed_cpu1 = 1;

            // I/O wait fixed
            proc[idx].start_io = time;
            proc[idx].finish_io = proc[idx].start_io + IO_WAIT;

            gantt[e_count].pid = proc[idx].pid;
            strcpy(gantt[e_count].type, "IO");
            gantt[e_count].start = proc[idx].start_io;
            gantt[e_count].end = proc[idx].finish_io;
            e_count++;

            // After IO, add to CPU2 queue with random burst
            proc[idx].cpu_burst2 = (rand() % 10) + 1;

            // The process is not yet completed overall, will complete after CPU2
            // Mark it as ready for CPU2 after finishing IO
        }

        // Check processes whose IO completed at or before current time, add to CPU2 ready queue
        for (int i = 0; i < n; i++) {
            if (proc[i].is_completed_cpu1 && !proc[i].is_completed && proc[i].finish_io <= time) {
                int already_in_queue = 0;
                for (int j = 0; j < cpu2_count; j++) {
                    if (ready_for_cpu2[j]->pid == proc[i].pid) {
                        already_in_queue = 1;
                        break;
                    }
                }
                if (!already_in_queue) {
                    ready_for_cpu2[cpu2_count++] = &proc[i];
                }
            }
        }
    }

    // Process CPU2 bursts FCFS
    for (cpu2_index = 0; cpu2_index < cpu2_count; cpu2_index++) {
        Process* p = ready_for_cpu2[cpu2_index];

        // If CPU idle waiting for process IO to complete
        if (time < p->finish_io) {
            if (e_count > 0 && strcmp(gantt[e_count - 1].type, "Idle") == 0) {
                gantt[e_count - 1].end = p->finish_io;
            } else {
                gantt[e_count].pid = -1;
                strcpy(gantt[e_count].type, "Idle");
                gantt[e_count].start = time;
                gantt[e_count].end = p->finish_io;
                e_count++;
            }
            time = p->finish_io;
        }

        p->start_cpu2 = time;
        p->finish_cpu2 = p->start_cpu2 + p->cpu_burst2;

        gantt[e_count].pid = p->pid;
        strcpy(gantt[e_count].type, "CPU2");
        gantt[e_count].start = p->start_cpu2;
        gantt[e_count].end = p->finish_cpu2;
        e_count++;

        time = p->finish_cpu2;

        p->completion_time = p->finish_cpu2;
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - (p->cpu_burst1 + IO_WAIT + p->cpu_burst2);
        p->is_completed = 1;
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

    // Print results
    printf("\nPID\tArrival\tPriority\tCPU1\tCPU2\tCompletion\tTurnaround\tWaiting\n");
    float total_turnaround = 0, total_waiting = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t\t%d\t%d\t%d\t\t%d\t\t%d\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].priority,
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

