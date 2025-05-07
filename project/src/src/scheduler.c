/**
 * scheduler.c - Implémentation du simulateur d'ordonnancement
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <dirent.h>
 #include <string.h>
 #include <dlfcn.h>
 #include "include/scheduler.h"
 
 // Tableau de politiques d'ordonnancement disponibles
 static SchedulerPolicy *available_policies = NULL;
 static int num_policies = 0;
 
 // Déclarations des politiques définies dans leurs fichiers respectifs
 extern SchedulerPolicy fifo_policy;
 extern SchedulerPolicy sjf_policy;
 extern SchedulerPolicy round_robin_policy;
 
 /**
  * Met à jour l'état des processus qui arrivent à cet instant
  */
 static void update_arriving_processes(ProcessSet *set, int current_time) {
     for (int i = 0; i < set->num_processes; i++) {
         Process *p = &set->processes[i];
         
         if (p->state == NEW && p->arrival_time <= current_time) {
             p->state = READY;
         }
     }
 }
 
 /**
  * Exécute une unité de temps pour le processus sélectionné
  */
 static void execute_process(Process *process, int current_time) {
     if (process->state != RUNNING) {
         process->state = RUNNING;
     }
     
     // Diminuer le temps restant du cycle en cours
     process->remaining_time--;
     
     // Vérifier si le cycle est terminé
     if (process->remaining_time <= 0) {
         process->current_cycle++;
         
         // Vérifier si tous les cycles sont terminés
         if (process->current_cycle >= process->num_cycles) {
             process->state = TERMINATED;
             process->turnaround_time = current_time + 1 - process->arrival_time;
             return;
         }
         
         // Passer au cycle suivant
         Cycle next_cycle = process->cycles[process->current_cycle];
         process->remaining_time = next_cycle.duration;
         
         // Si c'est un cycle d'E/S, bloquer le processus
         if (next_cycle.type == IO_CYCLE) {
             process->state = BLOCKED;
         }
     }
 }
 
 /**
  * Met à jour l'état des processus bloqués en E/S
  */
 static void update_blocked_processes(ProcessSet *set) {
     for (int i = 0; i < set->num_processes; i++) {
         Process *p = &set->processes[i];
         
         if (p->state == BLOCKED) {
             // Diminuer le temps restant du cycle d'E/S
             p->remaining_time--;
             
             // Vérifier si le cycle d'E/S est terminé
             if (p->remaining_time <= 0) {
                 p->current_cycle++;
                 
                 // Vérifier si tous les cycles sont terminés
                 if (p->current_cycle >= p->num_cycles) {
                     p->state = TERMINATED;
                 } else {
                     // Passer au cycle suivant (qui devrait être un cycle CPU)
                     Cycle next_cycle = p->cycles[p->current_cycle];
                     p->remaining_time = next_cycle.duration;
                     
                     // Le processus est prêt à s'exécuter
                     p->state = READY;
                 }
             }
         }
     }
 }
 
 /**
  * Met à jour les temps d'attente des processus prêts non sélectionnés
  */
 static void update_waiting_processes(ProcessSet *set, int selected_index) {
     for (int i = 0; i < set->num_processes; i++) {
         Process *p = &set->processes[i];
         
         if (i != selected_index && p->state == READY) {
             p->wait_time++;
         }
     }
 }
 
 /**
  * Affiche l'état actuel de la simulation
  */
 static void print_simulation_state(ProcessSet *set, int current_time, int selected_index) {
     printf("Time: %d\n", current_time);
     
     for (int i = 0; i < set->num_processes; i++) {
         Process *p = &set->processes[i];
         
         printf("  Process %s: ", p->name);
         
         switch (p->state) {
             case NEW:
                 printf("Not arrived yet\n");
                 break;
             case READY:
                 printf("Ready (waiting: %d)\n", p->wait_time);
                 break;
             case RUNNING:
                 printf("Running (remaining: %d)\n", p->remaining_time);
                 break;
             case BLOCKED:
                 printf("Blocked on I/O (remaining: %d)\n", p->remaining_time);
                 break;
             case TERMINATED:
                 printf("Terminated (turnaround: %d)\n", p->turnaround_time);
                 break;
             default:
                 printf("Unknown state\n");
         }
     }
     printf("\n");
 }
 
 /**
  * Effectue la simulation de l'ordonnancement
  */
 SimulationResults simulate(ProcessSet *set, SchedulerPolicy policy, bool verbose) {
     int current_time = 0;
     int last_running_index = -1;
     int cpu_busy_time = 0;
     
     // Cloner l'ensemble de processus pour ne pas modifier l'original
     ProcessSet *sim_set = process_set_create();
     for (int i = 0; i < set->num_processes; i++) {
         process_set_add(sim_set, set->processes[i]);
     }
     
     if (verbose) {
         printf("Starting simulation with policy: %s (%s)\n\n", 
                policy.name, policy.description);
     }
     
     // Boucle principale de simulation
     while (!all_processes_terminated(sim_set)) {
         // Mettre à jour les processus qui arrivent à cet instant
         update_arriving_processes(sim_set, current_time);
         
         // Sélectionner le prochain processus à exécuter selon la politique
         int selected_index = policy.func(sim_set, current_time, last_running_index, policy.quantum);
         
         // Exécuter le processus sélectionné
         if (selected_index >= 0) {
             execute_process(&sim_set->processes[selected_index], current_time);
             cpu_busy_time++;
             last_running_index = selected_index;
         } else {
             last_running_index = -1;
         }
         
         // Mettre à jour les processus bloqués en E/S
         update_blocked_processes(sim_set);
         
         // Mettre à jour les temps d'attente
         update_waiting_processes(sim_set, selected_index);
         
         // Afficher l'état actuel si demandé
         if (verbose) {
             print_simulation_state(sim_set, current_time, selected_index);
         }
         
         // Passer au cycle suivant
         current_time++;
     }
     
     // Calculer les statistiques
     float total_wait_time = 0;
     float total_turnaround_time = 0;
     
     for (int i = 0; i < sim_set->num_processes; i++) {
         total_wait_time += sim_set->processes[i].wait_time;
         total_turnaround_time += sim_set->processes[i].turnaround_time;
     }
     
     SimulationResults results = {
         .total_time = current_time,
         .avg_wait_time = total_wait_time / sim_set->num_processes,
         .avg_turnaround = total_turnaround_time / sim_set->num_processes,
         .cpu_utilization = (float)cpu_busy_time / current_time * 100
     };
     
     // Libérer la mémoire
     process_set_destroy(sim_set);
     
     return results;
 }
 
 /**
  * Charge statiquement les politiques d'ordonnancement disponibles
  */
 SchedulerPolicy* load_scheduler_policies() {
     // Si déjà chargé, retourner le tableau existant
     if (available_policies) {
         return available_policies;
     }
     
     // Pour cette version statique, on utilise les politiques prédéfinies
     num_policies = 3;  // FIFO, SJF, Round Robin
     
     available_policies = malloc(num_policies * sizeof(SchedulerPolicy));
     if (!available_policies) {
         fprintf(stderr, "Erreur: Impossible d'allouer de la mémoire pour les politiques\n");
         return NULL;
     }
     
     available_policies[0] = fifo_policy;
     available_policies[1] = sjf_policy;
     available_policies[2] = round_robin_policy;
     
     return available_policies;
 }
 
 /**
  * Récupère le nombre de politiques d'ordonnancement disponibles
  */
 int get_num_scheduler_policies() {
     return num_policies;
 }