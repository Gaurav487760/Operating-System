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
} Process;

void sort_by_arrival(Process proc[], int n) {
    Process temp;
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-1-i; j++) {
            if(proc[j].arrival_time > proc[j+1].arrival_time) {
                temp = proc[j];
                proc[j] = proc[j+1];
                proc[j+1] = temp;
            }
        }
    }
}

int main() {
    int n;
    Process proc[MAX_PROCESSES];
    srand(time(0)); // seed random

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(int i=0; i<n; i++) {
        proc[i].pid = i+1;
        printf("Enter arrival time for process %d: ", i+1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter first CPU burst time for process %d: ", i+1);
        scanf("%d", &proc[i].cpu_burst1);
    }

    // Sort processes by arrival time
    sort_by_arrival(proc, n);

    int current_time = 0;

    printf("\nProcess execution order and bursts:\n");
    for(int i=0; i<n; i++) {
        // If CPU idle until process arrives
        if(current_time < proc[i].arrival_time) {
            printf("CPU Idle from %d to %d\n", current_time, proc[i].arrival_time);
            current_time = proc[i].arrival_time;
        }

        // First CPU burst
        proc[i].start_cpu1 = current_time;
        proc[i].finish_cpu1 = proc[i].start_cpu1 + proc[i].cpu_burst1;
        current_time = proc[i].finish_cpu1;

        // I/O wait fixed
        proc[i].start_io = current_time;
        proc[i].finish_io = proc[i].start_io + IO_WAIT;
        current_time = proc[i].finish_io;

        // Second CPU burst random 1 to 10
        proc[i].cpu_burst2 = (rand() % 10) + 1;
        proc[i].start_cpu2 = current_time;
        proc[i].finish_cpu2 = proc[i].start_cpu2 + proc[i].cpu_burst2;
        current_time = proc[i].finish_cpu2;

        proc[i].completion_time = proc[i].finish_cpu2;

        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - (proc[i].cpu_burst1 + IO_WAIT + proc[i].cpu_burst2);

        printf("P%d: CPU1(%d-%d) -> IO(%d-%d) -> CPU2(%d-%d)\n",
            proc[i].pid,
            proc[i].start_cpu1, proc[i].finish_cpu1,
            proc[i].start_io, proc[i].finish_io,
            proc[i].start_cpu2, proc[i].finish_cpu2);
    }

    // Print Gantt chart
    printf("\nGantt Chart:\n|");
    current_time = 0;
    for(int i=0; i<n; i++) {
        if(current_time < proc[i].arrival_time) {
            printf(" Idle(%d-%d) |", current_time, proc[i].arrival_time);
            current_time = proc[i].arrival_time;
        }
        printf(" P%d_CPU1(%d-%d) | IO(%d-%d) | P%d_CPU2(%d-%d) |",
            proc[i].pid,
            proc[i].start_cpu1, proc[i].finish_cpu1,
            proc[i].start_io, proc[i].finish_io,
            proc[i].pid,
            proc[i].start_cpu2, proc[i].finish_cpu2);

        current_time = proc[i].finish_cpu2;
    }
    printf("\n");

    // Print turnaround and waiting times
    printf("\nPID\tArrival\tCPU1\tCPU2\tCompletion\tTurnaround\tWaiting\n");
    int total_turnaround = 0, total_waiting = 0;
    for(int i=0; i<n; i++) {
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

