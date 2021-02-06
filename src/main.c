#include "keyboard.h"
#include "graphics.h"
#include "mouse.h"
#include "hud.h"

#include <allegro5/allegro.h>
#include <stdio.h>

int main() {
    must_init(al_init(), "allegro");

    init_event_and_queue();
    init_display();
    initialize_keyboard();
    init_mouse();
    init_sprites();

    ALLEGRO_EVENT event;

    bool running = true;
    bool redraw  = false;

    al_start_timer(timer);

    while (running) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
        }

        update_keyboard(&event);
        update_mouse();

        if (key_is_pressed(ALLEGRO_KEY_Q)) {
            running = false;
        }

        if (redraw) {
            draw();
            redraw = false;
        }
    }

    destroy_sprites();
    destroy_display();
    destroy_event_and_queue();

    return 0;
}