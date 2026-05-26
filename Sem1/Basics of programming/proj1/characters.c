#include "characters.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

BIRD* InitBird(WIN* win, int posx, int posy, int dx, int dy){
    BIRD* B = (BIRD*)malloc(sizeof(BIRD));

    B -> win = win;
    B -> posx = posx;
    B -> posy = posy;
    B -> fx = (float)posx;
    B -> fy = (float)posy;
    B -> dx = dx;
    B -> dy = dy;
    B -> speed = 1;
    B -> color = LIFE_COLOR_3;
    B -> life = BIRD_LIFE;
    B -> invulnerable_timer = 0;
    B -> game_time = cfg.game_time;
    B -> star_count = 0;
    B -> taxi_active = 0;
    B -> taxi_timer = 0;
    B -> original_win = win;
    B -> last_posx = posx;
    B -> last_posy = posy;
    strcpy(B->animation1, "\\/");
    strcpy(B->animation2, "--");
    strcpy(B->animation3, "/\\");
    B->anim_timer = 0;
    B->current_frame = 0;
    B->windx = 0.0f;
    B->wind_y = 0.0f;

    return B;
}

void DrawBird(BIRD* B){
    wattron(B -> win -> window, COLOR_PAIR(B -> color));
    B->anim_timer++;
    if (B->anim_timer >= 5) {
        B->anim_timer = 0;
        B->current_frame = (B->current_frame + 1) % 3;
    }

    char *frame;
    if (B->current_frame == 0) frame = B->animation1;
    else if (B->current_frame == 1) frame = B->animation2;
    else frame = B->animation3;
    mvwprintw(B -> win -> window, B -> posy, B -> posx, "%s", frame);
    wattroff(B->win->window, COLOR_PAIR(B -> color));
}

void ClearBird(BIRD* B){
    mvwprintw(B -> win -> window, B -> posy, B -> posx, "  ");
}

void MoveBird(BIRD* B, float wind_x, float wind_y){
    if(B -> taxi_active) return;

    if (B->invulnerable_timer > 0) {
        B->invulnerable_timer--;
    }

    float move_x = (B->dx * B->speed) + (wind_x * 0.5f);
    float move_y = (B->dy * B->speed) + (wind_y * 0.5f);

    B->fx += move_x;
    B->fy += move_y;

    if (B->fx < BORDER) {
        B->fx = BORDER;
        B->dx = -(B->dx);
    } else if (B -> fx > B->win->width - BORDER - 2) {
        B->fx = B->win->width - BORDER - 2;
        B->dx = -(B->dx);
    }

    if (B->fy < BORDER) {
        B->fy = BORDER;
        B->dy = -(B->dy);
    } else if (B->fy > B->win->height - BORDER - 1) {
        B->fy = B->win->height - BORDER - 1;
        B->dy = -(B->dy);
    }

    B->posx = (int)B->fx;
    B->posy = (int)B->fy;

}

void SpawnStar(STAR* star, int max_stars, WIN* win) {
    for (int i = 0; i < max_stars; i++){
        if (!star[i].active){
            star[i].win = win;
            star[i].x = rand() % (WIDTH - 2 * BORDER) + BORDER;
            star[i].y = 1;
            star[i].fx = (float)star[i].x;
            star[i].fy = (float)star[i].y;
            star[i].speed = (rand() % 3) + 1;
            star[i].tick = 0;
            star[i].symbol = '*';
            star[i].active = 1;
            break;
        }
    }
}

void MoveStars(STAR* stars, int max_stars, float wind_x, float wind_y) {
    for (int i = 0; i < max_stars; i++) {
        if (stars[i].active) {
                
            float vertical_speed = 1.0f / (float)stars[i].speed;

            stars[i].fx += wind_x;
            stars[i].fy += vertical_speed + wind_y;

            if (stars[i].fx < BORDER) stars[i].fx = BORDER;
            if (stars[i].fx > WIDTH - BORDER - 1) stars[i].fx = WIDTH - BORDER - 1;

            stars[i].x = (int)stars[i].fx;
            stars[i].y = (int)stars[i].fy;
            
            if (stars[i].y >= BOARD_HEIGHT - 1) {
                stars[i].active = 0;
            }
        }
    }
}

void DrawStars(STAR* stars, int max_stars, WIN* playwin) {
    for (int i = 0; i < max_stars; i++) {
        if (stars[i].active) {
            if(stars[i].y > BOARD_HEIGHT - 10){
                if (rand() % 2 == 0) continue;
            }

            wattron(playwin->window, COLOR_PAIR(MAIN_COLOR));
            mvwprintw(playwin->window, stars[i].y, stars[i].x, "%c", stars[i].symbol);
            wattroff(playwin->window, COLOR_PAIR(MAIN_COLOR));
        }
    }
}

void UpdateOccupancyMap(int **map, int width, int height, HUNTER* hunters, int max_hunters, STAR* stars, int max_stars) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map[y][x] = 0;
        }
    }

    for (int i = 0; i < max_hunters; i++) {
        if (hunters[i].active) {
            for (int ry = 0; ry < hunters[i].height; ry++) {
                int y = hunters[i].posy + ry;
                if (y < 0 || y >= height) continue;
                for (int rx = 0; rx < hunters[i].width; rx++) {
                    int x = hunters[i].posx + rx;
                    if (x < 0 || x >= width) continue;
                    map[y][x] = i + 1;
                }
            }
        }
    }

    int STAR_OFFSET = 1000;
    for (int i = 0; i < max_stars; i++) {
        if (stars[i].active) {
            if (stars[i].x >= 0 && stars[i].x < width && stars[i].y >= 0 && stars[i].y < height) {
                map[stars[i].y][stars[i].x] = STAR_OFFSET + i;
            }
        }
    }
}

int CheckMapCollision(int **map, BIRD* bird, HUNTER* hunters, STAR* stars){
    if (bird->posx < 0 || bird->posx >= WIDTH - 1 || bird->posy < 0 || bird->posy >= BOARD_HEIGHT) {
        return 0;
    }
    
    int vals[2];
    vals[0] = map[bird->posy][bird->posx];
    vals[1] = map[bird->posy][bird->posx + 1];

    int hit_hunter = 0;
    int hit_star = 0;

    for(int i=0; i<2; i++){
        int val = vals[i];
        if (val > 0) {
            if (val < 1000){
                int hunter_index = val - 1;
                if(hunters[hunter_index].active){
                    hunters[hunter_index].active = 0;
                    hit_hunter = 1;
                }
            }else{
                int star_index = val - 1000;
                if(stars[star_index].active){
                    stars[star_index].active = 0;
                    bird->star_count++; // Zwiększamy licznik od razu tutaj
                    hit_star = 1;
                }
            }
        }
    }
    
    if (hit_hunter) return 1;
    if (hit_star) return 2;
    return 0;
}

int ApplyDamage(int condition, BIRD* bird){
    if(bird-> invulnerable_timer > 0) return 0;

    if(!condition) return 0;

    bird-> life--;
    bird-> invulnerable_timer = 10;

    if(bird-> life == 2) bird-> color = LIFE_COLOR_2;
    else if(bird-> life == 1) bird-> color = LIFE_COLOR_1;
    return 1;
}

void SetHunterTrajectory(HUNTER* hunter, BIRD* bird) {
    if(!bird -> taxi_active){
        float h_cx = hunter->fx + (hunter->width / 2.0f);
        float h_cy = hunter->fy + (hunter->height / 2.0f);
        float dx = (float)bird->posx - h_cx;
        float dy = (float)bird->posy - h_cy;
        float len = sqrtf(dx * dx + dy * dy);

        if (len > 0.001f) {
            hunter->dx = dx / len;
            hunter->dy = dy / len;
        } else {
            hunter->dx = 1.0f;
            hunter->dy = 0.0f;
        }
    }else{
        hunter->dx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        hunter->dy = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

        //Ensure hunter moves into the board if bird in safe zone
        if (hunter->fx <= BORDER + 1) hunter->dx = fabsf(hunter->dx); //  Left side -> go right
        if (hunter->fx + hunter->width >= hunter->win->width - BORDER - 1) hunter->dx = -fabsf(hunter->dx); // Right side -> go left
        
        if (hunter->fy <= BORDER + 1) hunter->dy = fabsf(hunter->dy); // Top -> go down
        if (hunter->fy + hunter->height >= hunter->win->height - BORDER - 1) hunter->dy = -fabsf(hunter->dy); // Bottom -> go up

        float len = sqrtf(hunter->dx * hunter->dx + hunter->dy * hunter->dy);
        if (len > 0.001f) {
            hunter->dx /= len;
            hunter->dy /= len;
        }
    }
}

void SpawnHunter(HUNTER* hunter, int max_hunters, BIRD* bird, int width, int height, char symbol, int posx, int posy, int bounces, int color, int speed, WIN* win) {
    float dx = (float)posx - bird->posx;
    float dy = (float)posy - bird->posy;
    if ((dx * dx + dy * dy) < 100.0f) return;
    
    for (int i = 0; i < max_hunters; i++){
        if(!hunter[i].active){
            hunter[i].win = win;
            hunter[i].width = width;
            hunter[i].height = height;
            hunter[i].symbol = symbol;
            hunter[i].posx = posx;
            hunter[i].posy = posy;
            hunter[i].fx = (float)posx;
            hunter[i].fy = (float)posy;
            hunter[i].bounces = bounces;
            hunter[i].color = color;
            hunter[i].speed = speed;
            hunter[i].active = 1;
            hunter[i].dash_cooldown = 1;
            hunter[i].stop_timer = 0;
            hunter[i].dash_timer = 0;
            hunter[i].dash_cooldown_timer = (rand() % 100) + 30;

            SetHunterTrajectory(&hunter[i], bird);
            break;
        }
    }
}

int ResolveHunterBounce(HUNTER* hunter) {
    int bounced = 0;
    int max_w = hunter->win->width - BORDER;
    int max_h = hunter->win->height - BORDER;

    if (hunter->posx < BORDER) {
        hunter->fx = BORDER;
        hunter->dx = -hunter->dx;
        bounced = 1;
    } else if (hunter->posx + hunter->width > max_w) {
        hunter->fx = max_w - hunter->width;
        hunter->dx = -hunter->dx;
        bounced = 1;
    }
    
    if (hunter->posy < BORDER) {
        hunter->fy = BORDER;
        hunter->dy = -hunter->dy;
        bounced = 1;
    } else if (hunter->posy + hunter->height > max_h) {
        hunter->fy = max_h - hunter->height;
        hunter->dy = -hunter->dy;
        bounced = 1;
    }
    return bounced;
}

void MoveHunter(HUNTER* hunter, BIRD* bird, float wind_x, float wind_y) {
    if (!hunter || !hunter->active) return;
    
    if (hunter->stop_timer > 0) {
        hunter->stop_timer--;
        //if bird is in safe zone, hunter stops to find bird but does not dash because he can't find him
        if (hunter->stop_timer == 0 && bird->taxi_active == 0) HunterDash(hunter, bird);
        return;
    }

    if (hunter->dash_cooldown_timer > 0) {
        hunter->dash_cooldown_timer--;
    } else if (hunter->dash_cooldown) {
        hunter->stop_timer = 20;
        hunter->dash_cooldown_timer = 0;
        hunter->dash_cooldown = 0;
        return;
    }

    float current_speed = hunter->speed;
    if (hunter->dash_timer > 0) {
        current_speed *= 3.0f;
        hunter->dash_timer--;
    }

    hunter->fx += (hunter->dx * current_speed) + wind_x;
    hunter->fy += (hunter->dy * current_speed) + wind_y;
    hunter->posx = (int)hunter->fx;
    hunter->posy = (int)hunter->fy;

    if(ResolveHunterBounce(hunter)){
        hunter->bounces--;
        if(hunter->bounces <= 0) hunter->active = 0;
        hunter->posx = (int)hunter->fx;
        hunter->posy = (int)hunter->fy;
    }
}

void HunterDash(HUNTER* hunter, BIRD* bird) {
    if(!hunter->active) return;

    float hunter_center_x = hunter->fx + (hunter->width / 2.0f);
    float hunter_center_y = hunter->fy + (hunter->height / 2.0f);

    float distance_x = (float)bird->posx - hunter_center_x;
    float distance_y = (float)bird->posy - hunter_center_y;
    
    float length = sqrtf(distance_x * distance_x + distance_y * distance_y);

    if(length > 0.001f){
        hunter->dx = distance_x / length;
        hunter->dy = distance_y / length;

        hunter->dash_timer = 5;
    }

}

void DrawHunters(HUNTER* hunters, int max_hunters) {
    for (int i = 0; i < max_hunters; i++) {
        if (hunters[i].active) {
            wattron(hunters[i].win->window, COLOR_PAIR(hunters[i].color));
            for (int ry = 0; ry < hunters[i].height; ry++) {
                int y = hunters[i].posy + ry;
                if (y < 0 || y >= hunters[i].win->height) continue;
                for (int rx = 0; rx < hunters[i].width; rx++) {
                    int x = hunters[i].posx + rx;
                    if (x < 0 || x >= hunters[i].win->width) continue;
                    mvwaddch(hunters[i].win->window, y, x, hunters[i].symbol);
                }
            }
            wattroff(hunters[i].win->window, COLOR_PAIR(hunters[i].color));
        }
    }
}

void MoveHunters(HUNTER* hunters, int max_hunters, BIRD* bird, float wind_x, float wind_y) {
    for (int i = 0; i < max_hunters; i++) {
        if (hunters[i].active) {
            MoveHunter(&hunters[i], bird, wind_x, wind_y);
        }
    }
}

void DrawWindParticles(WIN* playwin, WIND_PARTICLE* wind_particles) {
    wattron(playwin->window, COLOR_PAIR(WIND_COLOR));
    for (int i = 0; i < MAX_WIND_PARTICLES; i++) {
        if (wind_particles[i].active) {
            int x = (int)wind_particles[i].x;
            int y = (int)wind_particles[i].y;
            
            if (x >= BORDER && x <= WIDTH - BORDER - 2 && y >= BORDER && y <= BOARD_HEIGHT - BORDER - 1) {
                if (wind_particles[i].type == 0) {
                    mvwprintw(playwin->window, y, x, "_@");
                } else {
                    mvwprintw(playwin->window, y, x, "@_");
                }
            }
        }
    }
    wattroff(playwin->window, COLOR_PAIR(WIND_COLOR));
}

void ManageWindParticles(WIN* playwin, float wind_x, float wind_y, int **occupancy_map, WIND_PARTICLE* wind_particles) {
    for (int i = 0; i < MAX_WIND_PARTICLES; i++) {
        if (wind_particles[i].active) {
            wind_particles[i].x += wind_x * 2.0f; // Faster than wind
            wind_particles[i].y += wind_y * 2.0f;
            wind_particles[i].life--;

            if (wind_particles[i].life <= 0 || 
                wind_particles[i].x < BORDER || wind_particles[i].x > WIDTH - BORDER - 2 ||
                wind_particles[i].y < BORDER || wind_particles[i].y > BOARD_HEIGHT - BORDER - 1) {
                wind_particles[i].active = 0;
            }
        }
    }

    if (fabs(wind_x) > 0.05f || fabs(wind_y) > 0.05f) {
        if (rand() % 20 == 0) {
            for (int i = 0; i < MAX_WIND_PARTICLES; i++) {
                if (!wind_particles[i].active) {
                    int x = rand() % (WIDTH - 2 * BORDER) + BORDER;
                    int y = rand() % (BOARD_HEIGHT - 2 * BORDER) + BORDER;

                    if (occupancy_map[y][x] == 0 && occupancy_map[y][x+1] == 0) {
                        wind_particles[i].x = (float)x;
                        wind_particles[i].y = (float)y;
                        wind_particles[i].active = 1;
                        wind_particles[i].life = 30;
                        wind_particles[i].type = (wind_x > 0) ? 0 : 1;
                        break;
                    }
                }
            }
        }
    }
}