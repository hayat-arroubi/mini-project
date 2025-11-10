
#include "../include/process.h"
#include "policy.h"
#include <stdio.h>
#include <stdbool.h>

void sjf(Process* processes, int count) {
    bool done[count];
    for (int i = 0; i < count; i++) done[i] = false;

    int time = 0, completed = 0;
    printf("Scheduling with SJF:\n");

    while (completed < count) {
        int idx = -1, min_burst = 1e9;

        for (int i = 0; i < count; i++) {
            if (!done[i] && processes[i].arrival_time <= time && processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            printf("Time %d: Process %s starts (burst: %d)\n", time, processes[idx].name, processes[idx].burst_time);
            time += processes[idx].burst_time;
            done[idx] = true;
            completed++;
        } else {
            time++;
        }
    }
}
