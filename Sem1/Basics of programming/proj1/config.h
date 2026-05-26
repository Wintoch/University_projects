#ifndef CONFIG_H
#define CONFIG_H
#include "window.h"

typedef struct{
    int max_hunters;
    int max_stars;
    float game_time;
    int frame_time;
    int max_bird_speed;
    int min_bird_speed;
    int hunter_speed;
    int hunter_height;
    int hunter_width;
    int seed;
} CONFIG;

extern CONFIG cfg;

void  SetDefaults(void);
void LoadConfig(const char *filename, const char *level_name);
void SaveScoreToFile(const char* nickname, int score);
int CountScore(BIRD* b);
int PrintThreeHighest(char top_scores[3][256]);
int GetAvailableLevels(const char *filename, char ***levels);
void FreeLevels(char **levels, int count);
#endif