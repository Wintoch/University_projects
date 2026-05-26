#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

CONFIG cfg;

void SetDefaults(){
    cfg.max_stars = 3;
    cfg.max_hunters = 3;
    cfg.game_time = 60.0f;
    cfg.frame_time = 30;
    cfg.max_bird_speed = 5;
    cfg.min_bird_speed = 1;
    cfg.hunter_speed = 1;
    cfg.hunter_height = 3;
    cfg.hunter_width = 2;
    cfg.seed = 0;
}

static char *trim(char *s){
    char *end;
    while(isspace((unsigned char)*s)) s++;
    if(*s == 0) return s;
    end = s + strlen(s) - 1;
    while(end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

int GetAvailableLevels(const char *filename, char ***levels) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[256];
    int count = 0;
    int capacity = 10;
    *levels = malloc(capacity * sizeof(char*));
    
    while(fgets(line, sizeof line, file)){
        char *p = trim(line);
        if (p[0] == '#' || p[0] == '\0') continue;
        if (strchr(p, '=') == NULL) {
            if (count >= capacity) {
                capacity *= 2;
                *levels = realloc(*levels, capacity * sizeof(char*));
            }
            (*levels)[count] = strdup(p);
            count++;
        }
    }
    fclose(file);
    return count;
}

void FreeLevels(char **levels, int count) {
    for (int i = 0; i < count; i++) {
        free(levels[i]);
    }
    free(levels);
}

void LoadConfig(const char *filename, const char *level_name){
    SetDefaults();

    FILE *file = fopen(filename, "r");
    char line[256];
    if (!file) return;

    char buf[256];
    int run = 0;

    while(fgets(buf, sizeof buf, file)){
        char *p = trim(buf);
        if (*p == '#' || !*p) continue;

        char *eq = strchr(p, '=');
        if(!eq){
            run = level_name && !strcmp(level_name, p);
            continue;
        }

        if(!run) continue;

        *eq++ = 0;
        char *k = trim(p), *v = trim(eq);

        #define P(s,x) if(!strcmp(k,s)) cfg.x = atoi(v)
        P("MAX_HUNTERS", max_hunters);
        else P("MAX_STARS", max_stars);
        else P("FRAME_TIME", frame_time);
        else P("MAX_BIRD_SPEED", max_bird_speed);
        else P("MIN_BIRD_SPEED", min_bird_speed);
        else P("HUNTER_SPEED", hunter_speed);
        else P("HUNTER_HEIGHT", hunter_height);
        else P("HUNTER_WIDTH", hunter_width);
        else P("SEED", seed);
        else if(!strcmp(k, "GAME_TIME")) cfg.game_time = atof(v);
        #undef P
    }
    fclose(file);
}

void SaveScoreToFile(const char* nickname, int score) {
    FILE *fp = fopen("stats.txt", "a");
    if (fp == NULL) {
        return;
    }
    // Format: Nick - Score
    fprintf(fp, "%s - Score: %d\n", nickname, score);
    fclose(fp);
}
int PrintThreeHighest(char top_scores[3][256]) {
    FILE *fp = fopen("stats.txt", "r");

    if (fp == NULL) return 0;

    char line[256];
    int scores[3] = {0, 0, 0};
    char lines[3][256] = {"", "", ""};
    
    while (fgets(line, sizeof(line), fp)) {
        char *score_ptr = strrchr(line, ':');
        if(!score_ptr) continue;
        int score = atoi(score_ptr + 1);
        for(int i = 0; i<3; i++){
            if (score > scores[i]){
                for(int j = 2; j>i; j--){
                    scores[j] = scores[j-1];
                    strcpy(lines[j], lines[j-1]);
                }
                scores[i] = score;
                strcpy(lines[i], line);
                break;
            }
        }
    }
    fclose(fp);

    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (lines[i][0] != '\0') {
            strcpy(top_scores[count], lines[i]);
            top_scores[count][strcspn(top_scores[count], "\n")] = 0;
            count++;
        }
    }
    return count;
}


int CountScore(BIRD* b){
    int StarsCollected = b -> star_count;
    int RemainingLife = b -> life;
    int game_time = (int)(b -> game_time);

    int divisor = (StarsCollected > 0) ? StarsCollected : 1;
    int score = (int)(game_time * 10) + (StarsCollected * 100) + (RemainingLife * 50);
    return score;
}