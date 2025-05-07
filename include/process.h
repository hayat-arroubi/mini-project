#ifndef PROCESS_H
#define PROCESS_H

#define MAX_NAME_LEN 32
#define MAX_CYCLES 32

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} ProcessState;

typedef struct {
    char name[MAX_NAME_LEN];
    int arrival_time;
    int cycles[MAX_CYCLES];  // Alternating CPU and IO durations
    int cycle_count;         // Number of entries in cycles[]
    int current_cycle_index; // Index of the current cycle
    int remaining_time;      // Time left for the current cycle
    int priority;            // Optional
    ProcessState state;
} Process;

typedef struct ProcessNode {
    Process process;
    struct ProcessNode* next;
} ProcessNode;

typedef struct {
    ProcessNode* head;
} ProcessList;

// Function prototypes
Process create_process(const char* name, int arrival_time, int* cycles, int cycle_count);
void add_process(ProcessList* list, Process process);
void free_process_list(ProcessList* list);
void print_process(const Process* process);
void print_process_list(const ProcessList* list);

#endif
