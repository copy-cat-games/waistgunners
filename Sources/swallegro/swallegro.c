#include <stdio.h>
#include "include/swallegro.h"
#include <allegro5/allegro.h>

/*
    what we do here is all of the leg work
    handling enemies, game content, etc are the jobs of the Swift code
*/

void must_init(bool test, char* description) {
    if (test) return;
    printf("failed to initialize %s.\n", description);
    exit(1);
}

void crash(const int exit_code, const char* msg) {
    printf("something went wrong: %s (exit code %i)\n", msg, exit_code);
    exit(exit_code);
}

void initialize_allegro() {
    al_init();
}

/*
    display stuff
    the draw buffer and the window
*/

ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* buffer;
int display_scale;
int display_width;
int display_height;
int buffer_width;
int buffer_height;

void initialize_display(const int b_width, const int b_height, const int scale) {
    display_scale  = scale;
    buffer_width   = b_width;
    buffer_height  = b_height;
    display_width  = buffer_width * scale;
    display_height = buffer_height * scale;

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    printf("creating a display. width: %i, height: %i.\n", display_width, display_height);
    display = al_create_display(display_width, display_height);
    must_init(display, "display");
    buffer  = al_create_bitmap(buffer_width, buffer_height);
    must_init(buffer, "display_buffer");
}

void destroy_display() {
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}

void start_drawing(Colour background_colour) {
    al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb(background_colour.r, background_colour.g, background_colour.b));
}

void finish_drawing() {
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer,
        0, 0, buffer_width, buffer_height,
        0, 0, display_width, display_height,
    0);
    al_flip_display();
}

/*
    font stuff
*/

ALLEGRO_FONT* font;

void initialize_font() {
    font = al_create_builtin_font();
}

void draw_text(float x, float y, const char* text, Colour colour, int flags) {
    al_draw_text(font, al_map_rgb(colour.r, colour.g, colour.b), x, y, flags, text);
}

void destroy_font() {
    al_destroy_font(font);
}

/*
    sprites stuff
    instead of doing this for any generic game, i'll have to specifically tailor it to waistgunners
    this is because ALLEGRO_BITMAP is not available to Swift for some reason
*/

/*
    keyboard stuff
    not sure if i want to put the keyboard event handling here
*/

void initialize_keyboard() {
    al_install_keyboard();
}

/*
    timing and events stuff
*/

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_EVENT event;

void initialize_timer_and_event(const int frames_per_second) {
    timer = al_create_timer( 1.0 / frames_per_second );
    queue = al_create_event_queue();

    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void destroy_timer_and_event() {
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

void start_timer() {
    al_start_timer(timer);
}

void register_keyboard() {
    al_register_event_source(queue, al_get_keyboard_event_source());
}

void register_display() {
    al_register_event_source(queue, al_get_display_event_source(display));
}

int wait_for_event() {
    al_wait_for_event(queue, &event);
    return event.type;
}

bool event_queue_is_empty() {
    return al_is_event_queue_empty(queue);
}

// don't forget code to check which key is pressed