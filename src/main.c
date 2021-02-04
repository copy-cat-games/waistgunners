#include "keyboard.h"
#include "graphics.h"

#include <allegro5/allegro.h>
#include <stdio.h>

int main() {
    must_init(al_init(), "allegro");

    init_display();
    init_event_and_queue();
    initialize_keyboard();

    ALLEGRO_EVENT event;

    bool running = true;

    while (running) {
        al_wait_for_event(queue, &event);

        update_keyboard(&event);

        if (key_is_pressed(ALLEGRO_KEY_Q)) {
            running = false;
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
    }

    return 0;
}