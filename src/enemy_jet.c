#include "enemy_jet.h"
#include "graphics.h"

VECTOR JET_SIZE = { .x = 30, .y = 35 };

// void get_triangle(ENEMY_JET_DATA* jet, VECTOR vertices[]) {
//     if (jet->direction == DOWN) {
//         VECTOR points[] = {
//             jet->position,
//             { .x = jet->position.x + JET_SIZE.x, .y = jet->position.y },
//             { .x = jet->position.x + JET_SIZE.x / 2, .y = jet->position.y + JET_SIZE.y },
//         };
//         vertices = points;
//     } else {
//         VECTOR points[] = {

//         };
//     }
// }

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

    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &bullets[c];
        if (b->alliance != PLAYER_BULLET || !b->used) continue;
        // the jet requires a different collision detection method
        // we're going to approximate the jet as a triangle, and the bullet as a point

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