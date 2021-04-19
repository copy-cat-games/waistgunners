#include "game_state.h"
#include "vector.h"
#include "mouse.h"
#include "bullet.h"

#include <allegro5/allegro.h>

#ifndef _BUTTON
#define _BUTTON

extern VECTOR BUTTON_SIZE;
extern VECTOR BUTTON_TEXT_OFFSET;

extern ALLEGRO_COLOR BUTTON_COLOUR;
extern ALLEGRO_COLOR BUTTON_HOVER_COLOUR;
extern ALLEGRO_COLOR BUTTON_TEXT_COLOUR;

#define BUTTON_CORNER_RADIUS 3
#define MAX_BUTTONS 3

typedef struct BUTTON {
    char* text;
    float y;
    void (*action) ();
} BUTTON;

void init_buttons();
void update_buttons();
void reset_buttons();

BUTTON create_button(char* button_text, float button_y, void (*action) ());

// useful for drawing as well, that's why i'm putting them here
VECTOR get_button_position(BUTTON button);
VECTOR get_text_position(BUTTON button);
bool is_hovering(BUTTON button);

extern BUTTON menu_buttons[MAX_BUTTONS];

#endif
