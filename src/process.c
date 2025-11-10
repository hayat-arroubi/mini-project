#include "../include/process.h"
#include <stdio.h>
#include <stdlib.h>

Process* load_processes(const char* filename, int* process_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open process file");
        exit(1);
    }

    Process* list = malloc(100 * sizeof(Process));
    int i = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue; // Skip comments and blank lines
        if (sscanf(line, "%s %d %d", list[i].name, &list[i].arrival_time, &list[i].burst_time) == 3) {
            printf("Loaded: %s %d %d\n", list[i].name, list[i].arrival_time, list[i].burst_time);
            i++;
        }
    }

    fclose(file);
    *process_count = i;
    return list;
}
