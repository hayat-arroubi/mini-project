/**
 * fifo.c - Implémentation de la politique d'ordonnancement FIFO
 */

 #include "include/scheduler.h"

 /**
  * Politique d'ordonnancement FIFO (First In, First Out)
  * 
  * Sélectionne le processus arrivé en premier et l'exécute jusqu'à
  * ce qu'il soit bloqué ou terminé.
  */
 int fifo_scheduler(ProcessSet *set, int current_time, int last_running_index, int quantum) {
     // Si un processus est déjà en cours d'exécution, on continue avec lui
     if (last_running_index >= 0 && 
         set->processes[last_running_index].state == RUNNING) {
         return last_running_index;
     }
     
     // Sinon, on cherche le processus prêt arrivé en premier
     int earliest_arrival_time = -1;
     int selected_index = -1;
     
     for (int i = 0; i < set->num_processes; i++) {
         Process *p = &set->processes[i];
         
         // Vérifier si le processus est prêt et arrivé
         if (p->state == READY && p->arrival_time <= current_time) {
             if (earliest_arrival_time == -1 || p->arrival_time < earliest_arrival_time) {
                 earliest_arrival_time = p->arrival_time;
                 selected_index = i;
             }
         }
     }
     
     return selected_index;
 }
 
 // Structure exportée pour représenter cette politique
 SchedulerPolicy fifo_policy = {
     .name = "FIFO",
     .description = "First In, First Out",
     .func = fifo_scheduler,
     .quantum = 0  // Non utilisé pour FIFO
 };