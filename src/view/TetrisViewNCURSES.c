#include <ncurses.h>
#include "TetrisViewNCURSES.h"

/**
 * @brief initialise NCurses
 * 
 */
void initializeNCurses(){
    initscr(); // Initialiser l'environnement NCurses
    start_color(); // Activer la couleur

    // Définir des paires de couleurs (exemple avec une paire pour le texte en blanc sur fond noir)
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
}

/**
 * @brief ferme NCurses
 * 
 */
void cleanupNCurses(){
    endwin(); // Terminer l'environnement NCurses
}


/**
 * @brief Crée une fenêtre
 * 
 * @param height    dimension de la fenêtre
 * @param width     dimension de la fenêtre
 * @param starty    position
 * @param startx    position
 * @return WINDOW*  la fenêtre créer
 */
WINDOW* createWindow(int height, int width, int starty, int startx){
    WINDOW* local_win;

    local_win = newwin(height, width, starty, startx);

    return local_win;
}

/**
 * @brief Crée la box d'une fenêtre
 * 
 * @param win la fenêtre choisi
 */
void createBox(WINDOW* win){
    box(win, 0, 0); // Ajoute une boîte autour de la fenêtre
    wrefresh(win); // Rafraîchit la fenêtre
}

/**
 * @brief Crée un texte dans une fenêtre
 * 
 * @param win  la fenêtre choisi
 * @param text le texte
 * @param x    la position
 * @param y    la position
 */
void createText(WINDOW* win, const char* text, int x, int y){
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, y, x, text);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}


/**
 * @brief met à jour le score de la partie
 * 
 * @param win   la fenêtre où l'afficher
 * @param score le score
 * @param x     la position
 * @param y     la position
 */
void updateScoreText(WINDOW* win, int score, int x, int y){
    char textScore[20];

    if(score != 0){
        snprintf(textScore, sizeof(textScore), "Score : %d", score);
    }else{
        snprintf(textScore, sizeof(textScore), "Score :");
    }

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, y, x, textScore);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}

/**
 * @brief Crée un ImageControls d'une grille pour représenter les id de la grille en image
 * 
 * @param grid      grille utilisé pour la création du ImageControl
 * @param cellSize  la taille des cases
 * @return char** 
 */
char** createImageControls(TetrisGrid* grid){
    //création de la première dimension
    char** imageControls = (char**)malloc(grid->row * sizeof(char*));
    if (imageControls == NULL){
        fprintf(stderr, "Erreur d'allocation pour imageControls\n");
        exit(1);
    }

    //création de la deuxième
    for(int i = 0; i < grid->row ; i++){
        imageControls[i] = (char*)malloc(grid->column * sizeof(char));
        if (imageControls[i] == NULL){
            fprintf(stderr, "Erreur d'allocation pour imageControls[%d]\n", i);
            // Libérer la mémoire déjà allouée avant de quitter
            for (int j = 0; j < i; j++){
                free(imageControls[j]);
            }
            free(imageControls);
            exit(1);
        }
    }
    return imageControls;
}

/**
 * @brief Détruit un image control
 * 
 * @param imageControls l'image control à détruire
 * @param rows          nombre de ligne
 * @param columns       nombre de colonne
 */
void destroyImageControls(char** imageControls, int rows , int columns){
    for (int i = 0; i < rows; ++i) {
        free(imageControls[i]);
    }
    free(imageControls);
}


/**
 * @brief affiche les symboles de imageControls dans win
 * 
 * @param win           la fenetre utilisé
 * @param grid          la grille utilisé
 * @param imageControls l'ImageControl utilisé
 */
void SetUpImagesControls(WINDOW* win, TetrisGrid* grid, char** imageControls){
    for (int r = 2; r < grid->row; r++){//pour pas afficher les deux premiere ligne
        for (int c = 0; c < grid->column; c++){
            //affiche le symbole de imageControls
            char symbol = imageControls[r][c];

            // Utiliser mvwaddch pour définir le caractère à la position spécifiée dans la fenêtre
            mvprintw(r+38,c+35,"%c" ,symbol);//pour mettre au centre du win
        }
    }

    // Rafraîchir la fenêtre pour voir les modifications
    wrefresh(win);
}

/**
 * @brief affiche le block
 * 
 * @param win           la fenêtre où l'afficher
 * @param grid          la grille utilisé pour stocker les id des blocks
 * @param block         le block utilisé
 * @param imageControls l'imageControls utilisé pour garder les symboles représentant vide ou occupé
 */
void DrawBlock(WINDOW* win, TetrisGrid *grid, Block* block, char** imageControls){
    // Efface les anciennes positions dans imageControls
    for (int i = 0; i < 4; i++){
        Position p = tilePositions(block)[i];
        imageControls[p.row][p.column] = ' '; // Utilise un espace pour la case vide
    }

    // Met à jour les nouvelles positions dans imageControls
    for (int i = 0; i < 4; i++){
        Position p = tilePositions(block)[i];
        imageControls[p.row][p.column] = '#'; // Utilise '#' pour représenter le bloc
    }

    // Met à jour la grille avec les nouvelles positions du bloc
    for (int i = 0; i < 4; i++){
        Position p = tilePositions(block)[i];
        grid->grid[p.row][p.column] = block->id;
    }

    // Met à jour l'affichage dans win
    SetUpImagesControls(win, grid, imageControls);//comme werase dans set up ça fait l'actualisation
}

void DrawNext(WINDOW*win , GameState *gameState){
    drawRectangle(win , 5,5, 60 , 30);

    for(int i=0 ; i<4 ; i++){
        for(int j=0 ; j<4 ;j++){
            if(isPositionInBlock(&gameState->queue->nextBlock , (Position){i,j})){//si i j est une des positions on affiche # en i j 
                mvwprintw(win, 31 + j, 61 + i, '#');
            }
            else{
                mvwprintw(win, 31 + j, 61 + i, ' ');
            }
        }
    }
    wrefresh(win);
}

/**
 * @brief dessine un rectangle
 * 
 * @param win    la fenêtre
 * @param width  les dimensions
 * @param height les dimensions
 * @param x      la position
 * @param y      la position
 */
void drawRectangle(WINDOW*win ,int width, int height, int x, int y){
    for(int i=0 ; i<width;i++){
        for(int j=0 ; j<height;j++){
            if(i == 0 && j == 0){
                mvwprintw(win, j + y, i + x , '+');
            }
            else if(i == width -1 && j == height-1){
                mvwprintw(win, j + y, i + x , '+');
            }
            else if(i == width -1 && j == 0){
                mvwprintw(win, j + y, i + x , '+');
            }
            else if(i == 0 && j == height-1){
                mvwprintw(win, j + y, i + x , '+');
            }
            else if(j == 0 || j == height-1){
                mvwprintw(win, j + y, i + x , '-');
            }
            else if(i == 0 || i == width -1){
                mvwprintw(win, j + y, i + x , '|');
            }
        }
    }
}

/**
 * @brief Pour tout afficher
 * 
 * @param gameState     le gameState utilisé
 * @param win           la fenêtre
 * @param imageControls l'ImageControl utilisé
 */
void Draw(WINDOW* win, GameState* gameState, char** imageControls) {
    // Effacer l'écran
    werase(win);

    // Dessiner la grille
    SetUpImagesControls(win, gameState->grid, imageControls);

    // Dessiner le bloc actuel
    DrawBlock(win, gameState->grid, &gameState->current, imageControls);

    // Dessiner le bloc suivant
    DrawNext(win, gameState);

    // Dessiner le score
    updateScoreText(win, gameState->score, 2, 2);

    // Dessiner le message Game Over s'il y a lieu
    DrawGameOver(win,gameState);

    // Rafraîchir la fenêtre pour voir les modifications
    wrefresh(win);
}

/**
 * @brief Pour afficher le gameOver
 * 
 * @param gameState le gameState utilisé
 * @param win       la fenêtre
 */
void DrawGameOver(WINDOW* win,GameState *gameState){
    if(gameState->gameOver){
        createText(win, "Game Over", 35, 50);
        updateScoreText(win, gameState->score, 35 , 8);

    }
}

