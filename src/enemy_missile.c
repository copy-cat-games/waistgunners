#include "enemy_missile.h"
#include "graphics.h"

VECTOR MISSILE_SIZE        = { .x = 9, .y = 20 };
VECTOR MISSILE_FIRE_SIZE   = { .x = 9, .y = 7 };
VECTOR MISSILE_FIRE_OFFSET = { .x = 0, .y = -5 };

#define ENEMY_MISSILE_TOLERANCE 5

void update_enemy_missile(ENEMY_MISSILE_DATA* missile) {
    missile->position = add(missile->position, missile->motion);

    // much the same as the fighter's code
    if (missile->target) {
        float current_x  = missile->position.x + MISSILE_SIZE.x / 2;
        float target_x   = missile->target->position.x + ENGINE_SIZE.x / 2;
        float difference = target_x - current_x;
        if (fabs(difference) < ENEMY_MISSILE_TOLERANCE) {
            missile->motion.x = 0;
        } else if (difference > ENEMY_MISSILE_TOLERANCE) {
            missile->motion.x = ENEMY_MISSILE_X_SPEED;
        } else if (difference < -ENEMY_MISSILE_TOLERANCE) {
            missile->motion.x = -ENEMY_MISSILE_X_SPEED;
        }
    }
    // if the missile has no target, then just keep going
    // it's not like there's a pilot in the missile choosing a new target, is there?

    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &(bullets[c]);
        if (!b->used || b->alliance != PLAYER_BULLET) continue;
        if (collision(b->position, BULLET_COLLISION_SIZE, missile->position, MISSILE_SIZE)) {
            b->used = false;
            missile->health--;
            if (!missile->health) {
                score += ENEMY_MISSILE_POINTS;
                play_sound(ENGINE_DIE);
            }
        }
    }

    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* bomber = &bombers[c];
        if (bomber->down) continue;
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            ENGINE* engine = bomber->engines[d];
            // yanderedev style if statement nesting. fantastic.
            if (collision(missile->position, MISSILE_SIZE, engine->position, ENGINE_SIZE)) {
                if (!power_up_activated(TEMPORARY_INVINCIBILITY)) {
                    engine->health  = 0;
                    engine->dead    = true;
                }
                missile->health = 0;
            }
        }
    }

    if (missile->position.x < -MISSILE_SIZE.x || missile->position.x > BUFFER_WIDTH || missile->position.y > BUFFER_HEIGHT) {
        missile->health = 0;
    }
}

ENEMY_MISSILE_DATA create_missile(VECTOR pos) {
    ENGINE* target   = NULL;
    float shortest_x = 100000;
    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* bomber = &bombers[c];
        if (bomber->down) continue;
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            ENGINE* engine   = bomber->engines[d];
            float x_distance = fabs(engine->position.x - pos.x);
            if (x_distance < shortest_x) {
                shortest_x = x_distance;
                target     = engine;
            }
        }
    }

    ENEMY_MISSILE_DATA missile = {
        .position = pos,
        .motion   = {
            .x    = 0,
            .y    = ENEMY_MISSILE_Y_SPEED
        },
        .target = target,
        .health = ENEMY_MISSILE_MAX_HEALTH,
    };

    return missile;
}

void kill_enemy_missile(ENEMY_MISSILE_DATA* missile) {
    missile->health = 0;
}