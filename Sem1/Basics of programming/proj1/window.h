#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>

#define WIDTH 140
#define BOARD_HEIGHT 40
#define STAT_HEIGHT 6

#define MAIN_COLOR   1
#define STAT_COLOR   2
#define PLAY_COLOR   3

#define LIFE_COLOR_3 4
#define LIFE_COLOR_2 5
#define LIFE_COLOR_1 6
#define TAXI_COLOR   7
#define WIND_COLOR   8

typedef struct {
    WINDOW* window;
    int x, y;
    int width, height;
    int color;
} WIN;

typedef struct {
    WIN* win;
    int posx, posy;
    float fx, fy;
    int dx, dy;
    int speed;
    int color;
    int life;
    int invulnerable_timer;
    int star_count;
    float game_time;
    int taxi_active;
    int taxi_timer;
    WIN* original_win;
    int last_posx, last_posy;
    char nickname[50];
    char animation1[4];
    char animation2[4];
    char animation3[4];
    int anim_timer;
    int current_frame;
    float windx, wind_y;
} BIRD;

WINDOW* Start();
WIN* InitWin(WINDOW* parent, int height, int width, int starty, int startx, int color, int bo, int delay);
void CleanWin(WIN* W, int bo);
void ShowStatus(WIN* W, BIRD* b);
#endif