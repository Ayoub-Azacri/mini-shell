/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :

  Gestion des commandes internes du minishell (implémentation).

 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

 #include "builtin.h"
#include "processus.h"

/*
 La chaîne passée représente-t-elle une commande
 interne ?

 cmd : chaîne de caractères.

 Retourne 1 si la chaîne passée désigne une
 commande interne (cd, exit, ...)
 */
int is_builtin(const char* cmd) {
  assert(cmd!=NULL);


  return 0;
}

/*
  Exécute la commande interne

  proc : process_t dont le contenu a été initialisé
         au moment du parsing

  Retourne 0 si tout s'est bien passé, la valeur de
  retour de la commande ou -1 si la commande n'est
  pas reconnue.
 */

int builtin(process_t* proc) {
  assert(proc!=NULL);

  return -1;
}

/*
  Change directory : change le répertoire de travail
  courant du minishell.

  path : le chemin vers lequel déplacer le CWD
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel (erreur du cd)

  Retourne le code de retour de l'appel système.
 */

int cd(const char* path, int fderr) {
  assert(path!=NULL);

  if (path == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(path) != 0) {
      perror("lsh");
    }
  }
  return 1;

}

/*
  Ajout/modification d'une variable d'environnement.

  var : nom de la variable
  value : valeur à lui donner
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel

  Retourne le code de retour de l'appel système.
 */

int export(const char* var, const char* value, int fderr) {
 /*  assert(var!=NULL);
  assert(value!=NULL); */



  char output[strlen(var)+strlen(value)-1];


  sprintf(output,"%s=%s" ,var, value);
  putenv(output);

  char * test = getenv(var);

  printf("la valeur de %s est %s", var,test);

 return 1;
}

/*
  Quitter le minishell

  ret : code de retour du minishell
  fdout : descripteur de la sortie standard pour
          l'affichage éventuel d'un message de sortie

  Retourne un code d'erreur en cas d'échec.
 */

int exit_shell(int ret, int fdout) {

    return ret;
}

int main(int argc, char* argv[]) {

 char * var  = "rachid";
 char * value = "test";

  export(var,  value, 1);

return 1;
}


