#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PROCESSES 20
#define MAX_BURST 10

typedef struct {
    int pid;
    int arrival_time;
    int priority;
    int cpu_burst;
    int remaining_burst;
    int io_time;         // remaining I/O time (2 units fixed)
    int completed;       // flag for process completion
    int start_time;      // first time process gets CPU
    int completion_time; // time process finishes all bursts
    int waiting_time;
    int turnaround_time;
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
    int n;
    Process p[MAX_PROCESSES];
    GanttChartEntry gantt[1000];
    int gantt_count = 0;

    srand(time(NULL));

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process %d arrival time: ", p[i].pid);
        scanf("%d", &p[i].arrival_time);
        printf("Process %d first CPU burst: ", p[i].pid);
        scanf("%d", &p[i].cpu_burst);
        printf("Process %d priority (lower number means higher priority): ", p[i].pid);
        scanf("%d", &p[i].priority);

        p[i].remaining_burst = p[i].cpu_burst;
        p[i].io_time = 0;
        p[i].completed = 0;
        p[i].start_time = -1;
        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
    }

    int time = 0;
    int completed_processes = 0;
    int current_pid = -1; // currently running process pid
    int current_priority = 1000000; // very low priority initially

    // For waiting time calculation
    int last_switch_time = 0;

    printf("\nGantt Chart:\n");

    while (completed_processes < n) {
        // Find the process with highest priority (lowest priority number)
        // that has arrived, not completed, not in I/O, and has remaining burst
        int highest_priority = 1000000;
        int selected = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && !p[i].completed && p[i].remaining_burst > 0 && p[i].io_time == 0) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    selected = i;
                } else if (p[i].priority == highest_priority) {
                    // tie-breaker: choose process with earlier arrival time
                    if (p[i].arrival_time < p[selected].arrival_time) {
                        selected = i;
                    }
                }
            }
        }

        // If currently running process is not the selected one, preempt
        if (selected != current_pid) {
            // Save Gantt chart for previous process if running
            if (current_pid != -1 && time > last_switch_time) {
                gantt[gantt_count].pid = p[current_pid].pid;
                gantt[gantt_count].start_time = last_switch_time;
                gantt[gantt_count].end_time = time;
                gantt_count++;
            }
            current_pid = selected;
            last_switch_time = time;
        }

        if (current_pid == -1) {
            // No process ready; CPU idle
            time++;
            // Decrement I/O time of all processes that are in I/O
            for (int i = 0; i < n; i++) {
                if (p[i].io_time > 0) {
                    p[i].io_time--;
                    if (p[i].io_time == 0) {
                        // Process finished I/O, generate next CPU burst randomly
                        p[i].remaining_burst = getNextBurst();
                        //printf("Process %d finished I/O and got new burst %d at time %d\n", p[i].pid, p[i].remaining_burst, time);
                    }
                }
            }
            continue;
        }

        // Run the selected process for 1 unit time
        if (p[current_pid].start_time == -1) p[current_pid].start_time = time;

        p[current_pid].remaining_burst--;
        time++;

        // Decrement I/O time for other processes in I/O
        for (int i = 0; i < n; i++) {
            if (i != current_pid && p[i].io_time > 0) {
                p[i].io_time--;
                if (p[i].io_time == 0) {
                    p[i].remaining_burst = getNextBurst();
                    //printf("Process %d finished I/O and got new burst %d at time %d\n", p[i].pid, p[i].remaining_burst, time);
                }
            }
        }

        // Check if current process finished its CPU burst
        if (p[current_pid].remaining_burst == 0) {
            // Process goes to I/O if CPU burst not zero in future
            // But since we generate CPU burst randomly only when I/O finished, we simulate by fixed I/O here
            p[current_pid].io_time = 2;

            // Check if the next burst is 0 means process ends
            // So we check after I/O completes we generate next burst
            // But to keep logic simple, when the process completes after I/O and no new burst, we mark complete

            // For this simulation, we assume if after I/O the next burst generated is zero, then process ends.
            // But since we generate random bursts [1..10], process never ends in this simple simulation.
            // So we consider process completed if user entered only one burst, or simulate ending after one burst + I/O for demonstration.

            // So to simulate completion, we consider process ends after first burst + I/O (or you can modify)

            // For demonstration, let's end process after first burst + I/O (after current burst)

            p[current_pid].completed = 1;
            p[current_pid].completion_time = time + p[current_pid].io_time;  // will be end after I/O
            completed_processes++;

            // Reset current process to idle
            current_pid = -1;
        }
    }

    // Print last Gantt process
    if (current_pid != -1 && time > last_switch_time) {
        gantt[gantt_count].pid = p[current_pid].pid;
        gantt[gantt_count].start_time = last_switch_time;
        gantt[gantt_count].end_time = time;
        gantt_count++;
    }

    // Calculate turnaround and waiting times
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].cpu_burst;
        if (p[i].waiting_time < 0) p[i].waiting_time = 0;  // safety check
    }

    // Print Gantt chart
    printf("\nGantt Chart (Process : start_time -> end_time):\n");
    for (int i = 0; i < gantt_count; i++) {
        printf("P%d : %d -> %d\n", gantt[i].pid, gantt[i].start_time, gantt[i].end_time);
    }

    // Print process details
    printf("\nProcess\tArrival\tPriority\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t\t%d\t%d\t\t%d\t\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].priority,
               p[i].cpu_burst,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    // Calculate and print average times
    double avg_waiting = 0, avg_turnaround = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting += p[i].waiting_time;
        avg_turnaround += p[i].turnaround_time;
    }
    avg_waiting /= n;
    avg_turnaround /= n;

    printf("\nAverage Waiting Time = %.2lf\n", avg_waiting);
    printf("Average Turnaround Time = %.2lf\n", avg_turnaround);

    return 0;
}

