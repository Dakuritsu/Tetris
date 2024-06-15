#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Block.h"
#include "BlockQueue.h"

/**
 * @brief Créer un BlockQueue
 * 
 * @param blocks tableau de blocks contenant chaque pièce
 * @return       BlockQueue* 
 */
BlockQueue *createBlockQueue(Block blocks[7]){
    if (blocks == NULL){
        fprintf(stderr, "Erreur : le tableau de blocs ne doit pas être NULL.\n");
        exit(EXIT_FAILURE);
    }

    BlockQueue *queue = (BlockQueue*)malloc(sizeof(*queue));
    if(!queue){
        fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
		exit(EXIT_FAILURE);
    }

    queue->blocks = blocks;
    queue->nextBlock = getRandomBlock(blocks);

    return queue;
}


/**
 * @brief Pour obtenir et mettre à jour le prochain bloc
 * 
 * @param queue 
 * @return Block 
 */
Block getAndUpdateBlock(BlockQueue *queue){
    Block block = queue->nextBlock;
    
    do//pour ne pas avoir le même block
    {
       queue->nextBlock = getRandomBlock(queue->blocks);
    } while (block.id == queue->nextBlock.id);
    
    return block;
}


/**
 * @brief Libère la mémoire allouée pour un BlockQueue
 * 
 * @param queue 
 */
void destroyBlockQueue(BlockQueue *queue){
    if (queue){
        free(queue);
    }
}