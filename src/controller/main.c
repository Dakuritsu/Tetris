#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../model/TetrisGrid.h"
#include "../model/Block.h"
#include "../model/BlockQueue.h"
#include "../model/GameState.h"
#include "../view/TetrisViewSDL.h"

// gcc src/controller/main.c src/view/TetrisViewSDL.c src/view/TetrisViewSDL.h src/model/*.c  -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf  

/*
probleme lorsque l'on ne peut plus descendre on peut quand même rotate meme si on ne peut pas

//manque de temps :
    probleme des pieces qui peuent rotate à la fin
    relancer une partie 
    descendre immédiatement 
    répétition de code et optimisation
*/

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
int main(int argc, char *argv[]){
    Block *IBlock = createBlock(4, 4, tileArrayI, (Position){-1, 3}, 1);
    Block *JBlock = createBlock(4, 4, tileArrayJ, (Position){0, 3}, 2);
    Block *LBlock = createBlock(4, 4, tileArrayL, (Position){0, 3}, 3); 
    Block *OBlock = createBlock(4, 4, tileArrayO, (Position){0, 4}, 4);
    Block *SBlock = createBlock(4, 4, tileArrayS, (Position){0, 3}, 5);
    Block *TBlock = createBlock(4, 4, tileArrayT, (Position){0, 3}, 6);
    Block *ZBlock = createBlock(4, 4, tileArrayZ, (Position){0, 3}, 7);

    Block blockArray[7] = {*IBlock , *JBlock , *LBlock , *OBlock , *SBlock , *TBlock , *ZBlock};

//----------------------------------------------------------------

    initializeSDL();
    initialiseTTF();

    SDL_Window* window = createWindow("Tetris", 800, 600);
    SDL_Renderer* renderer = createRenderer(window);

    SDL_Surface* icon = createSurface("TetrisAssets/Icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    SDL_Color textColor = {250, 250, 230, 255};
    SDL_Color gridColor = {32, 32, 32, 255};


    Uint32 lastMoveTime = SDL_GetTicks();
    Uint32 moveInterval = 500; 

    
    GameState *gameState = createGameState(createBlockQueue(blockArray));
    Image **imageControl = createImageControls(gameState->grid, 25);


    // Charger et afficher le fond d'image
    create_background(renderer, "TetrisAssets/Background.png");
    drawRectangleGrid(renderer, 250, 500, gridColor);//510 pour voir le block qui nous tue

    // createText(renderer, "Hold", "TetrisAssets/text.ttf", 24, textColor, 150, 250);    
    // drawRectangle(renderer, 100, 100, gridColor, 125 , 300);

    createText(renderer, "Next", "TetrisAssets/text.ttf", 24, textColor, 600, 250);
    
    Draw(gameState , renderer, imageControl);
    SetupImageControls(renderer, gameState->grid , imageControl);
    SDL_RenderPresent(renderer);
    

    SDL_Event event;
    int gameExit = 0;

    while (!gameExit)
    {   
        // Vérifier le temps écoulé depuis le dernier déplacement automatique
        Uint32 currentTime = SDL_GetTicks();
    
        if (currentTime - lastMoveTime >= moveInterval && !gameState->gameOver){
            // Déplacer la pièce vers le bas
            moveBlockDown(gameState);
            DrawGameOver(gameState , renderer);
            updateScoreText(renderer, gameState->score , textColor, 350 , 15);

            // Mettre à jour le moment du dernier déplacement automatique
            lastMoveTime = currentTime;

            // Dessiner la grille et le bloc actuel
            Draw(gameState, renderer, imageControl);
        }  

        while (SDL_PollEvent(&event))
        {   
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            printf("LEFT :\n");
                            moveBlockLeft(gameState);
                            
                            Draw(gameState, renderer, imageControl);
                            break;

                        case SDLK_RIGHT:
                            printf("RIGHT :\n");
                            moveBlockRight(gameState);
                            
                            Draw(gameState, renderer, imageControl);

                            break;


                        case SDLK_DOWN:
                            printf("DOWN :\n");
                            moveBlockDown(gameState);
                            DrawGameOver(gameState , renderer);

                            updateScoreText(renderer, gameState->score , textColor, 350 , 15);
                            
                            Draw(gameState, renderer, imageControl);

                            break;

                        case SDLK_UP:
                            printf("UP :\n");
                            rotateBlockCW(gameState);

                            Draw(gameState, renderer, imageControl);

                            break;

                        case SDLK_z:
                            printf("Z :\n");
                            rotateBlockCCW(gameState);

                            Draw(gameState, renderer, imageControl);

                            break;

                        default:
                            break;
                        
                    }
                    break;
                
                case SDL_QUIT:
                    gameExit = 1;
                    break;

                default:
                    break;
            }
        }
    }


    destroyImageControls(imageControl , gameState->grid->row , gameState->grid->column);
    destroyGameState(gameState);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    destroyBlock(IBlock);
    destroyBlock(JBlock);
    destroyBlock(LBlock);
    destroyBlock(OBlock);
    destroyBlock(SBlock);
    destroyBlock(TBlock);
    destroyBlock(ZBlock);

    return 0;
}
