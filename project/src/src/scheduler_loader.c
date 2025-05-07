/**
 * dynamic_loader.c - Implémentation du chargement dynamique des politiques d'ordonnancement
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <dlfcn.h>
 #include <dirent.h>
 
 #include <limits.h> // For PATH_MAX

 #ifndef PATH_MAX
 #define PATH_MAX 4096 // Define a reasonable default value if not defined
 #endif
 #include "include/scheduler_loader.h"
 
 #define MAX_POLICIES 20
 
 // Structure pour stocker les handles des bibliothèques chargées
 typedef struct {
     void *handle;                // Handle de la bibliothèque partagée
     SchedulerPolicy policy;      // Politique d'ordonnancement
 } DynamicPolicy;
 
 // Tableau global pour stocker les handles des bibliothèques chargées
 static DynamicPolicy loaded_policies[MAX_POLICIES];
 static int num_loaded_policies = 0;
 
 SchedulerPolicy* load_dynamic_policies(const char *directory, int *num_policies) {
     DIR *dir;
     struct dirent *entry;
     
     // Ouvrir le répertoire
     dir = opendir(directory);
     if (!dir) {
         fprintf(stderr, "Erreur: Impossible d'ouvrir le répertoire %s\n", directory);
         *num_policies = 0;
         return NULL;
     }
     
     // Initialiser le compteur
     num_loaded_policies = 0;
     
     // Parcourir le répertoire
     while ((entry = readdir(dir)) != NULL && num_loaded_policies < MAX_POLICIES) {
         // Ne traiter que les fichiers .so
         if (strstr(entry->d_name, ".so") == NULL) {
             continue;
         }
         
         // Construire le chemin complet
         char full_path[PATH_MAX];
         snprintf(full_path, PATH_MAX, "%s/%s", directory, entry->d_name);
         
         // Charger la bibliothèque
         void *handle = dlopen(full_path, RTLD_LAZY);
         if (!handle) {
             fprintf(stderr, "Avertissement: Impossible de charger %s: %s\n", 
                     full_path, dlerror());
             continue;
         }
         
         // Charger la politique
         SchedulerPolicy *policy = (SchedulerPolicy *) dlsym(handle, "policy");
         if (!policy) {
             fprintf(stderr, "Avertissement: Symbole 'policy' non trouvé dans %s: %s\n", 
                     full_path, dlerror());
             dlclose(handle);
             continue;
         }
         
         // Vérifier si la fonction de sélection est définie
         if (!policy->func) {
             fprintf(stderr, "Avertissement: Fonction de sélection non définie dans %s\n", 
                     full_path);
             dlclose(handle);
             continue;
         }
         
         // Stocker la politique et le handle
         loaded_policies[num_loaded_policies].handle = handle;
         loaded_policies[num_loaded_policies].policy = *policy;
         num_loaded_policies++;
     }
     
     closedir(dir);
     
     // Si aucune politique n'a été chargée
     if (num_loaded_policies == 0) {
         fprintf(stderr, "Avertissement: Aucune politique d'ordonnancement trouvée dans %s\n", 
                 directory);
         *num_policies = 0;
         return NULL;
     }
     
     // Allouer le tableau de politiques
     SchedulerPolicy *policies = malloc(num_loaded_policies * sizeof(SchedulerPolicy));
     if (!policies) {
         fprintf(stderr, "Erreur: Impossible d'allouer de la mémoire pour les politiques\n");
         
         // Fermer les bibliothèques chargées
         for (int i = 0; i < num_loaded_policies; i++) {
             dlclose(loaded_policies[i].handle);
         }
         
         *num_policies = 0;
         return NULL;
     }
     
     // Copier les politiques
     for (int i = 0; i < num_loaded_policies; i++) {
         policies[i] = loaded_policies[i].policy;
     }
     
     *num_policies = num_loaded_policies;
     return policies;
 }
 
 void free_dynamic_policies(SchedulerPolicy *policies, int num_policies) {
     // Libérer le tableau de politiques
     free(policies);
     
     // Fermer les bibliothèques chargées
     for (int i = 0; i < num_loaded_policies; i++) {
         dlclose(loaded_policies[i].handle);
     }
     
     num_loaded_policies = 0;
 }