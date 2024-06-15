#ifndef __TETRISGRID_H__
#define __TETRISGRID_H__
#include <stdbool.h>
/**
 * @struct TetrisGrid
 * @brief Représente une grille Tetris
 */
typedef struct TetrisGrid{
    int **grid; // Le tableau représentant la grille
    int row;    // Nombre de ligne
    int column; // Nombre de colonne
}TetrisGrid;

void SDL_ExitWithError(const char *message);

void initializeSDL();
void initialiseTTF();

TetrisGrid *createGrid(int row , int column);
void destroyGrid(TetrisGrid *grid);

bool isIn(int row, int column , TetrisGrid *t);
bool isEmpty(int row , int column , TetrisGrid *t);
bool isRowFull(int row , TetrisGrid *t);
bool isRowEmpty(int row , TetrisGrid *t);
void clearRow(int row , TetrisGrid *t);
void moveRunDown(int row , int numRows , TetrisGrid *t);
int clearFullRows(TetrisGrid *t);

#endif