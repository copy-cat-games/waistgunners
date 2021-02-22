#include "vector.h"
#include "game_state.h"

#ifndef _SMOKE
#define _SMOKE

typedef struct SMOKE_DATA {
    VECTOR position;
    VECTOR motion;
    int lifetime;
    int size;
    bool thick; // thicc?
} SMOKE_DATA;

#define MAX_SMOKE_LIFETIME 50
#define MAX_SMOKE_RADIUS   10

void update_smoke(SMOKE_DATA* smoke);

#endif