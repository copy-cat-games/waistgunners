#include "mouse.h"
#include "graphics.h"

unsigned int mouse = 0;
int mouse_x; int mouse_y;
ALLEGRO_MOUSE_STATE mouse_state;

void init_mouse() {
    must_init(al_install_mouse(), "mouse");
    mouse = mouse_x = mouse_y = 0;
    al_hide_mouse_cursor(display);
}

void update_mouse() {
    al_get_mouse_state(&mouse_state);
    mouse_x = mouse_state.x / DISPLAY_SCALE;
    mouse_y = mouse_state.y / DISPLAY_SCALE;
    mouse   = al_mouse_button_down(&mouse_state, 1);
}

VECTOR get_mouse_position() {
    VECTOR mouse_position = { .x = mouse_x, .y = mouse_y };
    return mouse_position;
}