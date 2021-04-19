#include "keyboard.h"
#include "sounds.h"
#include "graphics.h"
#include "mouse.h"
#include "hud.h"

#include <allegro5/allegro.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// these functions will be used by the buttons

void start_game() {
    if (game_state == MAIN_MENU) game_state = PLAYING;

    reset_buttons();
}

void restart_game() {
    // reset the whole game
}

void show_credits();
void quit();

void main_menu() {
    switch (game_state) {
        case GAME_OVER:
        case CREDITS:
            game_state = MAIN_MENU;
            menu_buttons[0] = create_button("play", 75, start_game);
            menu_buttons[1] = create_button("credits", 110, show_credits);
            menu_buttons[2] = create_button("quit", 145, quit);
        // do nothing otherwise
    }
}

void show_credits() {
    if (game_state == MAIN_MENU) {
        game_state = CREDITS;
        reset_buttons();
        menu_buttons[0] = create_button("back", 10, main_menu);
    }
}

bool running = true;

void quit() {
    running = false;
}

int main() {
    // seed the rng
    time_t t;
    srand((unsigned) time(&t));

    must_init(al_init(), "allegro");

    init_event_and_queue();
    init_display();
    initialize_keyboard();
    init_mouse();
    init_sprites();
    init_sounds();
    init_buttons();

    ALLEGRO_EVENT event;
    reset_bombers();

    bool redraw  = false;

    debug = false;

    time_t now          = time(NULL);
    struct tm* time_now = localtime(&now);
    night               = (time_now->tm_hour > 21) || (time_now->tm_hour < 6);

    frames = 0;
    score  = 0;

    al_start_timer(timer);

    game_state = GAME_OVER;
    main_menu();

    while (running) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                if (!paused) {
                    update_buttons();
                    update_bullets();
                    update_bombers();
                    update_enemies();
                    update_particles();
                    update_clouds();
                    frames++;
                }
                update_hud();
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    paused = !paused && (game_state == PLAYING);
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                    debug = !debug;
                }
        }
        update_keyboard(&event);
        update_mouse();

        if (key_is_pressed(ALLEGRO_KEY_ESCAPE)) {
            running = false;
        }

        if (redraw) {
            draw();
            redraw = false;
        }
    }

    destroy_sprites();
    destroy_sounds();
    destroy_display();
    destroy_event_and_queue();

    return 0;
}