#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "Block.h"
#include "BlockQueue.h"
#include "TetrisGrid.h"

/**
 * @struct GameState
 * @brief Représente un GameState
 */
typedef struct {
    Block current;
    TetrisGrid *grid;
    BlockQueue *queue;
    bool gameOver;
    int score;
}GameState;

GameState *createGameState(BlockQueue *blockQueue);
void destroyGameState(GameState *gameState);

void rotateBlockCW(GameState *gameState);
void rotateBlockCCW(GameState *gameState);

void moveBlockLeft(GameState *gameState);
void moveBlockRight(GameState *gameState);
void moveBlockDown(GameState *gameState);

#endif