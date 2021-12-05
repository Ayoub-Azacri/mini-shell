# mini-shell

FONCTIONNALITÉS IMPLÉMENTÉES DANS NOTRE PROJET MINISHELL-2021 :
1. Exécutez toutes les commandes externes (ls, clear, etc.)
2. Implémentez les commandes internes : cd, pwd
3. Imprimer Initialiser et utiliser les variables d'environnement
4. Imprimer les variables d'environnement à l'aide de la commande echo
7. Tuyaux « | » (multiple) (Ex : ls | grep 'a' | wc)
8. Multiple command avec ";"  ( EX : echo test ; ls -l ; clear)
9. Gérer le signal d'interruption : en appuyant sur « Ctrl+C », la commande en cours d'exécution doit être terminée, votre programme ne doit pas se terminer.
10. Pour quitter le terminal, il faut saisir "exit".
11. Exécution de la commande en arrière-plan avec le signe '&' ( EX : ls -l & )

# execution de minishell
1.
 ```
     cd src
 ```
2.
 ```
     make -f Makefile
 ```
 3.
 ```
     ./minishell
 ```
