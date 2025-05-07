/**
 * parser.h - Fonctions de parsing pour le fichier de configuration
 */

 #ifndef PARSER_H
 #define PARSER_H
 
 #include "process.h"
 
 /**
  * Charge les processus depuis un fichier de configuration
  * 
  * Le format du fichier est le suivant:
  * 
  * # Commentaire
  * process <nom> <arrivée>
  * cpu <durée>
  * io <durée>
  * cpu <durée>
  * ...
  * 
  * process <nom> <arrivée>
  * ...
  * 
  * @param filename Nom du fichier de configuration
  * @return Un ensemble de processus
  */
 ProcessSet* load_processes_from_file(const char *filename);
 
 #endif /* PARSER_H */