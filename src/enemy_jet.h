#include "vector.h"
#include "triangle.h"
#include "game_state.h"
#include "enemy_missile.h"
#include "bullet.h"

#ifndef _ENEMY_JET
#define _ENEMY_JET

#define MAX_ENEMY_JET_HEALTH 26
#define ENEMY_JET_Y_SPEED 6
#define ENEMY_JET_X_SPEED 1

extern VECTOR JET_SIZE;

typedef enum ENEMY_JET_DIRECTION {
    DOWN = 0,
    UP,
} ENEMY_JET_DIRECTION;

typedef struct ENEMY_JET_DATA {
    VECTOR position;
    VECTOR motion;
    ENEMY_JET_DIRECTION direction;
    int health;
    bool fired;
} ENEMY_JET_DATA;

void update_enemy_jet(ENEMY_JET_DATA* jet);
ENEMY_JET_DATA create_enemy_jet();

#endif