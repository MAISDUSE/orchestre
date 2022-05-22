# Orchestre

Disclaimer :
- Ce projet a été développé sur MacOS
- Le niveau 3 (mode automatique) ne fonctionne pas encore, la fonctionnalité n'ayant pas été completement développée par manque de temps.

## Compilation et lancement
Pour compiler le maestro (serveur) :
```
make all && ./maestro
```

## Démarrage du son !
Pour démarrer un instruments (à chaque fois dans un nouveau terminal):
```
./musician
```

Pour intéragir avec le serveur, il faut sélectionner les options du menu du musicien.

- [1] : Play/Pause -> jouer ou arreter la source
- [2] : Position de la source -> permet de mettre a jour la position de la source
  - après selection de l'option, les 3 coordonnées vous seront demandées (des entiers sont attendus)
- [3] : Orientation de la source -> permet de mettre a jour la direction de la source
    - après selection de l'option, les 3 coordonnées vous seront demandées (des entiers sont attendus)
- [4] : Changer d'instrument -> permet de charger ou de changer d'instrument 
- [0] : Quitter -> permet de déconnecter le musicien


Exemple de lancement :
```
Dans un terminal : make all && ./maestro
Dans un 2nd : ./musician

4 (-> charge un instrument)
4 (-> sélectionne rttn.wav)

1 (-> lance la musique)

2 (-> mets a jour la position)
1 (-> coordonnées x)
0 (-> coordonnées y)
0 (-> coordonnées z) 

3 (-> mets a jour l'orientation)
1 (-> coordonnées x)
0 (-> coordonnées y)
0 (-> coordonnées z) 

4 (-> change d'instrument)
1 (-> sélectionne uplifting-flute.wav)

0 (-> déconnecte le musicien)
```



## Mode automatique (non fonctionnel)

Pour le mode automatique un csv contenant les directives de l'orchestre est requis.

Le lancement s'effectue comme ceci : `./maestro <monscript.csv>`

Les différentes commandes possibles sont les suivantes :
- `CREATE <entier>` : Indique le nombre de musicien pour ce concert, 
- `MUSIC <file> <musician>` : Indique le fichier son a jouer pour un musicien, 
- `PLAY <int boolean (0/1)> <musician>` : Play/Pause de la source pour un musicien, 
- `WAIT <seconds>` : Indique le temps a attendre avant l'execution de la prochaine commande, 
- `POSITION <array> <musician>` : Indique la nouvelle position de la source d'un musicien, 
- `DIRECTION <array> <musician>` : Indique la nouvelle orientation de la source d'un musicien, 
- `END` : Indique la fin du concert, déconnecte les musiciens.

A savoir : 
- La 1ere commande doit toujours être CREATE et la derniere END.
- Le séparateur de ce CSV est le `;`

Exemple de CSV (la première ligne de description est indispensable):
```csv
cmd;param;musician_id
create;1
music;sounds/rttn.wav;0
play;1;0
wait;10
end
```
Cet orchestre executera la création d'un musicien avec le son rrt.wav et le jouera pendant 10s

2ème exemple :

```csv
cmd;param;musician_id
create;1
music;sounds/rttn.wav;0
music;sounds/uplifting-flute.wav;1
play;1;0
wait;10
position;[1,0,0];1
play;1;1
play;0;0
wait;3
position;[-1,0,0];1
wait;3
play;1;0
wait;10
end
```
Cet orchestre executera la création de 2 musiciens, respectivement avec les sons rttn et uplifting-flute.

Le 1ere musicien va se mettre à jouer.

10 secondes après, le premier musicien s'arrete, le second joue a droite

3 secondes après, le second joue a gauche

3 secondes après, le premier musicien continue de jouer

10 secondes après le concert s'arrête