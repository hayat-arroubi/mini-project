/**
 * dynamic_loader.h - Chargement dynamique des politiques d'ordonnancement
 */

 #ifndef DYNAMIC_LOADER_H
 #define DYNAMIC_LOADER_H
 
 #include "scheduler.h"
 
 /**
  * Charge dynamiquement les politiques d'ordonnancement à partir de bibliothèques partagées
  * 
  * @param directory Répertoire contenant les bibliothèques partagées
  * @param num_policies Pointeur vers une variable qui recevra le nombre de politiques chargées
  * @return Tableau de politiques d'ordonnancement
  */
 SchedulerPolicy* load_dynamic_policies(const char *directory, int *num_policies);
 
 /**
  * Libère les ressources allouées pour les politiques d'ordonnancement
  */
 void free_dynamic_policies(SchedulerPolicy *policies, int num_policies);
 
 #endif /* DYNAMIC_LOADER_H */