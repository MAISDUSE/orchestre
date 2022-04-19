# Orchestre

Ce projet utilise les biliothèques openAL et freeAlut, utilisez votre gestionnaire de paquets pour installer ces dépendances si nécessaire.

## Compilation et lancement
Pour compiler l'outil de démonstration (vous pouvez écrire un Makefile si vous souhaitez):
```
gcc -O3 -Wall -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11 -pthread -lopenal -lalut demo.c semaphore.c -o demo && \
./demo 1234
```

## Démarrage du son !
Pour démarrer un instruments (à chaque fois dans un nouveau terminal):
```
nc 127.0.0.1 1234
```
ou
```
gcc -O3 -Wall -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11 sender.c -o sender && \
./sender 1234
```
