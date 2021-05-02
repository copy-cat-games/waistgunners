#include "enemy_jet.h"
#include "graphics.h"

VECTOR JET_SIZE = { .x = 30, .y = 35 };

TRIANGLE get_triangle(ENEMY_JET_DATA* jet) {
    if (jet->direction == DOWN) {
        VECTOR points[] = {
            jet->position,
            { .x = jet->position.x + JET_SIZE.x, .y = jet->position.y },
            { .x = jet->position.x + JET_SIZE.x / 2, .y = jet->position.y + JET_SIZE.y },
        };
        return create_triangle(points);
    } else {
        VECTOR points[] = {
            { .x = jet->position.x, .y = jet->position.y + JET_SIZE.y },
            { .x = jet->position.x + JET_SIZE.x / 2, .y = jet->position.y },
            add(jet->position, JET_SIZE)
        };
        return create_triangle(points);
    }
}

void update_enemy_jet(ENEMY_JET_DATA* jet) {
    /*
        the plan:
        - the jet goes from top down, firing one missile
        - the jet then goes from buttom to top, without firing
        repeat
    */
    jet->position = add(jet->position, jet->motion);

    if (jet->position.y > BUFFER_HEIGHT) {
        jet->direction = UP;
        jet->motion.y  = -ENEMY_JET_Y_SPEED;
    }

    if (jet->position.y < -JET_SIZE.y) {
        jet->direction = DOWN;
        jet->motion.y  = ENEMY_JET_Y_SPEED;
    }

    if (jet->position.x < -JET_SIZE.x || jet->position.x > BUFFER_WIDTH) {
        jet->motion.x *= -1;
    }

    TRIANGLE jet_triangle = get_triangle(jet);

    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &bullets[c];
        if (b->alliance != PLAYER_BULLET || !b->used) continue;
        // the jet requires a different collision detection method
        // we're going to approximate the jet as a triangle, and the bullet as a point
        if (triangle_collision(jet_triangle, b->position)) {
            b->used = false;
            jet->health--;
        }
    }

    if (frames % 2 == 0 || frames % 3 == 0) {
        VECTOR jet_center = add(jet->position, multiply(JET_SIZE, 0.5));
        VECTOR motion     = { .x = 0, .y = -jet->motion.y };
        add_smoke(jet_center, motion, jet->health < 0);
    }
}

ENEMY_JET_DATA create_enemy_jet() {
    /*
        to create an enemy jet:
        - choose a random x
        - set direction to DOWN, fired to false, health to maximum health
    */

    float x = between(BUFFER_WIDTH / 4, BUFFER_WIDTH / 4 * 3);

    ENEMY_JET_DATA jet = {
        .position = {
            .x    = x,
            .y    = -JET_SIZE.y,
        },
        .motion = {
            .x  = x < (BUFFER_WIDTH / 2) ? ENEMY_JET_X_SPEED : -ENEMY_JET_X_SPEED,
            .y  = ENEMY_JET_Y_SPEED,
        },
        .health    = MAX_ENEMY_JET_HEALTH,
        .fired     = false,
        .direction = DOWN,
    };

    return jet;
}