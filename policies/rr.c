
#include "../include/process.h"
#include "policy.h"
#include <stdio.h>
#include <stdbool.h>

void rr(Process* processes, int count, int quantum) {
    int remaining[count];
    for (int i = 0; i < count; i++) remaining[i] = processes[i].burst_time;

    int time = 0, completed = 0;
    printf("Scheduling with Round-Robin (quantum = %d):\n", quantum);

    while (completed < count) {
        bool idle = true;

        for (int i = 0; i < count; i++) {
            if (processes[i].arrival_time <= time && remaining[i] > 0) {
                int exec = remaining[i] > quantum ? quantum : remaining[i];
                printf("Time %d: Process %s runs for %d units\n", time, processes[i].name, exec);
                time += exec;
                remaining[i] -= exec;

                if (remaining[i] == 0) completed++;
                idle = false;
            }
        }

        if (idle) time++;
    }
}
