#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PROCESSES 20
#define MAX_BURST 10

typedef struct {
    int pid;
    int arrival_time;
    int cpu_burst;
    int remaining_burst;
    int io_time;            // I/O waiting time remaining (2 units fixed)
    int completed;          // flag if process finished
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int last_executed_time; // last time process was executed (for waiting calculation)
} Process;

typedef struct {
    int pid;
    int start_time;
    int end_time;
} GanttChartEntry;

int getNextBurst() {
    return (rand() % MAX_BURST) + 1;
}

int main() {
    int n, time_quantum;
    Process p[MAX_PROCESSES];
    GanttChartEntry gantt[1000];
    int gantt_count = 0;

    srand(time(NULL));

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time for process %d: ", p[i].pid);
        scanf("%d", &p[i].arrival_time);
        printf("Enter first CPU burst for process %d: ", p[i].pid);
        scanf("%d", &p[i].cpu_burst);
        p[i].remaining_burst = p[i].cpu_burst;
        p[i].io_time = 0;
        p[i].completed = 0;
        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].last_executed_time = p[i].arrival_time;
    }

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    int time = 0;
    int completed_processes = 0;
    int current_index = 0;
    bool cpu_idle = true;

    // Queue for processes (simple round robin queue using an array)
    int queue[1000];
    int front = 0, rear = 0;

    // Enqueue helper
    void enqueue(int x) {
        queue[rear++] = x;
    }
    // Dequeue helper
    int dequeue() {
        if (front == rear) return -1;
        return queue[front++];
    }
    // Check if queue is empty
    bool isEmpty() {
        return front == rear;
    }

    // Enqueue processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time == 0 && p[i].remaining_burst > 0 && p[i].io_time == 0 && !p[i].completed) {
            enqueue(i);
        }
    }

    printf("\nGantt Chart:\n");
    printf("Time\tProcess\n");

    int running_process = -1;
    int quantum_counter = 0;
    int last_time = 0;

    while (completed_processes < n) {

        // Add newly arrived processes at current time to the queue
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time == time && p[i].remaining_burst > 0 && p[i].io_time == 0 && !p[i].completed) {
                enqueue(i);
            }
        }

        // Decrement IO time for processes in IO
        for (int i = 0; i < n; i++) {
            if (p[i].io_time > 0) {
                p[i].io_time--;
                if (p[i].io_time == 0) {
                    // IO finished, generate next CPU burst randomly
                    p[i].remaining_burst = getNextBurst();
                    // Arrival time reset to current time for RR scheduling
                    p[i].arrival_time = time;
                    enqueue(i);
                }
            }
        }

        if (running_process == -1) {
            // If CPU is idle, dequeue next process
            running_process = dequeue();
            quantum_counter = 0;

            if (running_process != -1) {
                // Log Gantt chart start time for this process
                if (time != last_time) {
                    gantt[gantt_count].pid = running_process == -1 ? -1 : p[running_process].pid;
                    gantt[gantt_count].start_time = last_time;
                    gantt[gantt_count].end_time = time;
                    gantt_count++;
                    last_time = time;
                }
            }
        }

        if (running_process != -1) {
            // Execute running process for 1 unit
            if (p[running_process].last_executed_time < time) {
                p[running_process].waiting_time += (time - p[running_process].last_executed_time);
            }

            p[running_process].remaining_burst--;
            quantum_counter++;
            time++;

            // Print Gantt info
            printf("%d\tP%d\n", time - 1, p[running_process].pid);

            p[running_process].last_executed_time = time;

            // If burst completes
            if (p[running_process].remaining_burst == 0) {
                // Process goes to IO if more CPU bursts left (simulate here only one burst, or more if extended)
                p[running_process].io_time = 2;
                p[running_process].arrival_time = time + p[running_process].io_time; // next arrival time after IO

                // For this simulation, we assume process ends after first CPU burst + IO (can be extended)
                p[running_process].completed = 1;
                p[running_process].completion_time = time;
                completed_processes++;

                running_process = -1;
                quantum_counter = 0;
            } else if (quantum_counter == time_quantum) {
                // Time quantum finished, put process back to queue
                enqueue(running_process);
                running_process = -1;
                quantum_counter = 0;
            }
        } else {
            // CPU idle
            printf("%d\tIdle\n", time);
            time++;
            last_time = time;
        }
    }

    // Add last gantt entry
    if (gantt_count > 0 && last_time < time) {
        gantt[gantt_count].pid = -1; // no running process
        gantt[gantt_count].start_time = last_time;
        gantt[gantt_count].end_time = time;
        gantt_count++;
    }

    // Calculate turnaround time for each process
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        if (p[i].turnaround_time < 0) p[i].turnaround_time = 0;
    }

    // Print process info
    printf("\nProcess\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].cpu_burst,
               p[i].completion_time,
               p[i].waiting_time,
               p[i].turnaround_time);
    }

    // Calculate average waiting and turnaround times
    double avg_wait = 0, avg_turnaround = 0;
    for (int i = 0; i < n; i++) {
        avg_wait += p[i].waiting_time;
        avg_turnaround += p[i].turnaround_time;
    }
    avg_wait /= n;
    avg_turnaround /= n;

    printf("\nAverage Waiting Time = %.2lf\n", avg_wait);
    printf("Average Turnaround Time = %.2lf\n", avg_turnaround);

    return 0;
}

