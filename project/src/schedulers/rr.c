/**
 * round_robin.c - Implémentation de la politique d'ordonnancement Round Robin
 */

 #include "include/scheduler.h"

 // Variable statique pour suivre le quantum écoulé
 static int time_elapsed_in_quantum = 0;
 
 /**
  * Politique d'ordonnancement Round Robin
  * 
  * Exécute chaque processus pendant un quantum de temps puis passe au suivant.
  */
 int round_robin_scheduler(ProcessSet *set, int current_time, int last_running_index, int quantum) {
     // Si un processus est en cours d'exécution
     if (last_running_index >= 0 && 
         set->processes[last_running_index].state == RUNNING) {
         
         // Si le quantum n'est pas écoulé, on continue avec ce processus
         if (time_elapsed_in_quantum < quantum) {
             time_elapsed_in_quantum++;
             return last_running_index;
         }
         
         // Sinon, on passe au processus suivant et on réinitialise le compteur
         time_elapsed_in_quantum = 0;
     } else {
         // Pas de processus en cours d'exécution, on réinitialise le compteur
         time_elapsed_in_quantum = 0;
     }
     
     // Chercher le prochain processus prêt après le dernier exécuté
     int start_index = (last_running_index + 1) % set->num_processes;
     int i = start_index;
     
     do {
         Process *p = &set->processes[i];
         
         // Vérifier si le processus est prêt et arrivé
         if (p->state == READY && p->arrival_time <= current_time) {
             time_elapsed_in_quantum = 1;  // Premier cycle du quantum
             return i;
         }
         
         i = (i + 1) % set->num_processes;
     } while (i != start_index);
     
     // Si on arrive ici, aucun processus n'est prêt
     // On vérifie si le processus précédemment exécuté est toujours éligible
     if (last_running_index >= 0) {
         Process *p = &set->processes[last_running_index];
         if (p->state == READY && p->arrival_time <= current_time) {
             time_elapsed_in_quantum = 1;  // Premier cycle du quantum
             return last_running_index;
         }
     }
     
     return -1;  // Aucun processus prêt
 }
 
 // Structure exportée pour représenter cette politique
 SchedulerPolicy round_robin_policy = {
     .name = "RR",
     .description = "Round Robin",
     .func = round_robin_scheduler,
     .quantum = 2  // Quantum par défaut pour Round Robin
 };