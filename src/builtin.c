/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom : AZACRI - BENMOUMEN - LOUDAGH
  Prénom : Ayoub - Reda - Ali
  Num. étudiant : 22114660 - 22113231 - 22114007
  Groupe de projet : 27
  Date : 5/12/2021

  Gestion des commandes internes du minishell (implémentation).

 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include "builtin.h"
#include "processus.h"

char *builtin_str[] = {
  "cd",
  "export",
  "exit",
  "echo"
};

/*
 La chaîne passée représente-t-elle une commande
 interne ?

 cmd : chaîne de caractères.

 Retourne 1 si la chaîne passée désigne une
 commande interne (cd, exit, ...)
 */

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int is_builtin(const char* cmd) {
 // assert(cmd!=NULL);
  int i = 0;

  for (i = 0; i < num_builtins(); i++) {
      if (strcmp(cmd, builtin_str[i]) == 0) {
        return 1;
      }
  }

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

  if (proc->argv[0] != NULL){

      if (strcmp(proc->argv[0], "exit") == 0)
            exit_shell(1,1);
      if (strcmp(proc->argv[0], "echo") == 0){
            if(proc->argv[1]==NULL){
                printf("provide an argument for echo\n");
                return 1;
            }

              echo(proc->argv[1]);
              return 1;
      }

        if (strcmp(proc->argv[0], "export") == 0){
        if(proc->argv[1]==NULL || proc->argv[2]==NULL ){
            printf("provide 2 argument for export \n");
            return 1;
          }
          export(proc->argv[1], proc->argv[2], 1);

          return 1;
      }
      if (strcmp(proc->argv[0], "unsetVar") == 0){
        if(proc->argv[1]==NULL){
          printf("provide an argument for unsetVar\n");
          return 1;
      }
        unsetVar(proc->argv[1]);
        return 1;
    }
      if(strcmp("cd",proc->argv[0])==0){

                  cd(proc->argv[1],1);
                  return 1;
      }
  }
      return 1;
}

/*
  Change directory : change le répertoire de travail
  courant du minishell.

  path : le chemin vers lequel déplacer le CWD
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel (erreur du cd)

  Retourne le code de retour de l'appel système.
 */

int cd( char  *path, int fderr) {

    char *h=".";
    printf("path:%s",path);

    if(path==NULL)
            chdir(h);
    else if ((strcmp(path, "~")==0) || (strcmp(path, "~/")==0))
            chdir(h);
    else if(chdir(path)<0)
        printf("bash: cd: %s: No such file or directory\n", path);

      return 1;

}

void function_pwd(int command) {
    char temp[100];
    char cwdstr[100];
    char* path=getcwd(temp, sizeof(temp));
    if(path != NULL) {
        strcpy(cwdstr,temp);
        if(command==1){
            printf("%s\n",cwdstr);
        }
    } else perror("+--- Error in getcwd() : ");
}

/*
  Ajout/modification d'une variable d'environnement.

  var : nom de la variable
  value : valeur à lui donner
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel

  Retourne le code de retour de l'appel système.
 */

int export( char* var,  char* value, int fderr) {
    char output[strlen(var)+strlen(value)-1];
    sprintf(output,"%s=%s" ,var, value);
    putenv(output);
    char * test = getenv(var);
    printf("la valeur de %s est %s", var,test);

    return 1;
}

 int unsetVar( char* var) {
    unsetenv(var);
    char * test = getenv(var);
    printf("unset la valeur de %s est %s", var,test);
    return 1;
 }

 int echo( char* name){

    if(strcmp(name, "$$") == 0) {
            printf("%d\n", (int)getpid());
    } else if(strncmp(name, "$",1) == 0) {
        char* tmp = name + 1;

            printf("$%s = %s\n",tmp, getenv(tmp));
    } else {
            printf("%s\n", name);
    }

  return 1 ;
 }

/*
  Quitter le minishell

  ret : code de retour du minishell
  fdout : descripteur de la sortie standard pour
          l'affichage éventuel d'un message de sortie

  Retourne un code d'erreur en cas d'échec.
 */

int exit_shell(int ret, int fdout) {
      printf("Bye...\n");
      exit(0);
    return ret;
}