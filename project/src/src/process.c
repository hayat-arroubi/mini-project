/**
 * process.c - Implémentation des fonctions de gestion des processus
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "include/process.h"
 
 #define INITIAL_CAPACITY 10
 
 ProcessSet* process_set_create(void) {
     ProcessSet *set = malloc(sizeof(ProcessSet));
     if (!set) {
         return NULL;
     }
     
     set->processes = malloc(INITIAL_CAPACITY * sizeof(Process));
     if (!set->processes) {
         free(set);
         return NULL;
     }
     
     set->num_processes = 0;
     set->capacity = INITIAL_CAPACITY;
     
     return set;
 }
 
 void process_set_destroy(ProcessSet *set) {
     if (set) {
         free(set->processes);
         free(set);
     }
 }
 
 void process_set_add(ProcessSet *set, Process process) {
     if (set->num_processes >= set->capacity) {
         int new_capacity = set->capacity * 2;
         Process *new_processes = realloc(set->processes, new_capacity * sizeof(Process));
         
         if (!new_processes) {
             fprintf(stderr, "Erreur: Impossible d'allouer de la mémoire pour les processus\n");
             return;
         }
         
         set->processes = new_processes;
         set->capacity = new_capacity;
     }
     
     set->processes[set->num_processes++] = process;
 }
 
 Process process_create(const char *name, int arrival_time) {
     Process process;
     
     strncpy(process.name, name, MAX_PROCESS_NAME - 1);
     process.name[MAX_PROCESS_NAME - 1] = '\0'; // Assurer la terminaison
     
     process.arrival_time = arrival_time;
     process.num_cycles = 0;
     process.current_cycle = 0;
     process.remaining_time = 0;
     process.state = NEW;
     process.wait_time = 0;
     process.turnaround_time = 0;
     process.priority = 0; // Priorité par défaut
     
     return process;
 }
 
 void process_add_cycle(Process *process, CycleType type, int duration) {
     if (process->num_cycles >= MAX_CYCLES) {
         fprintf(stderr, "Erreur: Nombre maximum de cycles atteint pour le processus %s\n", process->name);
         return;
     }
     
     process->cycles[process->num_cycles].type = type;
     process->cycles[process->num_cycles].duration = duration;
     process->num_cycles++;
     
     // Si c'est le premier cycle et de type CPU, initialiser remaining_time
     if (process->num_cycles == 1 && type == CPU_CYCLE) {
         process->remaining_time = duration;
     }
 }
 
 bool all_processes_terminated(ProcessSet *set) {
     for (int i = 0; i < set->num_processes; i++) {
         if (set->processes[i].state != TERMINATED) {
             return false;
         }
     }
     return true;
 }