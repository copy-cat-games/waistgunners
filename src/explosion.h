#include "vector.h"
#include "game_state.h"
#include <allegro5/allegro.h>

#ifndef _EXPLOSION
#define _EXPLOSION

typedef struct EXPLOSION_DATA {
    VECTOR position; // in this case, it's the center
    int lifetime;
    float alpha;
    float radius;
    float max_radius;
    ALLEGRO_COLOR colour;
} EXPLOSION_DATA;

#define MAX_EXPLOSION_LIFETIME 50.0

void update_explosion(EXPLOSION_DATA* explosion);

#endif