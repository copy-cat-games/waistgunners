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
    reset_bombers();

    bool running = true;
    bool redraw  = false;

    frames = 0;
    score  = 0;

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

        update_bombers();
        update_hud();
        update_keyboard(&event);
        update_mouse();

        if (key_is_pressed(ALLEGRO_KEY_Q) || key_is_pressed(ALLEGRO_KEY_ESCAPE)) {
            running = false;
        }

        if (redraw) {
            draw();
            redraw = false;
            frames++;
        }

        // test
        if (frames % 50 == 0) {
            score += 100;
        }
    }

    destroy_sprites();
    destroy_display();
    destroy_event_and_queue();

    return 0;
}