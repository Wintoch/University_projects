#include "window.h"
#include "characters.h"
#include <stdlib.h>

WINDOW* Start(){
    WINDOW* win;

    if ((win = initscr()) == NULL){
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    int min_h = BOARD_HEIGHT + STAT_HEIGHT + OFFY + 5; // + margines
    int min_w = WIDTH + OFFX + 30; // + margines for TOP 3 and SAFE ZONE

    int term_h, term_w;
    getmaxyx(stdscr, term_h, term_w);

    if (term_h < min_h || term_w < min_w) {
        endwin();
        fprintf(stderr, "Terminal is too small! Minimum size: %dx%d\n", min_w, min_h);
        fprintf(stderr, "Current size: %dx%d\n", term_w, term_h);
        exit(EXIT_FAILURE);
    }

    start_color();

    init_pair(MAIN_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(STAT_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(PLAY_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(LIFE_COLOR_3, COLOR_GREEN, COLOR_BLACK);
    init_pair(LIFE_COLOR_2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LIFE_COLOR_1, COLOR_RED, COLOR_BLACK);
    init_pair(TAXI_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(WIND_COLOR, COLOR_BLUE, COLOR_BLACK);

    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    return win;
}

void CleanWin(WIN* W, int bo){
    int i ,j;

    wattron(W -> window, COLOR_PAIR(W -> color));

    if (bo) box(W -> window, 0, 0);

    for (i = 1; i < W -> height - 1; i++)
        for (j = 1; j < W -> width - 1; j++)
            mvwprintw(W -> window, i, j, " ");

    wrefresh(W -> window);
}

WIN* InitWin(WINDOW* parent, int height, int width, int starty, int startx, int color, int bo, int delay){
    WIN* W = (WIN*)malloc(sizeof(WIN));

    W -> x = startx;
    W -> y = starty;
    W -> width = width;
    W -> height = height;
    W -> color = color;

    W->window = subwin(parent, height, width, starty, startx);

    wbkgd(W -> window, COLOR_PAIR(color));

    CleanWin(W, bo);
    if (delay == 0) nodelay(W -> window, TRUE);

    wrefresh(W -> window);

    return W;
}

void ShowStatus(WIN* W, BIRD* b){
    wattron(W -> window, COLOR_PAIR(W -> color));

    int col_width = W->width / 4;
    int c1 = 2;
    int c2 = c1 + col_width;
    int c3 = c2 + col_width;
    int c4 = c3 + col_width;

    mvwprintw(W -> window, 1, c1, "PLAYER: %s", b->nickname);
    mvwprintw(W -> window, 2, c1, "LIFE: %d", b -> life);

    mvwprintw(W -> window, 1, c2, "STARS: %d/%d", b -> star_count, STAR_TO_COLLECT);
    mvwprintw(W -> window, 2, c2, "TIME: %.1f s", b -> game_time);

    mvwprintw(W -> window, 1, c3, "WIND X: %.2f", b -> windx);
    mvwprintw(W -> window, 2, c3, "WIND Y: %.2f", b -> wind_y);

    mvwprintw(W -> window, 1, c4, "SPEED: %d", b -> speed);
    mvwprintw(W -> window, 2, c4, "TAXI: %s", b -> taxi_active ? "ON" : "OFF");

    
    mvwprintw(W -> window, 4, c1, "CONTROLS: W/A/S/D - Move | Q - Quit | O/P - Speed | T - Taxi");
}