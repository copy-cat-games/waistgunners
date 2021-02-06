// not just the display, but also the sprites, too!

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game_state.h"
#include "event.h"
#include "hud.h"

#ifndef _GRAPHICS
#define _GRAPHICS

#define BUFFER_WIDTH   200
#define BUFFER_HEIGHT  300
#define DISPLAY_SCALE  2
#define DISPLAY_WIDTH  (BUFFER_WIDTH * DISPLAY_SCALE)
#define DISPLAY_HEIGHT (BUFFER_HEIGHT * DISPLAY_SCALE)

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_BITMAP* buffer;

void init_display();
void destroy_display();

#define BOMBER_WIDTH   63
#define BOMBER_HEIGHT  58
#define ENGINE_WIDTH   7
#define ENGINE_HEIGHT  14
#define REDICLE_WIDTH  18
#define REDICLE_HEIGHT 18

#define ENEMY_FIGHTER_WIDTH  26
#define ENEMY_FIGHTER_HEIGHT 23

typedef struct SPRITES {
    ALLEGRO_BITMAP* spritesheet;

    ALLEGRO_BITMAP* bomber;
    ALLEGRO_BITMAP* bomber_engine;
    ALLEGRO_BITMAP* bomber_engine_damaged;
    ALLEGRO_BITMAP* bomber_engine_dead;

    ALLEGRO_BITMAP* player_bullet_1;
    ALLEGRO_BITMAP* player_bullet_2;
    ALLEGRO_BITMAP* enemy_bullet_1;
    ALLEGRO_BITMAP* enemy_bullet_2;

    ALLEGRO_BITMAP* fighter;
    ALLEGRO_BITMAP* imposter;
    ALLEGRO_BITMAP* imposter_engine;
    ALLEGRO_BITMAP* imposter_engine_damaged;
    ALLEGRO_BITMAP* imposter_engine_dead;
    ALLEGRO_BITMAP* jet_up;
    ALLEGRO_BITMAP* jet_down;

    ALLEGRO_BITMAP* reticle_aiming;
    ALLEGRO_BITMAP* reticle_firing;
} SPRITES;

extern SPRITES sprites;

// get_sprite() is internally used, so i'm not going to
// declare it here

void init_sprites();
void destroy_sprites();

void display_pre_draw();
void display_post_draw();

void draw();

#endif