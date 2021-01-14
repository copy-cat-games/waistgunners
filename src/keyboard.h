#include <allegro5/allegro.h>

#define KEY_SEEN     1
#define KEY_RELEASED 2

void initialize_keyboard(ALLEGRO_EVENT_QUEUE* queue);
void update_keyboard(ALLEGRO_EVENT* event);
char key_is_pressed(int key);