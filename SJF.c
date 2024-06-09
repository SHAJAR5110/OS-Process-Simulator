#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Structure to represent a process
typedef struct {
    int id;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

// Structure to pass arguments to thread function
typedef struct {
    int id;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} ThreadArgs;

// Comparison function for sorting processes by burst time
int compare(const void *a, const void *b) {
    Process *processA = (Process *)a;
    Process *processB = (Process *)b;
    return processA->burst_time - processB->burst_time;
}

// Thread function to simulate process execution
void* execute_process(void *args) {
    ThreadArgs *process_args = (ThreadArgs *)args;
    printf("Process %d with burst time %d is starting.\n", process_args->id, process_args->burst_time);
    sleep(process_args->burst_time); // Simulate process execution
    printf("Process %d with burst time %d has finished.\n", process_args->id, process_args->burst_time);
    return NULL;
}

int main() {
    int n;

    // Input number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[n];
    pthread_t threads[n];
    ThreadArgs thread_args[n];

    // Input burst times for each process
    for (int i = 0; i < n; i++) {
        printf("Enter Burst/Service time for process %d: ", i);
        scanf("%d", &processes[i].burst_time);
        processes[i].id = i;
    }

    // Sort processes by burst time (Shortest Job First)
    qsort(processes, n, sizeof(Process), compare);

    // Calculate waiting times and turnaround times
    processes[0].waiting_time = 0;
    processes[0].turnaround_time = processes[0].burst_time;
    float total_wt = 0, total_tat = processes[0].turnaround_time;

    for (int i = 1; i < n; i++) {
        processes[i].waiting_time = processes[i-1].waiting_time + processes[i-1].burst_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        total_wt += processes[i].waiting_time;
        total_tat += processes[i].turnaround_time;
    }

    // Create and start threads based on sorted process order
    for (int i = 0; i < n; i++) {
        thread_args[i].id = processes[i].id;
        thread_args[i].burst_time = processes[i].burst_time;
        thread_args[i].waiting_time = processes[i].waiting_time;
        thread_args[i].turnaround_time = processes[i].turnaround_time;
        pthread_create(&threads[i], NULL, execute_process, &thread_args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // Output results
    printf("\n PROCESS \t BURST TIME \t WAITING TIME \t TURNAROUND TIME\n");
    for (int i = 0; i < n; i++) {
        printf("P%d \t\t %d \t\t %d \t\t %d\n", processes[i].id, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
    printf("\nAverage waiting time: %f ", total_wt/n);
    printf("\nAverage turnaround time: %f \n", total_tat/n);

    return 0;
}

