// not just the display, but also the sprites, too!

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game_state.h"

#define BUFFER_WIDTH   200
#define BUFFER_HEIGHT  300
#define DISPLAY_SCALE  2
#define DISPLAY_WIDTH  (BUFFER_WIDTH * DISPLAY_SCALE)
#define DISPLAY_HEIGHT (BUFFER_HEIGHT * DISPLAY_SCALE)

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_BITMAP* buffer;

void init_display();
void destroy_display();

void display_pre_draw();
void display_post_draw();

#define BOMBER_WIDTH   63
#define BOMBER_HEIGHT  58
#define ENGINE_WIDTH   7
#define ENGINE_HEIGHT  14
#define REDICLE_WIDTH  18
#define REDICLE_HEIGHT 18

#define ENEMY_FIGHTER_WIDTH  26
#define ENEMY_FIGHTER_HEIGHT 23

typedef struct SPRITES {
    ALLEGRO_BITMAP* _sprite_sheet;

    ALLEGRO_BITMAP* bomber;
    ALLEGRO_BITMAP* engine;
    ALLEGRO_BITMAP* redicle;
    ALLEGRO_BITMAP* redicle2;

    ALLEGRO_BITMAP* engine_dead;

    ALLEGRO_BITMAP* enemy_fighter;
    ALLEGRO_BITMAP* enemy_imposter;
    ALLEGRO_BITMAP* enemy_engine;
} SPRITES;

extern SPRITES sprites;

// get_sprite() is internally used, so i'm not going to
// declare it here

void init_sprites();
void destroy_spites();
