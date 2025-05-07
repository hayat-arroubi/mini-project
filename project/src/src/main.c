/**
 * main.c - Programme principal du simulateur d'ordonnancement
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include "process.h"
 #include "parser.h"
 #include "include/scheduler.h"
 
 /**
  * Affiche un menu et demande à l'utilisateur de choisir une option
  */
 int display_menu(SchedulerPolicy *policies, int num_policies) {
     printf("\n=== Menu de sélection de la politique d'ordonnancement ===\n");
     
     for (int i = 0; i < num_policies; i++) {
         printf("%d. %s - %s\n", i + 1, policies[i].name, policies[i].description);
     }
     
     printf("%d. Quitter\n", num_policies + 1);
     
     int choice;
     do {
         printf("\nChoisissez une politique (1-%d) : ", num_policies + 1);
         scanf("%d", &choice);
         
         // Vider le buffer
         int c;
         while ((c = getchar()) != '\n' && c != EOF);
         
     } while (choice < 1 || choice > num_policies + 1);
     
     return choice - 1;  // Retourne l'indice (ou num_policies pour quitter)
 }
 
 /**
  * Affiche les résultats de la simulation
  */
 void display_results(SimulationResults results) {
     printf("\n=== Résultats de la simulation ===\n");
     printf("Temps total d'exécution: %d unités\n", results.total_time);
     printf("Temps d'attente moyen: %.2f unités\n", results.avg_wait_time);
     printf("Temps de rotation moyen: %.2f unités\n", results.avg_turnaround);
     printf("Taux d'utilisation du CPU: %.2f%%\n", results.cpu_utilization);
 }
 
 /**
  * Point d'entrée du programme
  */
 int main(int argc, char *argv[]) {
     // Vérifier les arguments
     if (argc < 2) {
         fprintf(stderr, "Usage: %s <fichier_config>\n", argv[0]);
         return EXIT_FAILURE;
     }
     
     // Charger les processus depuis le fichier de configuration
     ProcessSet *process_set = load_processes_from_file(argv[1]);
     if (!process_set) {
         fprintf(stderr, "Erreur lors du chargement des processus\n");
         return EXIT_FAILURE;
     }
     
     printf("Processus chargés avec succès depuis %s\n", argv[1]);
     
     // Charger les politiques d'ordonnancement disponibles
     SchedulerPolicy *policies = load_scheduler_policies();
     if (!policies) {
         fprintf(stderr, "Erreur lors du chargement des politiques d'ordonnancement\n");
         process_set_destroy(process_set);
         return EXIT_FAILURE;
     }
     
     int num_policies = get_num_scheduler_policies();
     
     // Boucle principale
     bool quit = false;
     while (!quit) {
         int choice = display_menu(policies, num_policies);
         
         if (choice == num_policies) {
             quit = true;
         } else {
             SchedulerPolicy selected_policy = policies[choice];
             
             // Demander si on veut un affichage détaillé
             char verbose_choice;
             printf("Afficher le déroulement détaillé de la simulation ? (o/n) : ");
             scanf("%c", &verbose_choice);
             
             // Vider le buffer
             int c;
             while ((c = getchar()) != '\n' && c != EOF);
             
             bool verbose = (verbose_choice == 'o' || verbose_choice == 'O');
             
             // Exécuter la simulation
             SimulationResults results = simulate(process_set, selected_policy, verbose);
             
             // Afficher les résultats
             display_results(results);
             
             printf("\nAppuyez sur Entrée pour continuer...");
             getchar();
         }
     }
     
     // Libérer la mémoire
     process_set_destroy(process_set);
     free(policies);
     
     return EXIT_SUCCESS;
 }