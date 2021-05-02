#include "game_state.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#ifndef _SOUNDS
#define _SOUNDS

// include more sounds here as they are added
// make sure they correspond with sound_names!

typedef enum SOUNDS {
    ENEMY_FIGHTER_DIE = 0,
    ENEMY_FIGHTER_SHOOT,
    ENEMY_IMPOSTER_DIE,
    ENEMY_IMPOSTER_GUNNER_SHOOT,
    ENEMY_JET_PASSING,
    ENEMY_JET_DIE,
    ENGINE_DIE,
    GUNNER_SHOOT,
    POWERUP_PICKUP,
    NUMBER_OF_SOUNDS,
} SOUNDS;

#endif

void init_sounds();
void destroy_sounds();

void play_sound(int sound);