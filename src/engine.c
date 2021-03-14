#include "engine.h"
#include "bullet.h"
#include "particle.h"

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
    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* bullet = &bullets[c];
        if (!bullet->used || bullet->alliance == PLAYER_BULLET) continue;
        if (collision(engine->position, ENGINE_SIZE, bullet->position, BULLET_COLLISION_SIZE)) {
            // hidden mechanism to help the player
            // lets them go on for longer than they would otherwise, give them a fighting chance
            // not sure how this will turn out
            if (engine->health > 2 || rand() % 2 == 0) {
                engine->health--;
            }
            if (!engine->dead && engine->health <= 0) {
                play_sound(ENGINE_DIE);
            }
            engine->dead = engine->health <= 0;
            bullet->used = false;
        }
    }

    if ((int) between(0, ENGINE_MAX_HEALTH) > engine->health) {
        VECTOR engine_center = add(engine->position, multiply(ENGINE_SIZE, 0.5));
        VECTOR motion        = { .x = 0, .y = 1 };
        add_smoke(engine_center, motion, engine->health < 5);
    }
}