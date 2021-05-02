#include "game_state.h"
#include "vector.h"

#ifndef _ENEMY_MISSILE
#define _ENEMY_MISSILE

typedef struct ENEMY_MISSILE_DATA {
    VECTOR position;
    VECTOR motion;
    int health;
    // what frame the flame is in is determined by graphics.c
} ENEMY_MISSILE_DATA;

void update_enemy_missile(ENEMY_MISSILE_DATA* missile);

#endif