/**
 * sjf.c - Implémentation de la politique d'ordonnancement SJF
 */

 #include "include/scheduler.h"

 /**
  * Calcule la durée totale restante pour un processus
  */
 static int get_remaining_total_time(Process *process) {
     int total_time = process->remaining_time;
     
     // Ajouter le temps de tous les cycles CPU futurs
     for (int i = process->current_cycle + 1; i < process->num_cycles; i++) {
         if (process->cycles[i].type == CPU_CYCLE) {
             total_time += process->cycles[i].duration;
         }
     }
     
     return total_time;
 }
 
 /**
  * Politique d'ordonnancement SJF (Shortest Job First)
  * 
  * Sélectionne le processus qui a le temps d'exécution total restant le plus court
  * et l'exécute jusqu'à ce qu'il soit bloqué ou terminé.
  */
 int sjf_scheduler(ProcessSet *set, int current_time, int last_running_index, int quantum) {
     // Si un processus est déjà en cours d'exécution, on continue avec lui
     if (last_running_index >= 0 && 
         set->processes[last_running_index].state == RUNNING) {
         return last_running_index;
     }
     
     // Sinon, on cherche le processus prêt avec le temps d'exécution total restant le plus court
     int shortest_job_time = -1;
     int selected_index = -1;
     
     for (int i = 0; i < set->num_processes; i++) {
         Process *p = &set->processes[i];
         
         // Vérifier si le processus est prêt et arrivé
         if (p->state == READY && p->arrival_time <= current_time) {
             int remaining_time = get_remaining_total_time(p);
             
             if (shortest_job_time == -1 || remaining_time < shortest_job_time) {
                 shortest_job_time = remaining_time;
                 selected_index = i;
             }
         }
     }
     
     return selected_index;
 }
 
 // Structure exportée pour représenter cette politique
 SchedulerPolicy sjf_policy = {
     .name = "SJF",
     .description = "Shortest Job First",
     .func = sjf_scheduler,
     .quantum = 0  // Non utilisé pour SJF
 };