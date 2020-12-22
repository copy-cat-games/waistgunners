#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <math.h>

/*
    general stuff
    - [x] crashing (you never know!)
*/

void crash(const int exit_code, const char* msg);
void initialize_allegro();

/*
    display and graphics stuff
    - [ ] initializing the display and buffer
    - [x] setting the display and buffer
    - [ ] cleaning up the display and buffer
    - [x] colours
    - [ ] drawing various things to the display
        - [ ] rectangles (filled and unfilled)
        - [ ] circles
        - [ ] lines
    - [ ] creating sprites
        - [ ] from spritesheet
    - [ ] font and drawing text
*/
typedef struct Colour {
    int r;
    int g;
    int b;
    float a;
} Colour;

void initialize_display(const int b_width, const int b_height, const int scale);
void destroy_display();
void start_drawing(Colour background_colour);
void finish_drawing();

void initialize_font();
void destroy_font();
void draw_text(float x, float y, const char* text, Colour colour, int flags);

/*
    keyboard stuff
*/

void initialize_keyboard();

/*
    timing and event queue
*/

void initialize_timer_and_event(const int frames_per_second);
void destroy_timer_and_event();
void start_timer();
void register_keyboard();
void register_display();

int wait_for_event();
bool event_queue_is_empty();