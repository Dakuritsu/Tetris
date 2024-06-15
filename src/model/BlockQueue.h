#ifndef __BLOCKQUEUE_H__
#define __BLOCKQUEUE_H__

#include "Block.h"

/**
 * @struct BlockQueue
 * @brief Représente un BlockQueue
 */
typedef struct{
Block *blocks;// Tableau contenant chaque pièce
Block nextBlock;
}BlockQueue;

BlockQueue *createBlockQueue(Block blocks[7]);
Block getAndUpdateBlock(BlockQueue *queue);
void destroyBlockQueue(BlockQueue *queue);

#endif