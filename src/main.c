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
// they are declared in game_state.h

void start_game() {
    if (game_state == MAIN_MENU) game_state = PLAYING;

    reset_buttons();
}

void game_over() {
    game_state = GAME_OVER;
    // we need a button for restarting the game
    menu_buttons[0] = create_button("menu", 205, restart_game);
}

void restart_game() {
    // reset the whole game

    frames = 0;
    score  = 0;
    reset_display_score();

    // clear the enemies, reset the imposter countdown
    // any future enemies with special variables should also reset. i know i'll forget something, dammit
    imposter_countdown = DEFAULT_IMPOSTER_SPAWN_DELAY;
    enemy_imposters    = 0;

    // clear out the enemies
    for (int c = 0; c < MAX_ENEMIES; c++) {
        enemies[c].used = false;
    }

    // reset the bombers
    reset_bombers();

    reset_buttons();
    reset_stored_power_ups();
    main_menu();
}

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

void handle_key_down(int keycode) {
    switch (keycode) {
        case ALLEGRO_KEY_SPACE:
            paused = !paused && (game_state == PLAYING);
            break;
        case ALLEGRO_KEY_R:
            debug = !debug;
            break;
        case ALLEGRO_KEY_1:
            activate_power_up(0);
            break;
        case ALLEGRO_KEY_2:
            activate_power_up(1);
            break;
        case ALLEGRO_KEY_3:
            activate_power_up(2);
            break;
    }
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
        update_keyboard(&event);
        update_mouse();
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                if (!paused) {
                    update_buttons();
                    update_bullets();
                    if (update_bombers()) {
                        game_over();
                    }
                    update_power_ups();
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
                handle_key_down(event.keyboard.keycode);
                break;
        }

        if (key_is_pressed(ALLEGRO_KEY_ESCAPE)) {
            running = false;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
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