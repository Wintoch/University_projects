#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "window.h"
#include "characters.h"
#include "config.h"
#include <time.h>

HUNTER *hunters = NULL;
STAR *stars = NULL;
WIND_PARTICLE *wind_particles = NULL;

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
#define QUIT 'q'

void AskForNickname(WIN* win, char* buffer){
    werase(win->window);
    box(win->window, 0, 0);

    echo();
    curs_set(1);

    nodelay(win->window, FALSE);

    mvwprintw(win->window, win->height / 2 - 1, (win->width - 20) / 2, "Enter your nickname:");
    wrefresh(win->window);

    mvwgetnstr(win->window, win->height / 2, (win->width - 20) / 2, buffer, 49);
    nodelay(win->window, TRUE);

    noecho();
    curs_set(0);
}

void ShowTopThree(){
    char top_scores[3][256];
    int count = PrintThreeHighest(top_scores);

    attron(COLOR_PAIR(PLAY_COLOR));
    mvprintw(4, WIDTH + 12, "TOP 3 SCORES:");
    for(int i = 0; i < count; i++){
        move(5 + i, WIDTH + 12);
        clrtoeol();

        if(i < count){
            printw("%d %s %s", i + 1, ". ", top_scores[i]);
        }else{
            printw("---");
        }
    }
    switch(count){
        case 0:
            move(5, WIDTH + 12);
            clrtoeol();
            printw("1. -----");
        case 1:
            move(6, WIDTH + 12);
            clrtoeol();
            printw("2. -----");
        case 2:
            move(7, WIDTH + 12);
            clrtoeol();
            printw("3. -----");
            break;
    }
    attroff(COLOR_PAIR(PLAY_COLOR));
    refresh();
}

void PlayCinematic(WIN* win){
    int h = win->height;
    int w = win->width;
    
    nodelay(win->window, TRUE);

    
    for(int k = 0; k < 60; k++){ 
        for(int i = 0; i < (h * w) / 10; i++){
            int y = 1 + rand() % (h - 2);
            int x = 1 + rand() % (w - 2);
            
    
            char c;
            if (rand() % 3 == 0) {
                c = ' ';
            } else {
                const char* glitch = "@#$%&?!*+=-~";
                c = glitch[rand() % strlen(glitch)];
            }
            
            mvwaddch(win->window, y, x, c);
        }
        wrefresh(win->window);
        usleep(30000);
    }
    
    nodelay(win->window, FALSE);
}

static void FinishGame(WIN* pw, WIN* sw, BIRD* b, char* h, char* m, int c){
    PlayCinematic(pw);

    nodelay(pw->window, TRUE); 
    while(wgetch(pw->window) != ERR);
    nodelay(pw->window, FALSE);

    int s = CountScore(b);
    SaveScoreToFile(b->nickname, s);
    ShowTopThree();

    werase(sw->window); box(sw->window,0,0); ShowStatus(sw, b); wnoutrefresh(sw->window);
    werase(pw->window); box(pw->window,0,0);

    wattron(pw->window, COLOR_PAIR(c));
    mvwprintw(pw->window, BOARD_HEIGHT/2, (WIDTH-strlen(h))/2, "%s", h);
    mvwprintw(pw->window, BOARD_HEIGHT/2+1, (WIDTH-strlen(m))/2, "%s", m);
    mvwprintw(pw->window, BOARD_HEIGHT/2+2, (WIDTH-20)/2, "Score saved: %d", s);
    mvwprintw(pw->window, BOARD_HEIGHT/2+3, (WIDTH-25)/2, "Press any key to continue");
    wattroff(pw->window, COLOR_PAIR(c));
    
    wnoutrefresh(pw->window); doupdate();
    wgetch(pw->window); 
    nodelay(pw->window, 1);
}

void EndGame(WIN* playwin, WIN* statwin, int* game_over, BIRD* bird, char *message){
    *game_over = 1;
    FinishGame(playwin, statwin, bird, "GAME OVER", message, LIFE_COLOR_1);
}

void WinGame(WIN* playwin, WIN* statwin, int* game_over, BIRD* bird){
    *game_over = 1;
    FinishGame(playwin, statwin, bird, "YOU WON!", "Level Complete!", LIFE_COLOR_3);
}

void InitTaxi(BIRD* bird){
    char message[] = "You're in safe zone!";
    char message2[] = "Press any key to exit.";
    char message3[] = "Time is stopped until you exit.";
    if (bird -> taxi_active == 0) {
        bird -> taxi_active = 1;
        bird -> taxi_timer = 60;
        bird -> invulnerable_timer = 60;
        bird -> last_posx = bird -> posx;
        bird -> last_posy = bird -> posy;
    }
}

void CallTaxi(WIN* playwin, BIRD* bird){
    wattron(playwin->window, COLOR_PAIR(TAXI_COLOR));
    
    int bx = bird->posx;
    int by = bird->posy;

    // Corners
    mvwaddch(playwin->window, by - 1, bx - 1, ACS_ULCORNER); // Top left
    mvwaddch(playwin->window, by - 1, bx + 2, ACS_URCORNER); // Top right
    mvwaddch(playwin->window, by + 1, bx - 1, ACS_LLCORNER); // Bottom left
    mvwaddch(playwin->window, by + 1, bx + 2, ACS_LRCORNER); // Bottom right

    // Sides
    mvwaddch(playwin->window, by - 1, bx, ACS_HLINE);     // Top left of bird
    mvwaddch(playwin->window, by - 1, bx + 1, ACS_HLINE); // Top right of bird
    mvwaddch(playwin->window, by + 1, bx, ACS_HLINE);     // Bottom left of bird
    mvwaddch(playwin->window, by + 1, bx + 1, ACS_HLINE); // Bottom right of bird

    // Sides (lewo/prawo)
    mvwaddch(playwin->window, by, bx - 1, ACS_VLINE); // Left
    mvwaddch(playwin->window, by, bx + 2, ACS_VLINE); // Right

    mvwprintw(playwin->window, by - 2, bx - 1, "TAXI");
    
    wattroff(playwin->window, COLOR_PAIR(TAXI_COLOR));
}

void TaxiLogic(BIRD* bird, WIN* safe_zone, WIN* playwin){
    if (bird->taxi_active == 1) {
        bird->taxi_timer--;
        if (bird->taxi_timer <= 0) {
            ClearBird(bird);
            werase(safe_zone->window);
            bird->taxi_active = 2;
            bird->win = safe_zone;
            bird->posx = 4;
            bird->posy = 3;
        }else{
            bird -> speed = 0;
            CallTaxi(playwin, bird);
        }
    }

    if (bird->taxi_active == 2) {
        box(safe_zone->window, 0, 0);
        mvwprintw(safe_zone->window, 1, 1, "SAFE ZONE");
        DrawBird(bird);
        wnoutrefresh(safe_zone->window);

        attron(COLOR_PAIR(TAXI_COLOR));
        mvprintw(16, WIDTH + 12, "You're in safe zone!");
        mvprintw(17, WIDTH + 12, "Press any key to exit.");
        mvprintw(18, WIDTH + 12, "Time is stopped until you exit.");
        attroff(COLOR_PAIR(TAXI_COLOR));
        refresh();
    }

}

static int HandleInput(BIRD* bird, WIN* safe_zone){
    int ch = getch();
    if (ch == QUIT) return ch;

    if(bird -> taxi_active == 1) return ch;
    if(bird -> taxi_active == 2){
        if(ch != ERR){
            werase(safe_zone->window);
            box(safe_zone->window, 0, 0);
            mvwprintw(safe_zone->window, 1, 1, "SAFE ZONE");
            wrefresh(safe_zone->window);

            move(16, 0); clrtoeol();
            move(17, 0); clrtoeol();
            move(18, 0); clrtoeol();
            refresh();

            bird -> taxi_active = 0;
            bird -> win = bird -> original_win;
            bird -> posx = bird -> last_posx;
            bird -> posy = bird -> last_posy;
            bird -> invulnerable_timer = 10;
            bird -> speed = cfg.min_bird_speed;
        }
        return ch;
    };

    switch (ch) {
        case LEFT:  bird->dx = -1; bird->dy = 0; break;
        case RIGHT: bird->dx = 1;  bird->dy = 0; break;
        case UP:    bird->dx = 0;  bird->dy = -1; break;
        case DOWN:  bird->dx = 0;  bird->dy = 1;  break;
        case 't':
            InitTaxi(bird);
            break;
        default: break;
    }

    if (ch == 'p' && bird -> speed < cfg.max_bird_speed) bird -> speed++;
    if (ch == 'o' && bird -> speed > cfg.min_bird_speed) bird -> speed--;

    return ch;
}

void SpawnEntities(WIN* playwin, BIRD* bird, int hunter_bounces, int current_max_hunters){
    if (rand() % 10 == 0) {
        SpawnStar(stars, cfg.max_stars, playwin);
    }

    int hunter_width = 0;
    int hunter_height = 0;

    while(hunter_width * hunter_height < 2){
        hunter_width = 1 + rand() % cfg.hunter_width;
        hunter_height = 1 + rand() % cfg.hunter_height;
    }

    int side = rand() % 4;
    int hunter_spawn_x, hunter_spawn_y;

    switch(side) {
        case 0:
            hunter_spawn_y = BORDER;
            hunter_spawn_x = BORDER + rand() % (WIDTH - 2 * BORDER - hunter_width);
            break;
        case 1:
            hunter_spawn_y = BOARD_HEIGHT - BORDER - hunter_height;
            hunter_spawn_x = BORDER + rand() % (WIDTH - 2 * BORDER - hunter_width);
            break;
        case 2:
            hunter_spawn_x = BORDER;
            hunter_spawn_y = BORDER + rand() % (BOARD_HEIGHT - 2 * BORDER - hunter_height);
            break;
        case 3:
            hunter_spawn_x = WIDTH - BORDER - hunter_width;
            hunter_spawn_y = BORDER + rand() % (BOARD_HEIGHT - 2 * BORDER - hunter_height);
            break;
    }

    if (rand() % 50 == 0){
        SpawnHunter(hunters, current_max_hunters, bird, hunter_width, hunter_height, '#', hunter_spawn_x, hunter_spawn_y, hunter_bounces, LIFE_COLOR_1, cfg.hunter_speed, playwin);
    }
}

void CollisionLogic(char *message, int *game_over, BIRD* bird, WIN* playwin, WIN* statwin, int current_max_hunters, int **occupancy_map, STAR* stars){
    if (bird -> taxi_active) return;
    UpdateOccupancyMap(occupancy_map, WIDTH, BOARD_HEIGHT, hunters, current_max_hunters, stars, cfg.max_stars);

    int collision_result = CheckMapCollision(occupancy_map, bird, hunters, stars);

    if (collision_result == 1){
        if(ApplyDamage(1, bird)){
            if(bird-> life <= 0){
                message = "You're dead!";
                EndGame(playwin, statwin, game_over, bird, message);
                return;
            }
        }
    }
    
    if (bird->star_count >= STAR_TO_COLLECT){
        WinGame(playwin, statwin, game_over, bird);
        return;
    }
}

void HandleTime(WIN* playwin, WIN* statwin, int *game_over, BIRD* bird, char *message, int *hunter_bounces, int *difficulty_flag, int *difficulty_flag2, int *current_max_hunters){
        if (bird -> taxi_active) return;
        bird -> game_time -= (cfg.frame_time / 1000.0f);
        if(!(*difficulty_flag) && bird -> game_time < cfg.game_time * (2.0f/3.0f)){
            *current_max_hunters += 1;
            *hunter_bounces += 1;
            *difficulty_flag = 1;
        }

        if(!(*difficulty_flag2) && bird -> game_time < cfg.game_time * (1.0f/3.0f)){
            *current_max_hunters += 1;
            *hunter_bounces += 1;
            *difficulty_flag2 = 1;
        }

        if (bird -> game_time <= 0.0f){
            bird -> game_time = 0.0f;
            message = "You ran out of time!";
            EndGame(playwin, statwin, game_over, bird, message);
        }
}

void DrawGame(WIN* playwin, WIN* statwin, BIRD* bird, int current_max_hunters, HUNTER* hunters, STAR* stars, WIN* safe_zone){
    werase(playwin->window);
    box(playwin->window, 0, 0);

    DrawStars(stars, cfg.max_stars, playwin);
    DrawHunters(hunters, current_max_hunters);
    DrawWindParticles(playwin, wind_particles);
    TaxiLogic(bird, safe_zone, playwin);

    if(bird -> taxi_active != 2) DrawBird(bird);
}
void RefreshGame(WIN* playwin, WIN* statwin, BIRD* bird){
    werase(statwin->window);
    box(statwin->window, 0, 0);
    ShowStatus(statwin, bird);
    wnoutrefresh(statwin->window);
    wnoutrefresh(playwin->window);

    doupdate();
}


void UpdateWind(float* wx, float* wy, BIRD* b){
    static int s=0, t=0;
    static float tx=0, ty=0;
    
    int f[4] = {5000/cfg.frame_time, 2000/cfg.frame_time, 3000/cfg.frame_time, 2000/cfg.frame_time};
    for(int i=0;i<4;i++) if(f[i]<1) f[i]=1;

    if(s==0){ 
        *wx=*wy=0; 
        if(++t>f[0]){ s=1; t=0; tx=((rand()%100)/100.0f-0.5f)*0.6f; ty=((rand()%100)/100.0f-0.5f)*0.6f; }
    }
    else if(s==1){ 
        *wx=tx*(++t/(float)f[1]); *wy=ty*(t/(float)f[1]); 
        if(t>=f[1]){ s=2; t=0; }
    }
    else if(s==2){ 
        *wx=tx; *wy=ty; 
        if(++t>=f[2]){ s=3; t=0; }
    }
    else{ 
        *wx=tx*(1.0f-(++t/(float)f[3])); *wy=ty*(1.0f-(t/(float)f[3])); 
        if(t>=f[3]){ s=0; t=0; *wx=*wy=0; }
    }
    b->windx = *wx; b->wind_y = *wy;
}

static void UpdateWorld(WIN* playwin, BIRD* b, int bounces, int max_h, int** map, float wind_x, float wind_y){
    SpawnEntities(playwin, b, bounces, max_h);
    MoveStars(stars, cfg.max_stars, wind_x, wind_y);
    MoveHunters(hunters, max_h, b, wind_x, wind_y);
    MoveBird(b, wind_x, wind_y);
    ManageWindParticles(playwin, wind_x, wind_y, map, wind_particles);
}

//MAIN GAME LOOP
void MainLoop(WIN* playwin, WIN* statwin, BIRD* bird, int **occupancy_map, WIN* safe_zone){
    mvwprintw(safe_zone->window, 1, 1, "SAFE ZONE");
    wrefresh(safe_zone->window);

    int ch;
    int current_max_hunters = cfg.max_hunters;
    int hunter_bounces = 1 + rand() % 4;
    int difficulty_flag = 0;
    int difficulty_flag2 = 0;

    float wind_x = 0.0f;
    float wind_y = 0.0f;
    char *message = NULL;

    int game_over = 0;
    while (!game_over){
        if ((ch = HandleInput(bird, safe_zone)) == QUIT) break;

        UpdateWind(&wind_x, &wind_y, bird);
        UpdateWorld(playwin, bird, hunter_bounces, current_max_hunters, occupancy_map, wind_x, wind_y);

        CollisionLogic(message, &game_over, bird, playwin, statwin, current_max_hunters, occupancy_map, stars);

        if (game_over) break;
        
        DrawGame(playwin, statwin, bird, current_max_hunters, hunters, stars, safe_zone);

        HandleTime(playwin, statwin, &game_over, bird, message, &hunter_bounces, &difficulty_flag, &difficulty_flag2, &current_max_hunters);

        RefreshGame(playwin, statwin, bird);
        
        usleep(cfg.frame_time * 1000);
    }

}

char* ShowMenu(WIN* win){
    char **levels = NULL;
    int level_count = GetAvailableLevels("config.txt", &levels);
    if(level_count == 0)NULL;

    int selected = 0;
    int ch;

    nodelay(win->window, FALSE);
    keypad(win->window, TRUE);

    while(1){
        werase(win->window);
        box(win->window,0,0);
        mvwprintw(win->window, 2, (win->width - 12) / 2, "SELECT LEVEL");
        for(int i = 0; i < level_count; i++){
            if(i == selected) wattron(win->window, A_REVERSE);
            mvwprintw(win->window, 5 + i, (win->width - strlen(levels[i])) / 2, "%s", levels[i]);
            if(i == selected) wattroff(win->window, A_REVERSE);
        }
        mvwprintw(win->window, win->height - 2, 2, "Use UP/DOWN to navigate, ENTER to select, Q to quit");
        wrefresh(win->window);

        ch = wgetch(win->window);
        if(ch == KEY_UP || ch == 'w'){
            selected--;
            if(selected < 0) selected = level_count - 1;
        }else if(ch == KEY_DOWN || ch == 's'){
            selected++;
            if(selected >= level_count) selected = 0;
        }else if(ch == '\n' || ch == ' '){
            break;
        }else if(ch == 'q' || ch == 'Q'){
            FreeLevels(levels, level_count);
            return NULL;
        }
    }

    char* chosen_level = strdup(levels[selected]);
    FreeLevels(levels, level_count);

    nodelay(win->window, TRUE);
    return chosen_level;
}

void ClearWindowsAfterLevels(WIN* playwin, WIN* statwin, WIN* safe_zone){
    werase(safe_zone->window);
    box(safe_zone->window, 0, 0);
    mvwprintw(safe_zone->window, 1, 1, "SAFE ZONE");
    wrefresh(safe_zone->window);

    move(16, WIDTH + 12); clrtoeol();
    move(17, WIDTH + 12); clrtoeol();
    move(18, WIDTH + 12); clrtoeol();
    refresh();

    werase(statwin->window);
    box(statwin->window, 0, 0);
    wrefresh(statwin->window);
}

void PlayLevel(WIN* playwin, WIN* statwin, WIN* safe_zone, char* nickname){
    srand(cfg.seed ? cfg.seed : time(NULL));

    int **occupancy_map = malloc(BOARD_HEIGHT * sizeof(int*));
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        occupancy_map[i] = calloc(WIDTH, sizeof(int));
    }

    hunters = calloc(cfg.max_hunters + 2, sizeof(HUNTER));
    stars = calloc(cfg.max_stars, sizeof(STAR));
    wind_particles = calloc(MAX_WIND_PARTICLES, sizeof(WIND_PARTICLE));

    BIRD* bird = InitBird(playwin, WIDTH / 2, BOARD_HEIGHT / 2, 1, 0);
    strncpy(bird->nickname, nickname, sizeof(bird->nickname) - 1);
    bird->nickname[sizeof(bird->nickname) - 1] = '\0';

    MainLoop(playwin, statwin, bird, occupancy_map, safe_zone);

    ClearWindowsAfterLevels(playwin, statwin, safe_zone);

    free(bird);
    free(hunters);
    free(stars);
    free(wind_particles);
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        free(occupancy_map[i]);
    }
    free(occupancy_map);

    hunters = NULL;
    stars = NULL;
    wind_particles = NULL;
}

int main(){
    setlocale(LC_ALL, "");

    WINDOW *mainwin = Start();
    WIN* playwin = InitWin(mainwin, BOARD_HEIGHT, WIDTH, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
    WIN* statwin = InitWin(mainwin, STAT_HEIGHT, WIDTH, OFFY + BOARD_HEIGHT, OFFX, STAT_COLOR, BORDER, 0);
    WIN* safe_zone = InitWin(stdscr, 5, 11, 10, WIDTH + 20, TAXI_COLOR, 1, 0);
    
    while(1){
        char *level = ShowMenu(playwin);
        if(!level) break;

        werase(playwin->window);
        box(playwin->window, 0, 0);
        wrefresh(playwin->window);


        LoadConfig("config.txt", level);
        free(level);

        ShowTopThree();
        char nickname[50];
        AskForNickname(statwin, nickname);

        PlayLevel(playwin, statwin, safe_zone, nickname);
    }

    endwin();

    return 0;
}