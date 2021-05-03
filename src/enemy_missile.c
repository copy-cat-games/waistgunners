#include "enemy_missile.h"
#include "graphics.h"

VECTOR MISSILE_SIZE        = { .x = 9, .y = 20 };
VECTOR MISSILE_FIRE_SIZE   = { .x = 9, .y = 7 };
VECTOR MISSILE_FIRE_OFFSET = { .x = 0, .y = -5 };

void update_enemy_missile(ENEMY_MISSILE_DATA* missile) {
    missile->position = add(missile->position, missile->motion);
    if (missile->position.x < -MISSILE_SIZE.x || missile->position.x > BUFFER_WIDTH || missile->position.y > BUFFER_HEIGHT) {
        missile->health = 0;
    }
}

ENEMY_MISSILE_DATA create_missile(VECTOR pos, int x_direction) {
    ENGINE* target   = NULL;
    float shortest_x = 100000;
    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* bomber = &bombers[c];
        if (bomber->down) continue;
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            ENGINE* engine   = bomber->engines[d];
            float x_distance = fabs(engine->position.x - pos.x);
            if (x_distance < shortest_x && (sign(engine->position.x - pos.x) == x_direction)) {
                shortest_x = x_distance;
                target     = engine;
            }
        }
    }

    ENEMY_MISSILE_DATA missile = {
        .position = pos,
        .motion   = {
            .x    = x_direction * ENEMY_MISSILE_X_SPEED,
            .y    = ENEMY_MISSILE_Y_SPEED
        },
        .target = target,
        .health = ENEMY_MISSILE_MAX_HEALTH,
    };

    return missile;
}