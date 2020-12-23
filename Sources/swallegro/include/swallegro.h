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
    lines and shapes
*/

void set_thickness(float new_thickness);
void draw_line(float x, float y, float dx, float dy, Colour colour);
void draw_rectangle(float x, float y, float width, float height, Colour colour, bool filled);

/*
    sprite stuff
*/

typedef enum SPRITES {
    SPRITESHEET = 0,
    SPRITE_BOMBER,
    SPRITE_ENGINE,
    SPRITE_ENGINE_DAMAGED,
    SPRITE_ENGINE_DEAD,

    SPRITE_RETICLE_AIMING,
    SPRITE_RETICLE_FIRING,

    SPRITE_PLAYER_BULLET_1,
    SPRITE_PLAYER_BULLET_2,
    SPRITE_ENEMY_BULLET_1,
    SPRITE_ENEMY_BULLET_2,

    SPRITE_ENEMY_FIGHTER,
    SPRITE_ENEMY_JET_DOWN,
    SPRITE_ENEMY_JET_UP,
    SPRITE_ENEMY_IMPOSTER,
    SPRITE_ENEMY_IMPOSTER_ENGINE,
    SPRITE_ENEMY_IMPOSTER_ENGINE_DAMAGED,
    SPRITE_ENEMY_IMPOSTER_ENGINE_DEAD,

    // always last
    SPRITES_N,
} SPRITES;

void load_spritesheet(const char* path);
void load_sprite(int identifier, int start_x, int start_y, int width, int height);
void draw_sprite(int identifier, float x, float y, int flags);
void destroy_sprites();

/*
    keyboard and mouse stuff
*/

void initialize_keyboard();
void initialize_mouse();
void update_mouse();
bool mouse_is_down();
int get_mouse_x();
int get_mouse_y();

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
int get_keyboard_code();