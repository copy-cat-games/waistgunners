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

bool update_imposter_engines(ENEMY_IMPOSTER_ENGINE* engines[]) {
    // check for collision, make some smoke...

    bool all_engines_dead = true;
    for (int d = 0; d < ENGINES_PER_IMPOSTER; d++) {
        ENEMY_IMPOSTER_ENGINE* engine = engines[d];
        for (int c = 0; c < MAX_BULLETS; c++) {
            BULLET* bullet = &(bullets[c]);
            if (!bullet->used || bullet->alliance != PLAYER_BULLET) continue;
            if (collision(engine->position, IMPOSTER_ENGINE_SIZE, bullet->position, BULLET_COLLISION_SIZE)) {
                engine->health--;
                if (!engine->dead && engine->health <= 0) {
                    play_sound(ENGINE_DIE);
                    score += ENEMY_IMPOSTER_ENGINE_POINTS;
                }
                engine->dead = engine->health <= 0;
                bullet->used = false;
            }
        }
        if ((int) between(0, IMPOSTER_ENGINE_MAX_HEALTH) > engine->health) {
            VECTOR engine_center = add(engine->position, multiply(IMPOSTER_ENGINE_SIZE, 0.5));
            VECTOR motion        = { .x = 0, .y = 1 };
            add_smoke(engine_center, motion, engine->health < 5);
        }
        all_engines_dead = engine->dead && all_engines_dead;
    }

    return all_engines_dead;
}

void set_gunner_targets(ENEMY_IMPOSTER_GUNNER* gunners[], ENGINE* target) {
    for (int c = 0; c < GUNNERS_PER_IMPOSTER; c++) {
        gunners[c]->target = target;
    }
}

#define IMPOSTER_GUNNER_INACCURACY 0.1 // will need balancing

VECTOR get_imposter_gunner_inaccuracy() {
    VECTOR inaccuracy = {
        .x = between(-IMPOSTER_GUNNER_INACCURACY, IMPOSTER_GUNNER_INACCURACY),
        .y = between(-IMPOSTER_GUNNER_INACCURACY, IMPOSTER_GUNNER_INACCURACY),
    };
    return inaccuracy;
}

void update_imposter_gunners(ENEMY_IMPOSTER_GUNNER* gunners[]) {
    /*
        like the player's gunners
        cooldown and reload count down at the same time
        when a shot is fired, cooldown and reload are reset
        if reload reaches zero, then shots is reset
        if shots reaches zero, the gunner cannot fire

        the gunner can fire only if it has a target, shots > 0, and cooldown >= 0 and if the imposter is in formation
    */

    for (int c = 0; c < GUNNERS_PER_IMPOSTER; c++) {
        ENEMY_IMPOSTER_GUNNER* gunner = gunners[c];

        if (gunner->target != NULL && gunner->shots > 0 && gunner->cooldown <= 0) {
            VECTOR motion = add(get_imposter_gunner_inaccuracy(), scale(subtract(gunner->target->position, gunner->position), 1));
            add_bullet(gunner->position, motion, ENEMY_BULLET);
            play_sound(ENEMY_IMPOSTER_GUNNER_SHOOT);

            gunner->cooldown = IMPOSTER_GUNNER_COOLDOWN;
            gunner->reload   = IMPOSTER_GUNNER_RELOAD;
            gunner->shots--;
        } else {
            if (gunner->cooldown) gunner->cooldown--;
            if (gunner->reload) {
                gunner->reload--;
            } else {
                gunner->shots = IMPOSTER_GUNNER_MAX_SHOTS;
            }
        }
    }
}

void update_enemy_imposter(ENEMY_IMPOSTER_DATA* imposter) {
    if (imposter->down) {
        // slowly drift off the screen
        if (!(frames % 3)) {
            imposter->position.y++;
            for (int c = 0; c < ENGINES_PER_IMPOSTER; c++) {
                imposter->engines[c]->position.y++;
            }
            for (int c = 0; c < GUNNERS_PER_IMPOSTER; c++) {
                imposter->gunners[c]->position.y++;
            }
        }
    } else {
        /*
            enemy imposter: comes from the bottom up, aligns itself with the formation
            the target position is eighty pixels directly below the formation's position
            the imposter moves with the formation

            the imposter first targets a bomber that is not down
            then, it sets its gunners to target the bomber's engine. first the left one, then the right one
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

        if (imposter->target == NULL) {
            imposter->target = select_random_bomber();
        } else {
            if (imposter->target->engines[1]->dead) {
                set_gunner_targets(imposter->gunners, imposter->target->engines[0]);
            } else {
                set_gunner_targets(imposter->gunners, imposter->target->engines[1]);
            }
            if (imposter->position.y + IMPOSTER_SIZE.y < BUFFER_HEIGHT) {
                update_imposter_gunners(imposter->gunners);
            }
            if (imposter->target->down) {
                imposter->target = NULL;
            }
        }
    }

    bool all_engines_dead = update_imposter_engines(imposter->engines);
    if (all_engines_dead && !imposter->down) {
        play_sound(ENEMY_IMPOSTER_DIE);
    }
    imposter->down = all_engines_dead;
}