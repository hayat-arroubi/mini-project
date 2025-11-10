
#include "../include/scheduler.h"
#include "../policies/policy.h"
#include <string.h>
#include <stdio.h>

void run_scheduler(Process* processes, int count, const char* policy) {
    if (strcmp(policy, "FIFO") == 0) {
        fifo(processes, count);
    } else if (strcmp(policy, "SJF") == 0) {
        sjf(processes, count);
    } else if (strcmp(policy, "RR") == 0) {
        rr(processes, count, 1);
    } else {
        printf("Unknown policy: %s\n", policy);
    }
}
