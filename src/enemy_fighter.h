#include "vector.h"
#include "game_state.h"
#include "bomber.h"
#include "bullet.h"

#ifndef _ENEMY_FIGHTER
#define _ENEMY_FIGHTER

typedef struct ENEMY_FIGHTER_DATA {
    VECTOR position;
    ENGINE* target;
    int health;
    int gun;
    int cooldown;
    bool dead;
    int side; // 0 means left, 1 means right
    float angle;
} ENEMY_FIGHTER_DATA;

#define MAX_ENEMY_FIGHTER_HEALTH 15
#define MAX_ENEMY_FIGHTER_SPEED  1
#define ENEMY_FIGHTER_THRESHOLD  1
#define ENEMY_FIGHTER_COOLDOWN   40
#define ENEMY_FIGHTER_LEAN       ALLEGRO_PI / 300
#define ENEMY_FIGHTER_MAX_ANGLE  ALLEGRO_PI / 6

#endif

extern VECTOR FIGHTER_SIZE;

void update_enemy_fighter(ENEMY_FIGHTER_DATA* fighter);