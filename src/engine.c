#include "engine.h"

ENGINE create_engine() {
    VECTOR position = { .x = 0, .y = 0 };
    ENGINE engine   = { .position = position, .health = ENGINE_MAX_HEALTH, .dead = false };

    return engine;
}

void update_engine(ENGINE* engine) {
    // add the collision detection later
}