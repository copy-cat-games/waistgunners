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

void update_smoke(SMOKE_DATA* smoke);

#endif