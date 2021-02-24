/*
    to convert:
    $ for i in *.wav; do ffmpeg -i "$i" "$(basename "$i" .wav)".flac ; done
*/
#include "sounds.h"

ALLEGRO_SAMPLE* sounds[NUMBER_OF_SOUNDS];

char* sound_names[NUMBER_OF_SOUNDS] = {
    "enemy_fighter_die.flac",
    "enemy_fighter_shoot.flac",
    "enemy_imposter_die.flac",
    "engine_dies.flac",
    "gunner_shoot.flac",
    "imposter_gunner_shoot.flac",
    "powerup_pickup.flac"
};

void init_sounds() {
    must_init(al_install_audio(), "sounds");
    must_init(al_init_acodec_addon(), "acodec addon");

    al_reserve_samples(NUMBER_OF_SOUNDS);

    for (int c = 0; c < NUMBER_OF_SOUNDS; c++) {
        sounds[c] = al_load_sample(sound_names[c]);
        must_init(sounds[c], sound_names[c]);
    }
}

void destroy_sounds() {
    for (int c = 0; c < NUMBER_OF_SOUNDS; c++) {
        al_destroy_sample(sounds[c]);
    }
}

void play_sound(int sound) {
    al_play_sample(sounds[sound], 0.3, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}