/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom : AZACRI - BENMOUMEN - LOUDAGH
  Prénom : Ayoub - Reda - Ali
  Num. étudiant : 22114660 - 22113231 - 22114007
  Groupe de projet : 27
  Date : 5/12/2021

  Gestion des processus (implémentation).

 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>


#include "processus.h"
#include "parser.h"
#include "builtin.h"

#ifndef NDEBUG

int check_zero(void* ptr, size_t size) {
  int result=0;
  for (size_t i=0; i<size; ++i) {
    result+=*((char*) ptr++);
  }
  return result;
}
#endif

/*
  Initialiser une structure process_t avec les
  valeurs par défaut.

  proc : un pointeur sur la structure à initialiser

  Retourne 0 ou un code d'erreur.
 */

int init_process(process_t* proc) {
  assert(proc!=NULL);
 // assert(check_zero(proc, sizeof(*proc))==0);

  proc->path= (char *)malloc( sizeof(char) * MAX_LINE_SIZE); // Chemin de l'exécutable
  proc->argv = (char **)malloc( MAX_CMD_SIZE *sizeof(char*)); // Tableau des arguments

  proc->stdin = 0;
  proc->stdout = 1;
  proc->stderr =2 ;

  proc->next=NULL;
  proc->next_failure = NULL;
  proc->next_success = NULL ;

  return 0;

}

/*
  Remplacer les noms de variables d'environnement
  par leurs valeurs.

  proc : un pointeur sur la structure à modifier.

  Retourne 0 ou un code d'erreur.
 */
int set_env(process_t* proc) {
  assert(proc!=NULL);

}

/*
  Lancer la commande en fonction des attributs de
  la structure et initialiser les champs manquants.

  On crée un nouveau processus, on détourne
  éventuellement les entrée/sorties.
  On conserve le PID dans la structure du processus
  appelant (le minishell).
  On attend si la commande est lancée en "avant-plan"
  et on initialise le code de retour.
  On rend la main immédiatement sinon.

  proc : un pointeur sur la structure contenant les
         informations pour l'exécution de la commande.

  Retourne 0 ou un code d'erreur.

 */

int launch_cmd(process_t* proc) {
  assert(proc!=NULL);

  int numPipes = 0;
  int status;
  int i = 0;
  pid_t pid;

  process_t *  tmp  = proc ;
  process_t * command = proc ;

while ( tmp->next_success != NULL) {

      tmp = tmp->next_success;
      numPipes ++;
}

 int pipefds[2*numPipes];

 for(i = 0; i < (numPipes); i++) {
        if(pipe(pipefds + i*2) < 0) {
            perror("couldn't pipe");
            exit(EXIT_FAILURE);
        }
    }

  int j = 0;

  if(numPipes!=0){
    while(command) {
      if( command->argv[0] == NULL  )
               return 1;
        if( is_builtin(command->argv[0])){
            builtin(command);
            command = command->next;
            continue;
        }
      pid = fork();
      if(pid == 0) {


            if(command->next_success){
                if(dup2(pipefds[j + 1], 1) < 0){
                    perror("1dup2");
                    exit(EXIT_FAILURE);
                }
            }

            if( j != 0 ){
                if(dup2(pipefds[j-2], 0) < 0){
                    perror("2dup2");
                    exit(EXIT_FAILURE);

                }
            }
            for(i = 0; i < 2*numPipes; i++){
                    close(pipefds[i]);
            }
            if( execvp(command->path, command->argv) < 0 ){
                    printf("%s: command or arguments uncorrect\n", proc->argv[0]);
                    exit(EXIT_FAILURE);
            }
        } else if(pid < 0){
            perror("error");
            exit(EXIT_FAILURE);
        }
        command = command->next_success;
        j+=2;

    }
       for(i = 0; i < 2 * numPipes; i++){
        close(pipefds[i]);
    }
    for(i = 0; i < numPipes + 1; i++)
        wait(&status);
}else{

    while(command) {
      if( command->argv[0] == NULL  )
               return 1;
        if( is_builtin(command->argv[0])){

            builtin(command);
            command = command->next;
            continue;
        }
        pid = fork();
        if(pid == 0) {

            if( execvp(command->path, command->argv) < 0 ){
                    printf("%s: command or arguments uncorrect\n", proc->argv[0]);
                    exit(EXIT_FAILURE);
            }
        } else if(pid < 0){
            perror("error");
            exit(EXIT_FAILURE);
        }else {
                  if(proc->bg == 0){
             do {
                  int  wpid = waitpid(pid, &status, WUNTRACED);
               } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                  }else{
                       fprintf(stdout, "+--- Process running in inBackground. PID:%d\n",pid);
                  }
        }


        command = command->next;
    }

}
return 1;
}


