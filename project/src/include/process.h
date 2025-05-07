/**
 * process.h - Structures de données pour le simulateur d'ordonnancement de processus
 */

 #ifndef PROCESS_H
 #define PROCESS_H
 
 #include <stdbool.h>
 
 #define MAX_PROCESS_NAME 64
 #define MAX_CYCLES 100
 
 /**
  * Types de cycles pour un processus
  */
 typedef enum {
     CPU_CYCLE,  // Cycle d'utilisation du processeur
     IO_CYCLE    // Cycle d'attente d'E/S
 } CycleType;
 
 /**
  * Structure représentant un cycle d'exécution
  */
 typedef struct {
     CycleType type;      // Type de cycle (CPU ou IO)
     int duration;        // Durée du cycle en unités de temps
 } Cycle;
 
 /**
  * États possibles d'un processus
  */
 typedef enum {
     READY,      // Prêt à être exécuté
     RUNNING,    // En cours d'exécution
     BLOCKED,    // Bloqué en attente d'E/S
     TERMINATED, // Terminé
     NEW         // Nouveau processus non encore arrivé
 } ProcessState;
 
 /**
  * Structure représentant un processus
  */
 typedef struct {
     char name[MAX_PROCESS_NAME];  // Nom du processus
     int arrival_time;             // Date d'arrivée
     
     Cycle cycles[MAX_CYCLES];     // Tableau des cycles d'exécution
     int num_cycles;               // Nombre total de cycles
     
     int current_cycle;            // Indice du cycle en cours
     int remaining_time;           // Temps restant pour le cycle en cours
     
     ProcessState state;           // État actuel du processus
     
     int wait_time;                // Temps d'attente total
     int turnaround_time;          // Temps de rotation total
     
     int priority;                 // Priorité du processus (pour les politiques avancées)
 } Process;
 
 /**
  * Structure représentant un ensemble de processus à ordonnancer
  */
 typedef struct {
     Process *processes;           // Tableau de processus
     int num_processes;            // Nombre de processus
     int capacity;                 // Capacité du tableau
 } ProcessSet;
 
 /**
  * Initialise un ensemble de processus vide
  */
 ProcessSet* process_set_create(void);
 
 /**
  * Libère la mémoire allouée pour un ensemble de processus
  */
 void process_set_destroy(ProcessSet *set);
 
 /**
  * Ajoute un processus à l'ensemble
  */
 void process_set_add(ProcessSet *set, Process process);
 
 /**
  * Initialise un processus avec ses valeurs par défaut
  */
 Process process_create(const char *name, int arrival_time);
 
 /**
  * Ajoute un cycle à un processus
  */
 void process_add_cycle(Process *process, CycleType type, int duration);
 
 /**
  * Vérifie si tous les processus sont terminés
  */
 bool all_processes_terminated(ProcessSet *set);
 
 #endif /* PROCESS_H */