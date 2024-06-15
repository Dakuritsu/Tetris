#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "TetrisViewSDL.h"
#include "../model/TetrisGrid.h"
#include "../model/Block.h"
#include "../model/GameState.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//====================DÉCLARATIONS============================================

const char* tileImages[] = {
    "TetrisAssets/TileEmpty.png",
    "TetrisAssets/TileCyan.png",
    "TetrisAssets/TileBlue.png",
    "TetrisAssets/TileOrange.png",
    "TetrisAssets/TileYellow.png",
    "TetrisAssets/TileGreen.png",
    "TetrisAssets/TilePurple.png",
    "TetrisAssets/TileRed.png"
};

// Tableau de sources d'images pour les blocs
const char* blockImages[] = {
    "TetrisAssets/Block-Empty.png",
    "TetrisAssets/Block-I.png",
    "TetrisAssets/Block-J.png",
    "TetrisAssets/Block-L.png",
    "TetrisAssets/Block-O.png",
    "TetrisAssets/Block-S.png",
    "TetrisAssets/Block-T.png",
    "TetrisAssets/Block-Z.png"
};

//====================FONCTIONS============================================

/**
 * @brief Quitte la SDL avec affichage d'erreur.
 *
 * @param message Un message d'erreur
 */
void SDL_ExitWithError(const char *message)
{
    SDL_Log("Erreur : %s > %s\n", message, SDL_GetError());
    SDL_Quit(); // Il faut faire en sorte de quitter la SDL dans tous les cas
    exit(EXIT_FAILURE);
}

/**
 * @brief Initialise la SDL
 * 
 */
void initializeSDL(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_ExitWithError("Initialisation SDL");
    }
}

/**
 * @brief Initialise TTF
 * 
 */
void initialiseTTF(){
    if (TTF_Init() != 0) {
        // Gestion de l'erreur d'initialisation de SDL_ttf
        SDL_ExitWithError("Erreur : Initialisation SDL_ttf");
    }

}

/**
 * @brief Crée une fenêtre SDL
 * 
 * @param title  un titre pour la fenêtre SDL
 * @param width  dimension de la fenêtre SDL
 * @param height dimension de la fenêtre SDL
 * @return       SDL_Window* 
 */
SDL_Window* createWindow(char* title,int width, int height){
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL)
        SDL_ExitWithError("Création fenetre echouee");

    return window;
}

/**
 * @brief Crée un rendu SDL
 * 
 * @param window une fenêtre SDL
 * @return       SDL_Renderer* 
 */
SDL_Renderer* createRenderer(SDL_Window* window){
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
        SDL_ExitWithError("Création rendu echouee");

    return renderer;
}

/**
 * @brief Crée une surface SDL
 * 
 * @param imagePath chemin vers le fichier image
 * @return          SDL_Surface* 
 */
SDL_Surface* createSurface(const char* imagePath){
    SDL_Surface* surface = IMG_Load(imagePath);
    if (surface == NULL)
        SDL_ExitWithError("Erreur : Chargement de l'image échoué");

    return surface;
}

/**
 * @brief Crée une texture SDL
 * 
 * @param renderer un rendu SDL
 * @param image    une surface SDL
 * @return         SDL_Texture* 
 */
SDL_Texture* createTexture(SDL_Renderer* renderer, SDL_Surface* image){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == NULL)
        SDL_ExitWithError("Création rendu echouee");

    SDL_FreeSurface(image);

    return texture;   
}

/**
 * @brief Crée un background 
 * 
 * @param renderer  le rendu utilisé
 * @param imagePath le chemin de l'image pour le background
 */
void create_background(SDL_Renderer *renderer , const char* imagePath){
    SDL_Surface *backgroundSurface = createSurface(imagePath);
    SDL_Texture *backgroundTexture = createTexture(renderer, backgroundSurface);

    // Obtenir la taille de la fenêtre pour dessiner le fond à la bonne échelle
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // Définir la destination du rendu pour la taille de la fenêtre
    SDL_Rect destinationRect = {0, 0, windowWidth, windowHeight};

    // Dessiner le fond sur le rendu
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &destinationRect);

    // Libérer la texture lorsque vous avez fini de l'utiliser
    SDL_DestroyTexture(backgroundTexture);
}

/**
 * @brief Crée un texte SDL
 * 
 * @param renderer  le rendu utilisé
 * @param text      le texte à afficher
 * @param fontPath  le chemin vers le fichier de la police
 * @param fontSize  la taille de la police
 * @param textColor la couleur du texte
 * @param x         position x du texte
 * @param y         position y du texte
 */
void createText(SDL_Renderer* renderer, const char* text, const char* fontPath, int fontSize, SDL_Color textColor, int x, int y){

    // Charger la police depuis le chemin spécifié
    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (font == NULL){
        SDL_ExitWithError("Erreur : Chargement de la police échoué");
    }

    // Créer une surface texte à partir de la chaîne de caractères, la police et la couleur spécifiées
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL){
        TTF_CloseFont(font); // N'oubliez pas de libérer la police en cas d'échec
        SDL_ExitWithError("Erreur : Création de la surface texte échouée");
    }

    // Libérer la police car elle n'est plus nécessaire après la création de la surface texte
    TTF_CloseFont(font);

    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (texture == NULL){
        SDL_FreeSurface(textSurface); // Libérer la surface en cas d'échec
        SDL_ExitWithError("Erreur : Création de la texture échouée");
    }

    // Définir la destination du rendu avec les coordonnées spécifiées
    SDL_Rect destinationRect = {x, y, textSurface->w, textSurface->h};

    // Dessiner le texte sur le renderer à la position spécifiée
    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);

    // Libérer la surface et la texture, car elles ne sont plus nécessaires après le rendu
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(texture);
}

/**
 * @brief Met à jour le score
 * 
 * @param renderer  le renderer où l'afficher
 * @param score     le score à afficher
 * @param textColor la couleur du texte
 */
void updateScoreText(SDL_Renderer* renderer, int score, SDL_Color textColor, int x , int y){

    // Convertir le score en chaîne de caractères uniquement s'il est différent de zéro
    char textScore[20];

    if (score != 0){
        sprintf(textScore, "Score : %d", score);
    }
    else{
        sprintf(textScore, "Score :");
    }

    createText(renderer, textScore, "TetrisAssets/text.ttf", 24, textColor, x, y);
}

/**
 * @brief Créer le contour de la grille
 * 
 * @param renderer le rendu utilisé
 * @param width    dimension du rectangle
 * @param height   dimension du renctangle
 * @param color    la couleur du rectangle
 */
void drawRectangleGrid(SDL_Renderer* renderer, int width, int height, SDL_Color color){
    // Déterminer les coordonnées pour centrer le rectangle dans la fenêtre
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int x = (windowWidth - width) / 2;
    int y = (windowHeight - height) / 2;

    // Définir la couleur du rendu
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Dessiner le rectangle
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);

    // Mettre à jour l'affichage
    SDL_RenderPresent(renderer);
}

/**
 * @brief Créer un rectangle 
 * 
 * @param renderer le rendu utilisé
 * @param width    dimension du rectangle
 * @param height   dimension du rectangle
 * @param color    la oculeur du rectangle
 * @param x        la position
 * @param y        la position
 */
void drawRectangle(SDL_Renderer* renderer, int width, int height, SDL_Color color , int x , int y){
    // Définir la couleur du rendu
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Dessiner le rectangle
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);

    // Mettre à jour l'affichage
    SDL_RenderPresent(renderer);
}

/**
 * @brief Crée un ImageControls d'une grille pour représenter les id de la grille en image
 * 
 * @param grid      grille utilisé pour la création du ImageControl
 * @param cellSize  la taille des cases
 * @return Image** 
 */
Image** createImageControls(TetrisGrid* grid , int cellSize){
    //création de la première dimension
    Image** imageControls = (Image**)malloc(grid->row * sizeof(Image*));
    if (imageControls == NULL){
        fprintf(stderr, "Erreur d'allocation pour imageControls\n");
        exit(EXIT_FAILURE);
    }

    //création de la deuxième
    for(int i = 0; i < grid->row ; i++){
        imageControls[i] = (Image*)malloc(grid->column * sizeof(Image));
        if (imageControls[i] == NULL){
            fprintf(stderr, "Erreur d'allocation pour imageControls[%d]\n", i);
            // Libérer la mémoire déjà allouée avant de quitter
            for (int j = 0; j < i; j++){
                free(imageControls[j]);
            }
            free(imageControls);
            exit(EXIT_FAILURE);
        }
    }

    //initialise les sources
    for(int r = 0; r < grid->row; r++){
        for(int c = 0; c < grid->column; c++){
            //initialise la source de chaque case
            imageControls[r][c].source = (char*)malloc((sizeof(char) * 100));
            if (imageControls[r][c].source == NULL){
                fprintf(stderr, "Erreur d'allocation pour imageControls[%d][%d].source\n", r, c);
                // Libérer la mémoire déjà allouée avant de quitter
                for (int i = 0; i <= r; i++) {
                    for (int j = 0; j < grid->column; j++) {
                        free(imageControls[i][j].source);
                    }
                    free(imageControls[i]);
                }
                free(imageControls);
                exit(EXIT_FAILURE);
            }
            //initialise la taille des cases
            imageControls[r][c].width = cellSize;
            imageControls[r][c].height = cellSize;
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
void destroyImageControls(Image** imageControls, int rows , int columns){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            free(imageControls[i][j].source);
        }
        free(imageControls[i]);
    }
    free(imageControls);
}

/**
 * @brief créer les textures des src se trouvant dans imageControls
 * 
 * @param renderer      le rendu utilisé
 * @param grid          la grille utilisé
 * @param imageControls l'ImageControl utilisé
 */
void SetupImageControls(SDL_Renderer* renderer, TetrisGrid* grid , Image** imageControls){
    /**
    * pour placer la grille
    * 
    */
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int x = (windowWidth - 250) / 2;
    int y = (windowHeight - 500) / 2;    

    /**
     * parcours de la grille  
     */
    for(int r = 2; r < grid->row; r++){ //Mettre à 2 pour avoir l'affichage normal sinon 0 pour la grille avec les deux lignes cachées
        for(int c = 0; c < grid->column; c++){

            // Vérifier si le src correspond à l'image vide 
            /*  
                -> pour éviter que les anciennes positions du current reste de la même couleur 
                car case vide correspond à un contour donc ça ferai contour noir sur carré de la couleur du current
            */
            if (strcmp(imageControls[r][c].source, tileImages[0]) == 0){
                // Définir la couleur du rendu
                SDL_Color gridColor = {32, 32, 32, 255};
                SDL_SetRenderDrawColor(renderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                SDL_Rect destRect = {x + c * imageControls[r][c].width, (y - 50) + r * imageControls[r][c].width,imageControls[r][c].width, imageControls[r][c].width};
                SDL_RenderFillRect(renderer, &destRect);

            }


            //texture avec le src de la case
            SDL_Texture* texture = createTexture(renderer, createSurface(imageControls[r][c].source));
           

            // Définir la position de la texture
            SDL_Rect destRect;
            destRect.x = x + c * imageControls[r][c].width;
            destRect.y = (y-50) + r * imageControls[r][c].width;
            destRect.w = imageControls[r][c].width;
            destRect.h = imageControls[r][c].width;
            

            // Dessiner la texture sur le rendu
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
        }
    }
}


/**
 * @brief remplit le imageControls avec les id qui se trouvent dans la grid
 * 
 * @param renderer 
 * @param imageControls 
 * @param grid 
 */
void DrawGrid(Image** imageControls, TetrisGrid grid){
    /**
     * parcours de la grille
     * 
     */
    for (int r = 0; r < grid.row; r++){
        for (int c = 0; c < grid.column; c++){
            //récupération de l'id
            int id = grid.grid[r][c];
            //copie du src
            strcpy(imageControls[r][c].source , tileImages[id]);
        }
    }
}

/**
 * @brief Méthode pour dessiner un bloc dans la grille d'images
 * 
 * @param imageControls l'ImageControl utilisé
 * @param grid          la grille utilisé
 * @param block         le block à dessiner
 */
void DrawBlock(Image** imageControls, TetrisGrid grid, Block *block){
    // Efface les anciennes positions avec le src de la case vide
    for (int i = 0; i < 4; i++){
        Position p = tilePositions(block)[i];//les anciennes cases du block
        strcpy(imageControls[p.row][p.column].source , tileImages[0]); // Utilise le src de la case vide
    }

    // Met à jour les nouvelles positions avec le src du nouveau bloc
    for (int i = 0; i < 4; i++) {
        Position p = tilePositions(block)[i];
        strcpy(imageControls[p.row][p.column].source , tileImages[block->id]);
    }

    // Met à jour la grille avec les nouvelles positions du bloc
    for (int i = 0; i < 4; i++) {
        Position p = tilePositions(block)[i];
        grid.grid[p.row][p.column] = block->id;
    }
}


/**
 * @brief Pour afficher le block suivant
 * 
 * @param gameState le gameState utilisé
 * @param renderer  le renderer utilisé
 */
void DrawNext(GameState *gameState , SDL_Renderer* renderer){

    SDL_Texture* texture = createTexture(renderer, createSurface(blockImages[gameState->queue->nextBlock.id]));

    // Définir la position de la texture
    SDL_Rect destRect;
    destRect.x = 590;
    destRect.y = 300;
    destRect.w = 75;
    destRect.h = 75;
    

    // Dessiner la texture sur le rendu
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}

/**
 * @brief Pour tout dessiner
 * 
 * @param gameState     le gameState utilisé
 * @param renderer      le renderer utilisé
 * @param imageControls l'ImageControl utilisé
 */
void Draw(GameState *gameState , SDL_Renderer* renderer , Image** imageControls){
    DrawGrid(imageControls , *gameState->grid);
    DrawBlock(imageControls, *gameState->grid, &gameState->current);
    DrawNext(gameState, renderer);
    SetupImageControls(renderer, gameState->grid , imageControls);
    DrawGameOver(gameState , renderer);
    SDL_RenderPresent(renderer);
}

/**
 * @brief Pour afficher le gameOver
 * 
 * @param gameState le gameState utilisé
 * @param renderer  le renderer utilisé
 */
void DrawGameOver(GameState *gameState , SDL_Renderer* renderer){
    if(gameState->gameOver){
        
        SDL_Surface *end_surface = createSurface("TetrisAssets/gameover.jpg");
        SDL_Texture *end_texture = createTexture(renderer, end_surface);

        int windowWidth, windowHeight;
        SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

        SDL_Rect destinationRect = {200, 150, 400, 200};
        SDL_RenderCopy(renderer, end_texture, NULL, &destinationRect);

        SDL_DestroyTexture(end_texture);
        SDL_Color textColor = {250, 250, 230, 255};
        updateScoreText(renderer, gameState->score ,textColor, 350 , 175);

    }
}
