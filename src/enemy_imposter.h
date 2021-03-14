#include "game_state.h"
#include "vector.h"
#include "bomber.h"

#ifndef _ENEMY_IMPOSTER
#define _ENEMY_IMPOSTER

#define IMPOSTER_ENGINE_MAX_HEALTH 24

typedef struct ENEMY_IMPOSTER_ENGINE {
    VECTOR position;
    int health;
    bool dead;
} ENEMY_IMPOSTER_ENGINE;

#define IMPOSTER_GUNNER_MAX_SHOTS 3
#define IMPOSTER_GUNNER_COOLDOWN  10
#define IMPOSTER_GUNNER_RELOAD    45

typedef struct ENEMY_IMPOSTER_GUNNER {
    VECTOR position;
    ENGINE* target;
    int cooldown;
    int reload;
    int shots;
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

#define ENEMY_IMPOSTER_ENGINE_POINTS 175

void update_enemy_imposter(ENEMY_IMPOSTER_DATA* imposter);

#endif