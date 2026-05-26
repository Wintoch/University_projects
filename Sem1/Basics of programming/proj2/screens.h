#pragma once

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


#define SCREEN_WIDTH	1400
#define SCREEN_HEIGHT	800

extern int czarny;
extern int zielony;
extern int czerwony;
extern int niebieski;

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void DrawStringScaled(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset, float scale);
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface* screen, int x, int y, int w, int h, Uint32 color1, Uint32 color2);
void DrawBackground(SDL_Surface* screen, SDL_Surface* background, int cameraX, int cameraY);
int CallMenu(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer, char* playerNickname);
void FreeAll(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer);
void DrawStatSection(SDL_Surface* screen, SDL_Surface* charset, double worldTime, double fps, const char* nickname, int playerHealth, int enemy1Health, int enemy2Health,  int score);