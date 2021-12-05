/*
  Projet minishell - Licence 3 Info - PSI 2021

  Nom : AZACRI - BENMOUMEN - LOUDAGH
  Prénom : Ayoub - Reda - Ali
  Num. étudiant : 22114660 - 22113231 - 22114007
  Groupe de projet : 27
  Date : 5/12/2021

  Parsing de la ligne de commandes utilisateur (implémentation).

 */

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <unistd.h>
#include <fcntl.h>

#include "parser.h"
#include <stdlib.h>
#include "processus.h"

#define TOK_DELIM " \t\r\n\a"
#define TOK_BUFSIZE 64

/*
  Suppression des espaces en début et fin de chaîne.
      Ex : "   chaîne   " => "chaîne"

  str : chaîne à modifier

  Retourne 0 ou un code d'erreur.
 */

int trim (char * str ) {
    char *p= str ;
    int len;
    while (*p == ' ' && *p != '\0')
        p++;

    len  = strlen(p);
    memmove(str,p,len+1);
    p = str + len -1 ;
    while (*p == ' ' && p > str)
       p--;

    *(++p)='\0'  ;

  return 1;

}


/*
  Suppression des doublons d'espaces.
      Ex : "cmd1   -arg  ;   cmd2  <  input"
        => "cmd1 -arg ; cmd2 < input"

  str: chaîne à modifier

  Retourne 0 ou un code d'erreur.
 */

int clean (char * str) {

    int strLenght = strlen(str);
    char squeezed[strLenght+1];
    int j = 0;
    for (int i = 0; i < strLenght; i++) {
        if(str[i] == ' ' && str[i + 1] == ' ')
          continue;
        else
          squeezed[j++]= str[i];
    }
    squeezed[j]= '\0';
    strcpy(str, squeezed);

  return 1;
}
/*
  Découpage de la chaîne en ses éléments.
    Ex : "ls -l | grep ^a ; cat"
      => {"ls", "-l", "|", "grep", "^a", ";", "cat", NULL}
  str : chaîne à découper (peut être modifiée)
  tokens : tableau des pointeurs sur les éléments de la
           chaîne. Terminé par NULL.

  Retourne 0 ou un code d'erreur.
 */

char ** tokenize(char * str) {
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  //char * tokens[MAX_CMD_SIZE];
  char *token;

  if (!tokens) {
    printf("tokens: allocation error\n");
  }

  token = strtok(str, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        printf("tokens: allocation error\n");
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

void print_array (char **strings) {
    for (size_t i = 0; strings[i]; i++)
        printf("%s,", strings[i]);

    putchar ('\n');
}

/*
  S'agit-il d'un mot réservé du shell ?
  Les mots réservés sont par exemple :
    ";", "&", "<", "2>", "||", ...

  tok : la chaîne à tester

  Retourne 1 s'il s'agit d'un mot réservé, 0 sinon.
 */

int find_reserved_word ( char* tok)
 {

  for (int i = 0; i < sizeof tok; i++)
   if(tok[i] == ';' || tok[i] == '&' || tok[i] == '<' || tok[i] == '>'
      || tok[i] == '-' || tok[i] == '|'   )
        return 1;
  return 0;


}
/*
  Remplit le tableau de commandes en fonction du contenu
  de tokens.
    Ex : {"ls", "-l", "|", "grep", "^a", NULL} =>
         {{path = "ls",
           argv = {"ls", "-l", NULL},
           bg = 1,
           ...},
          {path = "grep",
           argv = {"grep", "^a", NULL},
           bg = 0,
           ...}}
  tokens : le tableau des éléments de la ligne de
           commandes (peut être modifié)
  commands : le tableau dans lequel sont stockés les
             différentes structures représentant le
             commandes.
  Retourne 0 ou un code d'erreur.
 */


int parse_cmd(char ** tokens, process_t* commands) {
   assert(tokens!=NULL);
   assert(commands!=NULL);
   int i=0,j=0,k=0;

    while (tokens[i]!=NULL)
    {
        if(strcmp(tokens[i],"|")==0) {
             commands[k].argv[j] = NULL;
             commands[k].next_success = &commands[k+1];
             k++;
             j=0;
        }
         else if(strcmp(tokens[i],";")==0) {
             commands[k].argv[j] = NULL;
             commands[k].next = &commands[k+1];
             k++;
             j=0;
        }
        else if(strcmp(tokens[i], "&")==0) {
             commands[k].bg=1;
        }
        else {
           if(j == 0){
              commands[k].path=tokens[i];
              commands[k].argv[j]=tokens[i];
               j++;
           } else {
              commands[k].argv[j]=tokens[i];
              j++;
           }
        }
        i++;
    }

  return 0;
}