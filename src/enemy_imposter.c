#include "enemy_imposter.h"
#include "graphics.h"

VECTOR IMPOSTER_SIZE        = { .x = 66, .y = 60 };
VECTOR IMPOSTER_ENGINE_SIZE = { .x = 8,  .y = 20 };

void update_enemy_imposter(ENEMY_IMPOSTER_DATA* imposter) {
    if (imposter->down) {
        // slowly drift off the screen
    } else {
        /*
            enemy imposter: comes from the bottom up, aligns itself with the formation
            the target position is forty pixels directly below the formation's position
            the imposter moves with the formation

            the imposter first targets a bomber that is not down
        */
    }
}