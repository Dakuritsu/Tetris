#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Block.h"
#include "BlockQueue.h"
#include "TetrisGrid.h"
#include "GameState.h"

/**
 * @brief Creer un GameState
 * 
 * @param blockQueue 
 * @return GameState* 
 */
GameState *createGameState(BlockQueue *blockQueue){
    GameState *gameState = (GameState *)malloc(sizeof(GameState));
    if(!gameState){
        fprintf(stderr , "Erreur : probleme allocation dynamique.\n");
        exit(EXIT_FAILURE);
    }
    
    gameState->grid = createGrid(22,10);
    gameState->queue = blockQueue;
    gameState->current = getAndUpdateBlock(gameState->queue);
    gameState->gameOver = false;
    gameState->score = 0;
    printf("le gameState créer a une grille de taille %d %d et le current a pour id -> %d\n", gameState->grid->row ,gameState->grid->column , gameState->current.id);
    
    return gameState;
}

/**
 * @brief Détruit le GameState
 * 
 * @param gameState 
 */
void destroyGameState(GameState *gameState){
    if (gameState){
        // Libérer la mémoire de la grille
        destroyGrid(gameState->grid);

        // Libérer la mémoire de la file de blocs
        destroyBlockQueue(gameState->queue);

        // Libérer la mémoire du bloc courant
        destroyBlock(&(gameState->current));

        // Libérer la mémoire de la structure GameState
        free(gameState);
    }
}


/**
 * @brief Fait pivoter le block courant dans le sens des aiguilles d'une montre si c'est possible
 * 
 * @param gameState 
 */
void rotateBlockCW(GameState *gameState){
    // On stocke les positions actuelles du bloc
    Position *currentPositions = tilePositions(&gameState->current);

    // On applique la rotation vers la droite
    rotateCW(&gameState->current);

    // On récupère les nouvelles positions du bloc après la rotation
    Position *newPositions = tilePositions(&gameState->current);

    // On vérifie que le décalage est possible (si vide ou déjà une position du bloc)
    for (int i = 0; i < 4; i++){
    
        if (!isEmpty(newPositions[i].row, newPositions[i].column, gameState->grid) &&
            !isPositionInBlock(&gameState->current, (Position){newPositions[i].row, newPositions[i].column})){
            
            // Annuler la rotation en cas de conflit
            rotateCCW(&gameState->current);
            return;
        }
    }

    // On met les anciennes positions à l'id 0
    for (int i = 0; i < 4; i++){
        gameState->grid->grid[currentPositions[i].row][currentPositions[i].column] = 0;
    }

    // On met les nouvelles positions à l'id du bloc courant
    for (int i = 0; i < 4; i++){
        gameState->grid->grid[newPositions[i].row][newPositions[i].column] = gameState->current.id;
    }
}

/**
 * @brief Fait pivoter le block courant dans le sens inverse des aiguilles d'une montre si c'est possible
 * 
 * @param gameState 
 */
void rotateBlockCCW(GameState *gameState){
    // On stocke les positions actuelles du bloc
    Position *currentPositions = tilePositions(&gameState->current);

    // On applique la rotation vers la droite
    rotateCCW(&gameState->current);

    // On récupère les nouvelles positions du bloc après la rotation
    Position *newPositions = tilePositions(&gameState->current);

    // On vérifie que le décalage est possible (si vide ou déjà une position du bloc)
    for (int i = 0; i < 4; i++){
        if (!isEmpty(newPositions[i].row, newPositions[i].column, gameState->grid) &&
            !isPositionInBlock(&gameState->current, (Position){newPositions[i].row, newPositions[i].column})){

            // Annuler la rotation en cas de conflit
            rotateCW(&gameState->current);
            return;
        }
    }

    // On met les anciennes positions à l'id 0
    for (int i = 0; i < 4; i++) {
        gameState->grid->grid[currentPositions[i].row][currentPositions[i].column] = 0;
    }

    // On met les nouvelles positions à l'id du bloc courant
    for (int i = 0; i < 4; i++) {
        gameState->grid->grid[newPositions[i].row][newPositions[i].column] = gameState->current.id;
    }
}

/**
 * @brief Deplace le block courant vers la gauche si c'est possible
 * 
 * @param gameState 
 */
void moveBlockLeft(GameState *gameState){
    //on récupère les positions du block
    Position *positions = tilePositions(&gameState->current);

    //on vérifie que le décalage est possible (si vide ou déjà une position du block)
    for(int i = 0; i < 4; i++){
        if(!isEmpty(positions[i].row, positions[i].column - 1, gameState->grid) && !isPositionInBlock(&gameState->current, (Position){positions[i].row, positions[i].column - 1})){
            return;
        }
            
    }

    //on met les positions actuels à l'id 0
    for(int i = 0; i < 4; i++){
        gameState->grid->grid[positions[i].row][positions[i].column] = 0;
    }

    //on move le block (on change juste le offset du block)
    move(&gameState->current , 0 , -1);

    // On récupère les nouvelles positions du block après le déplacement
    Position *newPositions = tilePositions(&gameState->current);

    //on met les positions actuels à l'id du current block
    for(int i = 0; i < 4; i++){
        gameState->grid->grid[newPositions[i].row][newPositions[i].column] = gameState->current.id;
    }   
    
}

/**
 * @brief Déplace le block courant vers la droite si c'est possible
 * 
 * @param gameState 
 */
void moveBlockRight(GameState *gameState){
    //on récupère les positions du block
    Position *positions = tilePositions(&gameState->current);

    //on vérifie que le décalage est possible (si vide ou déjà une position du block)
    for(int i = 0; i < 4; i++){
        if(!isEmpty(positions[i].row, positions[i].column + 1, gameState->grid) && !isPositionInBlock(&gameState->current, (Position){positions[i].row, positions[i].column + 1})){
            return;
        }
    }

    //on met les positions actuels à l'id 0
    for(int i = 0; i < 4; i++){
        gameState->grid->grid[positions[i].row][positions[i].column] = 0;
    }

    //on move le block (on change juste le offset du block)
    move(&gameState->current , 0 , 1);

    // On récupère les nouvelles positions du block après le déplacement
    Position *newPositions = tilePositions(&gameState->current);

    //on met les positions actuels à l'id du current block
    for(int i = 0; i < 4; i++){
        gameState->grid->grid[newPositions[i].row][newPositions[i].column] = gameState->current.id;
    }  
}


/**
 * @brief Déplace le block courant vers le bas si c'est possible
 * 
 * @param gameState 
 */
void moveBlockDown(GameState *gameState){
    //on récupère les positions du block
    Position *positions = tilePositions(&gameState->current);

    //on vérifie que le décalage est possible (si vide ou déjà une position du block)
    for(int i = 0; i < 4; i++){
        //si le block est en bas ou si les positions du block + 1 ne sont pas vide et ne sont pas déjà une position du block
        if((positions[i].row == gameState->grid->row -1) || (!isPositionInBlock(&gameState->current , (Position){positions[i].row + 1,positions[i].column}) && gameState->grid->grid[positions[i].row + 1][positions[i].column] != 0)){

            gameState->current = getAndUpdateBlock(gameState->queue);
            reset(&gameState->current);  // Utiliser la fonction reset pour réinitialiser le bloc
            gameState->score += clearFullRows(gameState->grid);

        // Pour faire apparaître le block directement 

            //on récupère les positions du block
            Position *newPos = tilePositions(&gameState->current);
                    
            for(int j=0 ; j < 4 ; j++){
                if(gameState->grid->grid[newPos[j].row + 1][newPos[j].column] != 0 &&  gameState->grid->grid[newPos[j].row + 2][newPos[j].column] != 0 ){
                    gameState->gameOver = true;
                    return;
                }
            }

            //on move le block (on change juste le offset du block)
            move(&gameState->current , 2, 0);

            // On récupère les nouvelles positions du block après le déplacement
            Position *newPositions = tilePositions(&gameState->current);

            //on met les positions actuels à l'id du current block
            for(int i = 0; i < 4; i++){
                gameState->grid->grid[newPositions[i].row][newPositions[i].column] = gameState->current.id;
            }  
            
            return;
        }          
    }

    //on met les positions actuels à l'id 0
    for(int i = 0; i < 4; i++){
        gameState->grid->grid[positions[i].row][positions[i].column] = 0;
    }

    //on move le block (on change juste le offset du block)
    move(&gameState->current , 1, 0);

    // On récupère les nouvelles positions du block après le déplacement
    Position *newPositions = tilePositions(&gameState->current);

    //on met les positions actuels à l'id du current block
    for(int i = 0; i < 4; i++){
        gameState->grid->grid[newPositions[i].row][newPositions[i].column] = gameState->current.id;
    }  
}
