#include "vector.h"
#include "game_state.h"

// serves as a wrapper for all enemies
// include each enemy type's header file here
// make sure the enemy types do not depend on ENEMY being defined!
#include "enemy_fighter.h"
#include "enemy_imposter.h"
#include "enemy_jet.h"

#ifndef _ENEMY
#define _ENEMY

typedef enum ENEMY_TYPE {
    ENEMY_FIGHTER = 0,
    ENEMY_IMPOSTER,
    ENEMY_JET,
} ENEMY_TYPE;

typedef union ENEMY_DATA {
    ENEMY_FIGHTER_DATA fighter;
    ENEMY_IMPOSTER_DATA imposter;
    ENEMY_JET_DATA jet;
} ENEMY_DATA;

typedef struct ENEMY {
    ENEMY_DATA data;
    ENEMY_TYPE type;
    bool used;
} ENEMY;

#define MAX_ENEMIES 128
extern ENEMY enemies[MAX_ENEMIES];

VECTOR get_enemy_position(ENEMY* enemy);
void update_enemies();
void add_enemy_fighter();
void add_enemy_imposter();
void add_enemy_jet();

// variables/constants needed for enemy spawning should go here, as spawning is handled by enemy.c

// enemy fighter has no spawning data

// enemy imposter spawning stuff
#define DEFAULT_IMPOSTER_SPAWN_DELAY FRAME_RATE * 100
extern int imposter_countdown;
extern int enemy_imposters;

// point values for each enemy type are defined in their respective header files
// they'll need balancing

#endif