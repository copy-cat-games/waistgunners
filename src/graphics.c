/*
    exactly what the file name is
    handles graphics -- both display and sprites
*/

#include "graphics.h"

ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* buffer;

void init_display() {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(display, "display");

    buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    must_init(display, "display");
}

void destroy_display() {
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}

ALLEGRO_BITMAP* get_sprite(int x, int y, int width, int height) {

}