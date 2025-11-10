
#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char name[20];
    int arrival_time;
    int burst_time;
} Process;

Process* load_processes(const char* filename, int* process_count);

#endif
