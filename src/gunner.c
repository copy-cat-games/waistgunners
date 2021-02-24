#include "gunner.h"


int GUNNER_RELOAD    = DEFAULT_GUNNER_RELOAD;
int GUNNER_MAX_SHOTS = DEFAULT_GUNNER_MAX_SHOTS;

GUNNER create_gunner() {
    VECTOR position = { .x = 0, .y = 0 };
    GUNNER gunner   = { .position = position, .cooldown = 0, .shots = GUNNER_MAX_SHOTS, .reload = 0 };
    return gunner;
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
     */

    if (mouse && gunner->shots > 0 && gunner->cooldown <= 0) {
        // fire
        VECTOR motion = scale(subtract(get_mouse_position(), gunner->position), 1);
        add_bullet(gunner->position, motion, PLAYER_BULLET);
        play_sound(GUNNER_SHOOT);

        // reset cooldown and reload
        gunner->cooldown = GUNNER_COOLDOWN;
        gunner->reload   = GUNNER_RELOAD;
        gunner->shots--;
    } else {
        if (gunner->cooldown) gunner->cooldown--;
        if (gunner->reload) {
            gunner->reload--;
        } else {
            gunner->shots = GUNNER_MAX_SHOTS;
        }
    }
}