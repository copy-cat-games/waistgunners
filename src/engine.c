#include "engine.h"

VECTOR ENGINE_SIZE = { .x = 7, .y = 18 };

ENGINE create_engine() {
    VECTOR position = { .x = 0, .y = 0 };
    ENGINE engine   = { .position = position, .health = ENGINE_MAX_HEALTH, .dead = false };

    return engine;
}

void update_engine(ENGINE* engine) {
    // add the collision detection later
}