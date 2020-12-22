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
    sprite stuff
*/
/*
#define SPRITESHEET  0
#define SPRITE_BOMBER 1
#define SPRITE_ENGINE 3
#define SPRITE_ENGINE_DAMAGED 4
#define SPRITE_ENGINE_DEAD 5

#define SPRITE_REDICLE_AIMING 6
#define SPRITE_REDICLE_FIRING 7

#define SPRITE_PLAYER_BULLET 8
#define SPRITE_ENEMY_BULLET 9

#define SPRITE_ENEMY_FIGHTER 10
#define SPRITE_ENEMY_JET 11
#define SPRITE_ENEMY_IMPOSTER 12
#define SPRITE_ENEMY_IMPOSTER_ENGINE 13
#define SPRITE_ENEMY_IMPOSTER_ENGINE_DAMAGED 14
#define SPRITE_ENEMY_IMPOSTER_ENGINE_DEAD 15

// always last
#define SPRITES_N 16
*/

typedef enum SPRITES {
    SPRITESHEET = 0,
    SPRITE_BOMBER,
    SPRITE_ENGINE,
    SPRITE_ENGINE_DAMAGED,
    SPRITE_ENGINE_DEAD,

    SPRITE_REDICLE_AIMING,
    SPRITE_REDICLE_FIRING,

    SPRITE_PLAYER_BULLET,
    SPRITE_ENEMY_BULLET,

    SPRITE_ENEMY_FIGHTER,
    SPRITE_ENEMY_JET,
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
int get_keyboard_code();