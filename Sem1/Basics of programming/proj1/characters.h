#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "window.h"
#include "config.h"

#define BORDER 1
#define OFFY 2
#define OFFX 5
#define BIRD_LIFE 3
#define STAR_TO_COLLECT 5
#define MAX_WIND_PARTICLES 7

typedef struct {
    WIN* win;
    int x, y;
    float fx, fy;
    int speed;
    int tick;
    char symbol;
    int active;
} STAR;

typedef struct {
    WIN* win;
    int height,width;
    char symbol;
    int posx, posy;
    float fx, fy;
    float dx, dy;
    int speed;
    int color;
    int bounces;
    int active;
    int dash_cooldown;
    int stop_timer;
    int dash_timer;
    int dash_cooldown_timer;
} HUNTER;

typedef struct {
    float x,y;
    int active;
    int life;
    int type;
}WIND_PARTICLE;

BIRD* InitBird(WIN* win, int posx, int posy, int dx, int dy);
void DrawBird(BIRD* B);
void ClearBird(BIRD* B);
void MoveBird(BIRD* B, float wind_x, float wind_y);
void SpawnStar(STAR* star, int max_stars, WIN* win);
void DrawStars(STAR* stars, int max_stars, WIN* playwin);
void MoveStars(STAR* stars, int max_stars, float wind_x, float wind_y);
void UpdateOccupancyMap(int **map, int width, int height, HUNTER* hunters, int max_hunters, STAR* stars, int max_stars);
int CheckMapCollision(int **map, BIRD* bird, HUNTER* hunters, STAR* stars);
int ApplyDamage(int condition, BIRD* bird);
void SetHunterTrajectory(HUNTER* hunter, BIRD* bird);
void SpawnHunter(HUNTER* hunter, int max_hunters, BIRD* bird, int width, int height, char symbol, int posx, int posy, int bounces, int color, int speed, WIN* win);
int ResolveHunterBounce(HUNTER* hunter);
void DrawHunters(HUNTER* hunters, int max_hunters);
void MoveHunter(HUNTER* hunter, BIRD* bird, float wind_x, float wind_y);
void MoveHunters(HUNTER* hunters, int max_hunters, BIRD* bird, float wind_x, float wind_y);
void HunterDash(HUNTER* hunter, BIRD* bird);
void DrawWindParticles(WIN* playwin, WIND_PARTICLE* wind_particles);
void ManageWindParticles(WIN* playwin, float wind_x, float wind_y, int **occupancy_map, WIND_PARTICLE* wind_particles);

#endif