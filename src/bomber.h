#include "vector.h"
#include "engine.h"
#include "keyboard.h"

#ifndef _BOMBER
#define _BOMBER

#define ENGINES_PER_BOMBER 2
#define GUNNERS_PER_BOMBER 2
#define BOMBER_SPEED 1

typedef struct BOMBER {
    VECTOR position; // top left corner
    ENGINE* engines[ENGINES_PER_BOMBER];
    // i'll add the gunners later
    bool down;
} BOMBER;

#define MAX_BOMBERS 4
#define BOMBER_MARGIN 10

extern VECTOR formation;
extern VECTOR BOMBER_SIZE;
extern BOMBER bombers[MAX_BOMBERS + 1];

void update_bombers();
void reset_bombers();

#endif