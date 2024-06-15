#ifndef VUETETRISNCURSES_H
#define VUETETRISNCURSES_H

#include <ncurses.h>
#include "../model/Block.h"
#include "../model/BlockQueue.h"
#include "../model/TetrisGrid.h"
#include "../model/GameState.h"

extern const char* tileImages[];
extern const char* blockImages[];

void initializeNCurses();
void cleanupNCurses();

WINDOW* createWindow(int height, int width, int starty, int startx);
void createBox(WINDOW* win);
void createText(WINDOW* win, const char* text, int x, int y);

void updateScoreText(WINDOW* win, int score, int x, int y);

/*
void drawRectangleGrid(WINDOW* win, int width, int height, int cellSize, int x, int y, chtype colorPair);
void drawRectangle(int width, int height, int color, int x, int y);
*/

char** createImageControls(TetrisGrid* grid);
void destroyImageControls(char** imageControls, int rows , int columns);
void SetUpImagesControls(WINDOW* win, TetrisGrid* grid, char** imageControls);

void DrawBlock(WINDOW* win, TetrisGrid *grid, Block* block, char** imageControls);
void DrawNext(WINDOW*win , GameState *gameState);
void drawRectangle(WINDOW*win ,int width, int height, int x, int y);
void DrawGameOver(WINDOW* win,GameState *gameState);

#endif