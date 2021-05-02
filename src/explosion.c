#include "explosion.h"

void update_explosion(EXPLOSION_DATA* explosion) {
    explosion->lifetime++;
    explosion->alpha  = 1 - ((float) explosion->lifetime / MAX_EXPLOSION_LIFETIME);
    explosion->radius = explosion->max_radius * explosion->lifetime / MAX_EXPLOSION_LIFETIME;

    char r; char g; char b;
    al_unmap_rgb(explosion->colour, &r, &g, &b);
    explosion->colour = al_map_rgba(r, g, b, explosion->alpha);
}