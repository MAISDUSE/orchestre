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