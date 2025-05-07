/**
 * parser.c - Implémentation des fonctions de parsing
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 
 #include "parser.h"
 
 #define MAX_LINE_LENGTH 256
 
 /**
  * Retire les espaces au début et à la fin d'une chaîne
  */
 static void trim(char *str) {
     char *start = str;
     char *end;
     
     // Supprime les espaces au début
     while (isspace((unsigned char)*start)) {
         start++;
     }
     
     if (*start == 0) { // Chaîne vide ou que des espaces
         *str = 0;
         return;
     }
     
     // Supprime les espaces à la fin
     end = start + strlen(start) - 1;
     while (end > start && isspace((unsigned char)*end)) {
         end--;
     }
     
     *(end + 1) = 0; // Termine la chaîne après le dernier caractère non-espace
     
     // Si nécessaire, déplace la chaîne au début du buffer
     if (start != str) {
         memmove(str, start, strlen(start) + 1);
     }
 }
 
 ProcessSet* load_processes_from_file(const char *filename) {
     FILE *file = fopen(filename, "r");
     if (!file) {
         fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", filename);
         return NULL;
     }
     
     ProcessSet *set = process_set_create();
     if (!set) {
         fclose(file);
         return NULL;
     }
     
     char line[MAX_LINE_LENGTH];
     Process current_process;
     bool in_process = false;
     
     while (fgets(line, sizeof(line), file)) {
         trim(line);
         
         // Ignorer les lignes vides et les commentaires
         if (line[0] == '\0' || line[0] == '#') {
             continue;
         }
         
         if (strncmp(line, "process", 7) == 0) {
             // Si on était déjà en train de définir un processus, on l'ajoute à l'ensemble
             if (in_process) {
                 process_set_add(set, current_process);
             }
             
             // Format: process <nom> <arrivée>
             char name[MAX_PROCESS_NAME];
             int arrival_time;
             
             if (sscanf(line + 7, "%63s %d", name, &arrival_time) != 2) {
                 fprintf(stderr, "Erreur de format pour la ligne: %s\n", line);
                 continue;
             }
             
             current_process = process_create(name, arrival_time);
             in_process = true;
         }
         else if (strncmp(line, "cpu", 3) == 0 && in_process) {
             // Format: cpu <durée>
             int duration;
             
             if (sscanf(line + 3, "%d", &duration) != 1) {
                 fprintf(stderr, "Erreur de format pour la ligne: %s\n", line);
                 continue;
             }
             
             process_add_cycle(&current_process, CPU_CYCLE, duration);
         }
         else if (strncmp(line, "io", 2) == 0 && in_process) {
             // Format: io <durée>
             int duration;
             
             if (sscanf(line + 2, "%d", &duration) != 1) {
                 fprintf(stderr, "Erreur de format pour la ligne: %s\n", line);
                 continue;
             }
             
             process_add_cycle(&current_process, IO_CYCLE, duration);
         }
         else if (strncmp(line, "priority", 8) == 0 && in_process) {
             // Format: priority <valeur>
             int priority;
             
             if (sscanf(line + 8, "%d", &priority) != 1) {
                 fprintf(stderr, "Erreur de format pour la ligne: %s\n", line);
                 continue;
             }
             
             current_process.priority = priority;
         }
         else {
             fprintf(stderr, "Ligne ignorée: %s\n", line);
         }
     }
     
     // Ajouter le dernier processus s'il en reste un
     if (in_process) {
         process_set_add(set, current_process);
     }
     
     fclose(file);
     return set;
 }