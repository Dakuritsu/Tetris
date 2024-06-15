#ifndef __BLOCK_H__
#define __BLOCK_H__
#include <stdbool.h>
/**
 * @struct Position
 * @brief Représente une position
 */
typedef struct Position{
    int row;
    int column;
}Position;

/**
 * @struct Block
 * @brief Représente un Block
 */
typedef struct Block{
    Position **tiles;       // Stock les cases représentant la pièce par rapport à la rotation   [Les différentes rotations][Un tableau de Position pour chaque rotation]
    int tilesI;             //dimensions du bloc
    int tilesJ;             //dimensions du bloc
    Position startOffset;   // Décalage de départ pour savoir ou le bloc apparait dans la grille
    int rotationState;      // Rotation actuel pour tourner la pièce
    Position offset;        // Décalage actuel pour déplacer la pièce
    int id;                 // Pour identifier les différents blocs
}Block;

Block *createBlock(int tilesI , int tilesJ , Position tiles[][4] ,  Position startOffset, int id);
void destroyBlock(Block *block);

Position* tilePositions(Block *block);
void rotateCW(Block *block);
void rotateCCW(Block *block);
void move(Block *block , int row , int column);
void reset(Block *block);

Block getRandomBlock(Block blocks[7]);
bool isPositionInBlock(Block *block , Position position);

#endif