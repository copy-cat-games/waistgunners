#include "enemy.h"
#include "graphics.h"

ENEMY enemies[MAX_ENEMIES];

VECTOR get_enemy_position(ENEMY* enemy) {
    VECTOR v = { .x = 3000, .y = 3000 };
    // if you see these values in a print statement somewhere, something is wrong!

    switch (enemy->type) {
        case ENEMY_FIGHTER:
            // access the enemy fighter
            // no need for a break statement
            return enemy->data.fighter.position;
        default:
            return v;
    }
}

void add_enemy(ENEMY_DATA data, ENEMY_TYPE type) {
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* e = &enemies[c];
        if (e->used) continue;
        e->data = data;
        e->type = type;
        e->used = true;
        return;
    }
}

void add_enemy_fighter() {
    float x = between(0, BUFFER_WIDTH);
    float y = 0; // for now

    VECTOR position                 = { .x = x, .y = -FIGHTER_SIZE.y };
    ENEMY_FIGHTER_DATA fighter_data = {
        .position = position,
        .target   = NULL,
        .health   = MAX_ENEMY_FIGHTER_HEALTH,
        .gun      = 0,
        .cooldown = 0,
        .dead     = false,
        .side     = 0,
        .angle    = 0,
    };

    ENEMY_DATA data = { .fighter = fighter_data };

    add_enemy(data, ENEMY_FIGHTER);
}

int enemy_imposters = 0; // only one imposter can be on the screen at a time

void add_enemy_imposter() {

}

void update_enemies() {
    // also handles spawning of enemies, as well
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* e = &enemies[c];
        if (!e->used) continue;
        switch (e->type) {
            case ENEMY_FIGHTER:
                ;
                ENEMY_FIGHTER_DATA* fighter = &(e->data.fighter);
                update_enemy_fighter(fighter);
                if (fighter->dead && (
                    fighter->position.x < -FIGHTER_SIZE.x ||
                    fighter->position.x > BUFFER_WIDTH
                )) {
                    e->used = false;
                }
                break;
        }
    }

    if (frames % 90 == 0 && rand() % 2 == 0) {
        add_enemy_fighter();
    }
}