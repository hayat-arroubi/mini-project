
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <config_file>\n", argv[0]);
        return 1;
    }

    int process_count;
    Process* processes = load_processes(argv[1], &process_count);

    char* policy = select_policy();
    run_scheduler(processes, process_count, policy);

    free(processes);
    return 0;
}
