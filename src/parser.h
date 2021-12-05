/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom : AZACRI - BENMOUMEN - LOUDAGH
  Prénom : Ayoub - Reda - Ali
  Num. étudiant : 22114660 - 22113231 - 22114007
  Groupe de projet : 27
  Date : 5/12/2021

  Parsing de la ligne de commandes utilisateur (headers).

 */

#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_SIZE 1024
#define MAX_CMD_SIZE 256

#include <string.h>

#include "processus.h"

// Effacer un tableau (tout mettre à zéro)
#define CLEAR(ptr, size) memset((ptr), 0, (size)*sizeof(*ptr))

int trim(char * str);
int clean(char * str);
char ** tokenize(char * str);
void print_array (char ** strings);

/*int is_reserved(const char* tok);*/
int is_reserved ( char * tok);
int parse_cmd(char ** tokens, process_t* commands);
#endif
