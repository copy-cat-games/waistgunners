#include "vector.h"
#include "game_state.h"

#ifndef _ENEMY_JET
#define _ENEMY_JET

#define MAX_ENEMY_JET_HEALTH 26

typedef struct ENEMY_JET_DATA {
    VECTOR position;
    VECTOR motion;
    int direction;
    int health;
} ENEMY_JET_DATA;

void update_enemy_jet(ENEMY_JET_DATA* jet);

#endif