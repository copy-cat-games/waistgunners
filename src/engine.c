#include "engine.h"
#include "bullet.h"

VECTOR ENGINE_SIZE = { .x = 7, .y = 18 };

ENGINE create_engine() {
    VECTOR position = { .x = 0, .y = 0 };
    ENGINE engine   = {
        .position   = position,
        .health     = ENGINE_MAX_HEALTH,
        .dead       = false
    };

    return engine;
}

void update_engine(ENGINE* engine) {
    // add the collision detection later
    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* bullet = &bullets[c];
        if (!bullet->used || bullet->alliance == PLAYER_BULLET) continue;
        if (collision(engine->position, ENGINE_SIZE, bullet->position, BULLET_COLLISION_SIZE)) {
            engine->health--;
            engine->dead = engine->health <= 0;
            bullet->used = false;
        }
    }
}