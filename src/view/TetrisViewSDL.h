#ifndef __VUETETRISSDL_H__
#define __VUETETRISSDL_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "../model/Block.h"
#include "../model/BlockQueue.h"
#include "../model/TetrisGrid.h"
#include "../model/GameState.h"

//====================DÉCLARATIONS==============================



// Structure pour représenter une image
typedef struct{
    char* source;
    int width;
    int height;
}Image;

// Tableau de chaînes de caractères pour les tuiles
extern const char* tileImages[];

// Tableau de chaînes de caractères pour les blocs
extern const char* blockImages[];

//================ENTÊTES DE FONCTIONS==================================

void SDL_ExitWithError(const char *message);
void initializeSDL();
void initialiseTTF();

SDL_Window* createWindow(char* title,int width, int height);
SDL_Renderer* createRenderer(SDL_Window* window);
SDL_Surface* createSurface(const char* imagePath);
SDL_Texture* createTexture(SDL_Renderer* renderer, SDL_Surface* image);

void create_background(SDL_Renderer *r , const char* imagePath);
void createText(SDL_Renderer* renderer, const char* text, const char* fontPath, int fontSize, SDL_Color textColor, int x, int y);
void drawRectangleGrid(SDL_Renderer* renderer, int width, int height, SDL_Color color);
void updateScoreText(SDL_Renderer* renderer, int score, SDL_Color textColor, int x , int y);
void drawRectangle(SDL_Renderer* renderer, int width, int height, SDL_Color color , int x , int y);

Image** createImageControls(TetrisGrid* grid , int cellSize);
void destroyImageControls(Image** imageControls, int rows , int columns);
void SetupImageControls(SDL_Renderer* renderer, TetrisGrid* grid , Image** imageControls);
void DrawGrid(Image** imageControls, TetrisGrid grid);
void DrawBlock(Image** imageControls, TetrisGrid grid, Block *block);
void DrawNext(GameState *gameState , SDL_Renderer* renderer);
void Draw(GameState *gameState , SDL_Renderer* renderer , Image** imageControls);
void DrawGameOver(GameState *gameState , SDL_Renderer* renderer);
#endif