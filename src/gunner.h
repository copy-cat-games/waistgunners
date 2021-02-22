#include "vector.h"
#include "game_state.h"
#include "mouse.h"
#include "bullet.h"

#ifndef _GUNNER
#define _GUNNER

typedef struct GUNNER {
    VECTOR position;
    int cooldown;
    int shots;
    int reload;
} GUNNER;

// the values below probably need some balancing.
// our testers will see.

#define GUNNER_COOLDOWN  5
#define GUNNER_RELOAD    100
#define GUNNER_MAX_SHOTS 5

GUNNER create_gunner();
void update_gunner(GUNNER* gunner);

#endif