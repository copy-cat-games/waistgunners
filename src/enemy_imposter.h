#include "game_state.h"
#include "vector.h"
#include "bomber.h"

#ifndef _ENEMY_IMPOSTER
#define _ENEMY_IMPOSTER

typedef struct ENEMY_IMPOSTER_ENGINE {
    VECTOR position;
    int health;
    bool dead;
} ENEMY_IMPOSTER_ENGINE;

typedef struct ENEMY_IMPOSTER_GUNNER {
    VECTOR position;
    ENGINE* target;
} ENEMY_IMPOSTER_GUNNER;

#define ENGINES_PER_IMPOSTER 2
#define GUNNERS_PER_IMPOSTER 2

typedef struct ENEMY_IMPOSTER_DATA {
    VECTOR position;
    ENEMY_IMPOSTER_ENGINE* engines[ENGINES_PER_IMPOSTER];
    ENEMY_IMPOSTER_GUNNER* gunners[GUNNERS_PER_IMPOSTER];
    BOMBER* target;
    bool down;
} ENEMY_IMPOSTER_DATA;

extern VECTOR IMPOSTER_SIZE;
extern VECTOR IMPOSTER_ENGINE_SIZE;
extern VECTOR IMPOSTER_ENGINES[2];
extern VECTOR IMPOSTER_GUNNERS[2];

void update_enemy_imposter(ENEMY_IMPOSTER_DATA* imposter);

#endif