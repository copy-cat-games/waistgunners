#include "enemy_fighter.h"
#include "graphics.h"
#include "particle.h"

VECTOR FIGHTER_SIZE          = { .x = 32, .y = 32 };
const VECTOR FIGHTER_GUNS[2] = {
    { .x = 5,  .y = 25 },
    { .x = 24, .y = 25 }
};

VECTOR FIGHTER_COLLISION_POSITION = { .x = 11, .y = 0 };
VECTOR FIGHTER_COLLISION_SIZE     = { .x = 10, .y = 31 };

void update_enemy_fighter(ENEMY_FIGHTER_DATA* fighter) {
    if (fighter->dead) {
        fighter->position.y += MAX_ENEMY_FIGHTER_SPEED * 2;

        fighter->side = (fighter->position.x + FIGHTER_SIZE.x / 2) > BUFFER_WIDTH / 2;
        if (fighter->side) {
            // right side
            fighter->position.x += MAX_ENEMY_FIGHTER_SPEED;
            fighter->angle       = fmaxf(fighter->angle - ENEMY_FIGHTER_LEAN,-ENEMY_FIGHTER_MAX_ANGLE);
        } else {
            // left side
            fighter->position.x -= MAX_ENEMY_FIGHTER_SPEED;
            fighter->angle       = fminf(fighter->angle + ENEMY_FIGHTER_LEAN, ENEMY_FIGHTER_MAX_ANGLE);
        }
    } else {
        /*
            enemy fighter: comes top down, aligns itself with your engines, shoots
            - if the fighter is not 1/3 of the way down the screen, then move down
            - if the target is NULL, select a new target
            - if there is a target, then
                - if the target's x is greater than the fighter, move right by fighter speed
                - if the target's x is smaller than the fighter, move left by fighter speed
                - if the target's x is within the threshold, then shoot
                - if the target is dead, then set target to NULL
        */
        fighter->position.y += MAX_ENEMY_FIGHTER_SPEED * (fighter->position.y <= BUFFER_HEIGHT / 4);
        if (fighter->target == NULL) {
            fighter->target = select_random_engine();
        } else {
            float current_x = fighter->position.x + (FIGHTER_SIZE.x / 2);
            float target_x  = fighter->target->position.x + (ENGINE_SIZE.x / 2);
            if (abs(target_x - current_x) <= ENEMY_FIGHTER_THRESHOLD) {
                if (fighter->cooldown == 0) {
                    fighter->cooldown   = ENEMY_FIGHTER_COOLDOWN;
                    VECTOR gun_position = add(fighter->position, FIGHTER_GUNS[fighter->gun]);
                    VECTOR motion       = scale(subtract(fighter->target->position, gun_position), 1);

                    add_bullet(gun_position, motion, ENEMY_BULLET);
                    fighter->gun = 1 - fighter->gun;
                }
            } else if (target_x > current_x) {
                fighter->position.x += MAX_ENEMY_FIGHTER_SPEED;
            } else if (target_x < current_x) {
                fighter->position.x -= MAX_ENEMY_FIGHTER_SPEED;
            }

            if (fighter->target->dead) {
                fighter->target = NULL;
            }
        }
    }

    // update the cooldown
    fighter->cooldown -= (fighter->cooldown > 0);

    // check for collision
    VECTOR collision_position = add(fighter->position, FIGHTER_COLLISION_POSITION);
    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &bullets[c];
        if (b->alliance != PLAYER_BULLET || !b->used) continue;
        if (collision(b->position, BULLET_COLLISION_SIZE, collision_position, FIGHTER_COLLISION_SIZE)) {
            b->used = false;
            fighter->health--;
        }
    }

    if (!fighter->dead && fighter->health <= 0) {
        score += ENEMY_FIGHTER_POINTS;
    }

    fighter->dead = fighter->health <= 0;

    if (fighter->health < 10 && frames % 3 == 0) {
        VECTOR fighter_center = add(fighter->position, multiply(FIGHTER_SIZE, 0.5));
        VECTOR motion         = { .x = 0, .y = -1 };
        add_smoke(fighter_center, motion, fighter->health <= 4);
    }
}