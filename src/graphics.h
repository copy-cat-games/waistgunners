// not just the display, but also the sprites, too!

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "game_state.h"
#include "event.h"
#include "button.h"
#include "power_up.h"
#include "cloud.h"
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

extern int landscape_horizontal_scroll;

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
    ALLEGRO_BITMAP* missile;
    ALLEGRO_BITMAP* missile_fire[4];

    ALLEGRO_BITMAP* reticle_aiming;
    ALLEGRO_BITMAP* reticle_firing;
    ALLEGRO_BITMAP* bullet_clip;

    ALLEGRO_BITMAP* icon;

    ALLEGRO_BITMAP* banner;

    ALLEGRO_BITMAP* landscape;
    // more landscape sprites might be added in the future, depends on how much time TheAyeStride has

    ALLEGRO_BITMAP* small_cirrus;
    ALLEGRO_BITMAP* medium_stratoculumus;
    // more cloud sprites might be added in the future, depends on who is voluntold for making these

    ALLEGRO_BITMAP* power_up_bigger_clip_size;
    ALLEGRO_BITMAP* power_up_faster_reload;
    ALLEGRO_BITMAP* power_up_faster_bullets;
    ALLEGRO_BITMAP* power_up_destroy_enemies;
    ALLEGRO_BITMAP* power_up_repair_engine;
    ALLEGRO_BITMAP* power_up_missing;
    ALLEGRO_BITMAP* power_up_invincibility;
    // more powerups might be added, depends on which poor chap ~~is forced~~ volunteers to make more sprites for them
} SPRITES;

extern SPRITES sprites;

// get_sprite() is internally used, so i'm not going to
// declare it here

void init_sprites();
void destroy_sprites();

void draw();

#endif