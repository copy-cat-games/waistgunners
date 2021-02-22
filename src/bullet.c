#include "bullet.h"
#include "graphics.h"

BULLET bullets[MAX_BULLETS];
VECTOR BULLET_SIZE           = { .x = 4, .y = 4 };
VECTOR BULLET_COLLISION_SIZE = { .x = 1, .y = 1 };

void initialize_bullets() {
    for (int c = 0; c < MAX_BULLETS; c++) {
        VECTOR position = { .x = 0, .y = 0 };
        VECTOR motion   = { .x = 0, .y = 0 };

        BULLET b = {
            .position = position,
            .motion   = motion,
            .alliance = PLAYER_BULLET,
            .used     = false
        };

        bullets[c] = b;
    }
}

void add_bullet(VECTOR position, VECTOR motion, ALLIANCE alliance) {
    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &(bullets[c]);
        if (b->used) continue;
        b->position = position;
        b->motion   = motion;
        b->alliance = alliance;
        b->used     = true;
        return;
    }
}

void destroy_bullet(BULLET* bullet) {
    bullet->used = false;
}

void update_bullets() {
    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &(bullets[c]);
        if (!b->used) continue;
        b->position = add(b->position, multiply(b->motion, BULLET_SPEED));
        if (
            b->position.x < 0 || b->position.y < 0 ||
            b->position.x > BUFFER_WIDTH || b->position.y > BUFFER_HEIGHT
        ) {
            b->used = false;
        }
    }
}