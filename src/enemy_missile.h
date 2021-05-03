#include "game_state.h"
#include "vector.h"
#include "bomber.h"

#ifndef _ENEMY_MISSILE
#define _ENEMY_MISSILE

#define ENEMY_MISSILE_X_SPEED 1
#define ENEMY_MISSILE_Y_SPEED 3

#define ENEMY_MISSILE_MAX_HEALTH 6

extern VECTOR MISSILE_SIZE;
extern VECTOR MISSILE_FIRE_SIZE;
extern VECTOR MISSILE_FIRE_OFFSET;

typedef struct ENEMY_MISSILE_DATA {
    VECTOR position;
    VECTOR motion;
    int health;
    // what frame the flame is in is determined by graphics.c
    ENGINE* target;
} ENEMY_MISSILE_DATA;

void update_enemy_missile(ENEMY_MISSILE_DATA* missile);
ENEMY_MISSILE_DATA create_missile(VECTOR pos, int x_direction);

#endif