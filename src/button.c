#include "button.h"

VECTOR BUTTON_SIZE        = { .x = 100, .y = 25 };
VECTOR BUTTON_TEXT_OFFSET = { .x = 10,  .y = 10 };

ALLEGRO_COLOR BUTTON_COLOUR;
ALLEGRO_COLOR BUTTON_HOVER_COLOUR;
ALLEGRO_COLOR BUTTON_TEXT_COLOUR;

BUTTON menu_buttons[MAX_BUTTONS];

void init_buttons() {
    BUTTON_COLOUR       = al_map_rgb(139, 0, 0);   // x11: "darkred"
    BUTTON_HOVER_COLOUR = al_map_rgb(178, 34, 34); // x11: "firebrick"
    BUTTON_TEXT_COLOUR  = al_map_rgb(255, 215, 0); // x11: "gold"

    reset_buttons();
}

void reset_buttons() {
    for (int c = 0; c < MAX_BUTTONS; c++) {
        BUTTON* b = &menu_buttons[c];
        b->text   = NULL;
        b->action = NULL;
        b->y      = 0;
    }
}

// checks if the mouse is held down over any of the buttons
// if so, then execute the button's action
void update_buttons() {
    for (int c = 0; c < MAX_BUTTONS; c++) {
        BUTTON* b = &menu_buttons[c];
        if (!b->action) continue;
        if (is_hovering(*b) && mouse) {
            b->action();
        }
    }
}

BUTTON create_button(char* button_text, float button_y, void (*action) ()) {
    BUTTON button = {
        .text   = button_text,
        .y      = button_y,
        .action = action,
    };

    return button;
}

// button, button...
// if you decide to press the button, you will receive $50 000. go on, what could happen?

VECTOR get_button_position(BUTTON button) {
    VECTOR position = {
        .x = 50, .y = button.y
    };

    return position;
}

VECTOR get_text_position(BUTTON button) {
    return add(BUTTON_TEXT_OFFSET, get_button_position(button));
}

bool is_hovering(BUTTON button) { // checks if the mouse is overlapping the button
    return collision(get_button_position(button), BUTTON_SIZE, get_mouse_position(), BULLET_COLLISION_SIZE);
}