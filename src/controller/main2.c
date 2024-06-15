#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ncurses.h>
#include <time.h>

#include "../model/TetrisGrid.h"
#include "../model/Block.h"
#include "../model/BlockQueue.h"
#include "../model/GameState.h"
#include "../view/TetrisViewNCURSES.h"





//====================DÉCLARATIONS==============================
Position tileArrayI[][4]    =  {    { {1, 0}, {1, 1}, {1, 2}, {1, 3} },
                                    { {0, 2}, {1, 2}, {2, 2}, {3, 2} },
                                    { {2, 0}, {2, 1}, {2, 2}, {2, 3} },
                                    { {0, 1}, {1, 1}, {2, 1}, {3, 1} }  };

Position tileArrayJ[][4]    =  {    { {0, 0}, {1, 0}, {1, 1}, {1, 2} },
                                    { {0, 1}, {0, 2}, {1, 1}, {2, 1} },
                                    { {1, 0}, {1, 1}, {1, 2}, {2, 2} },
                                    { {0, 1}, {1, 1}, {2, 0}, {2, 1} }  };

Position tileArrayL[][4]    =  {    { {0, 2}, {1, 0}, {1, 1}, {1, 2} },
                                    { {0, 1}, {1, 1}, {2, 1}, {2, 2} },
                                    { {1, 0}, {1, 1}, {1, 2}, {2, 0} },
                                    { {0, 0}, {0, 1}, {1, 1}, {2, 1} }  };

Position tileArrayO[][4]    =  {    { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
                                    { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
                                    {  {0, 0}, {0, 1}, {1, 0}, {1, 1} },
                                    {  {0, 0}, {0, 1}, {1, 0}, {1, 1} }  };

Position tileArrayS[][4]    =  {    { {0, 1}, {0, 2}, {1, 0}, {1, 1} },
                                    { {0, 1}, {1, 1}, {1, 2}, {2, 2} },
                                    { {1, 1}, {1, 2}, {2, 0}, {2, 1} },
                                    { {0, 0}, {1, 0}, {1, 1}, {2, 1} }  };

Position tileArrayT[][4]    =  {    { {0, 1}, {1, 0}, {1, 1}, {1, 2} },
                                    { {0, 1}, {1, 1}, {1, 2}, {2, 1} },
                                    { {1, 0}, {1, 1}, {1, 2}, {2, 1} },
                                    { {0, 1}, {1, 0}, {1, 1}, {2, 1} }  };

Position tileArrayZ[][4]    =  {    { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
                                    { {0, 2}, {1, 1}, {1, 2}, {2, 1} },
                                    { {1, 0}, {1, 1}, {2, 1}, {2, 2} },
                                    { {0, 1}, {1, 0}, {1, 1}, {2, 0} }  };


//================FONCTION MAIN=========================================
int main(int argc, char argv[]) {
    Block *IBlock = createBlock(4, 4, tileArrayI, (Position){-1, 3}, 1);
    Block *JBlock = createBlock(4, 4, tileArrayJ, (Position){0, 3}, 2);
    Block *LBlock = createBlock(4, 4, tileArrayL, (Position){0, 3}, 3); 
    Block *OBlock = createBlock(4, 4, tileArrayO, (Position){0, 4}, 4);
    Block *SBlock = createBlock(4, 4, tileArrayS, (Position){0, 3}, 5);
    Block *TBlock = createBlock(4, 4, tileArrayT, (Position){0, 3}, 6);
    Block *ZBlock = createBlock(4, 4, tileArrayZ, (Position){0, 3}, 7);

    Block blockArray[7] = {*IBlock , *JBlock , *LBlock , *OBlock , *SBlock , *TBlock , *ZBlock};

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    timeout(0); // Définir le temps d'attente pour getch() à 0 

    WINDOW *win = createWindow(60, 80, 0, 0);
    refresh();

    box(win, 0, 0);

    GameState *gameState = createGameState(createBlockQueue(blockArray));
    char **imageControl = createImageControls(gameState->grid);

    drawRectangle(win, 10, 22, 35, 38); // la grille
    createText(win, "Next :", 60, 25);

    wrefresh(win); // refresh que la fenetre

    int ch;
    clock_t lastMoveTime = clock(); // temps dudernier déplacement
    double moveInterval = 0.5 * CLOCKS_PER_SEC; // Interval de déplacement en secondes

    while ((ch = getch()) != ERR) {
        switch (ch) {
            case KEY_LEFT:
                // Action lorsque la touche gauche est pressée
                moveBlockLeft(gameState);
                break;

            case KEY_RIGHT:
                // Action lorsque la touche droite est pressée
                moveBlockRight(gameState);
                break;

            case KEY_DOWN:
                // Action lorsque la touche bas est pressée
                moveBlockDown(gameState);
                DrawGameOver(gameState, win);
                updateScoreText(win, gameState->score, 350, 15);
                Draw(gameState, win, imageControl);
                break;

            case KEY_UP:
                // Action lorsque la touche haut est pressée
                rotateBlockCW(gameState);
                Draw(gameState, win, imageControl);
                break;

            case 'z':
                // Action lorsque la touche 'z' est pressée
                rotateBlockCCW(gameState);
                Draw(gameState, win, imageControl);
                break;

            case 'q':
                goto end_ncurses;
        }

        // Pour vérifier le temps écoulé
        clock_t currentTime = clock();
        double elapsedMillis = ((double)(currentTime - lastMoveTime) / CLOCKS_PER_SEC) * 1000.0;

        if (elapsedMillis >= moveInterval && !gameState->gameOver) {
            // Déplacer la pièce vers le bas
            moveBlockDown(gameState);
            DrawGameOver(gameState, win);
            updateScoreText(win, gameState->score, 350, 15);
            Draw(gameState, win, imageControl);

            // Mettre à jour le moment du dernier déplacement 
            lastMoveTime = clock();
        }

        refresh();
    }

end_ncurses:
    endwin();

    return 0;
}
