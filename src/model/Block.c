#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Block.h"



/**
 * @brief Creer un block
 * 
 * @param tilesI        dimensions du bloc
 * @param tilesJ        dimensions du bloc
 * @param tiles         Stock les cases représentant la pièce par rapport à la rotation     [Les différentes rotations][Un tableau de Position pour chaque rotation]
 * @param startOffset   Décalage de départ pour savoir ou le bloc apparait dans la grille
 * @param id            Pour identifier les différents blocs
 * @return              Block* 
 */
Block *createBlock(int tilesI , int tilesJ , Position tiles[][4] , Position startOffset, int id){
    Block *newBlock = (Block*)malloc(sizeof(Block));
    if(!newBlock){
        fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
		exit(EXIT_FAILURE);
    }

    newBlock->tilesI = tilesI;
    newBlock->tilesJ = tilesJ;

//  -----------------------------------------------------------------------

    newBlock->tiles = (Position **)malloc(newBlock->tilesI * sizeof(Position *));
    if (!newBlock->tiles){
        fprintf(stderr, "Erreur : problème allocation dynamique.\n");
        free(newBlock);
        exit(EXIT_FAILURE);
    }

//  -----------------------------------------------------------------------

    for (int i = 0; i < newBlock->tilesI; ++i){
        newBlock->tiles[i] = (Position *)malloc(newBlock->tilesJ * sizeof(Position));
        if (!newBlock->tiles[i]){
            fprintf(stderr, "Erreur : problème allocation dynamique pour les lignes du tableau de tuiles.\n");

            // Libérer la mémoire allouée jusqu'à présent
            for (int j = 0; j < i; ++j){
                free(newBlock->tiles[j]);
            }

            free(newBlock->tiles);
            free(newBlock);
            exit(EXIT_FAILURE);
        }


        // Copier les données du tableau tiles dans newBlock->tiles
        for (int k = 0; k < newBlock->tilesJ; ++k){
            newBlock->tiles[i][k] = tiles[i][k];
        }
    }

    newBlock->startOffset = startOffset;
    newBlock->rotationState = 0;
    newBlock->offset = startOffset;//(Position){0,0};
    newBlock->id = id;

    return newBlock;
}

/**
 * @brief Libère la mémoire allouée pour un block
 * 
 * @param block 
 */
void destroyBlock(Block *block){
    for (int i=0; i < block->tilesI; ++i){
        free(block->tiles[i]);
    }

    free(block->tiles);
    free(block);
}


/**
 * @brief Pour obtenir les positions des tuiles par rapport à leur positions
 * 
 * @param block 
 * @return Position* 
 */
Position* tilePositions(Block *block){

    //On créer un tableau de position
    Position *result = (Position*)malloc(4 * sizeof(Position));// 4 le nombre de case occupé pour chaque pièce
    if (!result){
        fprintf(stderr, "Erreur : problème allocation dynamique pour le tableau de positions.\n");
        exit(EXIT_FAILURE);
    }

    // On récupère le tableau de la rotation actuel 
    Position *currentTile = block->tiles[block->rotationState]; 
    
    // On récupère les positions correspondants à cette rotation
    for (int i = 0; i < 4 ; ++i){
        result[i].row = currentTile[i].row + block->offset.row;
        result[i].column = currentTile[i].column + block->offset.column;
    }

    return result;
}


/**
 * @brief Pivoter dans le sens des aiguilles d'une montre
 * 
 * @param block 
 */
void rotateCW(Block *block){
    block->rotationState = (block->rotationState + 1) % 4;
}


/**
 * @brief Pivoter dans le sens inverse des aiguilles d'une montre
 * 
 * @param block 
 */
void rotateCCW(Block *block){
    if (block->rotationState == 0){
        block->rotationState = 4 - 1;
    } 
    else{
        block->rotationState--;
    }
}


/**
 * @brief Déplace le bloc d'un nombre donné de ligne et de colonne
 * 
 * @param block 
 * @param rows      le nombre de ligne
 * @param columns   le nombre de colonne
 */
void move(Block *block , int rows , int columns){
    //On modifie le offset
    block->offset.row += rows;
    block->offset.column += columns;

}


/**
 * @brief Réinitialise la rotation et la position
 * 
 * @param block 
 */
void reset(Block *block){
    
    block->rotationState = 0;
    block->offset = block->startOffset;
}


/**
 * @brief Retourne un Block aléatoire dans un tableau contenant chaque Block
 * 
 * @param blocks un tableau contenant chaque block
 * @return       Block 
 */
Block getRandomBlock(Block blocks[7]){
    return blocks[rand() % 7];
}

/**
 * @brief Vérifie si une position fait partie des positions d'un block
 * 
 * @param block    le block utilisé
 * @param position la position a vérifier
 * @return true 
 * @return false 
 */
bool isPositionInBlock(Block *block , Position position){
    Position* p = tilePositions(block);

    for(int i=0 ; i < 4 ; i++){
        if(p[i].row == position.row && p[i].column == position.column)
            return true;
    }
    return false;
}