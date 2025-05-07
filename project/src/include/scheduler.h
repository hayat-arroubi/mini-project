/**
 * scheduler.h - Interface pour les politiques d'ordonnancement
 */

 #ifndef SCHEDULER_H
 #define SCHEDULER_H
 
 #include "process.h"
 
 /**
  * Fonction de sélection pour une politique d'ordonnancement
  * 
  * Cette fonction est appelée à chaque cycle pour sélectionner
  * le prochain processus à exécuter.
  * 
  * @param set Ensemble des processus
  * @param current_time Temps actuel
  * @param last_running_index Index du processus qui était en cours d'exécution (-1 si aucun)
  * @param quantum Quantum de temps pour les politiques qui en ont besoin (e.g. Round Robin)
  * @return Index du processus à exécuter, ou -1 si aucun processus n'est prêt
  */
 typedef int (*SchedulerFunction)(ProcessSet *set, int current_time, int last_running_index, int quantum);
 
 /**
  * Structure représentant une politique d'ordonnancement
  */
 typedef struct {
     char *name;              // Nom de la politique
     char *description;       // Description de la politique
     SchedulerFunction func;  // Fonction de sélection
     int quantum;             // Quantum pour Round Robin (0 si non utilisé)
 } SchedulerPolicy;
 
 /**
  * Structure représentant les résultats de simulation
  */
 typedef struct {
     int total_time;          // Temps total d'exécution
     float avg_wait_time;     // Temps d'attente moyen
     float avg_turnaround;    // Temps de rotation moyen
     float cpu_utilization;   // Taux d'utilisation du CPU
 } SimulationResults;
 
 /**
  * Effectue la simulation de l'ordonnancement
  * 
  * @param set Ensemble des processus
  * @param policy Politique d'ordonnancement
  * @param verbose Si true, affiche le déroulement de la simulation
  * @return Résultats de la simulation
  */
 SimulationResults simulate(ProcessSet *set, SchedulerPolicy policy, bool verbose);
 
 /**
  * Charge dynamiquement les politiques d'ordonnancement disponibles
  * 
  * @return Tableau de politiques d'ordonnancement
  */
 SchedulerPolicy* load_scheduler_policies();
 
 /**
  * Récupère le nombre de politiques d'ordonnancement disponibles
  */
 int get_num_scheduler_policies();
 
 #endif /* SCHEDULER_H */