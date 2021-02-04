#include <allegro5/allegro.h>
#include "game_state.h"
#include "event.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

void initialize_keyboard();
void update_keyboard(ALLEGRO_EVENT* event);
char key_is_pressed(int key);