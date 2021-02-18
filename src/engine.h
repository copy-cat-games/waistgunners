#include "vector.h"
#include "game_state.h"

#ifndef _ENGINE
#define _ENGINE

typedef struct ENGINE {
    VECTOR position;
    int health;
    bool dead;
} ENGINE;

#endif

#define ENGINE_MAX_HEALTH 12

extern VECTOR ENGINE_SIZE;

ENGINE create_engine();
void update_engine(ENGINE* engine);