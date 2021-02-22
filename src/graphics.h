// not just the display, but also the sprites, too!

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "game_state.h"
#include "event.h"
#include "bomber.h"
#include "engine.h"
#include "bullet.h"
#include "enemy.h"
#include "particle.h"
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

#define BOMBER_WIDTH   64
#define BOMBER_HEIGHT  59
#define ENGINE_WIDTH   8
#define ENGINE_HEIGHT  15
#define REDICLE_WIDTH  19
#define REDICLE_HEIGHT 19

#define ENEMY_FIGHTER_WIDTH  27
#define ENEMY_FIGHTER_HEIGHT 24

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
    ALLEGRO_BITMAP* bullet_clip;
} SPRITES;

extern SPRITES sprites;

// get_sprite() is internally used, so i'm not going to
// declare it here

void init_sprites();
void destroy_sprites();

void draw();

#endif