# Tetris
Jeu Tetris avec deux interfaces utilisateur (ncurses et SDL) en utilisant le modèle MVC.
Pour compiler :
 - gcc src/controller/main.c src/view/TetrisViewSDL.c src/view/TetrisViewSDL.h src/model/*.c  -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf  

## Les fonctionnalités :
- Gestion de la gravité
- Gestion du score
- Gestion des mouvements et rotations
- Affichage de la pièce suivante
- Suppression des lignes

## Touches :
- Flèche gauche : mouvement à gauche
- Flèche droite : mouvement à droite
- Touche Z et Flèche du bas : Rotation
