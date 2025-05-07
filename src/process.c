#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

Process create_process(const char* name, int arrival_time, int* cycles, int cycle_count) {
    Process p;
    strncpy(p.name, name, MAX_NAME_LEN);
    p.arrival_time = arrival_time;
    memcpy(p.cycles, cycles, sizeof(int) * cycle_count);
    p.cycle_count = cycle_count;
    p.current_cycle_index = 0;
    p.remaining_time = cycles[0];
    p.priority = 0;  // default
    p.state = READY;
    return p;
}

void add_process(ProcessList* list, Process process) {
    ProcessNode* node = malloc(sizeof(ProcessNode));
    node->process = process;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        ProcessNode* temp = list->head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = node;
    }
}

void free_process_list(ProcessList* list) {
    ProcessNode* current = list->head;
    while (current != NULL) {
        ProcessNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
}

void print_process(const Process* p) {
    printf("Process %s | Arrival: %d | State: %d | Current Cycle: %d\n",
           p->name, p->arrival_time, p->state, p->current_cycle_index);
}

void print_process_list(const ProcessList* list) {
    ProcessNode* current = list->head;
    while (current != NULL) {
        print_process(&current->process);
        current = current->next;
    }
}
