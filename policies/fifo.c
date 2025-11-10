
#include "../include/process.h"
#include "policy.h"
#include <stdio.h>

void fifo(Process* processes, int count) {
    int time = 0;
    printf("Scheduling with FIFO:\n");

    for (int i = 0; i < count; i++) {
        if (time < processes[i].arrival_time)
            time = processes[i].arrival_time;

        printf("Time %d: Process %s starts (burst: %d)\n",
            time, processes[i].name, processes[i].burst_time);
        time += processes[i].burst_time;
    }
}
