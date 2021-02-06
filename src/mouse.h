#include <allegro5/allegro.h>
#include "vector.h"
#include "game_state.h"

extern unsigned int mouse;
extern int mouse_x;
extern int mouse_y;
extern ALLEGRO_MOUSE_STATE mouse_state;

void init_mouse();
void update_mouse();