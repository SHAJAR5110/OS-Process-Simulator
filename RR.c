#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Structure to represent a process
typedef struct {
    int id;
    int burst_time;
    int remaining_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
} Process;

// Structure to pass arguments to thread function
typedef struct {
    int id;
    int quantum;
    int *remaining_time;
} ThreadArgs;

pthread_mutex_t lock;
int current_time = 0;

// Thread function to simulate process execution
void* execute_process(void *args) {
    ThreadArgs *process_args = (ThreadArgs *)args;
    while (*process_args->remaining_time > 0) {
        pthread_mutex_lock(&lock);
        if (*process_args->remaining_time > 0) {
            int time_slice = (*process_args->remaining_time > process_args->quantum) ? process_args->quantum : *process_args->remaining_time;
            printf("Process %d is executing for %d time units.\n", process_args->id, time_slice);
            sleep(time_slice); // Simulate process execution
            current_time += time_slice;
            *process_args->remaining_time -= time_slice;
            printf("Process %d has %d time units remaining.\n", process_args->id, *process_args->remaining_time);
        }
        pthread_mutex_unlock(&lock);
        sleep(1); // Simulate context switch
    }
    return NULL;
}

int main() {
    int n, quantum;

    // Input number of processes and quantum time
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the quantum time: ");
    scanf("%d", &quantum);

    Process processes[n];
    pthread_t threads[n];
    ThreadArgs thread_args[n];

    // Input burst times for each process
    for (int i = 0; i < n; i++) {
        printf("Enter Burst/Service time for process %d: ", i);
        scanf("%d", &processes[i].burst_time);
        processes[i].id = i;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }

    // Initialize the mutex lock
    pthread_mutex_init(&lock, NULL);

    // Create and start threads
    for (int i = 0; i < n; i++) {
        thread_args[i].id = processes[i].id;
        thread_args[i].quantum = quantum;
        thread_args[i].remaining_time = &processes[i].remaining_time;
        pthread_create(&threads[i], NULL, execute_process, &thread_args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate waiting times and turnaround times
    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time = current_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }

    // Output results
    float total_wt = 0, total_tat = 0;
    printf("\n PROCESS \t BURST TIME \t WAITING TIME \t TURNAROUND TIME\n");
    for (int i = 0; i < n; i++) {
        printf("P%d \t\t %d \t\t %d \t\t %d\n", processes[i].id, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
        total_wt += processes[i].waiting_time;
        total_tat += processes[i].turnaround_time;
    }
    printf("\nAverage waiting time: %f ", total_wt/n);
    printf("\nAverage turnaround time: %f \n", total_tat/n);

    // Destroy the mutex lock
    pthread_mutex_destroy(&lock);

    return 0;
}

