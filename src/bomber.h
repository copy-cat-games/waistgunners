#include "vector.h"
#include "engine.h"
#include "gunner.h"
#include "keyboard.h"

#ifndef _BOMBER
#define _BOMBER

#define ENGINES_PER_BOMBER 2
#define GUNNERS_PER_BOMBER 2
#define BOMBER_SPEED 1

typedef struct BOMBER {
    VECTOR position; // top left corner
    ENGINE* engines[ENGINES_PER_BOMBER];
    GUNNER* gunners[GUNNERS_PER_BOMBER];
    bool down;
} BOMBER;

#define MAX_BOMBERS 4
#define BOMBER_MARGIN 10

extern VECTOR formation;
extern VECTOR BOMBER_SIZE;
extern BOMBER bombers[MAX_BOMBERS + 1];

void update_bombers();
void reset_bombers();

BOMBER* select_random_bomber();
ENGINE* select_random_engine();
GUNNER* select_gunner();

#endif