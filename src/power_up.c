#include "power_up.h"
#include "graphics.h"
#include "enemy.h"
#include "bomber.h"

POWER_UP stored_power_ups[MAX_STORED_POWER_UPS];

int MAX_POWER_UP_LIFETIMES[POWER_UP_N] = {
    15 * FRAME_RATE,
    15 * FRAME_RATE,
    15 * FRAME_RATE,
    2,
    2,
    10 * FRAME_RATE,
};

// not sure what this is for
char* names[] = {
    "bigger clip size",
    "faster reload",
    "faster bullets",
    "destroy enemies",
    "repair engine",
};

VECTOR POWER_UP_SIZE = { .x = 16, .y = 16 };

int num_common_power_ups         = 5;
POWER_UP_TYPE common_power_ups[] = {
    BIGGER_CLIP_SIZE, FASTER_RELOAD, FASTER_BULLETS, REPAIR_ENGINE, TEMPORARY_INVINCIBILITY
};

int num_rare_power_ups         = 1;
POWER_UP_TYPE rare_power_ups[] = {
    DESTROY_ENEMIES,
};

// not sure if these are needed
void (*power_up_actions[POWER_UP_N])();
void (*power_down_actions[POWER_UP_N])();

void update_power_ups() {
    // also handles generating powerups
    // they are generated at certain intervals
    // and randomly

    // updates the stored powerups
    for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
        POWER_UP* p = &stored_power_ups[c];
        if (!p->lifetime) continue;
        if (p->activated) p->lifetime--;
        if (p->activated && p->lifetime > 0) {
            // do whatever action
            switch (p->type) {
                case DESTROY_ENEMIES:
                    kill_all_enemies();
                    break;
                case REPAIR_ENGINE:
                    // choose an engine to repair
                    ;
                    // ENGINE* engine;
                    // bool repaired = false;
                    // for (int c = 0; c < MAX_BOMBERS && !repaired; c++) {
                    //     BOMBER* bomber = &(bombers[c]);
                    //     for (int d = 0; d < ENGINES_PER_BOMBER && !repaired; d++) {
                    //         ENGINE* engine = bomber->engines[d];
                    //         if (engine->health >= ENGINE_MAX_HEALTH) continue;
                    //         engine->health = ENGINE_MAX_HEALTH;
                    //         engine->dead   = false;
                    //         // printf("repaired engine %i of bomber %i\n", d, c);
                    //         repaired = true;
                    //     }
                    // }

                    ENGINE* engine = select_damaged_engine();
                    if (engine == NULL) break;
                    engine->health = ENGINE_MAX_HEALTH;
                    engine->dead   = false;
                    break;
                default:
                    // do nothing
                    break;
            }
        }
    }
}

// not sure if this is needed
void activate_power_up(int index) {
    if (index < 0 || index >= MAX_STORED_POWER_UPS) return;
    if (stored_power_ups[index].lifetime > 0) stored_power_ups[index].activated = true;
}

// counts how many powerups of a current type are activated
int power_up_activated(POWER_UP_TYPE type) {
    int activated_count = 0;
    for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
        POWER_UP power_up = stored_power_ups[c];
        if (power_up.type == type && power_up.activated && power_up.lifetime) {
            activated_count++;
        }
    }

    return activated_count;
}

POWER_UP create_power_up(VECTOR position, POWER_UP_TYPE type) {
    POWER_UP p = {
        .position  = position,
        .type      = type,
        .lifetime  = MAX_POWER_UP_LIFETIMES[type],
        .activated = false
    };
    return p;
}

VECTOR no_position = { .x = 0, .y = 0 };

// to generate a random powerup, simply pass NONE or -1
void add_power_up(POWER_UP_TYPE type) {
    for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
        if (stored_power_ups[c].lifetime) continue;
        int power_up_type   = type > -1 ? type : (int) floor(between(0, POWER_UP_N));
        stored_power_ups[c] = create_power_up(no_position, power_up_type);
        play_sound(POWERUP_PICKUP);
        return;
    }
}

void add_common_power_up() {
    for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
        if (stored_power_ups[c].lifetime) continue;
        int power_up_type   = rand() % num_common_power_ups;
        stored_power_ups[c] = create_power_up(no_position, common_power_ups[power_up_type]);
        play_sound(POWERUP_PICKUP);
        return;
    }
}

void add_rare_power_up() {
    for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
        if (stored_power_ups[c].lifetime) continue;
        int power_up_type   = rand() % num_rare_power_ups;
        stored_power_ups[c] = create_power_up(no_position, rare_power_ups[power_up_type]);
        play_sound(POWERUP_PICKUP);
        return;
    }
}

void reset_stored_power_ups() {
    for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
        stored_power_ups[c].lifetime = 0;
    }
}