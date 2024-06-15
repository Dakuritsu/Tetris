#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "TetrisGrid.h"


/**
 * @brief Creer une TetrisGrid
 * 
 * @param row    ligne
 * @param column colonne
 * @return       TetrisGrid* 
 */
TetrisGrid *createGrid(int row , int column){
    TetrisGrid *tetris = (TetrisGrid *)malloc(sizeof(TetrisGrid));
    if(!tetris){
        fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
		exit(EXIT_FAILURE);
    }

    tetris->row = row;
    tetris->column = column;

    tetris->grid = (int**)malloc((tetris->row * sizeof(int *)));
    if(!tetris->grid){
        fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
		exit(EXIT_FAILURE);
    }

    for(int i=0 ; i < tetris->row ; i++){
        tetris->grid[i] = (int*)malloc((tetris->column *sizeof(int)));
        if(!tetris->grid[i]){
            fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0 ; i < tetris->row ; i++){
        for(int j=0 ; j < tetris->column ; j++){
            tetris->grid[i][j] = 0;
        }
    }
    return tetris;
}

/**
 * @brief détruit une Tetris grid
 * 
 * @param grid la Tetris grid a détruire
 */
void destroyGrid(TetrisGrid *grid){
    if (grid){
        // Libérer la mémoire des lignes de la grille
        for (int i = 0; i < grid->row; ++i){
            free(grid->grid[i]);
        }

        // Libérer la mémoire du tableau de pointeurs de lignes
        free(grid->grid);

        // Libérer la mémoire de la structure TetrisGrid
        free(grid);
    }
}

/**
 * @brief Indique si la case (row , column) se trouve dans les limites de la grille
 * 
 * @param row       ligne
 * @param column    colonne
 * @param t         TetrisGrid
 * @return true     -> si ce trouve dans les limites
 * @return false    -> sinon
 */
bool isIn(int row, int column , TetrisGrid *t){
    return row >= 0 && row < t->row && column >= 0 && column < t->column;
}


/**
 * @brief Indique si la case (row , column) est vide
 * 
 * @param row       ligne
 * @param column    colonne
 * @param t         TetrisGrid
 * @return          true  -> si est vide
 * @return          false -> sinon
 */

bool isEmpty(int row , int column , TetrisGrid *t){
    return isIn(row , column , t) && t->grid[row][column] == 0;
}


/**
 * @brief Vérifie si une ligne est complète
 * 
 * @param row   ligne
 * @param t     TetrisGrid
 * @return      true  -> si la ligne est complète
 * @return      false -> sinon
 */
bool isRowFull(int row , TetrisGrid *t){
    for(int j=0 ; j < t->column ; j++){
        if(t->grid[row][j] == 0)
            return false;
    }
    return true;
}


/**
 * @brief Vérifie si une ligne est vide
 * 
 * @param row   ligne
 * @param t     TetrisGrid
 * @return      true  -> si la ligne est vide
 * @return      false -> sinon
 */
bool isRowEmpty(int row , TetrisGrid *t){
    for(int j=0 ; j < t->column ; j++){
        if(t->grid[row][j] != 0)
            return false;
    }
    return true;
}

/**
 * @brief Met chacune des cases de la ligne à (0,0)
 * 
 * @param row   ligne
 * @param t     TetrisGrid
 */
void clearRow(int row , TetrisGrid *t){
    for(int j=0 ; j < t->column ; j++){
        t->grid[row][j] = 0;
    }
}

/**
 * @brief Déplace la ligne à l'indice "row" de "numRows" vers le bas
 * 
 * @param row       ligne
 * @param numRows   nombre de ligne 
 * @param t         TetrisGrid
 */
void moveRunDown(int row , int numRows , TetrisGrid *t){
    for(int j=0 ; j < t->column ; j++){
        t->grid[row + numRows][j] = t->grid[row][j];
        t->grid[row][j] = 0;
    }
}


/**
 * @brief Efface toutes les lignes pleines et fait tomber les lignes
 * 
 * @param t    TetrisGrid 
 * @return int -> nombre de lignes effacées
 */
int clearFullRows(TetrisGrid *t){
    int cleared = 0;

    for(int i = (t->row - 1) ; i >= 0 ; i--){
        if(isRowFull(i , t)){
            clearRow(i , t);
            cleared++;
        }
        else if(cleared > 0){
            moveRunDown(i , cleared , t);
        }
    }
    printf("nombre de lignes effacées : %d\n", cleared);
    return cleared;

}