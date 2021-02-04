#include "keyboard.h"

unsigned char keys[ALLEGRO_KEY_MAX];

void initialize_keyboard() {
    must_init(al_install_keyboard(), "keyboard");
    al_register_event_source(queue, al_get_keyboard_event_source());
    memset(keys, 0, sizeof(keys));
}

void update_keyboard(ALLEGRO_EVENT* event) {
    switch (event->type) {
        case ALLEGRO_EVENT_TIMER:
            for (int c = 0; c < ALLEGRO_KEY_MAX; c++) {
                keys[c] &= KEY_SEEN;
            }
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            keys[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            keys[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

char key_is_pressed(int key) {
    return keys[key];
}