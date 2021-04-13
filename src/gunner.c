#include "gunner.h"

int GUNNER_RELOAD    = DEFAULT_GUNNER_RELOAD;
int GUNNER_MAX_SHOTS = DEFAULT_GUNNER_MAX_SHOTS;

int gunner_reload   = 0;
int gunner_cooldown = 0;
int gunner_shots    = 0;

// donald trump's favourite word: "fired"
bool fired = false;

GUNNER create_gunner() {
    VECTOR position = { .x = 0, .y = 0 };
    GUNNER gunner   = { .position = position };
    return gunner;
}

void update_gunner_data() {
    if (fired) {
        gunner_cooldown = GUNNER_COOLDOWN;
        gunner_reload   = GUNNER_RELOAD;
        gunner_shots--;
        play_sound(GUNNER_SHOOT);
    } else {
        if (gunner_cooldown) gunner_cooldown--;
        if (gunner_reload) {
            gunner_reload--;
        } else {
            gunner_shots = GUNNER_MAX_SHOTS;
        }
    }

    fired = false;
}

bool can_shoot() {
    return (mouse && gunner_shots > 0 && gunner_cooldown <= 0);
}

void update_gunner(GUNNER* gunner) {
    /*
     * how the gunner works:
     * after the last shot, cooldown and reload count down at the same time
     * if a shot is fired, cooldown and reload are reset
     * if reload reaches zero, then shots is reset
     * if shots reaches zero, then the gunner cannot fire until reload finishes counting down
     *
     * then, the gunner can fire if shots > 0 and cooldown == 0. and if the mouse is held down, of course
     *
     * all of this is held inside update_gunner_data()
     */

    if (can_shoot()) {
        VECTOR motion = scale(subtract(get_mouse_position(), gunner->position), 1);
        add_bullet(gunner->position, motion, PLAYER_BULLET);
        fired = true;
    }
}