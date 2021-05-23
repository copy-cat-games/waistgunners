#include "vector.h"
#include "triangle.h"
#include "game_state.h"
#include "enemy_missile.h"
#include "particle.h"
#include "bullet.h"

#ifndef _ENEMY_JET
#define _ENEMY_JET

#define MAX_ENEMY_JET_HEALTH 10
#define ENEMY_JET_Y_SPEED 5
#define ENEMY_JET_X_SPEED 1

#define ENEMY_JET_TURNAROUND 75
#define ENEMY_JET_POINTS     150

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
    int turnaround_countdown;
    bool fired;
    bool dead;
    bool turnaround;
} ENEMY_JET_DATA;

void update_enemy_jet(ENEMY_JET_DATA* jet);
TRIANGLE get_triangle(ENEMY_JET_DATA* jet);
ENEMY_JET_DATA create_enemy_jet();
void kill_enemy_jet(ENEMY_JET_DATA* jet);

#endif