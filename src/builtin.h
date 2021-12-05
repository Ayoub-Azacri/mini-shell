/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom : AZACRI - BENMOUMEN - LOUDAGH
  Prénom : Ayoub - Reda - Ali
  Num. étudiant : 22114660
  Groupe de projet : 27
  Date :

  Gestion des commandes internes du minishell (headers).

 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include "processus.h"

int num_builtins();
int is_builtin(const char* cmd);
int builtin(process_t* proc);

int cd( char* path, int fderr);
void function_pwd(int command);
int export( char* var,  char* value, int fderr);
int unsetVar( char* var);
int echo( char* name);
int exit_shell(int ret, int fdout);
#endif
