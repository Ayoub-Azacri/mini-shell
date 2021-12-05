/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :

  Interface du minishell.

 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <fcntl.h>
#include<signal.h>

#include "builtin.h"
#include "parser.h"
#include "processus.h"
#define LSH_RL_BUFSIZE 1024

/* clear the screen*/
void function_clear()
{
    const char* blank = "\e[1;1H\e[2J";
    write(STDOUT_FILENO,blank,12);
}



void prompt(){
  char shell[1000];
    char temp[100];
    char* path=getcwd(temp, sizeof(temp));
  if(path != NULL)
        {
          strcpy(shell, "My_shell:");
          strcat(shell, temp);
          strcat(shell, "$ ");

          printf("%s", shell);
        }
   else
       perror("getcwd() error");

}

void sigintHandler()
{

    signal(SIGINT, sigintHandler);
    printf("\n");
      prompt();
    fflush(stdout);
}
char *read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

int main(int argc, char* argv[]) {

  char * line;
  process_t cmds[MAX_CMD_SIZE];

 signal(SIGINT, sigintHandler);
  function_clear();

 while (1)
    {
      prompt();
// Lecture d'une ligne de commandes
line = read_line();

trim(line);
clean(line);

// "Nettoyage" de la ligne de commandes

// Découpage en "tokens"

 char ** cmdl ;
 cmdl =  tokenize(line);
// Parsing de la ligne pour remplir les structures
// de cmds.
for(int k =0 ; k < MAX_CMD_SIZE  ;k++){

 init_process(&cmds[k]);
}

parse_cmd(cmdl,cmds);

launch_cmd(cmds);
    }

return -1;
}

