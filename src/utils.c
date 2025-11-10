
#include "../include/utils.h"
#include <stdio.h>

void print_menu() {
    printf("Select scheduling policy:\n");
    printf("1. FIFO\n2. SJF\n3. Round-Robin\n");
}

char* select_policy() {
    print_menu();
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1: return "FIFO";
        case 2: return "SJF";
        case 3: return "RR";
        default: return "FIFO";
    }
}
