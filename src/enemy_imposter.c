#include "enemy_imposter.h"
#include "graphics.h"

VECTOR IMPOSTER_SIZE        = { .x = 66, .y = 60 };
VECTOR IMPOSTER_ENGINE_SIZE = { .x = 8,  .y = 20 };
VECTOR FORMATION_OFFSET     = { .x = 0,  .y = 80 };

VECTOR IMPOSTER_ENGINES[2] = {
    { .x = 16, .y = 12 },
    { .x = 42, .y = 12 },
};

VECTOR IMPOSTER_GUNNERS[2] = {
    { .x = 24, .y = 36 },
    { .x = 40, .y = 36 },
};

float sign(float number) {
    if (number) return (number / fabs(number));
    return 0;
}

void update_enemy_imposter(ENEMY_IMPOSTER_DATA* imposter) {
    if (imposter->down) {
        // slowly drift off the screen
    } else {
        /*
            enemy imposter: comes from the bottom up, aligns itself with the formation
            the target position is eighty pixels directly below the formation's position
            the imposter moves with the formation

            the imposter first targets a bomber that is not down
        */
        VECTOR target_position = add(formation, FORMATION_OFFSET);

        if (frames % 2) {
            // move
            VECTOR motion = {
                .x = sign(target_position.x - imposter->position.x),
                .y = sign(target_position.y - imposter->position.y)
            };
            imposter->position = add(imposter->position, motion);
            for (int c = 0; c < ENGINES_PER_IMPOSTER; c++) {
                imposter->engines[c]->position = add(imposter->engines[c]->position, motion);
            }
            for (int c = 0; c < GUNNERS_PER_IMPOSTER; c++) {
                imposter->gunners[c]->position = add(imposter->gunners[c]->position, motion);
            }
        }
    }
}